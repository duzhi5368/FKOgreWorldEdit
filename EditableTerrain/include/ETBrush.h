#ifndef __ETBRUSH_H__
#define __ETBRUSH_H__

#include "ETPrerequisites.h"

// forward declarations
namespace Ogre
{
  class Image;
}


namespace Ogre
{
  /** This class represents a brush used to deform terrain or edit splatting coverage */
  class ET_EXPORT Brush
  {
  public:
    /** Default constructor */
    Brush();
    /** Constructs a brush from a given array of floats */
    Brush(const float* brush, size_t width, size_t height);
    /** Constructs a brush from a given vector of floats */
    Brush(const std::vector<float>& brush, size_t width, size_t height);
    /** Copy constructor */
    Brush(const Brush& other);
    ~Brush();

    /** Copy assignment */
    Brush& operator=(const Brush& other);

    size_t getWidth() const { return mWidth; }
    size_t getHeight() const { return mHeight; }

    /** Access to the brush array */
    float& at(size_t x, size_t y)
    {
      return mBrushArray[x + y*mWidth];
    }
    /** Const access to the brush array */
    const float at(size_t x, size_t y) const
    {
      return mBrushArray[x + y*mWidth];
    }

    /** Exception-safe swap function */
    void swap(Brush& other);

  private:
    /** The actual brush array containing the brush values */
    float* mBrushArray;  // would have used vector<float>, but gives warnings with dll export
    /** The brush's dimensions */
    size_t mWidth, mHeight;
  };

  /** Loads a brush from a grayscale image */
  Brush ET_EXPORT loadBrushFromImage(const Image& image);

  /** Saves a brush to a grayscale image */
  void ET_EXPORT saveBrushToImage(const Brush& brush, Image& image);
}


#endif
