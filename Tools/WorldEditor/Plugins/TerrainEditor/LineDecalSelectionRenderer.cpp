#include "StdAfx.h"
#include "LineDecalSelectionRenderer.h"

namespace Ogre
{

#define MAX_VERTEX_COUNT 4096
	static const int VERTEX_PREQUAD = 6;
	static const int VERTEX_BINDING = 0;
	//-----------------------------------------------------------------------
	LineDecalSelectionRenderer::LineDecalSelectionRenderer(Selection *pSelection , TerrainBrush *brush)
			: SelectionRenderer(pSelection , brush)
	{
		mRenderOp.vertexData = new VertexData();
		mRenderOp.indexData = new IndexData();

		setMaterial("LineDecal");

		// Initialization stuff
		mRenderOp.vertexData->vertexCount = MAX_VERTEX_COUNT;
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.indexData->indexCount = mRenderOp.vertexData->vertexCount * VERTEX_PREQUAD;
		mRenderOp.indexData->indexStart = 0;
		mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST; // OT_LINE_LIST, OT_LINE_STRIP
		mRenderOp.useIndexes = true;

		VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

		size_t offset = 0;
		offset += decl->addElement (VERTEX_BINDING, offset, VET_FLOAT3, VES_POSITION).getSize();
		offset += decl->addElement (VERTEX_BINDING, offset, VET_COLOUR, VES_DIFFUSE).getSize();

		HardwareVertexBufferSharedPtr vbuf =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(VERTEX_BINDING),
			mRenderOp.vertexData->vertexCount,
			//HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			HardwareBuffer::HBU_DYNAMIC);

		// add by 王宏张 2007-8-8
		// 创建索引缓冲区
		mRenderOp.indexData->indexBuffer =
			HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT ,
			mRenderOp.indexData->indexCount,
			HardwareBuffer::HBU_DYNAMIC);


		bind->setBinding(VERTEX_BINDING, vbuf);

		setRenderQueueGroup(RENDER_QUEUE_3);

