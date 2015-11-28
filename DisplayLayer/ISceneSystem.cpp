#include "stdafx.h"
#include "ISceneSystem.h"
#include "SceneSystem.h"
#include "NullObject.h"
#include "EntityObject.h"
#include "CharacterObject.h"
#include "ParticleObject.h"
#include "RibbonTrailObject.h"
#include "CameraObject.h"
#include "TerrainObject.h"
#include "EnvironmentObject.h"
#include "BoxTriggerObject.h"
#include "SphereTriggerObject.h"
#include "AnimationTrackObject.h"

namespace Ogre
{
	// 单件模式实现
	template<> ISceneSystem* Singleton<ISceneSystem>::ms_Singleton = 0;
	ISceneSystem* ISceneSystem::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	ISceneSystem& ISceneSystem::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	static SceneSystem *s_SceneSystem = 0;

	// 初始化场景系统
	void ISceneSystem::init(SceneManager *sceneMgr)
	{
		if(s_SceneSystem)
			return;
		s_SceneSystem = new SceneSystem;
		s_SceneSystem->addFactory(new NullObjectFactory);
		s_SceneSystem->addFactory(new EntityObjectFactory);
		s_SceneSystem->addFactory(new CharacterObjectFactory);
		s_SceneSystem->addFactory(new ParticleObjectFactory);
		s_SceneSystem->addFactory(new RibbonTrailObjectFactory);
		s_SceneSystem->addFactory(new CameraObjectFactory);
		s_SceneSystem->addFactory(new TerrainObjectFactory);
		s_SceneSystem->addFactory(new EnvironmentObjectFactory);
		s_SceneSystem->addFactory(new BoxTriggerObjectFactory);
		s_SceneSystem->addFactory(new SphereTriggerObjectFactory);
		s_SceneSystem->addFactory(new AnimationTrackObjectFactory);
		s_SceneSystem->setOgreSceneManager(sceneMgr);
	}
	// 销毁场景系统
	void ISceneSystem::shutdown()
	{
		OGRE_DELETE(s_SceneSystem);
	}
};