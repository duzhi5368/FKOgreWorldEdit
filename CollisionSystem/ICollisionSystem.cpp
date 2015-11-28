#include "StdAfx.h"
#include "ICollisionSystem.h"
#include "OpcodeCollisionSystem.h"

namespace Ogre
{
	// 单件模式实现
	template<> ICollisionSystem* Singleton<ICollisionSystem>::ms_Singleton = 0;
	ICollisionSystem* ICollisionSystem::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	ICollisionSystem& ICollisionSystem::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	static OpcodeCollisionSystem *s_CollisionSystem = 0;

	// 初始化场景系统
	void ICollisionSystem::init(SceneManager *sceneMgr)
	{
		if(s_CollisionSystem)
			return;
		s_CollisionSystem = new OpcodeCollisionSystem;
	}
	// 销毁场景系统
	void ICollisionSystem::shutdown()
	{
		OGRE_DELETE(s_CollisionSystem);
	}

};