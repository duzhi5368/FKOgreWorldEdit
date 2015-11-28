#include "StdAfx.h"
#include "SphereTriggerObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "SphereTriggerObjectVisible.h"
#include "ISphereTriggerEntity.h"
namespace Ogre
{

	const String SphereTriggerObjectFactory::FACTORY_TYPE = "SphereTriggerObject";

	// 获得该工厂的类型名称
	const String &SphereTriggerObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *SphereTriggerObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		SphereTriggerObject *obj = new SphereTriggerObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void SphereTriggerObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	SphereTriggerObject::SphereTriggerObject(const String &name)
		: TriggerObject(name)
		, m_entity(0)
	{
	}

	SphereTriggerObject::~SphereTriggerObject(void)
	{
		destroy();
	}
	ITriggerEntity *SphereTriggerObject::getTriggerEntity()const
	{
		return m_entity;
	}
	// 创建对象实体
	void SphereTriggerObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// 创建实体
		m_entity = m_scene->getTriggerWorld()->createSphereTriggerEntity(getFullName());
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_node->setListener(this);
	}

	// 销毁对象实体
	void SphereTriggerObject::destroy()
	{
		if(m_entity)
		{
			// 销毁模型实体
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_entity);
			m_entity = 0;
		}
		NullObject::destroy();
	}

	// 节点更新时顺便更新触发器
	void SphereTriggerObject::nodeUpdated(const Node *node)
	{
		m_entity->setPosition(getWorldPosition());
		m_entity->setScale(getWorldScale());
		m_entity->setOrientation(getWorldOrientation());
	}

	// 枚举所有属性，以树的形式
	void SphereTriggerObject::enumPropertiesTree(VTProperty &properties)const
	{
		TriggerObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_SphereTrigger , PT_GROUP , "球形触发器设置" , "球形触发器设置" , true , false , false));
		properties.back().addChild(Property( ID_SphereRadius , PT_REAL , "球体半径" , "球体半径" , true , true , true));
	}

	// 设置属性
	bool SphereTriggerObject::setProperty(uint id , const Any &value)
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::setProperty(id , value);
		switch(id)
		{
		case ID_SphereTrigger:							// 触发器设置
			{
				return true;
			}
			break;
		case ID_SphereRadius:							// 球体半径
			{
				m_entity->setRadius(any_cast<Real>(value));
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
	Any SphereTriggerObject::getProperty(uint id)const
	{
		if((id > ID_TriggerObject_Begin && id < ID_TriggerObject_End)
			|| (id > ID_NullObject_Begin && id < ID_NullObject_End))
			return TriggerObject::getProperty(id);
		switch(id)
		{
		case ID_SphereTrigger:							// 触发器设置
			{
				return Any();
			}
			break;
		case ID_SphereRadius:							// 球体半径
			{
				return Any(m_entity->getRadius());
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* SphereTriggerObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* SphereTriggerObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *SphereTriggerObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new SphereTriggerObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

	// 获得MovableObject
	MovableObject *SphereTriggerObject::getOgreMovableObject()const
	{
		return 0;
	}


	// 设置相对缩放
	void SphereTriggerObject::setScale(const Vector3 &scale)
	{
		TriggerObject::setScale(Vector3(scale.x , scale.x , scale.x));
	}
	// 根据当前缩放值缩放
	void SphereTriggerObject::scale(const Vector3& scale)
	{
		TriggerObject::scale(Vector3(scale.x , scale.x , scale.x));
	}
};