		m_selection->getTerrain()->getTerrainImpl()->_getManager()->getRootSceneNode()->attachObject(this);

	}

	//-----------------------------------------------------------------------
	LineDecalSelectionRenderer::~LineDecalSelectionRenderer()
	{
		m_selection->getTerrain()->getTerrainImpl()->_getManager()->getRootSceneNode()->detachObject(this);

		delete mRenderOp.vertexData;
		delete mRenderOp.indexData;
	}


	//-----------------------------------------------------------------------
	Real LineDecalSelectionRenderer::getSquaredViewDepth(const Camera *cam) const
	{
		const Vector3 vMin = mBox.getMinimum();
		const Vector3 vMax = mBox.getMaximum();
		const Vector3 vMid = ((vMin - vMax) * 0.5) + vMin;
		const Vector3 vDist = cam->getDerivedPosition() - vMid;

		return vDist.squaredLength();
	}

	//-----------------------------------------------------------------------
	Real LineDecalSelectionRenderer::getBoundingRadius(void) const
	{
		return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}

	//-----------------------------------------------------------------------
	const Quaternion &LineDecalSelectionRenderer::getWorldOrientation(void) const
	{
		return Quaternion::IDENTITY;
	}

	//-----------------------------------------------------------------------
	const Vector3 &LineDecalSelectionRenderer::getWorldPosition(void) const
	{
		return Vector3::ZERO;
	}


	struct Vertex
	{
		Vector3 pos;
		uint32 colour;
	};
	
	void LineDecalSelectionRenderer::updateVertexBuffer()
	{
		mGridPos.clear();
		// 锁定顶点缓冲区
		HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(VERTEX_BINDING);             
		Vertex *pPos = static_cast<Vertex*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

		Vector3 vaabMin;
		Vector3 vaabMax;

		mRenderOp.vertexData->vertexCount = 0;

		RenderSystem *rs = Root::getSingleton().getRenderSystem();
		// 循环整个选区
		for(Selection::const_iterator iter = m_selection->begin() ; iter != m_selection->end() ; iter ++)
		{
			// 越界的不要
			if(size_t(iter->first.getX() + m_selection->getPosition().getX()) >= m_selection->getTerrain()->getTerrainInfo()->getWidth()
				|| size_t(iter->first.getY() + m_selection->getPosition().getY()) >= m_selection->getTerrain()->getTerrainInfo()->getHeight())
				continue;
			// 保存顶点数据
			pPos->pos = m_selection->getTerrain()->getTerrainPos(iter->first.getX() + m_selection->getPosition().getX(), iter->first.getY() + m_selection->getPosition().getY());
			// 保存颜色数据
			ColourValue color(1 , 0 , 0);
			color.a *= iter->second;
			rs->convertColourValue(color , &pPos->colour);
			// 包围盒初始值
			if(mGridPos.empty())
			{
				vaabMin = pPos->pos;
				vaabMax = pPos->pos;
			}
			mGridPos[iter->first] = (ushort)mRenderOp.vertexData->vertexCount;

			// 计算包围盒
			if(pPos->pos.x < vaabMin.x)
				vaabMin.x = pPos->pos.x;
			if(pPos->pos.y < vaabMin.y)
				vaabMin.y = pPos->pos.y;
			if(pPos->pos.z < vaabMin.z)
				vaabMin.z = pPos->pos.z;

			if(pPos->pos.x > vaabMax.x)
				vaabMax.x = pPos->pos.x;
			if(pPos->pos.y > vaabMax.y)
				vaabMax.y = pPos->pos.y;
			if(pPos->pos.z > vaabMax.z)
				vaabMax.z = pPos->pos.z;

			pPos++;
			mRenderOp.vertexData->vertexCount ++;
			// 不允许超过顶点缓冲区数量
			if(mRenderOp.vertexData->vertexCount >= MAX_VERTEX_COUNT)
				break;
		}
		
		// 解锁缓冲区
		vbuf->unlock();
		// 更新包围盒
		mBox.setExtents(vaabMin, vaabMax);

	}
	void LineDecalSelectionRenderer::updateIndexBuffer()
	{
		// 锁定索引缓冲区
		ushort* pIdx = static_cast<ushort*>(mRenderOp.indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD) );

		ushort iIndexBottomLeft = 0 , iIndexBottomRight = 0 , iIndexTopLeft = 0 , iIndexTopRight = 0;
		mRenderOp.indexData->indexCount = 0;
		MapGridVector2::iterator itPos;
		for(Selection::const_iterator iter = m_selection->begin() ; iter != m_selection->end() ; iter ++)
		{
			// 越界的不要
			if(size_t(iter->first.getX() + m_selection->getPosition().getX()) >= m_selection->getTerrain()->getTerrainInfo()->getWidth()
				|| size_t(iter->first.getY() + m_selection->getPosition().getY()) >= m_selection->getTerrain()->getTerrainInfo()->getHeight())
				continue;
			
			// 寻找左上点
			itPos = mGridPos.find(iter->first + GridVector2(0 , 1));
			if(itPos != mGridPos.end())
			{
				iIndexTopLeft = itPos->second;
				
				// 寻找右下点
				itPos = mGridPos.find(iter->first + GridVector2(1 , 0));
				if(itPos != mGridPos.end())
				{
					iIndexBottomRight = itPos->second;
					// 寻找右上点
					itPos = mGridPos.find(iter->first + GridVector2(1 , 1));
					if(itPos != mGridPos.end())
					{
						iIndexTopRight = itPos->second;

						// 设置顶点索引值
						// 画两个三角形，画三角形的顺序必须要跟地形画三角形顺序一致，否则会出现地形穿插的现象
						// 地形画三角形的顺序是逆时针先画左下的三角形，再画右上的三角形
						*pIdx++ = iIndexBottomLeft;
						*pIdx++ = iIndexTopLeft;
						*pIdx++ = iIndexBottomRight;

						*pIdx++ = iIndexTopLeft;
						*pIdx++ = iIndexTopRight;
						*pIdx++ = iIndexBottomRight;
						mRenderOp.indexData->indexCount += 6;
						// 不允许超过顶点缓冲区数量
						if(mRenderOp.indexData->indexCount >= MAX_VERTEX_COUNT * VERTEX_PREQUAD)
							break;
					}
				}

			}
			iIndexBottomLeft++;
		}
		mRenderOp.indexData->indexBuffer->unlock();
	}
};