#ifndef __ETTILE_H__
#define __ETTILE_H__

#include "ETPrerequisites.h"

#include "ETIndexHandler.h"
#include "ETOptions.h"

// forward declarations
namespace Ogre
{
	class SceneManager;
	class RenderOperation;
	class Matrix4;
	class Quaternion;
	class Vector3;
	class Camera;
}

namespace Ogre
{
	// forward declarations
	class TerrainInfo;
	class SutureRenderable;

	class IndexHandler;
	class TerrainImpl;

	// 自动锁定的硬件数据
	class ET_EXPORT AutoBuffer
	{
	public:
		AutoBuffer(HardwareBuffer* data)
		{
			m_data = data;
			m_buffer = m_data->lock(HardwareBuffer::HBL_DISCARD);
		}
		void *getBuffer()
		{
			return m_buffer;
		}
		virtual ~AutoBuffer()
		{
			m_data->unlock();
		}
	private:
		void *m_buffer;
		HardwareBuffer* m_data;
	};

	typedef SharedPtr<AutoBuffer> AutoBufferPtr;
	/** A tile is the unit of terrain used for rendering. */
	class ET_EXPORT Tile : public Renderable , public MovableObject
	{
	public:
		Tile(TerrainImpl* tm,
			IndexHandler* indexHandler, TerrainInfo* info, const Options &opts, size_t startx, size_t startz);
		~Tile();

		static const String MOVABLE_OBJECT_TYPE;

		uint32 getTypeFlags() const;

		const MaterialPtr& getMaterial() const;

		void getRenderOperation(RenderOperation& op);

		void getWorldTransforms(Matrix4* m) const;

		const Quaternion& getWorldOrientation() const;

		const Vector3& getWorldPosition() const;

		Real getSquaredViewDepth(const Camera* cam) const;

		const LightList& getLights() const;

		const String& getMovableType() const;

		const AxisAlignedBox& getBoundingBox() const;

		Real getBoundingRadius() const;

		void _updateRenderQueue(RenderQueue* queue);

		/** Determines the LOD to use based on the current camera distance */
		void _notifyCurrentCamera(Camera* cam);

		void setRenderQueue(uint8 qid);

		/** Updates the custom morph factor parameter for the morph vertex shader */
		void _updateCustomGpuParameter(const GpuProgramParameters::AutoConstantEntry& constEntry,
			GpuProgramParameters* params) const;


		/** Tells the tile to update its contents based on updated heightmap data.
		* This is what makes terrain deforming working.
		*/
		void updateTerrain(size_t startx, size_t startz, size_t endx, size_t endz);
		// 附加Tile到相关节点
		void attachTile(SceneNode* parent);



		// 获得保存在系统内存中的顶点缓冲区
		Vector3 *getVertexSysBuffer()
		{
			return mVertexSysBuffer;
		}

		// 获得保存在系统内存中的法线缓冲区
		Vector3 *getNormalSysBuffer()
		{
			return mNormalSysBuffer;
		}

		// 获得保存在系统内存中的索引
		ushort* getIndexSysBuffer();
		// 获得保存在系统内存中的索引
		size_t getIndexSysBufferCount();

		// 测试ray和地形tile之间的碰撞，返回true则有碰撞，posIntersection中是碰撞交点
		bool collide(const Ray& ray , Vector3 &posIntersection);

		// 测试ray和地形tile之间的碰撞，返回true则有碰撞，distance中是碰撞交点离射线起点的距离
		bool collide(const Ray& ray , float &distance);
		// 创建碰撞模型
		void createCollisionShape();

		// 获得Lod
		ushort getLod()const
		{
			return mLOD;
		}


		// 获得顶点缓冲区
		VertexData *_getVertexData()const
		{
			return mVertexData;
		}
	private:
		/** Initialises the vertices */
		void createVertexData(size_t startx, size_t startz);

		// 重新计算所有法线
		void _calculateNormals(size_t startx, size_t startz, size_t endx, size_t endz);

		// 销毁碰撞模型
		void destroyCollisionShape();

		// 渲染缝合边缘
		void renderSutureData(RenderQueue* queue , Tile *tileNeighbour , IndexHandler::Edge edge);

		/** Retrieves the position vector for the given vertex */
		Vector3 getVector(size_t x, size_t z) const;

		/** Convenience function to empty a delta buffer */
		void emptyBuffer(HardwareVertexBufferSharedPtr buf);

		TerrainImpl* mTerrainImpl;
		IndexHandler* mIndexHandler;

		ushort mLOD;
		TerrainInfo* mInfo;
		const Options &mOpt;
		SutureRenderable *m_sutureRenderable;

		/** Info about this tile in regards to all tiles */
		size_t mTileX, mTileZ;
		size_t mStartX, mStartZ;

		AxisAlignedBox mBounds;
		Real mBoundingRadius;
		Vector3 mCenter;

		mutable bool mLightListDirty;
		mutable LightList mLightList;

		/** The tile's vertices */
		VertexData* mVertexData;
		HardwareVertexBufferSharedPtr mMainBuffer;

		// 保存在系统内存中的顶点缓冲区
		Vector3 *mVertexSysBuffer;
		// 保存在系统内存中的法线缓冲区
		Vector3 *mNormalSysBuffer;

		// 碰撞模型
		ICollisionShapePtr m_collisionShape;
	};
}

#endif
