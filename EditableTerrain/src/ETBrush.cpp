#include "ETPrerequisites.h"
#include "ETBrush.h"

#include <OgreImage.h>
#include <OgreException.h>

using namespace std;


namespace Ogre
{
  Brush::Brush()
  : mWidth(0), mHeight(0), mBrushArray(0)
  {
  }

  Brush::Brush(const float* brush, size_t width, size_t height)
  : mWidth(width), mHeight(height)
  {
    mBrushArray = new float[width*height];
    memcpy(mBrushArray, brush, width*height*sizeof(float));
  }

  Brush::Brush(const vector<float>& brush, size_t width, size_t height)
  : mWidth(width), mHeight(height)
  {
    mBrushArray = new float[width*height];
    copy(brush.begin(), brush.end(), mBrushArray);
  }

  Brush::Brush(const Brush& other)
  : mWidth(other.mWidth), mHeight(other.mHeight)
  {
    mBrushArray = new float[mWidth*mHeight];
    memcpy(mBrushArray, other.mBrushArray, mWidth*mHeight*sizeof(float));
  }

  Brush::~Brush()
  {
    delete[] mBrushArray;
  }


  Brush& Brush::operator=(const Brush& other)
  {
    Brush tmp (other);
    tmp.swap(*this);
    return *this;
  }

  void Brush::swap(Brush& other)
  {
    std::swap(mWidth, other.mWidth);
    std::swap(mHeight, other.mHeight);
    std::swap(mBrushArray, other.mBrushArray);
  }

  Brush loadBrushFromImage(const Image& image)
  {
    size_t width = image.getWidth();
    size_t height = image.getHeight();
    vector<float> brush (width*height);

    // determine the bytes per pixel used in the image
	size_t bpp = PixelUtil::getNumElemBytes(image.getFormat());

    // now fill the brush array
    const uchar* imageData = image.getData();
	for (size_t i = 0; i < brush.size(); ++i)
	{
		uchar val[4] = {0};
		PixelUtil::unpackColour(&val[0] , &val[1] , &val[2] , &val[3] , image.getFormat() , imageData);
		imageData += bpp;
		brush[i] = float(val[0] * 0.299f + val[1] * 0.587f + val[2] * 0.114f + (255.0f - val[3])) / 255.0f;
    }

    return Brush(brush, width, height);
  }


  void saveBrushToImage(const Brush& brush, Image& image)
  {
    // save brush as a 16bit grayscale image
    ushort* data = new ushort[brush.getWidth()*brush.getHeight()];
    for (size_t x = 0; x < brush.getWidth(); ++x)
      for (size_t y = 0; y < brush.getHeight(); ++y)
        data[y*brush.getWidth() + x] = ushort(brush.at(x, y) * 0xffff);

    // pass the data to the image, image takes over ownership
    image.loadDynamicImage((uchar*)data, brush.getWidth(), brush.getHeight(), 1, PF_L16, true);
  }
}
