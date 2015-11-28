#include "StdAfx.h"
#include "OctreeTriggerWorld.h"
#include "OctreeBoxTriggerEntity.h"
#include "OctreeSphereTriggerEntity.h"
#include "OctreeTriggerPlayer.h"

namespace Ogre
{

	OctreeTriggerWorld::OctreeTriggerWorld(const String &name)
		: m_name(name)
		, m_sceneMgr(0)
	{
#ifdef _DEBUG
		m_sceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager" , m_name);
#else
		m_sceneMgr = Root::getSingleton().createSceneManager("OctreeSceneManager" , m_name);
#endif
		Root::getSingleton().addFrameListener(this);
	}

	OctreeTriggerWorld::~OctreeTriggerWorld(void)
	{
		Root::getSingleton().removeFrameListener(this);
		destroyAllTriggerEntity();
		Root::getSingleton().destroySceneManager(m_sceneMgr);
	}

	// 每帧更新场景管理器
	bool OctreeTriggerWorld::frameStarted(const FrameEvent& evt)
	{
		// Process queued needUpdate calls 
		Node::processQueuedUpdates();

		// Cascade down the graph updating transforms & world bounds
		// In this implementation, just update from the root
		// Smarter SceneManager subclasses may choose to update only
		//   certain scene graph branches
		m_sceneMgr->getRootSceneNode()->_update(true, false);

		return true;

	}


	// 创建盒子触发器实例
	IBoxTriggerEntity* OctreeTriggerWorld::createBoxTriggerEntity(const String &name)
	{
		if(hasTriggerEntity(name , OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"触发器名称为 '" + name + "' 的触发器已经存在", 
				"OctreeTriggerWorld::createBoxTriggerEntity");
			return 0;
		}
		NameValuePairList params;
		params["world"] = StringConverter::toString((long)this);
		OctreeBoxTriggerEntity *obj = static_cast<OctreeBoxTriggerEntity*>(m_sceneMgr->createMovableObject(name , OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME , &params));
		return obj;

	}
	// 创建球体触发器实例
	ISphereTriggerEntity* OctreeTriggerWorld::createSphereTriggerEntity(const String &name)
	{
		if(hasTriggerEntity(name , OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"触发器名称为 '" + name + "' 的触发器已经存在", 
				"OctreeTriggerWorld::createSphereTriggerEntity");
			return 0;
		}
		NameValuePairList params;
		params["world"] = StringConverter::toString((long)this);
		OctreeSphereTriggerEntity *obj = static_cast<OctreeSphereTriggerEntity*>(m_sceneMgr->createMovableObject(name , OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME , &params));
		return obj;
	}
	// 创建玩家实例
	ITriggerPlayer * OctreeTriggerWorld::createTriggerPlayer(const String &name)
	{
		if(hasTriggerEntity(name , OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"触发器名称为 '" + name + "' 的触发器已经存在", 
				"OctreeTriggerWorld::createTriggerPlayer");
			return 0;
		}
		NameValuePairList params;
		params["world"] = StringConverter::toString((long)this);
		OctreeTriggerPlayer *obj = static_cast<OctreeTriggerPlayer*>(m_sceneMgr->createMovableObject(name , OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME , &params));
		return obj;
	}

	// 销毁所有触发器实例
	void OctreeTriggerWorld::destroyAllTriggerEntity()
	{
		m_sceneMgr->destroyAllMovableObjectsByType(OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME);
		m_sceneMgr->destroyAllMovableObjectsByType(OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME);
		m_sceneMgr->destroyAllMovableObjectsByType(OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME);
	}

	// 销毁触发器
	void OctreeTriggerWorld::destroyTriggerEntity(const String &name , const String &triggerType)
	{
		m_sceneMgr->destroyMovableObject(name , triggerType);
	}

	void OctreeTriggerWorld::destroyTriggerEntity(ITriggerEntity *obj)
	{
		destroyTriggerEntity(obj->getName() , obj->getType());
	}

	// 获得一个触发器
	ITriggerEntity *OctreeTriggerWorld::getTriggerEntity(const String &name , const String &triggerType)const
	{
		MovableObject *obj = m_sceneMgr->getMovableObject(name , triggerType);
		return (ITriggerEntity*)obj;
		//if(!obj)
		//{
		//	return 0;
		//}
		//if(obj->getMovableType() == OctreeBoxTriggerEntityFactory::FACTORY_TYPE_NAME)
		//	return static_cast<OctreeBoxTriggerEntity*>(obj);
		//if(obj->getMovableType() == OctreeSphereTriggerEntityFactory::FACTORY_TYPE_NAME)
		//	return static_cast<OctreeSphereTriggerEntity*>(obj);
		//if(obj->getMovableType() == OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME)
		//	return static_cast<OctreeTriggerPlayer*>(obj);
		//return 0;
	}
	// 是否存在指定名字的触发器
	bool OctreeTriggerWorld::hasTriggerEntity(const String &name , const String &triggerType)const
	{
		return m_sceneMgr->hasMovableObject(name , triggerType);
	}	

	// 获得所有对象的列表
	OctreeTriggerWorld::TriggerEntityIterator OctreeTriggerWorld::getTriggerEntityIterator(const String &triggerType)
	{
		SceneManager::MovableObjectIterator iter = m_sceneMgr->getMovableObjectIterator(triggerType);
		return (TriggerEntityIterator&)iter;
	}


	// 获得所有玩家的列表
	OctreeTriggerWorld::TriggerPlayerIterator OctreeTriggerWorld::getTriggerPlayerIterator()
	{
		SceneManager::MovableObjectIterator iter = m_sceneMgr->getMovableObjectIterator(OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME);
		return (TriggerPlayerIterator&)iter;
	}
};