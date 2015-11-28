#include "StdAfx.h"
#include "ITriggerSystem.h"
#include "OctreeTriggerSystem.h"

namespace Ogre
{
	// 单件模式实现
	template<> ITriggerSystem* Singleton<ITriggerSystem>::ms_Singleton = 0;
	ITriggerSystem* ITriggerSystem::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	ITriggerSystem& ITriggerSystem::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	static OctreeTriggerSystem *s_TriggerSystem = 0;

	// 初始化场景系统
	void ITriggerSystem::init()
	{
		if(s_TriggerSystem)
			return;
		s_TriggerSystem = new OctreeTriggerSystem;
	}
	// 销毁场景系统
	void ITriggerSystem::shutdown()
	{
		OGRE_DELETE(s_TriggerSystem);
	}

};