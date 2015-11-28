#include "StdAfx.h"
#include "ParticleObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "ParticleObject.h"

namespace Ogre
{

	ParticleObjectVisible::ParticleObjectVisible(ParticleObject *obj)
		: m_object(obj)
		, m_showBorder(false)
		, m_showEntity(false)
		, m_showPlacingEntity(false)
		, m_scene(0)
	{
		m_scene = m_object->getScene();
	}

	ParticleObjectVisible::~ParticleObjectVisible(void)
	{
	}

	// 显示边框
	void ParticleObjectVisible::showBorder(bool show)
	{
		m_showBorder = show;
		m_object->getOgreSceneNode()->showBoundingBox(show);
	}

	// 是否显示边框
	bool ParticleObjectVisible::isShowBorder() const
	{
		return m_showBorder;
	}

	// 显示实体
	void ParticleObjectVisible::showEntity(bool show)
	{
		m_showEntity = show;
		if(m_object->getOgreParticleSystem())
		{
			m_object->getOgreParticleSystem()->setVisible(show);
		}
	}
	// 是否显示实体
	bool ParticleObjectVisible::isShowEntity() const
	{
		return m_showEntity;
	}

	// 获得场景对象
	ISceneObject *ParticleObjectVisible::getSceneObject()const
	{
		return m_object;
	}

	// 显示正在摆放的模型
	void ParticleObjectVisible::showPlacingEntity(bool show)
	{
		m_showPlacingEntity = show;
		if(m_object->getOgreParticleSystem())
		{
			m_object->getOgreParticleSystem()->setVisible(show);
		}
	}
	// 是否显示正在摆放的模型
	bool ParticleObjectVisible::isShowPlacingEntity() const
	{
		return m_showPlacingEntity;
	}

	// 获得可见对象的碰撞实体
	ICollisionEntity *ParticleObjectVisible::getCollisionEntity()const
	{
		return 0;
	}

};