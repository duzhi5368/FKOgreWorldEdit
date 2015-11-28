#pragma once
#include "ISceneObject.h"
#include "PropertyDef.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API NullObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API NullObject : public ISceneObject
	{
	public:
		NullObject(const String &name);
		virtual ~NullObject(void);

		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();

		// 获得创建他的场景
		virtual IScene *getScene()const;

		// 获得在Ogre中的对象名称
		virtual String getFullName()const;

		// 重命名对象
		virtual bool setName(const String &name);
		// 获得对象名称
		virtual const String &getName()const;
		// 获得对象类型
		virtual const String &getType()const
		{
			return NullObjectFactory::FACTORY_TYPE;
		}

		// 设置是否显示
		virtual void setVisible(bool visible);
		// 是否显示
		virtual bool isVisible()const;


		// 设置相对坐标
		virtual void setPosition(const Vector3 &pos);
		// 获得相对坐标
		virtual const Vector3 &getPosition()const;
		// 设置世界坐标
		virtual void setWorldPosition(const Vector3 &pos);
		// 获得世界坐标
		virtual const Vector3 &getWorldPosition()const;

		// 设置相对缩放
		virtual void setScale(const Vector3 &scale);
		// 获得相对坐标
		virtual const Vector3 &getScale()const;
		// 设置世界缩放
		virtual void setWorldScale(const Vector3 &scale);
		// 获得世界缩放
		virtual const Vector3 &getWorldScale()const;

		// 设置相对方位
		virtual void setOrientation(const Quaternion &orientation);
		// 获得相对坐标
		virtual const Quaternion &getOrientation()const;
		// 设置世界方位
		virtual void setWorldOrientation(const Quaternion &orientation);
		// 获得世界方位
		virtual const Quaternion &getWorldOrientation()const;

		// 设置父对象
		virtual void setParent(ISceneObject *parent);
		// 获得父对象
		virtual ISceneObject* getParent()const;

		// 望向某个世界坐标，这个函数会在内部设置世界方位
		virtual void lookAt(const Vector3& targetPoint);
		// 根据当前缩放值缩放
		virtual void scale(const Vector3& scale);
		// 根据当前位置移动
		virtual void translate(const Vector3& distance);
		// 根据当前方位旋转
		virtual void rotate(const Quaternion& rotate);

		// 把一个场景物件挂接到这里
		virtual void addChildObject(ISceneObject *obj);
		// 把一个场景物件从到这里取消挂接
		virtual void removeChildObject(ISceneObject *obj);
		// 把这个场景物件从父场景附件取消挂接
		virtual void removeFromParentObject();

		// 获得已附加的场景物件枚举
		virtual ChildObjectIterator getChildObjectIterator();

		// 根据类型和名称创建一个此场景对象的子场景对象
		virtual ISceneObject *createChildObject(const String &name , const String &type, const NameValuePairList* params = 0);
		// 根据类型和名称创建一个此场景对象的子场景对象
		virtual ISceneObject *createChildObject(const String &type, const NameValuePairList* params = 0);
		// 销毁一个此场景对象的子场景对象
		virtual void destroyChildObject(const String &name);
		virtual void destroyChildObject(ISceneObject *obj);
		// 获得一个此场景对象的子场景对象
		virtual ISceneObject *getChildObject(const String &name);
		// 是否存在指定名字的此场景对象的子场景对象
		virtual bool hasChildObject(const String &name)const;

		// 获得Ogre的场景节点
		virtual SceneNode* getOgreSceneNode()const
		{
			return m_node;
		}

		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return 0;
		}

		// 获得对象创建参数
		virtual const NameValuePairList& getCreationParams()const
		{
			return m_creationParams;
		}

		// 用newName作为名称克隆一个新的对象
		virtual ISceneObject *clone(const String &newName , IScene *newScene = 0);
		// 用自动生成的名称作为名称克隆一个新的对象
		virtual ISceneObject *clone(IScene *newScene = 0);
		// 把当前对象中的数据克隆到target中
		virtual void cloneData(ISceneObject *target);

		// 设置属性
		virtual bool setProperty(uint id , const Any &value);
		// 获取属性值
		virtual Any getProperty(uint id)const;
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const;

		// 获得碰撞实体
		virtual ICollisionEntity *getCollisionEntity()const
		{
			return 0;
		}

		// 设置是否参加碰撞
		virtual void setCanCollide(bool can);
		// 设置是否参加碰撞
		virtual bool getCanCollide()const;


		// 设置用户变量
		virtual void setUserAny(const Any& anything)
		{
			m_userAny = anything;
		}
		// 获得用户变量
		virtual const Any& getUserAny() const
		{
			return m_userAny;
		}
		// 获得场景对象的可见对象，用来在编辑器里选择
		virtual ISceneObjectVisible *getSceneObjectVisible();
	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// 用户变量
		Any m_userAny;
		// 实体名称
		String m_name;
		// Ogre的场景节点
		SceneNode *m_node;
		// 是否需要自动删除Ogre节点
		bool m_autoDeleteNode;
		// 创建此对象的场景
		IScene *m_scene;
		// 父节点
		ISceneObject *m_parent;
		// 可见对象
		ISceneObjectVisible *m_sceneObjectVisible;
		// 是否可视
		bool m_visible;
		// 是否参加碰撞计算
		bool m_canCollide;
		// 创建参数，请派生类们都把实例化本类所需要的参数保存到这里，如mesh名称之类
		NameValuePairList m_creationParams;
		// 子对象
		MapChildObject m_mapChildObject;
	};


};