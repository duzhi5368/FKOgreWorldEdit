#pragma once
#include "PropertyContainer.h"

namespace Ogre
{
	class IScene;
	class ISceneObjectVisible;
	// 场景物件
	class ISceneObject : public PropertyContainer
	{
	public:

		ISceneObject(void)
		{
		}

		virtual ~ISceneObject(void)
		{
		}

		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params) = 0;
		// 销毁对象实体
		virtual void destroy() = 0;

		// 获得创建他的场景
		virtual IScene *getScene()const = 0;

		// 重命名对象
		virtual bool setName(const String &name) = 0;
		// 获得对象名称
		virtual const String &getName()const = 0;
		// 获得对象类型
		virtual const String &getType()const = 0;

		// 获得对象全称（场景名称+对象名称）
		virtual String getFullName()const = 0;

		// 设置是否显示
		virtual void setVisible(bool visible) = 0;
		// 是否显示
		virtual bool isVisible()const = 0;

		// 设置相对坐标
		virtual void setPosition(const Vector3 &pos) = 0;
		// 获得相对坐标
		virtual const Vector3 &getPosition()const = 0;
		// 设置世界坐标
		virtual void setWorldPosition(const Vector3 &pos) = 0;
		// 获得世界坐标
		virtual const Vector3 &getWorldPosition()const = 0;

		// 设置相对缩放
		virtual void setScale(const Vector3 &scale) = 0;
		// 获得相对坐标
		virtual const Vector3 &getScale()const = 0;
		// 设置世界缩放
		virtual void setWorldScale(const Vector3 &scale) = 0;
		// 获得世界缩放
		virtual const Vector3 &getWorldScale()const = 0;

		// 设置相对方位
		virtual void setOrientation(const Quaternion &orientation) = 0;
		// 获得相对坐标
		virtual const Quaternion &getOrientation()const = 0;
		// 设置世界方位
		virtual void setWorldOrientation(const Quaternion &orientation) = 0;
		// 获得世界方位
		virtual const Quaternion &getWorldOrientation()const = 0;

		// 望向某个世界坐标，这个函数会在内部设置世界方位
		virtual void lookAt(const Vector3& targetPoint) = 0;
		// 根据当前缩放值缩放
		virtual void scale(const Vector3& scale) = 0;
		// 根据当前位置移动
		virtual void translate(const Vector3& distance) = 0;
		// 根据当前方位旋转
		virtual void rotate(const Quaternion& rotate) = 0;

		// 把一个场景物件挂接到这里
		virtual void addChildObject(ISceneObject *obj) = 0;
		// 把一个场景物件从到这里取消挂接
		virtual void removeChildObject(ISceneObject *obj) = 0;
		// 把这个场景物件从父场景附件取消挂接
		virtual void removeFromParentObject() = 0;

		// 设置父对象
		virtual void setParent(ISceneObject *parent) = 0;
		// 获得父对象
		virtual ISceneObject* getParent()const = 0;

		typedef std::map<String , ISceneObject*> MapChildObject;
		typedef MapIterator<MapChildObject> ChildObjectIterator;
		// 获得已附加的场景物件枚举
		virtual ChildObjectIterator getChildObjectIterator() = 0;

		// 根据类型和名称创建一个此场景对象的子场景对象
		virtual ISceneObject *createChildObject(const String &name , const String &type, const NameValuePairList* params = 0) = 0;
		// 根据类型和名称创建一个此场景对象的子场景对象
		virtual ISceneObject *createChildObject(const String &type, const NameValuePairList* params = 0) = 0;
		// 销毁一个此场景对象的子场景对象
		virtual void destroyChildObject(const String &name) = 0;
		virtual void destroyChildObject(ISceneObject *obj) = 0;
		// 获得一个此场景对象的子场景对象
		virtual ISceneObject *getChildObject(const String &name) = 0;
		// 是否存在指定名字的此场景对象的子场景对象
		virtual bool hasChildObject(const String &name)const = 0;

		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const = 0;

		// 获得SceneNode
		virtual SceneNode *getOgreSceneNode()const = 0;

		// 获得对象创建参数
		virtual const NameValuePairList& getCreationParams()const = 0;

		// 用newName作为名称克隆一个新的对象
		virtual ISceneObject *clone(const String &newName , IScene *newScene = 0) = 0;
		// 用自动生成的名称作为名称克隆一个新的对象
		virtual ISceneObject *clone(IScene *newScene = 0) = 0;
		// 把当前对象中的数据克隆到target中
		virtual void cloneData(ISceneObject *target) = 0;

		// 获得碰撞实体
		virtual ICollisionEntity *getCollisionEntity()const = 0;

		// 设置是否参加碰撞
		virtual void setCanCollide(bool can) = 0;
		// 设置是否参加碰撞
		virtual bool getCanCollide()const = 0;

		// 获得场景对象的可见对象，用来在编辑器里选择
		virtual ISceneObjectVisible *getSceneObjectVisible() = 0;

		// 设置用户变量
		virtual void setUserAny(const Any& anything) = 0;
		// 获得用户变量
		virtual const Any& getUserAny() const = 0;
	};

	// 把Any的值转换成T，转换失败则返回0
	template<typename T>
	inline T safe_cast(const Any & anything)
	{
		T* obj = any_cast<T>((Any*)&anything);
		return obj ? *obj : T();
	};

	// 把Any的值转换成ISceneObject *，转换失败则返回0
	inline ISceneObject *obj_cast(const Any &anything)
	{
		return safe_cast<ISceneObject*>(anything);
	};

	// 场景物件的对象工厂
	class ISceneObjectFactory
	{
	public:
		// 获得该工厂的类型名称
		virtual const String &getType() const = 0;
		// 工厂对象的创建方法
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params) = 0;
		// 工厂对象的销毁方法
		virtual void destroy(ISceneObject *obj) = 0;
	};
}