#include "ETPrerequisites.h"
#include "ETTerrainImpl.h"
#include "ETTerrainInfo.h"
#include "ETTile.h"
#include "ETTerrain.h"
#include "ETIndexHandler.h"

#include <OgreStringConverter.h>
using namespace std;

namespace Ogre
{
	uint checkTileSizeAndGetMaxLOD(ushort tileSize)
	{
		// check whether the tilesize suffices 2^n+1
		for (unsigned int i = 1; i <= 10; ++i)
		{
			if (tileSize == (uint)(1 << i) + 1)
			{
				return i+1;
			}
		}
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Given tilesize does not satisfy 2^n+1", "Impl::checkTileSizeAndMaxLOD");
	}

	uint checkInfo(TerrainInfo* info, ushort tileSize)
	{
		uint ret = checkTileSizeAndGetMaxLOD(tileSize);
		// check width and height
		if (info->getWidth() == 1 || info->getWidth() % (tileSize-1) != 1)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Heightmap width must be a multiple of (tileSize-1) + 1", "Impl::checkInfo");
		if (info->getHeight() == 1 || info->getHeight() % (tileSize-1) != 1)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Heightmap height must be a multiple of (tileSize-1) + 1", "Impl::checkInfo");
		return ret;
	}


	TerrainImpl::TerrainImpl(ETTerrain *terrain , const string& name , const String &useMovableType)
		: MovableObject(name)
		, mTerrainLoaded(false)
		, mIndexHandler(0)
		, mViewportHeight(0)
		, mUseMovableType(useMovableType)
		, m_terrain(terrain)
		, mInfo(0)
	{
		mOpt.maxPixelError = 3;
		mOpt.useLODMorph = false;
		mOpt.factorC = 0.0f;

		setRenderQueueGroup(RENDER_QUEUE_WORLD_GEOMETRY_1);
	}

	TerrainImpl::~TerrainImpl()
	{
		destroyTerrain();
	}


	void TerrainImpl::createTerrain(TerrainInfo* info, ushort tileSize)
	{
		// ensure we have a viewport height set
		if (mOpt.factorC <= 0)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "You need to set a valid pixel error margin.", "TerrainImpl::createTerrain");

		// delete currently loaded terrain (if any)
		destroyTerrain();

		// load new terrain
		mInfo = info;
		mOpt.tileSize = tileSize;
		mIndexHandler = new IndexHandler(mOpt.tileSize);
		createTiles();

