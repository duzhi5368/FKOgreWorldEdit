#pragma once
#include "IBoxTriggerEntity.h"

namespace Ogre
{
	class OctreeTriggerWorld;
	// 盒子触发器的实现
	class OctreeBoxTriggerEntity : public IBoxTriggerEntity
		, public MovableObject
	{
	public:
		OctreeBoxTriggerEntity(const String &name , OctreeTriggerWorld *world);
		virtual ~OctreeBoxTriggerEntity(void);

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
		virtual void setBox(const AxisAlignedBox &box);
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

		// 移动时更新所有玩家
		virtual void _notifyMoved(void);

		// 是否与玩家发生了碰撞
		virtual bool collide(ITriggerPlayer *player);

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
		// 包围盒
		AxisAlignedBox m_aabb;

		// 有向包围盒
		OBB m_obb;
	};

	/** Factory object for creating Entity instances */
	class OctreeBoxTriggerEntityFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		OctreeBoxTriggerEntityFactory() {}
		virtual ~OctreeBoxTriggerEntityFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);

	};
};