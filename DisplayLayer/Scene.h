#pragma once
#include "IScene.h"
#include "TListener.h"

namespace Ogre
{
	
	class DISPLAYLAYER_API Scene : public IScene
		, public FrameListener
		, public TListener<IScene::Listener>
	{
	public:
		Scene(SceneManager *mgr, const String &name , ICollisionWorld *collisionWorld , ITriggerWorld *triggerWorld);
		virtual ~Scene(void);

		// 获得名称
		virtual const String &getName() const
		{
			return m_name;
		}
		// 通知这个场景是在哪个文件载入的
		virtual void _notifyOrigin(const String &origin);
		// 获得用于创建这个场景的文件名
		virtual const String &getOrigin()const;

		// 添加侦听器
		virtual void addListener(Listener *listener , bool toBack = true)
		{
			TListener<IScene::Listener>::addListener(listener , toBack);
		}
		// 删除侦听器
		virtual void removeListener(Listener *listener)
		{
			TListener<IScene::Listener>::removeListener(listener);
		}


		// 创建一个动画控制器
		virtual IAnimationStateController* createAnimationStateController(const String &name , const String &animationName);
		// 创建一个动画控制器
		virtual IAnimationStateController* createAnimationStateController(const String &animationName);
		// 获得一个动画控制器
		virtual IAnimationStateController* getAnimationStateController(const String &name);
		// 是否存在这个动画控制器
		virtual bool hasAnimationStateController(const String &name);
		// 是否存在这个动画
		virtual bool hasAnimation(const String &animationName);
		// 销毁一个动画控制器
		virtual void destroyAnimationStateController(const String &name);
		// 销毁一个动画控制器
		virtual void destroyAnimationStateController(IAnimationStateController *ani);

		// 销毁所有物件
		virtual void clear();

		// 根据类型和名称创建一个场景对象
		virtual ISceneObject *createSceneObject(const String &name , const String &type, const NameValuePairList* params = 0);
		// 根据类型和名称创建一个场景对象
		virtual ISceneObject *createSceneObject(const String &type, const NameValuePairList* params = 0);
		// 销毁一个场景对象
		virtual void destroySceneObject(const String &name);
		virtual void destroySceneObject(ISceneObject *obj);
		// 获得一个场景对象
		virtual ISceneObject *getSceneObject(const String &name);
		// 是否存在指定名字的场景对象
		virtual bool hasSceneObject(const String &name)const;
		// 是否存obj这个场景对象
		virtual bool hasSceneObject(ISceneObject *obj)const;
		// 这个指针相对于此场景是否有效
		virtual bool isSceneObjectValid(ISceneObject *obj)const;

		// 获得Ogre的场景管理器
		virtual SceneManager *getOgreSceneManager()const;
		// 获得所有对象的列表
		virtual SceneObjectIterator getSceneObjectIterator()
		{
			return SceneObjectIterator(m_mapSceneObject.begin() , m_mapSceneObject.end());
		};
		// 重命名对象
		bool renameSceneObject(const String &oldName , const String &newName);

		// 获得一个场景内唯一ID
		virtual uint32 getUniqueID();
		// 获得一个场景内唯一名称
		virtual String getUniqueName();

		// 获得根场景对象
		virtual ISceneObject *getRootObject()
		{
			return m_rootObject;
		}

		// 帧侦听器的每帧执行
		bool frameStarted( const FrameEvent& evt );
		// 获得碰撞世界
		virtual ICollisionWorld *getCollisionWorld()
		{
			return m_collisionWorld;
		}
		// 获得触发器世界
		virtual ITriggerWorld *getTriggerWorld()
		{
			return m_triggerWorld;
		}

		// 射线碰撞检测
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @info 输出射线碰撞检测信息
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , RayCheckInfo& info);

		// 射线碰撞检测，仅返回第一个发生碰撞的信息
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @intersection 输出交点
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection);

		// 射线碰撞检测
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @infoList 输出射线碰撞检测信息
		// @sortByDistance 输出的infoList按距离排序
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , ListRayCheckInfo& info);

		// 射线碰撞检测
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @infoList 输出射线碰撞检测信息
		// @sortByDistance 输出的infoList按距离排序
		// @returnFirstOnly 只返回第一个碰撞数据
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , bool returnFirstOnly , ListRayCheckInfo& info);

	protected:
		// 场景名称
		String m_name;

		// 场景对象列表
		MapSceneObject m_mapSceneObject;

		// Ogre的场景管理器
		SceneManager *m_sceneMgr;

		// 载入的文件名
		String m_origin;

		// 递增ID
		uint32 m_index;

		// 根场景对象
		ISceneObject *m_rootObject;

		// 碰撞世界
		ICollisionWorld *m_collisionWorld;
		// 射线查询器
		RaySceneQuery *m_rayQuery;

		// 触发器世界
		ITriggerWorld *m_triggerWorld;

		typedef std::map<String , IAnimationStateController*> MapAnimationStateController;
		// 正在播放的动画列表
		MapAnimationStateController m_mapAnimationStateController;
	};

};