#pragma once

namespace Ogre
{
	// 碰撞信息
	struct RayCollisionInfo
	{
		RayCollisionInfo()
			: distance(0.0f)
			, a1(Vector3::ZERO)
			, b1(Vector3::ZERO)
			, c1(Vector3::ZERO)
			, a2(Vector3::ZERO)
			, b2(Vector3::ZERO)
			, c2(Vector3::ZERO)
		{

		}
		float distance;				// 沿射线的碰撞距离
		Vector3 a1, b1, c1;			// 第一个碰撞三角形
		Vector3 a2, b2, c2;			// 第二个碰撞三角形
	};

	// 碰撞模型
	class COLLISIONSYSTEM_API ICollisionShape
	{
	public:
		ICollisionShape(void){};
		virtual ~ICollisionShape(void){};

		// 获得名称
		virtual const String &getName()const = 0;
		// 获得类型
		virtual const String &getType()const = 0;

	};
	typedef SharedPtr<ICollisionShape> ICollisionShapePtr;
};