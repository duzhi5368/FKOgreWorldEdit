#pragma once
#include "NullObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API ParticleObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// 获得该工厂的类型名称
		virtual const String &getType() const;
		// 工厂对象的创建方法
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// 工厂对象的销毁方法
		virtual void destroy(ISceneObject *obj);
	};

	// 模型实体对象
	class DISPLAYLAYER_API ParticleObject : public NullObject
	{
	public:
		ParticleObject(const String &name);
		virtual ~ParticleObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return ParticleObjectFactory::FACTORY_TYPE;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();
		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_particleSystem;
		}
		// 获得ParticleSystem
		virtual ParticleSystem *getOgreParticleSystem()const
		{
			return m_particleSystem;
		}

		// 设置粒子模板
		void setParticleTemplate(const String &templateName);
		// 获得粒子模板名称
		const String &getParticleTemplate()const
		{
			return m_particleTemplateName;
		}

		// 设置属性
		virtual bool setProperty(uint id , const Any &value);
		// 获取属性值
		virtual Any getProperty(uint id)const;
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const;

		// 获得场景对象的可见对象，用来在编辑器里选择
		virtual ISceneObjectVisible *getSceneObjectVisible();

	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// 销毁粒子系统
		void destroyParticle();
		// Ogre粒子系统
		ParticleSystem *m_particleSystem;
		// 粒子模板名称
		String m_particleTemplateName;
	};


};