#include "StdAfx.h"
#include "MathEx.h"
namespace Ogre
{
	// 从欧拉角转换到四元数
	Quaternion MathEx::EularToQuaternion(const Vector3 &eular)
	{
		Matrix3 mat;
		mat.FromEulerAnglesXYZ(Radian(eular.x) , Radian(eular.y) , Radian(eular.z));
		return Quaternion(mat);
	}
	// 从四元数转换到欧拉角
	Vector3 MathEx::QuaternionToEular(const Quaternion &orientation)
	{
		Matrix3 mat;
		orientation.ToRotationMatrix(mat);
		Radian x , y , z;
		mat.ToEulerAnglesXYZ(x , y , z);
		return Vector3(x.valueRadians() , y.valueRadians() , z.valueRadians());
	}
	// 返回从一个位置指向另外一个位置的方向的四元数
	Quaternion MathEx::PositionToQuaternion(const Vector3 &srcPosition , const Vector3 &dstPosition , const Vector3 &forward)
	{
		return DirectionToQuaternion((dstPosition - srcPosition).normalisedCopy() , forward);
	}

	// 从单位方向转换到四元数
	Quaternion MathEx::DirectionToQuaternion(const Vector3 &direction , const Vector3 &forward)
	{
		return forward.getRotationTo(direction);
	}
	// 从四元数转换到单位方向
	Vector3 MathEx::QuaternionToDirectionZ(const Quaternion &orientation)
	{
		return orientation.zAxis();
	}
	Vector3 MathEx::QuaternionToDirectionY(const Quaternion &orientation)
	{
		return orientation.yAxis();
	}
	Vector3 MathEx::QuaternionToDirectionX(const Quaternion &orientation)
	{
		return orientation.xAxis();
	}

	// 计算圆上一点相对于x轴的角度
	Radian MathEx::calcCircleRadian(const Vector2 &center , const Vector2 &pointOnCircle)
	{
		return calcCircleRadian(pointOnCircle - center);
	}
	// 计算圆上一点相对于x轴的角度，假设圆心为0,0
	Radian MathEx::calcCircleRadian(const Vector2 &pointOnCircle)
	{
		float k = 0.0f;
		// 在第1/2象限
		if(pointOnCircle.x >= 0)
		{
			k = 0.0f;
		}
		else
		{
			k = 1.0f;
		}
		return Radian(k * Math::PI + atan(pointOnCircle.y / pointOnCircle.x));
	}


};