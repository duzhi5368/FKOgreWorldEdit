#ifndef __ETTERRAININFO_H__
#define __ETTERRAININFO_H__

#include "ETPrerequisites.h"

// forward declarations
namespace Ogre
{
  class AxisAlignedBox;
  class Image;
  class ColourValue;
  class DataStream;
  class Ray;
}



namespace Ogre
{
  /**
   * This class stores all basic information about the terrain, i. e. its heightmap,
   * its extents and the scaling.
   */
  class ET_EXPORT TerrainInfo
  {
  public:
    /** Default constructor */
    TerrainInfo();
    /** Constructs with a heightmap given as an array of floats. */
    TerrainInfo(size_t width, size_t height, float* heightmap = 0 , bool autoDelete = false);

	virtual ~TerrainInfo();
    /** Sets a new heightmap from an array of floats. */
    void setHeightmap(size_t width, size_t height, float* heightmap = 0, bool autoDelete = false);

    /** Retrieves the raw heightmap data. */
    const float* getHeightmapData() const { return mHeightmap; }

    /** Retrieves a value from the heightmap. */
    float& at(size_t x, size_t y) { return mHeightmap[x + y*mWidth]; }
    /** Retrieves a const value from the heightmap. */
    const float at(size_t x, size_t y) const { return mHeightmap[x + y*mWidth]; }

    /** Retrieves the width of the heightmap. */
    size_t getWidth() const { return mWidth; }
    /** Retrieves the height of the heightmap. */
    size_t getHeight() const { return mHeight; }

    /** Sets the extents of the terrain in Ogre units. */
    void setExtents(const AxisAlignedBox& extents);
    /** Retrieves the terrain extents in Ogre units. */
    AxisAlignedBox getExtents() const;

    /** Retrieves the scaling of one vertex in Ogre units. */
    const Vector3& getScaling() const { return mScale; }

    /** Retrieves the offset point of the terrain. */
    const Vector3& getOffset() const { return mOffset; }

    /** Scales from Ogre positions to vertex indexes. */
    int posToVertexX(float x) const { return (int) ((x-mOffset.x)/mScale.x); }
    /** Scales from Ogre positions to vertex indexes. */
    int posToVertexZ(float z) const { return (int) ((z-mOffset.z)/mScale.z); }
    /** Scales from vertex indexes to Ogre positions. */
    float vertexToPosX(int x) const { return mOffset.x + x*mScale.x; }
    /** Scales from vertex indexes to Ogre positions. */
    float vertexToPosZ(int z) const { return mOffset.z + z*mScale.z; }
    /** Scales from vertex indexes to Ogre positions. */
    float vertexToPosY(float y) const { return mOffset.y + y*mScale.y; }

    /** Calculates the height at the given coordinates. */
    float getHeightAt(float x, float z) const;

    /** Calculates the terrain normal at the given coordinates. */
    Vector3 getNormalAt(float x, float z) const;

    /**
     * Checks if a ray intersects with the terrain.
     * @param ray  The ray to check for intersection with the terrain.
     * @return (true, point of intersection) if the terrain is intersected.
     */
    std::pair<bool, Vector3> rayIntersects(const Ray& ray) const;

  private:
    /** extents of the heightmap */
    size_t mWidth, mHeight;
    /** the heightmap */
    float* mHeightmap; // why does this NOT generate warnings?! it did with Brush
	// 是否自动删除mHeightmap
	bool mAutoDelete;

    /** offset at which the (0, 0) point of the terrain is placed in Ogre */
    Vector3 mOffset;
    /** scale of a terrain vertex */
    Vector3 mScale;
  };


  // add by 王宏张 2007-7-21
  // 直接从float串行数据读取高度信息
  void ET_EXPORT loadHeightmapFromFloatRawData(TerrainInfo* info, DataStreamPtr& stream,
    size_t width, size_t height);
  // add by 王宏张 2007-7-21
  // 直接保存到float串行数据
  void ET_EXPORT saveHeightmapToFloatRawData(TerrainInfo* info, std::ostream& stream);



  /**
   * Derives a lightmap from the terrain described by the given TerrainInfo.
   * The lightmap is stored in the passed image.
   * @param info      Terrain info to generate the lightmap for
   * @param image     The image to store the lightmap into
   * @param width     The width of the resulting lightmap
   * @param height    The height of the resulting lightmap
   * @param lightDir  The direction from which the light is coming
   * @param lightCol  The colour of the light
   * @param ambient   Ambient colour
   * @param shadowed  Whether to include terrain self-shadows in the lightmap
   */
  void ET_EXPORT createTerrainLightmap(TerrainInfo* info, Image& image,
    size_t width, size_t height, Vector3 lightDir, const ColourValue& lightCol,
    const ColourValue& ambient, bool shadowed = true);

}


#endif
