#pragma once

namespace Ogre
{
	// 有向包围盒的实现
	class OGREPRO_API OrientedBox
	{
	public:

		OrientedBox()
			: m_center()
			, m_extents(0.5,0.5,0.5)
			, m_rot(Matrix3::IDENTITY)
		{
		}

		OrientedBox( const AxisAlignedBox& box )
			: m_rot(Matrix3::IDENTITY)
		{ 
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
		}

		OrientedBox( const AxisAlignedBox& box , const Matrix3& axes)
			: m_rot(axes)
		{ 
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
		}

		// 拷贝构造
		OrientedBox(const OrientedBox& obb )
			: m_center(obb.m_center)
			, m_extents(obb.m_extents)
			, m_rot(obb.m_rot)
		{
		}

		// 完全构造
		OrientedBox(const Vector3& c, const Vector3& ex, const Matrix3& mat  )
			: m_center(c)
			, m_extents(ex)
			, m_rot(mat)
		{
		}

		// 完全构造
		OrientedBox(const Vector3& c, const Vector3& ex, const Quaternion& quat  )
			: m_center(c)
			, m_extents(ex)
		{
			quat.ToRotationMatrix(m_rot);
		}

		// 设置包围盒和旋转
		void setByAABB(const AxisAlignedBox& box , const Matrix3& axes)
		{
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
			m_rot = axes;
		}
		// 设置包围盒和旋转
		void setByAABB(const AxisAlignedBox& box , const Quaternion& quat)
		{
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
			quat.ToRotationMatrix(m_rot);
		}
		// 设置中心点
		void setCenter(const Vector3 &pos)
		{
			m_center = pos;
		}
		// 设置大小
		void setExtents(const Vector3 &ext)
		{
			m_extents = ext;
		}
		// 设置方向
		void setOrientation(const Matrix3 &mat)
		{
			m_rot = mat;
		}
		// 设置方向
		void setOrientation(const Quaternion &quat)
		{
			quat.ToRotationMatrix(m_rot);
		}

		// 获得大小
		const Vector3& getExtents() const
		{
			return m_extents;
		}

		// 获得中心点
		const Vector3& getCenter() const
		{
			return m_center;
		}

		// 获得方向
		const Matrix3& getOrientation() const
		{
			return m_rot;
		}

		// 计算体积
		inline Real volume() const
		{
			return m_extents.x *m_extents.y *m_extents.z * 8.0;
		}

		// 返回给定点point是否在obb之内
		bool contains(const Vector3& point ) const
		{
			Vector3 L = point - m_center;

			Real coord = m_rot.GetColumn(0).dotProduct( L );
			if( coord > m_extents.x || coord < -m_extents.x ) return false;

			coord = m_rot.GetColumn(1).dotProduct( L );
			if( coord > m_extents.y || coord < -m_extents.y ) return false;

			coord = m_rot.GetColumn(2).dotProduct( L );
			if( coord > m_extents.z || coord < -m_extents.z ) return false;

			return true;
		}

		// 返回包围盒box是否与obb相交
		bool intersects( const AxisAlignedBox& box ) const;

		// 返回另外一个obb是否与此obb相交
		bool intersects( const OrientedBox& obb ) const;

		// 返回一个球体是否与obb相交
		bool intersects( const Sphere& s )const;

	public:

		// 中心点
		Vector3 m_center;

		// 盒子大小（一半长度，相对于轴向包围盒中的getHalfSize）
		Vector3 m_extents;

		// 旋转矩阵
		/// A rotation matrix describing the orientation of this box.
		/// Each of its collumns define the axes corresponding to the orientation of this box.
		Matrix3 m_rot;
	};

	// 更简单的别名：）
	typedef OrientedBox OBB;	
};