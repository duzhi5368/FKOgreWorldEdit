/******************************************************************
** 文件名:	DecalSystem.cpp
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:29
** 描  述:
******************************************************************/
#include "stdafx.h"
#include "GridDecal.h"


namespace Ogre
{
	static const Vector3 Vector3Max(FLT_MAX , FLT_MAX , FLT_MAX);

	GridDecal::GridDecal(SceneManager *mgr , const String &name)
		: SimpleDecal(mgr , name)
		, m_decal(0)
		, m_widthGridCount(8)
		, m_heightGridCount(8)
		, m_gridPositions(0)
	{
	}

	GridDecal::~GridDecal()
	{
		OGRE_DELETE_ARRAY(m_gridPositions);
		if(m_decal)
		{
			m_sceneMgr->destroyManualObject(m_decal);
			m_decal = 0;
		}
	}
	// 当属性改变了，需要重构贴花的时候
	void GridDecal::updateDecal()
	{
#define WIDTH_POS_COUNT (m_widthGridCount + 1)
#define HEIGHT_POS_COUNT (m_heightGridCount + 1)
		// 显示整个节点
		m_node->setVisible(m_visible);
		// 不可视就不更新了
		if(!m_visible)
		{
			return;
		}
		// 错误数据检查
		if(m_heightGridCount == 0 || m_widthGridCount == 0)
			return;
		// 如果网格大小改变了，就重构
		if((m_dirty & (GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
			| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT))
			|| m_gridPositions == 0
			)
		{
			OGRE_DELETE_ARRAY(m_gridPositions);
			m_gridPositions = new Vector3[WIDTH_POS_COUNT * HEIGHT_POS_COUNT];
			memset(m_gridPositions , 0 , sizeof(Vector3) * WIDTH_POS_COUNT * HEIGHT_POS_COUNT);
		}

		// 位置、深度、大小会导致平面重新定义
		if(m_dirty & (DIRTY_POSITION
			| DIRTY_DEPTH
			| DIRTY_SIZE
			| GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
			| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT)
			)
		{
			for(uint y = 0 ; y < WIDTH_POS_COUNT ; y ++)
			{
				for(uint x = 0 ; x < HEIGHT_POS_COUNT ; x ++)
				{
					// 找到与地形相交的点
					if(!IDecalSystem::getSingleton().worldRayQuery(m_gridPositions[y * WIDTH_POS_COUNT + x]
						, m_position + Vector3(m_size.x * ((float)x / (float)WIDTH_POS_COUNT) - m_size.x / 2.0f , m_depth , m_size.y * ((float)y / (float)HEIGHT_POS_COUNT) - m_size.y / 2.0f)
						, m_position + Vector3(m_size.x * ((float)x / (float)WIDTH_POS_COUNT) - m_size.x / 2.0f , -m_depth , m_size.y * ((float)y / (float)HEIGHT_POS_COUNT) - m_size.y / 2.0f)
						)
						)
					{
						return;
					}
					// 移动到相对位置
					m_gridPositions[y * WIDTH_POS_COUNT + x] -= m_position;
				}
			}
			// 从三点定义贴花平面
			m_decalPlane.redefine(m_gridPositions[WIDTH_POS_COUNT / 2]
								, m_gridPositions[WIDTH_POS_COUNT * (HEIGHT_POS_COUNT - 1)]
								, m_gridPositions[WIDTH_POS_COUNT * HEIGHT_POS_COUNT - 1]
								);
		}

		// 改变节点位置
		if(m_dirty & DIRTY_POSITION)
		{
			m_node->setPosition(m_position);
		}

		// 材质名和大小会影响贴花对象的重构
		if(m_dirty & (DIRTY_POSITION
			| DIRTY_DEPTH
			| DIRTY_MATERIAL_NAME
			| DIRTY_SIZE
			| GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
			| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT)
			)
		{
			// 材质贴花
			if(!m_materialName.empty())
			{
				// 创建贴花对象
				if(m_decal == 0)
				{
					m_decal = m_sceneMgr->createManualObject(m_name + "_GridDecal");
					m_decal->setDynamic(true);
					m_decal->begin(m_materialName);
					m_decal->position(Vector3::ZERO);
					m_decal->textureCoord(0,0);
					m_decal->index(0);
					m_decal->end();
					// 贴花在地形之后，物件之前
					m_decal->setRenderQueueGroup(RENDER_QUEUE_DECAL);
					attachObject(m_decal);
				}

				// 设置材质
				if(m_dirty & DIRTY_MATERIAL_NAME)
				{
					m_decal->getSection(0)->setMaterialName(m_materialName);
				}
				// 更新顶点
				if(m_dirty & (DIRTY_POSITION
					| DIRTY_DEPTH
					| DIRTY_SIZE
					| GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
					| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT)
					)
				{
					m_decal->beginUpdate(0);
					// 生成所有顶点
					for(uint y = 0 ; y < HEIGHT_POS_COUNT ; y ++)
					{
						for(uint x = 0 ; x < WIDTH_POS_COUNT ; x ++)
						{
							m_decal->position(m_gridPositions[y * WIDTH_POS_COUNT + x]);
							m_decal->textureCoord((float)x / (float)WIDTH_POS_COUNT
								, (float)y / (float)HEIGHT_POS_COUNT);
						}
					}
					// 根据顶点生成索引缓冲区
					for(uint y = 0 ; y < m_heightGridCount ; y ++)
					{
						for(uint x = 0 ; x < m_widthGridCount ; x ++)
						{
							uint tl = y * WIDTH_POS_COUNT + x;
							uint tr = y * WIDTH_POS_COUNT + x + 1;
							uint bl = (y + 1) * WIDTH_POS_COUNT + x;
							uint br = (y + 1) * WIDTH_POS_COUNT + x + 1;
							m_decal->quad(tl
								, bl
								, br
								, tr
								);
						}
					}
					
					m_decal->end();
				}
			}
			else
			{
				if(m_decal)
				{
					m_sceneMgr->destroyManualObject(m_decal);
					m_decal = 0;
				}
			}
		}

		// 改变附加对象的节点旋转
		if(m_nodeAttachObject)
		{
			// 位置信息改变才需要重新设置
			if(m_dirty & (DIRTY_POSITION
				| DIRTY_DEPTH
				| DIRTY_SIZE
				| DIRTY_OFFSET)
				)
			{
				m_nodeAttachObject->setPosition(m_offsetPosition);
			}
		}
	}
	// 设置横向网格数量
	void GridDecal::setWidthGridCount(uint gridCount)
	{
		if(m_widthGridCount != gridCount)
		{
			m_dirty |= GRID_DECAL_DIRTY_WIDTH_GRID_COUNT;
		}
		m_widthGridCount = gridCount;
	}
	// 设置竖向网格数量
	void GridDecal::setHeightGridCount(uint gridCount)
	{
		if(m_heightGridCount != gridCount)
		{
			m_dirty |= GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT;
		}
		m_heightGridCount = gridCount;
	}


};