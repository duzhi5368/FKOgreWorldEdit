/******************************************************************
** 文件名:	DecalSystem.cpp
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:29
** 描  述:
******************************************************************/
#include "stdafx.h"
#include "NormalDecal.h"


namespace Ogre
{
	static const Vector3 Vector3Max(FLT_MAX , FLT_MAX , FLT_MAX);

	NormalDecal::NormalDecal(SceneManager *mgr , const String &name)
		: SimpleDecal(mgr , name)
		, m_decal(0)
	{
	}
	
	NormalDecal::~NormalDecal()
	{
		if(m_decal)
		{
			m_sceneMgr->destroyManualObject(m_decal);
			m_decal = 0;
		}
	}

	// 当属性改变了，需要重构贴花的时候
	void NormalDecal::updateDecal()
	{
		// 显示整个节点
		m_node->setVisible(m_visible);
		// 不可视就不更新了
		if(!m_visible)
		{
			return;
		}

		// 位置、深度、大小会导致平面重新定义
		if((m_dirty & DIRTY_POSITION)
			|| (m_dirty & DIRTY_DEPTH)
			|| (m_dirty & DIRTY_SIZE)
			)
		{
			Vector3 centerTop(m_position + Vector3(0 , m_depth , 0));
			Vector3 bottomTop(m_position + Vector3(0 , -m_depth , m_size.y));
			Vector3 bottomLeft(m_position + Vector3(-m_size.x , -m_depth , -m_size.y));
			Vector3 bottomRight(m_position + Vector3(m_size.x , -m_depth , -m_size.y));

			// 用射线检测计算三个点，这三个点用来算法线
			Vector3 pos1(Vector3Max) , pos2(Vector3Max) , pos3(Vector3Max);

			// 如果其中一个点找不到，就不更新了
			if(!IDecalSystem::getSingleton().worldRayQuery(pos1 , centerTop , bottomTop))return;
			if(!IDecalSystem::getSingleton().worldRayQuery(pos2 , centerTop , bottomLeft))return;
			if(!IDecalSystem::getSingleton().worldRayQuery(pos3 , centerTop , bottomRight))return;

			// 从三点定义贴花平面
			m_decalPlane.redefine(pos1 , pos2 , pos3);
		}
		
		// 改变节点位置
		if(m_dirty & DIRTY_POSITION)
		{
			m_node->setPosition(m_position);
		}

		// 材质名和大小会影响贴花对象的重构
		if((m_dirty & DIRTY_MATERIAL_NAME)
			|| (m_dirty & DIRTY_SIZE)
			)
		{
			// 材质贴花
			if(!m_materialName.empty())
			{
				// 创建贴花对象
				if(m_decal == 0)
				{
					m_decal = m_sceneMgr->createManualObject(m_name + "_NormalDecal");
					m_decal->setDynamic(true);
					m_decal->begin(m_materialName);
					m_decal->position(Vector3::ZERO);
					m_decal->textureCoord(0,0);
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
				if(m_dirty & DIRTY_SIZE)
				{
					m_decal->beginUpdate(0);
					m_decal->position(-m_size.x , 0 , m_size.y);
					m_decal->textureCoord(0 , 0);
					m_decal->position(m_size.x , 0 , -m_size.y);
					m_decal->textureCoord(1 , 1);
					m_decal->position(-m_size.x , 0 , -m_size.y);
					m_decal->textureCoord(0 , 1);

					m_decal->position(-m_size.x , 0 , m_size.y);
					m_decal->textureCoord(0 , 0);
					m_decal->position(m_size.x , 0 , m_size.y);
					m_decal->textureCoord(1 , 0);
					m_decal->position(m_size.x , 0 , -m_size.y);
					m_decal->textureCoord(1 , 1);
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
			if((m_dirty & DIRTY_POSITION)
				|| (m_dirty & DIRTY_DEPTH)
				|| (m_dirty & DIRTY_SIZE)
				|| (m_dirty & DIRTY_OFFSET)
				)
			{
				m_nodeAttachObject->setOrientation(Vector3::UNIT_Y.getRotationTo(-m_decalPlane.normal) * m_offsetOrientation);
				m_nodeAttachObject->setPosition(m_offsetPosition);
				m_nodeAttachObject->setScale(m_offsetScale);
			}
		}
	}


};