#pragma once
#include "ICollisionShape.h"
#include "ICollisionEntity.h"

namespace Ogre
{
	class ICollisionWorld
	{
	public:
		ICollisionWorld(){};
		virtual ~ICollisionWorld(void){};

		// 获得名称
		virtual const String &getName()const = 0;

		// 测试ray和col之间的碰撞，返回碰撞信息，返回true则有碰撞
		virtual bool collide(const Ray& ray
			, const ICollisionShapePtr& col
			, const Matrix4& worldTrans
			, RayCollisionInfo& collisionInfo
			) = 0;

		// 从一个Entity创建碰撞实例
		virtual ICollisionEntity* createEntityCollisionEntity(Entity *entity , CollisionShapeType type = CT_NONE , const String &shapeName = StringUtil::BLANK) = 0;
		// 销毁碰撞实例
		virtual void destroyCollisionEntity(ICollisionEntity *entity) = 0;
	};
};