#include "StdAfx.h"
#include "OrientedBox.h"

namespace Ogre
{

	//------------------------------------------------------------------------
	bool OrientedBox::intersects( const AxisAlignedBox& box ) const
	{ 
		// Optimize this: return intersects( OrientedBox(box) );

		//translation, in parent frame (note: A = this, B = obb)		   
		Vector3 v = box.getCenter() - m_center;		   
		Vector3 boxExtents = (box.getMaximum() - box.getMinimum()) * 0.5;

		//translation, in A's frame
		Vector3 T( v.dotProduct( m_rot.GetColumn(0)),
			v.dotProduct( m_rot.GetColumn(1)),
			v.dotProduct( m_rot.GetColumn(2)) );

		//B's basis with respect to A's local frame
		Real R[3][3];
		Real FR[3][3];
		Real ra, rb, t;
		long i, k;
		//calculate rotation matrix from "obb" to "this" - palso recomputes the fabs matrix
		for( i=0 ; i<3 ; i++ )
		{
			for( k=0 ; k<3 ; k++ )
			{
				R[i][k] = m_rot.GetColumn(k)[i];
				// fabs and shift borrowed from RAPID
				FR[i][k] = 1e-6f + Math::Abs(R[i][k]);
			}
		}

		// Separating axis theorem: test of all 15 potential separating axes
		// These axes are always parallel to each OBB edges or its normal plane
		const Vector3 &a = m_extents,
			&b = boxExtents;

		// First stage: each obb's axis!
		//A's basis vectors
		for( i=0 ; i<3 ; i++ )
		{
			ra = a[i];
			rb = b.x*FR[i][0] + b.y*FR[i][1] + b.z*FR[i][2];
			t = Math::Abs( T[i] );

			if( t > ra + rb ) return false;
		}

		//B's basis vectors
		for( k=0 ; k<3 ; k++ )
		{
			ra = a.x*FR[0][k] + a.y*FR[1][k] + a.z*FR[2][k];
			rb = b[k];
			t  = Math::Abs( T[0]*R[0][k] + T[1]*R[1][k] + T[2]*R[2][k] );

			if( t > ra + rb ) return false;
		}

		// Second stage: 9 cross products
		//L = A0 x B0
		ra = a[1]*FR[2][0] + a[2]*FR[1][0];
		rb = b[1]*FR[0][2] + b[2]*FR[0][1];
		t  = Math::Abs( T[2]*R[1][0] -  T[1]*R[2][0] );
		if( t > ra + rb ) return false;

		//L = A0 x B1
		ra = a[1]*FR[2][1] + a[2]*FR[1][1];
		rb = b[0]*FR[0][2] + b[2]*FR[0][0];
		t = Math::Abs( T[2]*R[1][1] - T[1]*R[2][1] );
		if( t > ra + rb ) return false;

		//L = A0 x B2
		ra = a[1]*FR[2][2] + a[2]*FR[1][2];
		rb = b[0]*FR[0][1] + b[1]*FR[0][0];
		t = Math::Abs( T[2]*R[1][2] - T[1]*R[2][2] );
		if( t > ra + rb ) return false;

		//L = A1 x B0
		ra = a[0]*FR[2][0] + a[2]*FR[0][0];
		rb = b[1]*FR[1][2] + b[2]*FR[1][1];
		t = Math::Abs( T[0]*R[2][0] - T[2]*R[0][0] );
		if( t > ra + rb ) return false;

		//L = A1 x B1
		ra = a[0]*FR[2][1] + a[2]*FR[0][1];
		rb = b[0]*FR[1][2] + b[2]*FR[1][0];
		t = Math::Abs( T[0]*R[2][1] - T[2]*R[0][1] );
		if( t > ra + rb ) return false;

		//L = A1 x B2
		ra = a[0]*FR[2][2] + a[2]*FR[0][2];
		rb = b[0]*FR[1][1] + b[1]*FR[1][0];
		t = Math::Abs( T[0]*R[2][2] - T[2]*R[0][2] );
		if( t > ra + rb ) return false;

		//L = A2 x B0
		ra = a[0]*FR[1][0] + a[1]*FR[0][0];
		rb = b[1]*FR[2][2] + b[2]*FR[2][1];
		t =  Math::Abs( T[1]*R[0][0] - T[0]*R[1][0] );
		if( t > ra + rb ) return false;

		//L = A2 x B1
		ra = a[0]*FR[1][1] + a[1]*FR[0][1];
		rb = b[0]*FR[2][2] + b[2]*FR[2][0];
		t = Math::Abs( T[1]*R[0][1] - T[0]*R[1][1] );
		if( t > ra + rb ) return false;

		//L = A2 x B2
		ra = a[0]*FR[1][2] + a[1]*FR[0][2];
		rb = b[0]*FR[2][1] + b[1]*FR[2][0];
		t = Math::Abs( T[1]*R[0][2] - T[0]*R[1][2] );
		if( t > ra + rb )	return false;


		// Phew! No separating axis found, no overlap!
		return true;
	}
	//------------------------------------------------------------------------
	bool OrientedBox::intersects( const OrientedBox& obb ) const
	{           
		//translation, in parent frame (note: A = this, B = obb)
		Vector3 v = obb.m_center - m_center;		   
		//translation, in A's frame
		Vector3 T( v.dotProduct( m_rot.GetColumn(0)),
			v.dotProduct( m_rot.GetColumn(1)),
			v.dotProduct( m_rot.GetColumn(2)) );

		//B's basis with respect to A's local frame
		Real R[3][3];
		Real FR[3][3];
		Real ra, rb, t;
		long i, k;
		//calculate rotation matrix from "obb" to "this" - palso recomputes the fabs matrix
		for( i=0 ; i<3 ; i++ )
		{
			for( k=0 ; k<3 ; k++ )
			{
				R[i][k] = m_rot.GetColumn(i).dotProduct(obb.m_rot.GetColumn(k) );
				// fabs and shift borrowed from RAPID
				FR[i][k] = 1e-6f + Math::Abs(R[i][k]);
			}
		}

		// Separating axis theorem: test of all 15 potential separating axes
		// These axes are always parallel to each OBB edges or its normal plane
		const Vector3 &a = m_extents,
			&b = obb.m_extents;

		// First stage: each obb's axis!
		//A's basis vectors
		for( i=0 ; i<3 ; i++ )
		{
			ra = a[i];
			rb = b.x*FR[i][0] + b.y*FR[i][1] + b.z*FR[i][2];
			t = Math::Abs( T[i] );

			if( t > ra + rb ) return false;
		}

		//B's basis vectors
		for( k=0 ; k<3 ; k++ )
		{
			ra = a.x*FR[0][k] + a.y*FR[1][k] + a.z*FR[2][k];
			rb = b[k];
			t  = Math::Abs( T[0]*R[0][k] + T[1]*R[1][k] + T[2]*R[2][k] );

			if( t > ra + rb ) return false;
		}

		// Second stage: 9 cross products
		//L = A0 x B0
		ra = a[1]*FR[2][0] + a[2]*FR[1][0];
		rb = b[1]*FR[0][2] + b[2]*FR[0][1];
		t  = Math::Abs( T[2]*R[1][0] -  T[1]*R[2][0] );
		if( t > ra + rb ) return false;

		//L = A0 x B1
		ra = a[1]*FR[2][1] + a[2]*FR[1][1];
		rb = b[0]*FR[0][2] + b[2]*FR[0][0];
		t = Math::Abs( T[2]*R[1][1] - T[1]*R[2][1] );
		if( t > ra + rb ) return false;

		//L = A0 x B2
		ra = a[1]*FR[2][2] + a[2]*FR[1][2];
		rb = b[0]*FR[0][1] + b[1]*FR[0][0];
		t = Math::Abs( T[2]*R[1][2] - T[1]*R[2][2] );
		if( t > ra + rb ) return false;

		//L = A1 x B0
		ra = a[0]*FR[2][0] + a[2]*FR[0][0];
		rb = b[1]*FR[1][2] + b[2]*FR[1][1];
		t = Math::Abs( T[0]*R[2][0] - T[2]*R[0][0] );
		if( t > ra + rb ) return false;

		//L = A1 x B1
		ra = a[0]*FR[2][1] + a[2]*FR[0][1];
		rb = b[0]*FR[1][2] + b[2]*FR[1][0];
		t = Math::Abs( T[0]*R[2][1] - T[2]*R[0][1] );
		if( t > ra + rb ) return false;

		//L = A1 x B2
		ra = a[0]*FR[2][2] + a[2]*FR[0][2];
		rb = b[0]*FR[1][1] + b[1]*FR[1][0];
		t = Math::Abs( T[0]*R[2][2] - T[2]*R[0][2] );
		if( t > ra + rb ) return false;

		//L = A2 x B0
		ra = a[0]*FR[1][0] + a[1]*FR[0][0];
		rb = b[1]*FR[2][2] + b[2]*FR[2][1];
		t =  Math::Abs( T[1]*R[0][0] - T[0]*R[1][0] );
		if( t > ra + rb ) return false;

		//L = A2 x B1
		ra = a[0]*FR[1][1] + a[1]*FR[0][1];
		rb = b[0]*FR[2][2] + b[2]*FR[2][0];
		t = Math::Abs( T[1]*R[0][1] - T[0]*R[1][1] );
		if( t > ra + rb ) return false;

		//L = A2 x B2
		ra = a[0]*FR[1][2] + a[1]*FR[0][2];
		rb = b[0]*FR[2][1] + b[1]*FR[2][0];
		t = Math::Abs( T[1]*R[0][2] - T[0]*R[1][2] );
		if( t > ra + rb )	return false;


		// Phew! No separating axis found, no overlap!
		return true;
	}
	//------------------------------------------------------------------------


