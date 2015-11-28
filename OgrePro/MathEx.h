#pragma once

namespace Ogre
{
	// 数学加强库
	class OGREPRO_API MathEx
	{
	public:
		// 从欧拉角转换到四元数，欧拉角的分量用弧度表示
		static Quaternion EularToQuaternion(const Vector3 &eular);
		// 从四元数转换到欧拉角，欧拉角的分量用弧度表示
		static Vector3 QuaternionToEular(const Quaternion &orientation);

		// 返回从一个位置指向另外一个位置的方向的四元数
		static Quaternion PositionToQuaternion(const Vector3 &srcPosition , const Vector3 &dstPosition , const Vector3 &forward = Vector3::NEGATIVE_UNIT_Z);

		// 从单位方向转换到四元数
		static Quaternion DirectionToQuaternion(const Vector3 &direction , const Vector3 &forward = Vector3::NEGATIVE_UNIT_Z);
		// 从四元数转换到单位方向
		static Vector3 QuaternionToDirectionZ(const Quaternion &orientation);
		static Vector3 QuaternionToDirectionY(const Quaternion &orientation);
		static Vector3 QuaternionToDirectionX(const Quaternion &orientation);

		// 计算从初速度加速到末速度所需的时间
		static float calcTime(float startVelocity , float endVelocity , float accel)
		{
			// t = (v2 - v2) / a
			return (endVelocity - startVelocity) / accel;
		}

		// 计算从初速度加速到末速度所需的路程
		static float calcDistanceByTime(float startVelocity , float t , float accel)
		{
			// S = v1*t + 1/2 * a * t * t
			return startVelocity * t + 0.5f * accel * t * t;
		}

		// 计算从初速度加速到末速度所需的路程
		static float calcDistance(float startVelocity , float endVelocity , float accel)
		{
			float t = calcTime(startVelocity , endVelocity , accel);
			// S = v1*t + 1/2 * a * t * t
			return startVelocity * t + 0.5f * accel * t * t;
		}

		// 根据末速度、加速度、距离计算初速度
		static float calcStartVelocity(float endVelocity , float accel , float distance)
		{
			return Math::Sqrt(endVelocity * endVelocity - 2.0f * accel * distance);
		}
		// 计算末速度
		static float calcEndVelocity(float startVelocity , float accel , float distance)
		{
			float t = (Math::Sqrt(startVelocity * startVelocity + 2.0f * accel * distance) - startVelocity) / accel;
			return startVelocity + accel * t;
		}
		// 计算中间最大速度
		static float calcMiddleVelocity(float startVelocity , float accel , float distance)
		{
			return Math::Sqrt(accel * distance + 0.5f * startVelocity * startVelocity);
		}
		// 计算到达中间最大速度所需的路程
		static float calcMiddleDistance(float startVelocity , float accel , float distance)
		{
			float v2 = calcMiddleVelocity(startVelocity , accel , distance);
			return MathEx::calcDistance(startVelocity , v2 , accel);
		}

		// 计算圆上一点相对于x轴的角度
		static Radian calcCircleRadian(const Vector2 &center , const Vector2 &pointOnCircle);
		// 计算圆上一点相对于x轴的角度，假设圆心为0,0
		static Radian calcCircleRadian(const Vector2 &pointOnCircle);

	};
};