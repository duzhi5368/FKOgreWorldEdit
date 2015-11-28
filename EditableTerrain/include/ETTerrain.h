// ============================================================
// 工程项目	:	MMORPG
// 文件名	:	ETTerrain.h
// 创建人	:	王宏张
// 说明		:	
// 创建时间	:	2007-7-12 13:57:01
// ============================================================
#pragma once
#include "ETPrerequisites.h"
#include "ETTerrainImpl.h"
#include "ETTerrainInfo.h"
#include "ETBrush.h"
#include "ETSplattingManager.h"

namespace Ogre
{
	class ET_EXPORT ETTerrain
	{
	public:
		ETTerrain()
			: mSplatManager(0)
			, mTerrainImpl(0)
			, mBaseDir("../../../Media/ET/")
			, mResourceGroupName("ET")
			, mDataStreamType("FileSystem")
		{
		}
		~ETTerrain()
		{
			shutdown();
		};

		void init(SceneManager *pSceneMgr , ICollisionWorld *world , const String &name);
		void shutdown();

		bool createTerrain(size_t width , size_t height , const AxisAlignedBox &boundingBox, float defaultHeight = 0.5f , ushort tileSize = 33);
		bool loadTerrain(const String &strName);
		bool saveTerrain(const String &strName);
		void closeTerrain();

		SplattingManager *getSplattingManager(){return mSplatManager;};

		TerrainImpl *getTerrainImpl(){return mTerrainImpl;};
		TerrainInfo *getTerrainInfo()const{return mTerrainImpl->getTerrainInfo();};

		void setResourceGroupName(const String &strResourceGroupName){mResourceGroupName = strResourceGroupName;}
		const String& getResourceGroupName()const{return mResourceGroupName;}

		void setBaseDir(const String &strBaseDir){mBaseDir = strBaseDir;}
		void setDataStreamType(const String &strDataStreamType){mDataStreamType = strDataStreamType;}

		const String &getBaseDir()const{return mBaseDir;};

		void updateLightmap(Vector3 lightDir = Vector3(1, -1, 1), const ColourValue& lightCol = ColourValue(1,1,1),
			const ColourValue& ambient = ColourValue(0.3, 0.3,  0.3), bool shadowed = true);

		std::pair<bool, Vector3> rayIntersects(const Ray& ray) const
		{
			return mTerrainImpl->getTerrainInfo()->rayIntersects(ray);
		}
		// add by 王宏张 2007-7-12
		// 从屏幕坐标与地形做相交，返回世界坐标，屏幕坐标从0~1
		std::pair<bool, Vector3> rayIntersects(Camera *pCamera , Real screenx, Real screeny) const;

		// add by 王宏张 2007-7-13
		// 获得每个Tile对应的世界坐标大小
		Vector3 getTileSize()
		{
			TerrainInfo *info = mTerrainImpl->getTerrainInfo();
			return info->getExtents().getSize() / Vector3(info->getWidth() , 1.0f , info->getHeight());
		}

		// add by 王宏张 2007-7-12
		// 根据地形坐标获得所在地面的世界坐标
		Vector3 getTerrainPos(int x , int z)
		{
			TerrainInfo *info = mTerrainImpl->getTerrainInfo();
			return Vector3(info->vertexToPosX(x) , info->vertexToPosY(info->at(x , z)) , info->vertexToPosZ(z));
		}
		// add by 王宏张 2007-7-23
		// 根据世界坐标获得地形网格索引
		Vector2 getTerrainGrid(const Vector3 &pos)
		{
			TerrainInfo *info = mTerrainImpl->getTerrainInfo();
			return Vector2(info->posToVertexX(pos.x) , info->posToVertexZ(pos.z));
		}
		void setLODErrorMargin(unsigned int maxPixelError, unsigned int viewportHeight)
		{
			mTerrainImpl->setLODErrorMargin(maxPixelError , viewportHeight);
		}
		void setUseLODMorphing(bool lodMorph = true, float startMorphing = 0.25f, 
			const std::string& morphParamName = "morphFactor")
		{
			mTerrainImpl->setUseLODMorphing(lodMorph , startMorphing , morphParamName);
		}

		// 设置用户数据到这个对象
		void setUserAny(const Any& anything);

		// 获得用户数据
		const Any& getUserAny(void) const;

		// 获得碰撞世界
		ICollisionWorld *getCollisionWorld()const
		{
			return m_world;
		}

		// 设置覆盖在地形上的贴图重复在一个地形中多少遍
		void setSplattingSplit(float splitX , float splitY);

		// 获得障碍区域信息
		TexturePtr &getBlockerTexture()
		{
			return mBlockerTexture;
		}

		static const String MOVABLE_TYPE;
	private:
		bool createTerrain(TerrainInfo *terrainInfo, ushort tileSize = 33);
		// 碰撞世界
		ICollisionWorld *m_world;
		SplattingManager *mSplatManager;
		TerrainImpl *mTerrainImpl;
		String mTerrainName;
		String mResourceGroupName;
		String mBaseDir;
		String mDataStreamType;

		// 障碍区域信息
		TexturePtr mBlockerTexture;
	};
};