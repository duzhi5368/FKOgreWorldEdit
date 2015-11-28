#pragma once
#include "ITriggerWorld.h"

namespace Ogre
{
	class OctreeTriggerWorld : public ITriggerWorld
		, public FrameListener
	{
	public:
		OctreeTriggerWorld(const String &name);
		virtual ~OctreeTriggerWorld(void);


		// 获得名称
		virtual const String &getName()const
		{
			return m_name;
		}


		// 创建盒子触发器实例
		virtual IBoxTriggerEntity* createBoxTriggerEntity(const String &name = StringUtil::BLANK);
		// 创建球体触发器实例
		virtual ISphereTriggerEntity* createSphereTriggerEntity(const String &name = StringUtil::BLANK);
		// 创建玩家实例
		virtual ITriggerPlayer * createTriggerPlayer(const String &name = StringUtil::BLANK);
		// 销毁触发器实例
		virtual void destroyTriggerEntity(ITriggerEntity *entity);
		// 销毁触发器实例
		virtual void destroyTriggerEntity(const String &name , const String &triggerType);
		// 获得一个触发器实例
		virtual ITriggerEntity* getTriggerEntity(const String &name , const String &triggerType)const;
		// 是否有这个触发器实例
		virtual bool hasTriggerEntity(const String &name , const String &triggerType)const;

		// 销毁所有触发器实例
		virtual void destroyAllTriggerEntity();

		// 获得所有对象的列表
		virtual TriggerEntityIterator getTriggerEntityIterator(const String &triggerType);

		// 获得Ogre场景管理器
		SceneManager *getSceneManager()const
		{
			return m_sceneMgr;
		}
		typedef std::map<String , ITriggerPlayer*> MapTriggerPlayer;
		typedef MapIterator<MapTriggerPlayer> TriggerPlayerIterator;
		// 获得所有玩家的列表
		virtual TriggerPlayerIterator getTriggerPlayerIterator();

		// 每帧更新场景管理器
		virtual bool frameStarted(const FrameEvent& evt);

	protected:
		String m_name;
		// Ogre场景管理器
		SceneManager *m_sceneMgr;
	};

};