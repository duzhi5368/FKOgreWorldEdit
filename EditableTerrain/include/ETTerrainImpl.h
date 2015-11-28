#ifndef __ETTERRAINIMPL_H__
#define __ETTERRAINIMPL_H__

#include "ETPrerequisites.h"
#include "ETTerrainInfo.h"
#include "ETBrush.h"
#include "ETOptions.h"
#include "ETSelection.h"


namespace Ogre
{
	class Tile;
	class ETTerrain;
	class IndexHandler;
	typedef std::vector<Tile*> TileCol;
	typedef std::vector<TileCol> TileGrid;

	class ET_EXPORT TerrainImpl : public MovableObject
	{
	public:
		TerrainImpl(ETTerrain *terrain , const std::string& name , const String &useMovableType);
		~TerrainImpl();

		void createTerrain(TerrainInfo* info, ushort tileSize = 33);
		void destroyTerrain();

		bool isTerrainLoaded() const { return mTerrainLoaded; }

		TerrainInfo* getTerrainInfo() const { return mInfo; }

		void setLODErrorMargin(unsigned int maxPixelError, unsigned int viewportHeight);

		void setUseLODMorphing(bool lodMorph, float startMorphing, const std::string& morphParamName);

		void setMaterial(MaterialPtr material) { mMaterial = material; }
		const MaterialPtr& getMaterial() const { return mMaterial; }

		void deform(int x, int z, const Brush& brush, float intensity);
		void deform(const Selection& selection, float intensity);
		void smooth(const Selection& selection, float intensity);
		void avgFlatten(const Selection& selection, float intensity);
		void moveSelection(const Selection& selection, float intensity);
		void setHeights(int x, int z, const Brush& brush);
		void getHeights(int x, int z, Brush& brush) const;


		virtual const String& getMovableType(void) const;
		virtual const AxisAlignedBox& getBoundingBox(void) const;
		virtual Real getBoundingRadius(void) const;
		virtual void _updateRenderQueue(RenderQueue* queue);
		virtual void _notifyCurrentCamera(Camera* cam);
		virtual void _notifyAttached(Node* parent, bool isTagPoint = false);

		TileGrid &getTileGrid()
		{
			return mTiles;
		}
		// 替换指定纹理层的纹理
		bool setPaintTexutreName(size_t nPaintChannel , const String &strTextureName);

		ETTerrain *getTerrain()
		{
			return m_terrain;
		}

		// 设置用户值
		virtual void setUserAny(const Any& anything);

		// 获得选项
		const Options &getOption()const
		{
			return mOpt;
		}

	private:
		void createTiles(); 
		void updateTiles(int fromX, int fromZ, int toX, int toZ);

		// 附加所有Tile到相关节点
		void attachTiles(SceneNode* parent);
		// 更新包围盒
		void updateBounds();
		bool mTerrainLoaded;
		IndexHandler* mIndexHandler;
		TerrainInfo *mInfo;
		Options mOpt;
		int mViewportHeight;
		MaterialPtr mMaterial;
		// 使用这个类型
		String mUseMovableType;

		// 本地包围盒
		AxisAlignedBox m_aabb;

		/** Our terrain tile grid.*/
		TileGrid mTiles;

		ETTerrain *m_terrain;
	};
}

#endif
