#include "ISceneSystem.h"
#pragma once
namespace Ogre
{
	// 场景系统实现
	class DISPLAYLAYER_API SceneSystem : public ISceneSystem
	{
	public:
		SceneSystem(void);
		virtual ~SceneSystem(void);

		// 获得一个全局唯一ID
		virtual uint32 getUniqueID();
		// 获得一个全局唯一名称
		virtual String getUniqueName();

		// 根据实例名称和文件名载入一个场景
		virtual IScene* loadScene(const String &name , const String &fileName);
		// 根据文件名载入一个场景
		virtual IScene* loadScene(const String &fileName , IScene *scene = 0);
		// 根据文件名保存一个场景
		virtual bool saveScene(IScene* scene , const String &fileName);
		// 根据实例名称和文件名保存一个场景
		virtual bool saveScene(const String &name , const String &fileName);

		// 创建一个场景
		virtual IScene* createScene(const String &name);
		// 创建一个场景
		virtual IScene* createScene();
		// 销毁场景
		virtual void destroyScene(const String &name);
		virtual void destroyScene(IScene *scene);
		// 获得一个场景
		virtual IScene *getScene(const String &name);
		// 是否存在指定名字的场景
		virtual bool hasScene(const String &name)const;
		// 设置使用这个Ogre场景管理器
		virtual void setOgreSceneManager(SceneManager *mgr)
		{
			m_sceneMgr = mgr;
		}
		// 获得Ogre场景管理器
		virtual SceneManager *getOgreSceneManager()const
		{
			return m_sceneMgr;
		}

		// 设置使用这个碰撞世界
		virtual void setCollisionWorld(ICollisionWorld *world)
		{
			m_collisionWorld = world;
		}
		// 获得碰撞世界
		virtual ICollisionWorld *getCollisionWorld()const
		{
			return m_collisionWorld;
		}

		// 设置使用这个触发器世界
		virtual void setTriggerWorld(ITriggerWorld *world)
		{
			m_triggerWorld = world;
		}
		// 获得触发器世界
		virtual ITriggerWorld *getTriggerWorld()const
		{
			return m_triggerWorld;
		}


		// 为一个Viewport开启运动模糊
		virtual void setMotionBlurEnabled(Viewport *vp , bool enable);


	protected:

		// 确保一个混合器已经添加到vp中
		void ensureCompositorAdd(Viewport *vp , const String &compositor);
		// 在vp上允许或禁用某个compositor
		void setCompositorEnabled(Viewport *vp , const String &compositor , bool enable);

		typedef std::map<String , IScene*> MapScene;
		// 场景列表
		MapScene m_mapScene;
		// Ogre场景管理器
		SceneManager *m_sceneMgr;

		// 碰撞世界
		ICollisionWorld *m_collisionWorld;
		// 触发器世界
		ITriggerWorld *m_triggerWorld;

		// 运动模糊的混合器
		CompositorPtr m_motionBlur;
	};
};