#include "StdAfx.h"
#include "OctreeTriggerSystem.h"
#include "OctreeTriggerWorld.h"
#include "OctreeBoxTriggerEntity.h"
#include "OctreeSphereTriggerEntity.h"
#include "OctreeTriggerPlayer.h"
namespace Ogre
{
	static OctreeBoxTriggerEntityFactory s_OctreeBoxTriggerEntityFactory;
	static OctreeSphereTriggerEntityFactory s_OctreeSphereTriggerEntityFactory;
	static OctreeTriggerPlayerFactory s_OctreeTriggerPlayerFactory;
	OctreeTriggerSystem::OctreeTriggerSystem(void)
	{
		Root::getSingleton().addMovableObjectFactory(&s_OctreeBoxTriggerEntityFactory , true);
		Root::getSingleton().addMovableObjectFactory(&s_OctreeSphereTriggerEntityFactory , true);
		Root::getSingleton().addMovableObjectFactory(&s_OctreeTriggerPlayerFactory , true);
	}

	OctreeTriggerSystem::~OctreeTriggerSystem(void)
	{
		for(TriggerWorldMap::iterator iter = m_worldMap.begin() ; iter != m_worldMap.end() ; iter ++)
		{
			delete iter->second;
		}
		m_worldMap.clear();
		Root::getSingleton().removeMovableObjectFactory(&s_OctreeTriggerPlayerFactory);
		Root::getSingleton().removeMovableObjectFactory(&s_OctreeSphereTriggerEntityFactory);
		Root::getSingleton().removeMovableObjectFactory(&s_OctreeBoxTriggerEntityFactory);
	}

	// 创建一个触发器世界
	ITriggerWorld *OctreeTriggerSystem::createWorld(const String &name)
	{
		if(hasWorld(name))
		{
			return 0;
		}
		ITriggerWorld *world = new OctreeTriggerWorld(name);
		m_worldMap[name] = world;
		return world;
	}

	// 获得一个触发器世界
	ITriggerWorld *OctreeTriggerSystem::getWorld(const String &name)
	{
		TriggerWorldMap::iterator iter = m_worldMap.find(name);
		if(iter != m_worldMap.end())
		{
			return iter->second;
		}
		else
		{
			return 0;
		}
	}

	// 是否有这个名称的触发器世界
	bool OctreeTriggerSystem::hasWorld(const String &name)
	{
		return m_worldMap.find(name) != m_worldMap.end();
	}

	// 销毁一个触发器世界
	void OctreeTriggerSystem::destroyWorld(const String &name)
	{
		TriggerWorldMap::iterator iter = m_worldMap.find(name);
		if(iter != m_worldMap.end())
		{
			delete iter->second;
			m_worldMap.erase(iter);
		}
	}

	// 销毁一个触发器世界
	void OctreeTriggerSystem::destroyWorld(ITriggerWorld *world)
	{
		destroyWorld(world->getName());
	}

};