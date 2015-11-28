#pragma once
#include "ICollisionEntity.h"
namespace Ogre
{

	// 碰撞实体的实现
	class OpcodeCollisionEntity : public ICollisionEntity
	{
	public:
		OpcodeCollisionEntity(ICollisionWorld *world);
		virtual ~OpcodeCollisionEntity(void);

		// 初始化
		bool init(Entity *entity , CollisionShapeType type , const String &shapeName);
		// 销毁
		void destroy();

		// 是否已经初始化
		bool isInited()const
		{
			return m_entity && m_entity->getParentSceneNode() && !m_shape.isNull();
		}


		// 获得碰撞模型
		virtual ICollisionShapePtr &getCollisionShape()
		{
			return m_shape;
		}

		// 测试ray和本实体之间的碰撞，返回碰撞信息，返回true则有碰撞
		virtual bool collide(const Ray& ray , RayCollisionInfo& collisionInfo);

		// 获得碰撞世界
		virtual ICollisionWorld *getCollisionWorld()const
		{
			return m_world;
		};

		// 设置碰撞模型类型
		void setCollisionShapeType(CollisionShapeType type);
		// 获得碰撞模型类型
		CollisionShapeType getCollisionShapeType()const
		{
			return m_collisionType;
		}
		// 设置碰撞模型
		void setCollisionShapeName(const String &name);
		// 获得碰撞模型名称
		const String &getCollisionShapeName()const
		{
			return m_collisionName;
		}
	protected:
		// 载入碰撞模型
		void loadCollisionShape(CollisionShapeType type , const String &name);
		// 销毁碰撞模型
		void unloadCollisionShape();

		Entity *m_entity;
		ICollisionShapePtr m_shape;
		ICollisionWorld *m_world;
		// 碰撞体类型
		CollisionShapeType m_collisionType;
		// 碰撞模型名称
		String m_collisionName;

	};

};