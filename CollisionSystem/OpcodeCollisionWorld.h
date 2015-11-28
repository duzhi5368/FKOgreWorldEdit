#pragma once
#include "ICollisionWorld.h"
namespace Opcode
{
	class AABBTreeCollider;
	struct BVTCache;
	class RayCollider;
	class CollisionFaces;
}

namespace IceMaths
{
	class Matrix4x4;
	class Point;
}

namespace Ogre
{
	class OpcodeCollisionWorld : public ICollisionWorld
	{
	public:
		OpcodeCollisionWorld(const String &name);
		virtual ~OpcodeCollisionWorld(void);


		// 获得名称
		virtual const String &getName()const
		{
			return m_name;
		}

		// 测试ray和col之间的碰撞，返回碰撞信息，返回true则有碰撞
		virtual bool collide(const Ray& ray
			, const ICollisionShapePtr& col
			, const Matrix4& worldTrans
			, RayCollisionInfo& collisionInfo
			);

		// 从一个Entity创建碰撞实例
		virtual ICollisionEntity* createEntityCollisionEntity(Entity *entity , CollisionShapeType type = CT_NONE , const String &shapeName = StringUtil::BLANK);
		// 销毁碰撞实例
		virtual void destroyCollisionEntity(ICollisionEntity *entity);

	protected:
		String m_name;
		// 从Ogre矩阵转换到OpCode矩阵
		IceMaths::Matrix4x4* getIceMathsMatrix(const Matrix4& matrix) const;

		Opcode::AABBTreeCollider* mCollider; //opcode collider
		Opcode::BVTCache* mColCache; //opcode cache
		Opcode::RayCollider* mRayCollider; //ray collider
		Opcode::CollisionFaces* mRayColfaces;	//ray collision faces

	};

};