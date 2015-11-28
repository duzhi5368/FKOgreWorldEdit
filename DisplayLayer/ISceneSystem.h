#pragma once
#include "ISceneObject.h"
#include "TFactorySystem.h"
namespace Ogre
{
	class IScene;
	class ETTerrain;
	// 场景系统，一个场景系统可以由多个场景组成
	class DISPLAYLAYER_API ISceneSystem : public Singleton<ISceneSystem>
		, public TFactorySystem<ISceneObjectFactory>
	{
	public:

		ISceneSystem(void)
		{
		}

		virtual ~ISceneSystem(void)
		{
		}
		// 单间模式接口
		static ISceneSystem& getSingleton(void);
		static ISceneSystem* getSingletonPtr(void);

		// 初始化场景系统
		static void init(SceneManager *sceneMgr);
		// 销毁场景系统
		static void shutdown();

		// 获得一个全局唯一ID
		virtual uint32 getUniqueID() = 0;
		// 获得一个全局唯一名称
		virtual String getUniqueName() = 0;

		// 根据文件名保存一个场景
		virtual bool saveScene(IScene* scene , const String &fileName) = 0;
		// 根据实例名称和文件名保存一个场景
		virtual bool saveScene(const String &name , const String &fileName) = 0;
		// 根据实例名称和文件名载入一个场景
		virtual IScene* loadScene(const String &name , const String &fileName) = 0;
		// 根据文件名载入一个场景
		virtual IScene* loadScene(const String &fileName , IScene *scene = 0) = 0;
		// 创建一个场景
		virtual IScene* createScene(const String &name) = 0;
		// 创建一个场景
		virtual IScene* createScene() = 0;
		// 销毁场景
		virtual void destroyScene(const String &name) = 0;
		virtual void destroyScene(IScene *scene) = 0;
		// 获得一个场景
		virtual IScene *getScene(const String &name) = 0;
		// 是否存在指定名字的场景
		virtual bool hasScene(const String &name)const = 0;

		// 设置使用这个Ogre场景管理器
		virtual void setOgreSceneManager(SceneManager *mgr) = 0;
		// 获得Ogre场景管理器
		virtual SceneManager *getOgreSceneManager()const = 0;

		// 设置使用这个碰撞世界
		virtual void setCollisionWorld(ICollisionWorld *world) = 0;
		// 获得碰撞世界
		virtual ICollisionWorld *getCollisionWorld()const = 0;

		// 设置使用这个触发器世界
		virtual void setTriggerWorld(ITriggerWorld *world) = 0;
		// 获得触发器世界
		virtual ITriggerWorld *getTriggerWorld()const = 0;

		// 为一个Viewport开启运动模糊
		virtual void setMotionBlurEnabled(Viewport *vp , bool enable) = 0;
	};
};