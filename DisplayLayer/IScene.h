#pragma once
#include "TFactorySystem.h"

namespace Ogre
{
	class ISceneObject;
	class IAnimationStateController;


	// 碰撞信息
	struct RayCheckInfo
	{
		RayCheckInfo()
			: obj(0)
		{

		};
		// 碰撞数据
		RayCollisionInfo info;
		// 发生碰撞的场景对象（如果有对象发生碰撞的话）
		ISceneObject *obj;

		/// Comparison operator for sorting
		bool operator < (const RayCheckInfo& rhs) const
		{
			return info.distance < rhs.info.distance;
		}
	};
	typedef std::vector<RayCheckInfo> ListRayCheckInfo;


	// 一个场景
	class IScene
	{
	public:

		IScene(void)
		{
		}

		virtual ~IScene(void)
		{
		}
		// 场景侦听器
		struct Listener
		{
			// 当创建一个场景对象后调用
			virtual void onCreateSceneObject(ISceneObject *obj){};
			// 当销毁一个场景对象前调用
			virtual void onDestroySceneObject(ISceneObject *obj){};
		};

		// 添加侦听器
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// 删除侦听器
		virtual void removeListener(Listener *listener) = 0;

		// 通知这个场景是在哪个文件载入的
		virtual void _notifyOrigin(const String &origin) = 0;
		// 获得用于创建这个场景的文件名
		virtual const String &getOrigin() const= 0;
		// 获得名称
		virtual const String &getName() const= 0;

		// 创建一个动画控制器
		virtual IAnimationStateController* createAnimationStateController(const String &name , const String &animationName) = 0;
		// 创建一个动画控制器
		virtual IAnimationStateController* createAnimationStateController(const String &animationName) = 0;
		// 获得一个动画控制器
		virtual IAnimationStateController* getAnimationStateController(const String &name) = 0;
		// 是否存在这个动画控制器
		virtual bool hasAnimationStateController(const String &name) = 0;
		// 是否存在这个动画
		virtual bool hasAnimation(const String &animationName) = 0;
		// 销毁一个动画控制器
		virtual void destroyAnimationStateController(const String &name) = 0;
		// 销毁一个动画控制器
		virtual void destroyAnimationStateController(IAnimationStateController *ani) = 0;

		// 销毁所有物件
		virtual void clear() = 0;

		// 根据类型和名称创建一个场景对象
		virtual ISceneObject *createSceneObject(const String &name , const String &type, const NameValuePairList* params = 0) = 0;
		// 根据类型和名称创建一个场景对象
		virtual ISceneObject *createSceneObject(const String &type, const NameValuePairList* params = 0) = 0;
		// 销毁一个场景对象
		virtual void destroySceneObject(const String &name) = 0;
		virtual void destroySceneObject(ISceneObject *obj) = 0;
		// 获得一个场景对象
		virtual ISceneObject *getSceneObject(const String &name) = 0;
		// 是否存在指定名字的场景对象
		virtual bool hasSceneObject(const String &name)const = 0;
		// 是否存obj这个场景对象
		virtual bool hasSceneObject(ISceneObject *obj)const = 0;
		// 这个指针相对于此场景是否有效
		virtual bool isSceneObjectValid(ISceneObject *obj)const = 0;

		typedef std::map<String , ISceneObject*> MapSceneObject;
		typedef MapIterator<MapSceneObject> SceneObjectIterator;
		// 获得所有对象的列表
		virtual SceneObjectIterator getSceneObjectIterator() = 0;

		// 获得Ogre的场景管理器
		virtual SceneManager *getOgreSceneManager()const = 0;

		// 获得一个场景内唯一ID
		virtual uint32 getUniqueID() = 0;
		// 获得一个场景内唯一名称
		virtual String getUniqueName() = 0;

		// 获得根场景对象
		virtual ISceneObject *getRootObject() = 0;

		// 获得碰撞世界
		virtual ICollisionWorld *getCollisionWorld() = 0;

		// 获得触发器世界
		virtual ITriggerWorld *getTriggerWorld() = 0;

		// 射线碰撞检测，仅返回第一个发生碰撞的信息
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @info 输出射线碰撞检测信息
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , RayCheckInfo& info) = 0;


		// 射线碰撞检测，仅返回第一个发生碰撞的信息
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @intersection 输出交点
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection) = 0;

		// 射线碰撞检测
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @infoList 输出射线碰撞检测信息
		// @sortByDistance 输出的infoList按距离排序
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , ListRayCheckInfo& info) = 0;
	};

};