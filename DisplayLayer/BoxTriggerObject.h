#pragma once
#include "TriggerObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API BoxTriggerObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API BoxTriggerObject : public TriggerObject
		, public Node::Listener
	{
	public:
		BoxTriggerObject(const String &name);
		virtual ~BoxTriggerObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return BoxTriggerObjectFactory::FACTORY_TYPE;
		}
		ITriggerEntity *getTriggerEntity()const;
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();
		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const;

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

		// 节点更新时顺便更新触发器
		virtual void nodeUpdated(const Node *node);

	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// 盒子触发器实体
		IBoxTriggerEntity *m_entity;
	};


};