#include "StdAfx.h"
#include "OctreeTriggerPlayer.h"
#include "OctreeTriggerWorld.h"

namespace Ogre
{
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME = "OctreeTriggerPlayer";
	//-----------------------------------------------------------------------
	const String& OctreeTriggerPlayerFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	MovableObject* OctreeTriggerPlayerFactory::createInstanceImpl( const String& name,
		const NameValuePairList* params)
	{
		OctreeTriggerWorld *world = 0;
		if (params != 0)
		{
			NameValuePairList::const_iterator ni = params->find("world");
			if (ni != params->end())
			{
				world = (OctreeTriggerWorld*)StringConverter::parseLong(ni->second);
			}

		}
		if (world == 0)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"'world' parameter required when constructing an OctreeTriggerPlayer.",
				"OctreeTriggerPlayerFactory::createInstance");
		}

		return new OctreeTriggerPlayer(name, world);
	}
	//-----------------------------------------------------------------------
	void OctreeTriggerPlayerFactory::destroyInstance( MovableObject* obj)
	{
		delete obj;
	}
	const String& OctreeTriggerPlayer::getMovableType(void) const
	{
		return OctreeTriggerPlayerFactory::FACTORY_TYPE_NAME;
	}

	OctreeTriggerPlayer::OctreeTriggerPlayer(const String &name , OctreeTriggerWorld *world)
		: m_name(name)
		, m_world(world)
		, m_sceneMgr(0)
		, m_node(0)
		, m_radius(1)
		, m_query(0)
		, m_aabb(-0.5 , -0.5 , -0.5 , 0.5 , 0.5 , 0.5)
		, MovableObject(name)
	{
		m_sceneMgr = world->getSceneManager();
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(name);
		m_node->attachObject(this);
		// 玩家不参加碰撞计算
		setQueryFlags(0);
		MovableObject::setUserAny(Any(static_cast<ITriggerEntity*>(this)));

		// 创建一个缺省的包围盒查询器
		m_query = m_sceneMgr->createAABBQuery(AxisAlignedBox());
	}

	OctreeTriggerPlayer::~OctreeTriggerPlayer(void)
	{
		// 结束前也要更新
		updateTrigger();
		if(m_node)
		{
			m_node->detachObject(this);
			m_sceneMgr->getRootSceneNode()->removeAndDestroyChild(m_node->getName());
			m_node = 0;
		}
		if(m_query)
		{
			m_sceneMgr->destroyQuery(m_query);
			m_query = 0;
		}
	}
	// 获得触发器世界
	ITriggerWorld *OctreeTriggerPlayer::getTriggerWorld()const
	{
		return m_world;
	}

	// 设置相对坐标
	void OctreeTriggerPlayer::setPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos);
	}
	// 获得相对坐标
	const Vector3 &OctreeTriggerPlayer::getPosition()const
	{
		return m_node->getPosition();
	}

	// 设置相对缩放
	void OctreeTriggerPlayer::setScale(const Vector3 &scale)
	{
		m_node->setScale(scale);
	}
	// 获得相对坐标
	const Vector3 &OctreeTriggerPlayer::getScale()const
	{
		return m_node->getScale();
	}

	// 设置相对方位
	void OctreeTriggerPlayer::setOrientation(const Quaternion &orientation)
	{
		m_node->setOrientation(orientation);
	}
	// 获得相对坐标
	const Quaternion &OctreeTriggerPlayer::getOrientation()const
	{
		return m_node->getOrientation();
	}

	// 更新触发器数据，调用这个会导致一次遍历查询，并回调
	void OctreeTriggerPlayer::updateTrigger()
	{
		if(m_aabb.isNull())
			return;
		// 更新有向包围盒信息
		m_obb.setCenter(getPosition());
		m_obb.setExtents(getBoundingBox().getHalfSize() * getScale());
		m_obb.setOrientation(getOrientation());

		m_query->setBox(getWorldBoundingBox(true));
		SceneQueryResult& result = m_query->execute();

		// 现在发生了碰撞的列表
		MapCollideTriggerEntity mapCollideTriggerEntity;

		// 遍历结果
		for(SceneQueryResultMovableList::iterator iter = result.movables.begin() ; iter != result.movables.end() ; iter ++)
		{
			// 先转换成ITriggerEntity
			ITriggerEntity *entity = trigger_cast((*iter)->getUserAny());
			if(entity)
			{
				// 做碰撞计算，如果发生碰撞了
				if(entity->collide(this))
				{				
					// 添加进去
					mapCollideTriggerEntity[entity->getName()] = entity;
				}
			}
		}

		// 遍历发生碰撞的结果
		for(MapCollideTriggerEntity::iterator iter = mapCollideTriggerEntity.begin() ; iter != mapCollideTriggerEntity.end() ; iter ++)
		{
			// 如果不在里面，就说明进去了
			if(m_mapCollideTriggerEntity.find(iter->second->getName()) == m_mapCollideTriggerEntity.end())
			{
				// 回调到玩家
				const ITriggerPlayer::ListListener &listPlayer = getListenerList();
				for(size_t i = 0 ; i < listPlayer.size() ; i ++)
				{
					listPlayer[i]->onEnter(iter->second , this);
				}
				// 回调到触发器
				const ITriggerEntity::ListListener &listEntity = iter->second->getListenerList();
				for(size_t i = 0 ; i < listEntity.size() ; i ++)
				{
					listEntity[i]->onEnter(iter->second , this);
				}
				// 回调到世界
				const ITriggerWorld::ListListener &listWorld = m_world->getListenerList();
				for(size_t i = 0 ; i < listWorld.size() ; i ++)
				{
					listWorld[i]->onEnter(iter->second , this);
				}
			}
		}

		// 遍历未发生碰撞的结果
		for(MapCollideTriggerEntity::iterator iter = m_mapCollideTriggerEntity.begin() ; iter != m_mapCollideTriggerEntity.end() ; iter ++)
		{
			// 如果不在里面，就说明出去了
			if(mapCollideTriggerEntity.find(iter->second->getName()) == mapCollideTriggerEntity.end())
			{
				// 回调到玩家
				const ITriggerPlayer::ListListener &listPlayer = getListenerList();
				for(size_t i = 0 ; i < listPlayer.size() ; i ++)
				{
					listPlayer[i]->onLeave(iter->second , this);
				}
				// 回调到触发器
				const ITriggerEntity::ListListener &listEntity = iter->second->getListenerList();
				for(size_t i = 0 ; i < listEntity.size() ; i ++)
				{
					listEntity[i]->onLeave(iter->second , this);
				}
				// 回调到世界
				const ITriggerWorld::ListListener &listWorld = m_world->getListenerList();
				for(size_t i = 0 ; i < listWorld.size() ; i ++)
				{
					listWorld[i]->onLeave(iter->second , this);
				}
			}
		}

		// 保存新的结果
		m_mapCollideTriggerEntity = mapCollideTriggerEntity;
	}

};