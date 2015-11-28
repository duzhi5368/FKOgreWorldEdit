#include "StdAfx.h"
#include "EntityObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "EntityObject.h"

namespace Ogre
{

	EntityObjectVisible::EntityObjectVisible(EntityObject *obj)
		: m_object(obj)
		, m_showBorder(false)
		, m_showEntity(false)
		, m_showPlacingEntity(false)
		, m_scene(0)
		, m_alphaController(0)
		, m_collisionEntity(0)
		, m_entityBorder(0)
	{
		m_scene = m_object->getScene();
		delayInit();
	}

	EntityObjectVisible::~EntityObjectVisible(void)
	{
		OGRE_DELETE(m_alphaController);
		if(m_collisionEntity)
		{
			m_scene->getCollisionWorld()->destroyCollisionEntity(m_collisionEntity);
			m_collisionEntity = 0;
		}
		if(m_entityBorder)
		{
			m_scene->getOgreSceneManager()->destroyEntity(m_entityBorder);
			m_entityBorder = 0;
		}
	}
	void EntityObjectVisible::delayInit()
	{
		if(m_alphaController)
			return;
		if(m_object->getOgreEntity() == 0)
			return;
		m_alphaController = new EntityMaterialInstance(m_object->getOgreEntity());
		m_collisionEntity = m_scene->getCollisionWorld()->createEntityCollisionEntity(m_object->getOgreEntity() , CT_SELF_MESH);

		m_entityBorder = m_object->getOgreEntity()->clone(m_object->getOgreEntity()->getName() + "_Border");
		m_object->getOgreSceneNode()->attachObject(m_entityBorder);
		m_entityBorder->setVisible(false);
		m_entityBorder->setMaterialName("BaseWhiteNoLightingWireFrameMode");

	}

	// 显示边框
	void EntityObjectVisible::showBorder(bool show)
	{
		delayInit();
		m_showBorder = show;
		if(m_entityBorder)
		{
			m_entityBorder->setVisible(show);
		}
	}
	// 是否显示边框
	bool EntityObjectVisible::isShowBorder() const
	{
		return m_showBorder;
	}

	// 显示实体
	void EntityObjectVisible::showEntity(bool show)
	{
		delayInit();
		m_showEntity = show;
		if(m_alphaController)
		{
			m_object->getOgreEntity()->setVisible(show);
			m_alphaController->setTransparency(1.0f);
		}
	}
	// 是否显示实体
	bool EntityObjectVisible::isShowEntity() const
	{
		return m_showEntity;
	}

	// 获得场景对象
	ISceneObject *EntityObjectVisible::getSceneObject()const
	{
		return m_object;
	}


	// 显示正在摆放的模型
	void EntityObjectVisible::showPlacingEntity(bool show)
	{
		delayInit();
		m_showPlacingEntity = show;
		if(m_alphaController)
		{
			if(show)
			{
				m_object->getOgreEntity()->setVisible(true);
				m_alphaController->setTransparency(0.5f);
			}
			else
			{
				showEntity(isShowEntity());
			}
		}
	}
	// 是否显示正在摆放的模型
	bool EntityObjectVisible::isShowPlacingEntity() const
	{
		return m_showPlacingEntity;
	}

	// 获得可见对象的碰撞实体
	ICollisionEntity *EntityObjectVisible::getCollisionEntity()const
	{
		return m_collisionEntity;
	}
};