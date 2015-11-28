#ifndef __ETSPLATTINGMANAGER_H__
#define __ETSPLATTINGMANAGER_H__

#include "ETPrerequisites.h"

#include "ETSelection.h"

// forward declarations
namespace Ogre
{
  class Image;
}
namespace Ogre
{
  namespace Impl
  {
    struct SplattingImpl;
  }
  class Brush;
}


namespace Ogre
{
  typedef std::vector<std::string> NameList;
  typedef std::vector<ColourValue> ColourList;

  /** 
   * SplattingManager allows you to create and edit an arbitrary amount of
   * alpha splatting maps which can be used with a splatting shader
   * to render multiple splatting textures per pass onto the terrain.
   * You can use 1 to 4 channels per map.
   */
  class ET_EXPORT SplattingManager
  {
  public:
    /** 
     * Constructs the splatting manager.
     * @param baseName   base name for the map textures (will be appended by their number)
     * @param group      the resource group the textures will be placed in
     * @param width      width of the textures in pixels
     * @param height     height of the textures in pixels
     * @param channels   Number of channels per texture (must be in {1, 2, 3, 4})
     */
    SplattingManager(const std::string& baseName, const std::string& group, 
      unsigned int width, unsigned int height, unsigned int channels = 4);
    ~SplattingManager();

    /** 
     * Sets the amount of splatting textures you need coverage maps for. 
     * Note that this will round up to the next multiple of the channels
     * per map.
     */
    void setNumTextures(unsigned int numTextures);

    /** Retrieves the current amount of splatting textures covered. */
    unsigned int getNumTextures() const;

    /** Sets the number of coverage maps to manage. */
    void setNumMaps(unsigned int numMaps);

    /** Retrieves the current amount of coverage maps. */
    unsigned int getNumMaps() const;

    /** Retrieves the names of the map textures for use in materials. */
    NameList getMapTextureNames() const;

    /** Loads a coverage map texture from an Image. */
    void loadMapFromImage(unsigned int mapNum, const Image& image);

    /** Saves a coverage map texture to an Image. */
    void saveMapToImage(unsigned int mapNum, Image& image);

    /** Paint with the specified texture and selected brush and intensity. */
    void paint(unsigned int textureNum, int x, int y, const Brush& brush, float intensity = 1.0f);

	// add by 王宏张 2007-7-25
	// 对一个被选区域进行paint
    void paint(unsigned int textureNum,const Selection& selection, float intensity = 1.0f);

    /** Create a colour map with a given list of colours for the textures. */
    void createColourMap(Image& image, const ColourList& colours);


  private:
    // Implementation hiding via PIMPL idiom
    Impl::SplattingImpl* mImpl;

  };


  /** Modulates a colour map by a lightmap. Can be used as a minimap of the terrain. */
  Image ET_EXPORT createMinimap(const Image& colourMap, const Image& lightMap);
}

#endif
