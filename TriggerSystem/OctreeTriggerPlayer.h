#pragma once
#include "ITriggerPlayer.h"

namespace Ogre
{
	class OctreeTriggerWorld;
	// 玩家的实现
	class OctreeTriggerPlayer : public ITriggerPlayer
		, public MovableObject
	{
	public:
		OctreeTriggerPlayer(const String &name , OctreeTriggerWorld *world);
		virtual ~OctreeTriggerPlayer(void);

		// 获得名称
		virtual const String &getName()const
		{
			return m_name;
		}
		// 获得类型
		virtual const String &getType()const
		{
			return getMovableType();
		};
		// 获得触发器世界
		virtual ITriggerWorld *getTriggerWorld()const;

		// 设置相对坐标
		virtual void setPosition(const Vector3 &pos);
		// 获得相对坐标
		virtual const Vector3 &getPosition()const;

		// 设置相对缩放
		virtual void setScale(const Vector3 &scale);
		// 获得相对坐标
		virtual const Vector3 &getScale()const;


		// 设置相对方位
		virtual void setOrientation(const Quaternion &orientation);
		// 获得相对坐标
		virtual const Quaternion &getOrientation()const;

		// 设置盒子
		virtual void setBox(const AxisAlignedBox &box)
		{
			Vector3 size = m_aabb.getHalfSize();
			m_radius = std::max(std::max(size.x , size.z) , size.y);
			m_aabb = box;
		}
		// 获得盒子
		virtual const AxisAlignedBox &getBox()
		{
			return m_aabb;
		}

		virtual const String& getMovableType(void) const;
		virtual const AxisAlignedBox& getBoundingBox(void) const
		{
			return m_aabb;
		}
		virtual Real getBoundingRadius(void) const
		{
			return m_radius;
		}
		virtual void _updateRenderQueue(RenderQueue* queue){};

		// 更新触发器数据，调用这个会导致一次遍历查询，并回调
		virtual void updateTrigger();


		// 获得所有与之发生碰撞的触发器
		virtual CollideTriggerEntityIterator getCollideTriggerEntityIterator()
		{
			return CollideTriggerEntityIterator(m_mapCollideTriggerEntity.begin() , m_mapCollideTriggerEntity.end());
		}


		// 是否与玩家发生了碰撞
		virtual bool collide(ITriggerPlayer *player)
		{
			return true;
		}

		// 返回有向包围盒
		const OBB &getOBB()const
		{
			return m_obb;
		}

	protected:
		// 名称
		String m_name;
		// 八叉树世界
		OctreeTriggerWorld *m_world;
		// 场景管理器
		SceneManager *m_sceneMgr;
		// 八叉树节点
		SceneNode *m_node;
		// 包围半径
		Real m_radius;
		// 包围和
		AxisAlignedBox m_aabb;
		// 与之发生碰撞的触发器列表
		MapCollideTriggerEntity m_mapCollideTriggerEntity;

		// 有向包围盒
		OBB m_obb;

		// 包围盒查询
		AxisAlignedBoxSceneQuery* m_query;
	};


	/** Factory object for creating Entity instances */
	class OctreeTriggerPlayerFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		OctreeTriggerPlayerFactory() {}
		virtual ~OctreeTriggerPlayerFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);

	};
};