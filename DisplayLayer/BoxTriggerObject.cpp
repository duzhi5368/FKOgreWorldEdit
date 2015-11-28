#include "StdAfx.h"
#include "BoxTriggerObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "BoxTriggerObjectVisible.h"
#include "IBoxTriggerEntity.h"
namespace Ogre
{

	const String BoxTriggerObjectFactory::FACTORY_TYPE = "BoxTriggerObject";

	// 获得该工厂的类型名称
	const String &BoxTriggerObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *BoxTriggerObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		BoxTriggerObject *obj = new BoxTriggerObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void BoxTriggerObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	BoxTriggerObject::BoxTriggerObject(const String &name)
		: TriggerObject(name)
		, m_entity(0)
	{
	}

	BoxTriggerObject::~BoxTriggerObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void BoxTriggerObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// 创建实体
		m_entity = m_scene->getTriggerWorld()->createBoxTriggerEntity(getFullName());
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_node->setListener(this);
	}

	// 销毁对象实体
	void BoxTriggerObject::destroy()
	{
		if(m_entity)
		{
			// 销毁模型实体
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_entity);
			m_entity = 0;
		}
		NullObject::destroy();
	}
	ITriggerEntity *BoxTriggerObject::getTriggerEntity()const
	{
		return m_entity;
	}
	// 节点更新时顺便更新触发器
	void BoxTriggerObject::nodeUpdated(const Node *node)
	{
		m_entity->setPosition(getWorldPosition());
		m_entity->setScale(getWorldScale());
		m_entity->setOrientation(getWorldOrientation());
	}

	// 枚举所有属性，以树的形式
	void BoxTriggerObject::enumPropertiesTree(VTProperty &properties)const
	{
		TriggerObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_BoxTrigger , PT_GROUP , "盒子触发器设置" , "盒子触发器设置" , true , false , false));
		properties.back().addChild(Property( ID_BoxTriggerAxisAlignedBox , PT_AXISALIGNEDBOX , "包围盒" , "盒子触发器的包围盒" , true , true , true));
	}

	// 设置属性
	bool BoxTriggerObject::setProperty(uint id , const Any &value)
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::setProperty(id , value);
		switch(id)
		{
		case ID_BoxTrigger:							// 触发器设置
			{
				return true;
			}
			break;
		case ID_BoxTriggerAxisAlignedBox:							// 包围盒的min值
			{
				m_entity->setBox(any_cast<AxisAlignedBox>(value));
				// 重建可见对象
				if(m_sceneObjectVisible)
				{
					m_sceneObjectVisible->_notifyRebuild();
				}
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any BoxTriggerObject::getProperty(uint id)const
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::getProperty(id);
		switch(id)
		{
		case ID_BoxTrigger:							// 触发器设置
			{
				return Any();
			}
			break;
		case ID_BoxTriggerAxisAlignedBox:							// 包围盒的min值
			{
				return Any(m_entity->getBox());
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* BoxTriggerObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* BoxTriggerObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *BoxTriggerObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new BoxTriggerObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

	// 获得MovableObject
	MovableObject *BoxTriggerObject::getOgreMovableObject()const
	{
		return 0;
	}
};