		mTerrainLoaded = true;
	} 

	void TerrainImpl::createTiles()
	{
		// create a root node for the terrain tiles
		SceneNode* terrainNode = 0;

		// create the tile grid
		size_t numTilesCol = (mInfo->getWidth()-1) / (mOpt.tileSize-1);
		size_t numTilesRow = (mInfo->getHeight()-1) / (mOpt.tileSize-1);
		mTiles.assign(numTilesCol, TileCol(numTilesRow, (Tile*)0));

		// for each tile, create a scene node to attach to and create the tile
		for (size_t i = 0; i < numTilesCol; ++i)
		{
			for (size_t j = 0; j < numTilesRow; ++j)
			{
				mTiles[i][j] = new Tile(this, mIndexHandler, mInfo, mOpt, i*(mOpt.tileSize-1), j*(mOpt.tileSize-1));
				mTiles[i][j]->setUserAny(mUserAny);
			}
		}
		if(!mParentIsTagPoint)
		{
			attachTiles((SceneNode*)mParentNode);
		}
		updateBounds();

	}

	void TerrainImpl::destroyTerrain()
	{
		for (size_t i = 0; i < mTiles.size(); ++i)
			for (size_t j = 0; j < mTiles[i].size(); ++j)
				delete mTiles[i][j];
		mTiles.clear();
		m_aabb.setNull();
		delete mIndexHandler;
		mIndexHandler = 0;
		OGRE_DELETE(mInfo);
		mTerrainLoaded = false;
	}


	void TerrainImpl::setLODErrorMargin(unsigned int maxPixelError, unsigned int viewportHeight)
	{
		mOpt.maxPixelError = maxPixelError;

		// calculate the C factor used in LOD distance calculations
		float T = 2 * maxPixelError / float(viewportHeight);
		mOpt.factorC = 1 / T;
	}

	void TerrainImpl::setUseLODMorphing(bool lodMorph, float startMorphing, const std::string& morphParamName)
	{
		mOpt.useLODMorph = lodMorph;
		mOpt.lodMorphStart = startMorphing;
		mOpt.lodMorphParam = morphParamName;
	}


	void TerrainImpl::deform(int x, int z, const Brush& brush, float intensity)
	{
		// positions given are supposed to be the mid of the brush
		// so adjust accordingly
		x -= (int)brush.getWidth()/2;
		z -= (int)brush.getHeight()/2;

		// iterate over all fields of the brush array and apply them to the
		// heightmap data if they lie within
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			int posX = x + (int)i;
			if (posX < 0 || posX >= (int)mInfo->getWidth())
				continue;
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				int posZ = z + (int)j;
				if (posZ < 0 || posZ >= (int)mInfo->getHeight())
					continue;

				float& height = mInfo->at(size_t(posX), size_t(posZ));

				height += intensity * brush.at(i, j);
				if (height > 1)
					height = 1;
				if (height < 0)
					height = 0;
			}
		}

		updateTiles(x, z, x+(int)brush.getWidth(), z+(int)brush.getHeight());
	}

	void TerrainImpl::deform(const Selection& selection, float intensity)
	{
		for(Selection::const_iterator iter = selection.begin() ; iter != selection.end() ; iter ++)
		{
			if(size_t(iter->first.getX() + selection.getPosition().getX()) >= mInfo->getWidth()
				|| size_t(iter->first.getY() + selection.getPosition().getY()) >= mInfo->getHeight())
				continue;
			float& height = mInfo->at(size_t(iter->first.getX() + selection.getPosition().getX())
				, size_t(iter->first.getY() + selection.getPosition().getY()));
			height += intensity * iter->second;
		}
		updateTiles(selection.getMin().getX(), selection.getMin().getY(), selection.getMax().getX(), selection.getMax().getY());
	}

	void TerrainImpl::smooth(const Selection& selection, float intensity)
	{

		// add by 王宏张 2007-7-25
		// 计算平均高度
		float avgHeiht = 0.0f;
		for(Selection::const_iterator iter = selection.begin() ; iter != selection.end() ; ++iter)
		{
			if(size_t(iter->first.getX() + selection.getPosition().getX()) >= mInfo->getWidth()
				|| size_t(iter->first.getY() + selection.getPosition().getY()) >= mInfo->getHeight())
				continue;
			float height = mInfo->at(size_t(iter->first.getX() + selection.getPosition().getX())
				, size_t(iter->first.getY() + selection.getPosition().getY()));
			avgHeiht += height;
		}
		avgHeiht /= selection.size();

		for(Selection::const_iterator iter = selection.begin() ; iter != selection.end() ; ++iter)
		{
			if(size_t(iter->first.getX() + selection.getPosition().getX()) >= mInfo->getWidth()
				|| size_t(iter->first.getY() + selection.getPosition().getY()) >= mInfo->getHeight())
				continue;
			float& height = mInfo->at(size_t(iter->first.getX() + selection.getPosition().getX())
				, size_t(iter->first.getY() + selection.getPosition().getY()));
			height += (avgHeiht - height) * intensity * iter->second; // smooth算法

		}
		updateTiles(selection.getMin().getX(), selection.getMin().getY(), selection.getMax().getX(), selection.getMax().getY());
	}

	void TerrainImpl::avgFlatten(const Selection& selection, float intensity)
	{
		// add by 王宏张 2007-7-25
		// 计算平均高度
		float avgHeiht = 0.0f;
		for(Selection::const_iterator iter = selection.begin() ; iter != selection.end() ; ++iter)
		{
			if(size_t(iter->first.getX() + selection.getPosition().getX()) >= mInfo->getWidth()
				|| size_t(iter->first.getY() + selection.getPosition().getY()) >= mInfo->getHeight())
				continue;
			float height = mInfo->at(size_t(iter->first.getX() + selection.getPosition().getX())
				, size_t(iter->first.getY() + selection.getPosition().getY()));
			avgHeiht += height;
		}
		avgHeiht /= selection.size();

		for(Selection::const_iterator iter = selection.begin() ; iter != selection.end() ; ++iter)
		{
			if(size_t(iter->first.getX() + selection.getPosition().getX()) >= mInfo->getWidth()
				|| size_t(iter->first.getY() + selection.getPosition().getY()) >= mInfo->getHeight())
				continue;
			float& height = mInfo->at(size_t(iter->first.getX() + selection.getPosition().getX())
				, size_t(iter->first.getY() + selection.getPosition().getY()));
			height = avgHeiht; // avgFlatten算法
		}
		updateTiles(selection.getMin().getX(), selection.getMin().getY(), selection.getMax().getX(), selection.getMax().getY());
	}

	void TerrainImpl::moveSelection(const Selection& selection, float intensity)
	{
		// add by 王宏张 2007-7-30
		// 更新选区
		updateTiles(selection.getMin().getX(), selection.getMin().getY(), selection.getMax().getX(), selection.getMax().getY());
	}

	void TerrainImpl::setHeights(int x, int z, const Brush& brush)
	{
		// positions given are supposed to be the mid of the brush
		// so adjust accordingly
		x -= (int)brush.getWidth()/2;
		z -= (int)brush.getHeight()/2;

		// iterate over all fields of the brush array and apply them to
		// the heightmap data if they lie within
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			int posX = x + (int)i;
			if (posX < 0 || posX >= (int)mInfo->getWidth())
				continue;
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				int posZ = z + (int)j;
				if (posZ < 0 || posZ >= (int)mInfo->getHeight())
					continue;

				float& height = mInfo->at(size_t(posX), size_t(posZ));
				height = brush.at(i, j);
				if (height > 1)
					height = 1;
				if (height < 0)
					height = 0;
			}
		}

		updateTiles(x, z, x+(int)brush.getWidth(), z+(int)brush.getHeight());
	}

	void TerrainImpl::getHeights(int x, int z, Brush& brush) const
	{
		// positions given are supposed to be the mid of the brush
		// so adjust accordingly
		x -= (int)brush.getWidth()/2;
		z -= (int)brush.getHeight()/2;

		// iterate over all fields of the brush array and fill them
		// if they lie within
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			int posX = x + (int)i;
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				int posZ = z + (int)j;
				if (posX < 0 || posX >= (int)mInfo->getWidth() || posZ < 0 || posZ >= (int)mInfo->getHeight())
				{
					brush.at(i, j) = -1;
					continue;
				}

				brush.at(i, j) = mInfo->at(size_t(posX), size_t(posZ));
			}
		}
	}
	// 更新包围盒
	void TerrainImpl::updateBounds()
	{
		m_aabb.setNull();
		for (size_t i = 0; i < mTiles.size(); ++i)
		{
			for (size_t j = 0; j < mTiles[i].size(); ++j)
			{
				// update this tile
				m_aabb.merge(mTiles[i][j]->getBoundingBox());
			}
		}
		if(getParentSceneNode())
		{
			getParentSceneNode()->needUpdate();
		}
	}


	void TerrainImpl::updateTiles(int fromX, int fromZ, int toX, int toZ)
	{
		//OgreProfile("updateTiles");
		{
			//OgreProfile("updateTerrain");

			// 更新地形数据
			for (size_t i = 0; i < mTiles.size(); ++i)
			{
				int startX = (int) ((mOpt.tileSize-1)*i);
				int endX = (int) ((mOpt.tileSize-1)*(i+1));
				for (size_t j = 0; j < mTiles[i].size(); ++j)
				{
					int startZ = (int) ((mOpt.tileSize-1)*j);
					int endZ = (int) ((mOpt.tileSize-1)*(j+1));
					size_t x1 = (size_t) max(fromX, startX);
					size_t x2 = (size_t) min(toX, endX);
					size_t z1 = (size_t) max(fromZ, startZ);
					size_t z2 = (size_t) min(toZ, endZ);
					if (x2 >= x1 && z2 >= z1)
					{
						// update this tile
						mTiles[i][j]->updateTerrain(x1, z1, x2, z2);
					}
				}
			}
		}
		{
			//OgreProfile("updateCollision");
			// 重新生成碰撞模型
			for (size_t i = 0; i < mTiles.size(); ++i)
			{
				int startX = (int) ((mOpt.tileSize-1)*i);
				int endX = (int) ((mOpt.tileSize-1)*(i+1));
				for (size_t j = 0; j < mTiles[i].size(); ++j)
				{
					int startZ = (int) ((mOpt.tileSize-1)*j);
					int endZ = (int) ((mOpt.tileSize-1)*(j+1));
					size_t x1 = (size_t) max(fromX, startX);
					size_t x2 = (size_t) min(toX, endX);
					size_t z1 = (size_t) max(fromZ, startZ);
					size_t z2 = (size_t) min(toZ, endZ);
					if (x2 >= x1 && z2 >= z1)
					{
						// update this tile
						mTiles[i][j]->createCollisionShape();
					}
				}
			}
		}


		{
			//OgreProfile("updateBounds");
			// 更新包围盒
			updateBounds();
		}
	}


	const String& TerrainImpl::getMovableType(void) const
	{
		return mUseMovableType;
	}

	const AxisAlignedBox& TerrainImpl::getBoundingBox(void) const
	{
		return m_aabb;
	}

	Real TerrainImpl::getBoundingRadius(void) const
	{
		Vector3 size = m_aabb.getSize();
		return std::max(std::max(size.x , size.y) , size.z);
	}

	void TerrainImpl::_updateRenderQueue(RenderQueue* queue)
	{

	}

	void TerrainImpl::_notifyCurrentCamera(Camera* cam)
	{
		MovableObject::_notifyCurrentCamera(cam);
	}
	void TerrainImpl::_notifyAttached(Node* parent, bool isTagPoint)
	{
		if(!isTagPoint)
		{
			attachTiles((SceneNode*)parent);
		}
		MovableObject::_notifyAttached(parent , isTagPoint);
	}

	// 附加所有Tile到相关节点
	void TerrainImpl::attachTiles(SceneNode* parent)
	{
		for (size_t i = 0; i < mTiles.size(); ++i)
		{
			for (size_t j = 0; j < mTiles[i].size(); ++j)
			{
				Tile *tile = mTiles[i][j];
				tile->attachTile(parent);
				tile->createCollisionShape();
			}
		}
	}
	// 替换指定纹理层的纹理
	bool TerrainImpl::setPaintTexutreName(size_t nPaintChannel , const String &strTextureName)
	{
		Technique *pTech = mMaterial->getTechnique(0);
		Pass *pass = pTech->getPass(nPaintChannel / SPLATTING_TEXTURE_NUM);
		if(pass)
		{
			TextureUnitState *texture = pass->getTextureUnitState(COVERAGE_TEXTURE_NUM + (nPaintChannel % SPLATTING_TEXTURE_NUM));
			if(texture)
			{
				texture->setTextureName(strTextureName);
				return texture->getTextureName() == strTextureName && !texture->_getTexturePtr().isNull() && texture->_getTexturePtr()->isLoaded();
			}
		}
		return false;
	}

	// 设置用户值
	void TerrainImpl::setUserAny(const Any& anything)
	{
		mUserAny = anything;
		for (size_t i = 0; i < mTiles.size(); ++i)
		{
			for (size_t j = 0; j < mTiles[i].size(); ++j)
			{
				Tile *tile = mTiles[i][j];
				tile->setUserAny(anything);
			}
		}
	}

};
