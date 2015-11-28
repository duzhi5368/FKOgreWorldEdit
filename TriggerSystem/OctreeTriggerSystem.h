#pragma once
#include "ITriggerSystem.h"

namespace Ogre
{
	// 触发器系统实现
	class OctreeTriggerSystem : public ITriggerSystem
	{
	public:
		OctreeTriggerSystem(void);
		virtual ~OctreeTriggerSystem(void);


		// 创建一个触发器世界
		virtual ITriggerWorld *createWorld(const String &name);
		// 获得一个触发器世界
		virtual ITriggerWorld *getWorld(const String &name);
		// 是否有这个名称的触发器世界
		virtual bool hasWorld(const String &name);
		// 销毁一个触发器世界
		virtual void destroyWorld(const String &name);
		// 销毁一个触发器世界
		virtual void destroyWorld(ITriggerWorld *world);

	protected:
		typedef std::map<String , ITriggerWorld*> TriggerWorldMap;
		TriggerWorldMap m_worldMap;

	};
};