	/** Is this oriented box intersecting the given Sphere?
	*/
	bool OrientedBox::intersects( const Sphere& s )const
	{
		//  Modified from Magic-Software http://www.magic-software.com
		Vector3 kCDiff = s.getCenter() - m_center;

		Real fAx = Math::Abs(kCDiff.dotProduct(m_rot.GetColumn(0)) );
		Real fAy = Math::Abs(kCDiff.dotProduct(m_rot.GetColumn(1)) );
		Real fAz = Math::Abs(kCDiff.dotProduct(m_rot.GetColumn(2)) );
		Real fDx = fAx - m_extents.x;
		Real fDy = fAy - m_extents.y;
		Real fDz = fAz - m_extents.z;

		if ( fAx <= m_extents[0] )
		{
			if ( fAy <= m_extents[1] )
			{
				if ( fAz <= m_extents[2] )
					// Sphere m_center inside box
					return true;
				else
					// potential Sphere-face intersection with face z
					return fDz <= s.getRadius();
			}
			else
			{
				if ( fAz <= m_extents[2] )
					// potential Sphere-face intersection with face y
					return fDy <= s.getRadius();
				else
					// potential Sphere-edge intersection with edge formed
					// by faces y and z							
					return fDy*fDy + fDz*fDz <= (s.getRadius()*s.getRadius());
			}
		}
		else
		{
			if ( fAy <= m_extents[1] )
			{
				if ( fAz <= m_extents[2] )
					// potential Sphere-face intersection with face x
					return fDx <= s.getRadius();
				else
					// potential Sphere-edge intersection with edge formed
					// by faces x and z
					return fDx*fDx + fDz*fDz <= (s.getRadius()*s.getRadius());
			}
			else
			{
				if ( fAz <= m_extents[2] )
					// potential Sphere-edge intersection with edge formed
					// by faces x and y
					return fDx*fDx + fDy*fDy <= (s.getRadius()*s.getRadius());
				else
					// potential Sphere-vertex intersection at corner formed
					// by faces x,y,z
					return fDx*fDx + fDy*fDy + fDz*fDz <= (s.getRadius()*s.getRadius());
			}
		}
	}

};