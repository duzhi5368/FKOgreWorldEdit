// ============================================================
// 工程项目	:	MMORPG
// 文件名	:	ETTerrain.cpp
// 创建人	:	王宏张
// 说明		:	用来载入和保存ET地形的
// 创建时间	:	2007-7-12 13:57:05
// ============================================================
#include "ETPrerequisites.h"
#include "ETTerrain.h"
#include "OgreSceneManager.h"
#include "OgreViewPort.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreHardwarePixelBuffer.h"

namespace Ogre
{
	
	const String ETTerrain::MOVABLE_TYPE("ETTerrain");
	void ETTerrain::init(SceneManager *pSceneMgr , ICollisionWorld *world , const String &name)
	{
		shutdown();
		// create terrain manager
		m_world = world;
		mTerrainImpl = new TerrainImpl(this , name , MOVABLE_TYPE);
		pSceneMgr->injectMovableObject(mTerrainImpl);
		mTerrainImpl->_notifyManager(pSceneMgr);
		mTerrainImpl->setUseLODMorphing(true, 0.2, "morphFactor");
	}

	void ETTerrain::shutdown()
	{
		closeTerrain();
		OGRE_DELETE(mSplatManager);
		OGRE_DELETE(mTerrainImpl);
		m_world = 0;
	}

	void ETTerrain::closeTerrain()
	{
		if(!mBlockerTexture.isNull())
		{
			TextureManager::getSingleton().remove(mBlockerTexture->getName());
			mBlockerTexture.setNull();
		}
		OGRE_DELETE(mSplatManager);
		if(mTerrainImpl)
		{
			// add by 王宏张 2007-7-18
			// 卸载材质
			const MaterialPtr mat = mTerrainImpl->getMaterial();
			if(!mat.isNull())
			{
				if(mat->getName() == "ETTerrainMaterial")
				{
					mat->unload();
				}
				else
				{
					MaterialManager::getSingleton().remove(mat->getName());
				}
			}
			TextureManager::getSingleton().remove("ETLightmap");
			mTerrainImpl->destroyTerrain();
		}
		mTerrainName.clear();
	}

	bool ETTerrain::createTerrain(size_t width , size_t height , const AxisAlignedBox &boundingBox, float defaultHeight , ushort tileSize)
	{
		// 创建高度图
		float *heightMap = new float[width * height];
		for(size_t i = 0 ; i < width * height ; i ++)
		{
			heightMap[i] = defaultHeight;
		}
		TerrainInfo *terrainInfo = new TerrainInfo(width, height, heightMap , true);
		// 设置大小
		terrainInfo->setExtents(boundingBox);

		return createTerrain(terrainInfo , tileSize);
	}

	// 函 数 名 : loadTerrain
	// 注    释 : 载入地形
	// 返回类型 : bool 
	// 作    者 : 王宏张
	// 编写时间 : 2007-7-12 13:39:28
	// 参    数 : const String &strName							地形名称
	bool ETTerrain::loadTerrain(const String &strName)
	{
		closeTerrain();

		mTerrainName = strName;

		String mapDir = mBaseDir + strName;

		try
		{
			ResourceGroupManager::getSingleton().removeResourceLocation(mapDir, mResourceGroupName);
		}
		catch(Exception)
		{

		}
		ResourceGroupManager::getSingleton().addResourceLocation(mapDir, mDataStreamType, mResourceGroupName);

		// Load resource paths from config file
		ConfigFile cf;
		cf.load(strName + ".cfg" , mResourceGroupName , "\t:=" , true);

		// Go through all sections & settings in the file
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		// add by 王宏张 2007-7-11
		// 读取地形配置文件
		size_t heightMapWidth = 257;
		size_t heightMapHeight = 257;
		AxisAlignedBox box(0, 0, 0, 0, 0, 0);
		String secName, typeName, archName;
		Options option;
		if (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i = settings->find("BoxMin");
			if(i != settings->end())
			{
				box.setMinimum(StringConverter::parseVector3(i->second));
			}
			i = settings->find("BoxMax");
			if(i != settings->end())
			{
				box.setMaximum(StringConverter::parseVector3(i->second));
			}

			i = settings->find("tileSize");
			if(i != settings->end())
			{
				option.tileSize = StringConverter::parseInt(i->second);
			}

			i = settings->find("maxPixelError");
			if(i != settings->end())
			{
				option.maxPixelError = StringConverter::parseInt(i->second);
			}

			i = settings->find("useLODMorph");
			if(i != settings->end())
			{
				option.useLODMorph = StringConverter::parseBool(i->second);
			}

			i = settings->find("lodMorphStart");
			if(i != settings->end())
			{
				option.lodMorphStart = StringConverter::parseReal(i->second);
			}

			i = settings->find("lodMorphParam");
			if(i != settings->end())
			{
				option.lodMorphParam = i->second;
			}

			i = settings->find("HeightMapWidth");
			if(i != settings->end())
			{
				heightMapWidth = StringConverter::parseUnsignedLong(i->second);
			}

			i = settings->find("HeightMapHeight");
			if(i != settings->end())
			{
				heightMapHeight = StringConverter::parseUnsignedLong(i->second);
			}
		}


		// now we revert the process and load the data back from disk
		// recreate terrain...
		Image image;
		DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(strName + "_ETterrain.hgt", mResourceGroupName);
		TerrainInfo *terrainInfo = new TerrainInfo();
		loadHeightmapFromFloatRawData(terrainInfo, stream , heightMapWidth , heightMapHeight);
		terrainInfo->setExtents(box);
		mTerrainImpl->setUseLODMorphing(option.useLODMorph , option.lodMorphStart , option.lodMorphParam);
		mTerrainImpl->setLODErrorMargin(option.maxPixelError , 600);
		mTerrainImpl->createTerrain(terrainInfo , option.tileSize);

		// create the splatting manager
		String splattingName("ETSplatting_" + mTerrainImpl->getName());

		mSplatManager = new SplattingManager(splattingName, mResourceGroupName, terrainInfo->getWidth() - 1, terrainInfo->getHeight() - 1, COVERAGE_CHANNEL_NUM);
		// specify number of splatting textures we need to handle
		mSplatManager->setNumTextures(SPLATTING_TEXTURE_NUM);

		// now load the splatting maps
		for (uint i = 0; i < mSplatManager->getNumMaps(); ++i)
		{
			image.load(strName + "_ETcoverage." + StringConverter::toString(i) + ".tga", mResourceGroupName);
			mSplatManager->loadMapFromImage(i, image);
		}

		// add by 王宏张 2007-7-18
		// 分析材质脚本并设置
		MaterialManager::getSingleton().parseScript(ResourceGroupManager::getSingleton().openResource(strName + ".material" , mResourceGroupName , false) , mResourceGroupName);
		MaterialPtr material (MaterialManager::getSingleton().getByName(strName + "_ETTerrainMaterial"));
		mTerrainImpl->setMaterial(material);

		// 载入障碍区域贴图
		mBlockerTexture = TextureManager::getSingleton().load(strName + "_Blocker.tga"
			, mResourceGroupName
			, TEX_TYPE_2D
			, 0
			, 1
			, false
			, PF_A8R8G8B8
			);
		return true;
	}

