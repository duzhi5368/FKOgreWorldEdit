#pragma once
#include "TriggerObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API SphereTriggerObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API SphereTriggerObject : public TriggerObject
		, public Node::Listener
	{
	public:
		SphereTriggerObject(const String &name);
		virtual ~SphereTriggerObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return SphereTriggerObjectFactory::FACTORY_TYPE;
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

		// 设置相对缩放
		virtual void setScale(const Vector3 &scale);
		// 根据当前缩放值缩放
		virtual void scale(const Vector3& scale);

		// 设置相对方位
		virtual void setOrientation(const Quaternion &orientation){};
		// 望向某个世界坐标，这个函数会在内部设置世界方位
		virtual void lookAt(const Vector3& targetPoint){};
		// 根据当前方位旋转
		virtual void rotate(const Quaternion& rotate){};

	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// 盒子触发器实体
		ISphereTriggerEntity *m_entity;
	};


};