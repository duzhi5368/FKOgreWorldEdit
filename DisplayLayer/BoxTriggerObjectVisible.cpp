#include "StdAfx.h"
#include "BoxTriggerObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "BoxTriggerObject.h"
#include "IBoxTriggerEntity.h"

namespace Ogre
{

	BoxTriggerObjectVisible::BoxTriggerObjectVisible(BoxTriggerObject *obj)
		: m_object(obj)
		, m_showBorder(false)
		, m_showEntity(false)
		, m_showPlacingEntity(false)
		, m_scene(0)
		, m_entity(0)
	{
		m_scene = m_object->getScene();
		createEntity();
	}

	BoxTriggerObjectVisible::~BoxTriggerObjectVisible(void)
	{
		destroyEntity();
	}
	// 创建模型实体
	void BoxTriggerObjectVisible::createEntity()
	{
		destroyEntity();

		m_entity = m_scene->getOgreSceneManager()->createManualObject(m_object->getFullName() + "_VisibleObject");
		m_entity->begin(DisplaySystem::getSingleton().getColorMaterial(ColourValue(1 , 1 , 0 , 1)) , RenderOperation::OT_LINE_LIST);
		const AxisAlignedBox &box = static_cast<IBoxTriggerEntity*>(m_object->getTriggerEntity())->getBox();
		Vector3 vmax = box.getMaximum();
		Vector3 vmin = box.getMinimum();

		Real maxx = vmax.x;
		Real maxy = vmax.y;
		Real maxz = vmax.z;

		Real minx = vmin.x;
		Real miny = vmin.y;
		Real minz = vmin.z;
		// line 0
		m_entity->position(minx , miny , minz);
		m_entity->position(maxx , miny , minz);
		// line 1
		m_entity->position(minx , miny , minz);
		m_entity->position(minx , miny , maxz);
		// line 2
		m_entity->position(minx , miny , minz);
		m_entity->position(minx , maxy , minz);
		// line 3
		m_entity->position(minx , maxy , minz);
		m_entity->position(minx , maxy , maxz);
		// line 4
		m_entity->position(minx , maxy , minz);
		m_entity->position(maxx , maxy , minz);
		// line 5
		m_entity->position(maxx , miny , minz);
		m_entity->position(maxx , miny , maxz);
		// line 6
		m_entity->position(maxx , miny , minz);
		m_entity->position(maxx , maxy , minz);
		// line 7
		m_entity->position(minx , maxy , maxz);
		m_entity->position(maxx , maxy , maxz);
		// line 8
		m_entity->position(minx , maxy , maxz);
		m_entity->position(minx , miny , maxz);
		// line 9
		m_entity->position(maxx , maxy , minz);
		m_entity->position(maxx , maxy , maxz);
		// line 10
		m_entity->position(maxx , miny , maxz);
		m_entity->position(maxx , maxy , maxz);
		// line 11
		m_entity->position(minx , miny , maxz);
		m_entity->position(maxx , miny , maxz);
		m_entity->end();
		m_object->getOgreSceneNode()->attachObject(m_entity);
		m_entity->setVisible(false);
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(m_object)));
		showEntity(isShowEntity());
	}
	// 销毁模型实体
	void BoxTriggerObjectVisible::destroyEntity()
	{
		if(m_entity)
		{
			m_scene->getOgreSceneManager()->destroyManualObject(m_entity);
			m_entity = 0;
		};
	}
	// 显示边框
	void BoxTriggerObjectVisible::showBorder(bool show)
	{
		m_showBorder = show;
		m_object->getOgreSceneNode()->showBoundingBox(show);
	}
	// 是否显示边框
	bool BoxTriggerObjectVisible::isShowBorder() const
	{
		return m_showBorder;
	}

	// 显示实体
	void BoxTriggerObjectVisible::showEntity(bool show)
	{
		m_showEntity = show;
		m_entity->setVisible(show);
	}
	// 是否显示实体
	bool BoxTriggerObjectVisible::isShowEntity() const
	{
		return m_showEntity;
	}

	// 获得场景对象
	ISceneObject *BoxTriggerObjectVisible::getSceneObject()const
	{
		return m_object;
	}


	// 显示正在摆放的模型
	void BoxTriggerObjectVisible::showPlacingEntity(bool show)
	{
		m_showPlacingEntity = show;
		m_entity->setVisible(show);
	}
	// 是否显示正在摆放的模型
	bool BoxTriggerObjectVisible::isShowPlacingEntity() const
	{
		return m_showPlacingEntity;
	}

	// 获得可见对象的碰撞实体
	ICollisionEntity *BoxTriggerObjectVisible::getCollisionEntity()const
	{
		return 0;
	}
};