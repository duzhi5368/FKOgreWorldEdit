/******************************************************************
** 文件名:	DecalSystem.cpp
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:29
** 描  述:
******************************************************************/
#include "stdafx.h"
#include "MeshDecal.h"
#include "MeshDecalClipper.h"
#include "ETTerrain.h"
#include "ETTerrainImpl.h"
#include "ETTile.h"
namespace Ogre
{
	MeshDecal::MeshDecal(SceneManager *mgr , const String &name)
		: SimpleDecal(mgr , name)
		, m_decal(0)
	{
	}

	MeshDecal::~MeshDecal()
	{
		OGRE_DELETE(m_decal);
	}

	// 当属性改变了，需要重构贴花的时候
	void MeshDecal::updateDecal()
	{
		// 显示整个节点
		m_node->setVisible(m_visible);
		// 不可视就不更新了
		if(!m_visible)
		{
			return;
		}
		// 创建贴花对象
		if(m_decal == 0)
		{
			m_decal = new DecalObject;
			// 贴花在地形之后，物件之前
			m_decal->setRenderQueueGroup(RENDER_QUEUE_DECAL);
			m_sceneMgr->getRootSceneNode()->attachObject(m_decal);
		}

		if(m_dirty & (DIRTY_POSITION
			| DIRTY_SIZE
			| DIRTY_DEPTH
			| DIRTY_MATERIAL_NAME
			))
		{
			// 建立贴花剪裁器
			MeshDecalClipper clipper(m_position
				, Vector3::UNIT_Y
				, Vector3::UNIT_X
				, m_size.x
				, m_size.y
				, m_depth);

			// 开始剪裁
			clipper.begin();

			Vector3 posIntersect;
			ETTerrain *terrain = IDecalSystem::getSingleton().worldRayQuery(posIntersect , Ray(m_position + Vector3(0 , 10000 , 0) , Vector3::NEGATIVE_UNIT_Y));

			if(terrain)
			{
				TileGrid &grids = terrain->getTerrainImpl()->getTileGrid();
				for (size_t i = 0; i < grids.size(); ++i)
				{
					for (size_t j = 0; j < grids[i].size(); ++j)
					{
						Tile *tile = grids[i][j];
						clipper.clipMesh(tile->getIndexSysBufferCount() , tile->getIndexSysBuffer() , tile->getVertexSysBuffer() , tile->getNormalSysBuffer());
					}
				}
			}
			
			//TerrainRenderable *renderable;
			//TerrainSceneManager *sceneMgr = ((TerrainSceneManager*)m_sceneMgr);

			//const float tileCount = 2.0f;
			//const float startTile = - (CHUNK_LENGTH * tileCount) / 2.0f;
			//const float endTile = -startTile;

			//if(m_intersectWMOCellID == -1)
			//{
			//	//// 找到附近的地形块
			//	for(float y = startTile ; y <= endTile ; y += CHUNK_LENGTH)
			//	{
			//		for(float x = startTile ; x <= endTile ; x += CHUNK_LENGTH)
			//		{
			//			renderable = sceneMgr->getTerrainTile(m_position + Vector3(x , 0 , y));
			//			// 只有当地形块与贴花相交才需要剪裁到贴花
			//			if(renderable && clipper.intersects(renderable->getWorldBoundingBox()))
			//			{
			//				// 获得缓冲区
			//				Vector3 *pNormals = renderable->getNormalBuffer();
			//				Vector3 *pVertices = renderable->getPositionBuffer();
			//				ushort *pIndex = renderable->getIndicesBuffer();
			//				// 剪裁网格到贴花
			//				clipper.clipMesh(renderable->getIndexCount()
			//					, pIndex
			//					, pVertices
			//					, pNormals
			//					);
			//			}
			//		}
			//	}
			//}
			//else
			//{
			//	std::vector<WMOMeshDecalDataChunk> meshDataList;
			//	IInDoorSceneManager::getSingleton().getMeshDataByBoundBox(clipper.getWorldBoundingBox()
			//		, m_intersectWMOCellID
			//		, m_intersectWMOCellName
			//		, meshDataList);
			//	for(int i = 0 ; i < meshDataList.size() ; i ++)
			//	{
			//		WMOMeshDecalDataChunk &chunk = meshDataList[i];
			//		for(int j = 0 ; j < chunk.indicesList.size() ; j ++)
			//		{
			//			clipper.clipMesh(chunk.triangleCountList[j] * 3
			//				, chunk.indicesList[j]
			//				, chunk.VertexData
			//					, chunk.TriNormalList
			//					);
			//		}
			//	}
			//}
			//
			//
			// 结束剪裁
			clipper.end();
			// 填充数据
			m_decal->fillData(&clipper);
			m_decal->setMaterial(m_materialName);
		}
	}

#define VERTEX_BINDING 0

