#include "StdAfx.h"
#include "ParticleObject.h"
#include "IScene.h"
#include "ParticleObjectVisible.h"
namespace Ogre
{

	const String ParticleObjectFactory::FACTORY_TYPE = "ParticleObject";

	// 获得该工厂的类型名称
	const String &ParticleObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *ParticleObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		ParticleObject *obj = new ParticleObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void ParticleObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	ParticleObject::ParticleObject(const String &name)
		: NullObject(name)
		, m_particleSystem(0)
	{
	}

	ParticleObject::~ParticleObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void ParticleObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		if(params && params->find("templateName") != params->end())
		{
			setParticleTemplate(params->find("templateName")->second);
		}
	}

	// 销毁对象实体
	void ParticleObject::destroy()
	{
		destroyParticle();
		NullObject::destroy();
	}
	// 设置粒子模板
	void ParticleObject::setParticleTemplate(const String &templateName)
	{
		destroyParticle();
		// 创建实体
		m_particleSystem = m_scene->getOgreSceneManager()->createParticleSystem(getFullName() , templateName);
		m_particleSystem->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// 挂接上去
		m_node->attachObject(m_particleSystem);

		m_particleTemplateName = templateName;
	}

	// 销毁粒子系统
	void ParticleObject::destroyParticle()
	{
		if(m_particleSystem)
		{
			// 销毁模型实体
			m_scene->getOgreSceneManager()->destroyMovableObject(m_particleSystem);
			m_particleSystem = 0;
			m_particleTemplateName.clear();
		}
	}



	// 枚举所有属性，以树的形式
	void ParticleObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Particle , PT_GROUP , "粒子设置" , "粒子设置" , true , false , false));
		properties.back().addChild(Property(ID_ParticleTemplate , PT_STRING , "粒子模板" , "粒子模板" , true , true , true));
	}

	// 设置属性
	bool ParticleObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Particle:		// 一般设置
			{
				return true;
			}
			break;
		case ID_ParticleTemplate:	// 模型文件
			{
				setParticleTemplate(any_cast<String>(value));
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any ParticleObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Particle:		// 模型设置
			{
				return Any();
			}
			break;
		case ID_ParticleTemplate:	// 模型文件
			{
				return Any(getParticleTemplate());
			}
			break;
		default:
			return Any();
			break;
		}
	}


	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* ParticleObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* ParticleObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *ParticleObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new ParticleObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
};