	bool ETTerrain::saveTerrain(const String &strName)
	{
		String oldFolder = mBaseDir + mTerrainName + "/";
		String newFolder = mBaseDir + strName + "/";

		RemoveDirectory(newFolder.c_str());
		CreateDirectory(newFolder.c_str() , 0);

		//remove_all(newFolder.c_str());
		//create_directory(newFolder.c_str());

		mTerrainName = strName;
 

		// just for demonstration, we'll save anything we can
		// start with the terrain
		{
			String fileName(newFolder + strName + "_ETterrain.hgt");
			DeleteFile(fileName.c_str());
			std::ofstream stream(fileName.c_str() , std::ios::binary);
			saveHeightmapToFloatRawData(mTerrainImpl->getTerrainInfo(), stream);
			stream.close();
		}
		
		Image image;

		// now save the splatting maps
		for (uint i = 0; i < mSplatManager->getNumMaps(); ++i)
		{
			mSplatManager->saveMapToImage(i, image);
			image.save(newFolder + strName + "_ETcoverage." + StringConverter::toString(i) + ".tga");
		}


		std::ofstream of((newFolder + strName + ".cfg").c_str());

		if (!of)
		{
			OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
				"saveTerrain");
			return false;
		}

		//of << "[Setting]"  << std::endl;
		of << "BoxMin=" << StringConverter::toString(mTerrainImpl->getTerrainInfo()->getExtents().getMinimum()) << std::endl;
		of << "BoxMax=" << StringConverter::toString(mTerrainImpl->getTerrainInfo()->getExtents().getMaximum()) << std::endl;
		of << "HeightMapWidth=" << StringConverter::toString(mTerrainImpl->getTerrainInfo()->getWidth()) << std::endl;
		of << "HeightMapHeight=" << StringConverter::toString(mTerrainImpl->getTerrainInfo()->getHeight()) << std::endl;
		const Options &option = mTerrainImpl->getOption();
		of << "tileSize=" << StringConverter::toString(option.tileSize) << std::endl;
		of << "maxPixelError=" << StringConverter::toString(option.maxPixelError) << std::endl;
		of << "useLODMorph=" << StringConverter::toString(option.useLODMorph) << std::endl;
		of << "lodMorphStart=" << StringConverter::toString(option.lodMorphStart) << std::endl;
		of << "lodMorphParam=" << option.lodMorphParam << std::endl;


		of.close();


		// add by 王宏张 2007-7-18
		// 导出材质脚本
		MaterialSerializer serializer;
		MaterialPtr mat;
		if(mTerrainImpl->getMaterial()->getName() == strName + "_ETTerrainMaterial")
		{
			mat = mTerrainImpl->getMaterial();
		}
		else
		{
			// add by 王宏张 2007-7-18
			// 克隆一份出来
			mat = mTerrainImpl->getMaterial()->clone(strName + "_ETTerrainMaterial");
			// add by 王宏张 2007-7-18
			// 把原来的材质卸载
			if(mTerrainImpl->getMaterial()->getName() == "ETTerrainMaterial")
			{
				mTerrainImpl->getMaterial()->unload();
			}
			else
			{
				MaterialManager::getSingleton().remove(mTerrainImpl->getMaterial()->getName());
			}
		}
		serializer.exportMaterial(mat , newFolder + strName + ".material");

