#include "ETPrerequisites.h"
#include "ETTerrainInfo.h"
#include <OgreVector3.h>
#include <OgreAxisAlignedBox.h>
#include <OgreException.h>
#include <OgreRay.h>


using namespace std;

namespace Ogre
{
	namespace Impl
	{
		struct InfoImpl
		{
			/** extents of the heightmap */
			size_t width, height;
			/** the heightmap */
			vector<float> heightmap;

			/** offset at which the (0, 0) point of the terrain is placed in Ogre */
			Vector3 offset;
			/** scale of a terrain vertex */
			Vector3 scale;
		};
	}

	TerrainInfo::TerrainInfo()
		: mWidth(0)
		, mHeight(0)
		, mOffset(Vector3::ZERO)
		, mScale(Vector3::UNIT_SCALE)
		, mAutoDelete(false)
		, mHeightmap(0)
	{
	}

	TerrainInfo::TerrainInfo(size_t width, size_t height, float* heightmap , bool autoDelete)
		: mWidth(0)
		, mHeight(0)
		, mOffset(Vector3::ZERO)
		, mScale(Vector3::UNIT_SCALE)
		, mAutoDelete(autoDelete)
		, mHeightmap(0)
	{
		setHeightmap(width, height, heightmap , autoDelete);
	}
	TerrainInfo::~TerrainInfo()
	{
		if(mAutoDelete)
		{
			OGRE_DELETE_ARRAY(mHeightmap);
		}

	}


	void TerrainInfo::setHeightmap(size_t width, size_t height, float* heightmap , bool autoDelete)
	{
		if(mAutoDelete)
		{
			OGRE_DELETE_ARRAY(mHeightmap);
		}
		if(mAutoDelete)
		{
			mHeightmap = heightmap;
		}
		else
		{
			mHeightmap = new float[width * height];
			memcpy(mHeightmap , heightmap , width * height * sizeof(float));
		}
		mWidth = width;
		mHeight = height;
		mAutoDelete = autoDelete;
	}

	void TerrainInfo::setExtents(const AxisAlignedBox& extents)
	{
		if (mWidth == 0)
			OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "You must set a heightmap first.", "TerrainInfo::setExtents");

		mOffset = extents.getMinimum();
		mScale = extents.getMaximum() - extents.getMinimum();
		mScale.x /= (mWidth - 1.0f);
		mScale.z /= (mHeight - 1.0f);
	}

	AxisAlignedBox TerrainInfo::getExtents() const
	{
		if (mWidth == 0)
			OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "You must set a heightmap first.", "TerrainInfo::setExtents");

		Vector3 maximum = mScale;
		maximum.x *= mWidth;
		maximum.z *= mHeight;
		maximum += mOffset;
		return AxisAlignedBox(mOffset, maximum);
	}


	float TerrainInfo::getHeightAt(float x, float z) const
	{
		// scale down
		x -= mOffset.x;
		z -= mOffset.z;
		x /= mScale.x;
		z /= mScale.z;
		if (x < 0.0 || x > mWidth-1 || z < 0.0 || z > mHeight-1)
		{
			// out of bounds
			return mOffset.y - 1;
		}

		// retrieve height from heightmap via bilinear interpolation
		size_t xi = (size_t) x, zi = (size_t) z;
		float xpct = x - xi, zpct = z - zi;
		if (xi == mWidth-1)
		{
			// one too far
			--xi;
			xpct = 1.0f;
		}
		if (zi == mHeight-1)
		{
			--zi;
			zpct = 1.0f;
		}

		// retrieve heights
		float heights[4];
		heights[0] = at(xi, zi);
		heights[1] = at(xi, zi+1);
		heights[2] = at(xi+1, zi);
		heights[3] = at(xi+1, zi+1);

		// interpolate
		float w[4];
		w[0] = (1.0 - xpct) * (1.0 - zpct);
		w[1] = (1.0 - xpct) * zpct;
		w[2] = xpct * (1.0 - zpct);
		w[3] = xpct * zpct;
		float ipHeight = w[0]*heights[0] + w[1]*heights[1] + w[2]*heights[2] + w[3]*heights[3];

		// scale to actual height
		ipHeight *= mScale.y;
		ipHeight += mOffset.y;

		return ipHeight;
	}


	Vector3 TerrainInfo::getNormalAt(float x, float z) const
	{
		int flip = 1;
		Vector3 here (x, getHeightAt(x, z), z);
		Vector3 left (x-1, getHeightAt(x-1, z), z);
		Vector3 down (x, getHeightAt(x, z+1), z+1);
		if (left.x < 0.0)
		{
			flip *= -1;
			left = Vector3(x+1, getHeightAt(x+1, z), z);
		}
		if (down.z >= mOffset.z + mScale.z*mHeight)
		{
			flip *= -1;
			down = Vector3(x, getHeightAt(x, z-1), z-1);
		}
		left -= here;
		down -= here;

		Vector3 norm = flip * left.crossProduct(down);
		norm.normalise();

		return norm;
	}


	std::pair<bool, Vector3> TerrainInfo::rayIntersects(const Ray& ray) const
	{
		AxisAlignedBox box = getExtents();
		Vector3 point = ray.getOrigin();
		Vector3 dir = ray.getDirection();

		// first, does the ray start from inside the terrain extents?
		if (!box.contains(point))
		{
			// not inside the box, so let's see if we are actually
			// colliding with it
			pair<bool, Real> res = ray.intersects(box);
			if (!res.first)
				return make_pair(false, Vector3::ZERO);
			// update point to the collision position
			// add by 王宏张 2007-8-13
			// 不应该移动点，否则会产生bug
			//point = ray.getPoint(res.second);
		}

		// now move along the ray until we intersect or leave the bounding box
		while (true)
		{
			// have we arived at or under the terrain height?
			// note that this approach means that ray queries from below won't work
			// correctly, but then again, that shouldn't be a usual case...
			float height = getHeightAt(point.x, point.z);
			if (point.y <= height)
			{
				point.y = height;
				return make_pair(true, point);
			}

			// move further...
			point += dir;

			// check if we are still inside the boundaries
			if (point.x < box.getMinimum().x || point.z < box.getMinimum().z
				|| point.x > box.getMaximum().x || point.z > box.getMaximum().z)
				return make_pair(false, Vector3::ZERO);

		}
	}

}
