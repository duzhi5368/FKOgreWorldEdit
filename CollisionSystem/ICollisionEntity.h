#pragma once
#include "ICollisionShape.h"
namespace Ogre
{
	// 碰撞模型类型
	enum CollisionShapeType
	{
		CT_NONE = 0,		// 没有碰撞模型
		CT_SELF_MESH,		// 本Entity的Mesh
		CT_SELF_SUBMESH,	// 本Entity的Mesh中的一个SubMesh
		CT_OTHER_MESH,		// 外部Mesh

		CT_UNKNOWN = 0xFFFF,// 未知类型
	};
	class ICollisionWorld;
	// 碰撞实体
	class COLLISIONSYSTEM_API ICollisionEntity
	{
	public:
		ICollisionEntity(void){}
		virtual ~ICollisionEntity(void){}

		// 是否已经初始化
		virtual bool isInited()const = 0;

		// 获得碰撞模型
		virtual ICollisionShapePtr &getCollisionShape() = 0;

		// 测试ray和本实体之间的碰撞，返回碰撞信息，返回true则有碰撞
		virtual bool collide(const Ray& ray , RayCollisionInfo& collisionInfo) = 0;

		// 获得碰撞世界
		virtual ICollisionWorld *getCollisionWorld()const = 0;

		// 设置碰撞模型类型
		virtual void setCollisionShapeType(CollisionShapeType type) = 0;
		// 获得碰撞模型类型
		virtual CollisionShapeType getCollisionShapeType()const = 0;
		// 设置碰撞模型
		virtual void setCollisionShapeName(const String &name) = 0;
		// 获得碰撞模型名称
		virtual const String &getCollisionShapeName()const = 0;

	};
};