		mTerrainImpl->setMaterial(mat);

		// 保存障碍区域到文件
		uchar *buffer = new uchar[PixelUtil::getMemorySize(mBlockerTexture->getWidth() , mBlockerTexture->getHeight() , 1 , mBlockerTexture->getFormat())];
		PixelBox box(mBlockerTexture->getWidth() , mBlockerTexture->getHeight() , 1 , mBlockerTexture->getFormat() , buffer);
		mBlockerTexture->getBuffer()->blitToMemory(box);
		Image imageBlocker;
		imageBlocker.loadDynamicImage(buffer , box.getWidth() , box.getHeight() , box.getDepth() , box.format , true);
		imageBlocker.save(newFolder + strName + "_Blocker.tga");

		return true;
	}

	void ETTerrain::updateLightmap(Vector3 lightDir, const ColourValue& lightCol, const ColourValue& ambient, bool shadowed)
	{
		TerrainInfo *terrInfo = mTerrainImpl->getTerrainInfo();
		Image lightmap;
		createTerrainLightmap(terrInfo, lightmap
			, terrInfo->getWidth() - 1
			, terrInfo->getHeight() - 1
			, lightDir
			, lightCol
			, ambient
			, shadowed);

		// get our dynamic texture and update its contents
		TexturePtr tex = TextureManager::getSingleton().getByName("ETLightmap");
		if(!tex.isNull())
		{
			tex->getBuffer(0, 0)->blitFromMemory(lightmap.getPixelBox(0, 0));
		}
	}

	std::pair<bool, Vector3> ETTerrain::rayIntersects(Camera *pCamera , Real screenx, Real screeny) const
	{
		// whenever the mouse is moved, we update the position on the
		// terrain to which the mouse is currently pointing
		Ray mouseRay = pCamera->getCameraToViewportRay(screenx, screeny);

		// since ETM is no longer a scene manager, we don't use a ray scene query,
		// but instead query the terrain info directly
		return rayIntersects(mouseRay);
	}

	bool ETTerrain::createTerrain(TerrainInfo *terrainInfo, ushort tileSize)
	{
		closeTerrain();
		mTerrainImpl->createTerrain(terrainInfo, tileSize);
		// add by 王宏张 2007-7-14
		// 载入默认的地形贴图
		MaterialPtr material(MaterialManager::getSingleton().getByName("ETTerrainMaterial"));
		material = material->clone("ETTerrainMaterial" + mTerrainImpl->getName());

		String splattingName("ETSplatting_" + mTerrainImpl->getName());

		for(int i = 0 ; i < COVERAGE_TEXTURE_NUM ; i ++)
		{
			material->getTechnique(0)->getPass(0)->getTextureUnitState(i)->setTextureName(splattingName + StringConverter::toString(i));
		}

		mTerrainImpl->setMaterial(material);

		// create the splatting manager
		mSplatManager = new SplattingManager(splattingName, "ET", terrainInfo->getWidth() - 1, terrainInfo->getHeight() - 1, COVERAGE_CHANNEL_NUM);
		// specify number of splatting textures we need to handle
		mSplatManager->setNumTextures(SPLATTING_TEXTURE_NUM);


		// 创建与地形大小相等的碰撞区域贴图
		mBlockerTexture = TextureManager::getSingleton().createManual("ETBlocker_" + mTerrainImpl->getName()
			, mResourceGroupName
			, TEX_TYPE_2D
			, terrainInfo->getWidth() - 1
			, terrainInfo->getHeight() - 1
			, 0
			, PF_A8R8G8B8
			, TU_DYNAMIC
			);
		// 清空纹理
		HardwarePixelBufferSharedPtr buffer = mBlockerTexture->getBuffer();
		uchar *colorBuffer = (uchar*)buffer->lock(0 , buffer->getSizeInBytes() , HardwareBuffer::HBL_DISCARD);
		memset(colorBuffer , 0 , buffer->getSizeInBytes());
		buffer->unlock();
		return true;
	}
	// 设置覆盖在地形上的贴图重复在一个地形中多少遍
	void ETTerrain::setSplattingSplit(float splitX , float splitY)
	{
		const MaterialPtr mat = mTerrainImpl->getMaterial();
		if(!mat.isNull())
		{
			mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("splatScaleX" , splitX);
			mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("splatScaleZ" , splitY);
		}
	}


	// 设置用户数据到这个对象
	void ETTerrain::setUserAny(const Any& anything)
	{
		mTerrainImpl->setUserAny(anything);
	}

	// 获得用户数据
	const Any& ETTerrain::getUserAny(void) const
	{
		return mTerrainImpl->getUserAny();
	}
};