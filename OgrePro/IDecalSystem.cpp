/******************************************************************
** 文件名:	IDecalSystem.cpp
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:29
** 描  述:
******************************************************************/
#include "stdafx.h"
#include "IDecalSystem.h"
#include "DecalSystem.h"

namespace Ogre
{
	template<> IDecalSystem* Singleton<IDecalSystem>::ms_Singleton = 0;
	IDecalSystem* IDecalSystem::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	IDecalSystem& IDecalSystem::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	IDecalSystem::IDecalSystem()
	{
	}

	IDecalSystem::~IDecalSystem()
	{

	}


	static DecalSystem *g_decalSystem = 0;

	// 初始化贴花系统
	void IDecalSystem::init(SceneManager *mgr)
	{
		if(!g_decalSystem)
		{
			g_decalSystem = new DecalSystem(mgr);
		}
	}
	// 销毁贴花系统
	void IDecalSystem::shutdown()
	{
		OGRE_DELETE(g_decalSystem);
	}
};