	DecalObject::DecalObject()
		: m_radius(0.0f)
	{
		mRenderOp.vertexData = new VertexData();
		mRenderOp.indexData = new IndexData();

		mRenderOp.vertexData->vertexCount = MaxDecalVertices; 
		mRenderOp.vertexData->vertexStart = 0; 

		mRenderOp.indexData->indexCount = MaxDecalVertices * 3; 
		mRenderOp.indexData->indexStart = 0; 

		mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST; 
		mRenderOp.useIndexes = true; 

		VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

		size_t offset = 0;
		decl->addElement(VERTEX_BINDING, offset, VET_FLOAT3, VES_POSITION);
		offset += VertexElement::getTypeSize(VET_FLOAT3);
		decl->addElement(VERTEX_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES , 0);
		offset += VertexElement::getTypeSize(VET_FLOAT2);
		decl->addElement(VERTEX_BINDING, offset, VET_COLOUR, VES_DIFFUSE);
		offset += VertexElement::getTypeSize(VET_COLOUR);


		// 创建顶点缓冲区
		HardwareVertexBufferSharedPtr vbuf = 
			HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(VERTEX_BINDING)
			, mRenderOp.vertexData->vertexCount
			, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// Bind buffer
		bind->setBinding(VERTEX_BINDING, vbuf);

		// 创建索引缓冲区
		mRenderOp.indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT
			, mRenderOp.indexData->indexCount
			, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	}
	DecalObject::~DecalObject()
	{
		delete mRenderOp.vertexData;
		delete mRenderOp.indexData;
	}

	// 填充Decal数据
	void DecalObject::fillData(MeshDecalClipper *clipper)
	{
		HardwareVertexBufferSharedPtr vbuf =
			mRenderOp.vertexData->vertexBufferBinding->getBuffer(VERTEX_BINDING);     


		// 锁定顶点缓冲区
		MeshDecalClipper::Vertex* vertex = static_cast<MeshDecalClipper::Vertex*>(
			vbuf->lock(HardwareBuffer::HBL_DISCARD));
		size_t count = clipper->getDecalVertexCount();
		RenderSystem* rs = Root::getSingleton().getRenderSystem();
		const MeshDecalClipper::Vertex* vertexArray = clipper->getVertexArray();
		// 写入顶点数据
		memcpy(vertex , vertexArray , sizeof(MeshDecalClipper::Vertex) * count);
		vbuf->unlock();
		// 重新计算包围盒
		mBox.setNull();
		for(size_t i = 0 ; i < count ; i ++)
		{
			mBox.merge(vertexArray[i].pos);
		}

		m_radius = (mBox.getMaximum() - mBox.getMinimum()).length() / 2.0f;

		// 顶点数量
		mRenderOp.vertexData->vertexCount = count;

		// 锁定索引缓冲区
		ushort* indices = static_cast<ushort*>(
			mRenderOp.indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD));
		memcpy(indices , clipper->getIndexArray() , clipper->getDecalIndexCount() * sizeof(ushort));
		mRenderOp.indexData->indexBuffer->unlock();
		// 索引数量
		mRenderOp.indexData->indexCount = clipper->getDecalIndexCount();
	}

	// Override this method to prevent parent transforms (rotation,translation,scale)
	void DecalObject::getWorldTransforms( Matrix4* xform ) const
	{
		// return identity matrix to prevent parent transforms
		*xform = Matrix4::IDENTITY;
	}
	//-----------------------------------------------------------------------
	const Quaternion& DecalObject::getWorldOrientation(void) const
	{
		return Quaternion::IDENTITY;
	}
	//-----------------------------------------------------------------------
	const Vector3& DecalObject::getWorldPosition(void) const
	{
		return Vector3::ZERO;
	}
	//-----------------------------------------------------------------------
	Real DecalObject::getSquaredViewDepth(const Camera* cam) const
	{
		Vector3 min, max, mid, dist;
		min = mBox.getMinimum();
		max = mBox.getMaximum();
		mid = ((max - min) * 0.5) + min;
		dist = cam->getDerivedPosition() - mid;
		return dist.squaredLength();
	}

};