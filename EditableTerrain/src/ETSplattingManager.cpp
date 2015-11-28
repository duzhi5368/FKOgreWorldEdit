#include "ETPrerequisites.h"
#include "ETSplattingManager.h"
#include "ETBrush.h"

#include <OgreResource.h>
#include <OgreImage.h>
#include <OgreTextureManager.h>
#include <OgreStringConverter.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreException.h>

#include <OgreLogManager.h>
#include <sstream>


using namespace std;


namespace Ogre
{
	namespace Impl
	{
		/** Handles a single alpha coverage map texture */
		class CoverageMap : public ManualResourceLoader
		{
		public:
			CoverageMap(const String& name, const String& group, uint width, uint height, int channels, bool black = true);
			~CoverageMap();

			/** Retrieve coverage map from image. */
			void loadFromImage(const Image& image);
			/** Save coverage map to image. */
			void saveToImage(Image& image);

			/** Get the map's value for texture index c at position (x, y) */
			uchar getValue(uint x, uint y, uint c) const;
			/** Set the map's value for texture index c at position (x, y) */
			void setValue(uint x, uint y, uint c, uchar val);

			/** Copy the editing buffer into the texture's pixel buffer. */
			void updateTexture();

			/** Implemented from ManualResourceLoader to support reloading of the coverage map. */
			void loadResource(Resource*);

			/** Texture resource name for this coverage map. */
			const String& getName() const;

		private:
			PixelFormat getFormat(int channels);
			int getChannels(PixelFormat format);

			uint mWidth, mHeight, mSize;
			int mChannels;
			uchar* mData;
			TexturePtr mTexture;
		};


		CoverageMap::CoverageMap(const String& name, const String& group, uint width, uint height, int channels, bool black)
			: mWidth(width), mHeight(height), mChannels(channels), mSize(width*height*channels)
		{
			mData = new uchar[mSize];
			memset(mData, 0, mSize);
			// the first channel of the first coverage map is set to full value so that
			// terrain initially is rendered with the first splatting texture
			if (!black)
				for (uint i = 0; i < mSize; i += mChannels)
					mData[i] = 255;
			// create a manually managed texture resource
			mTexture = TextureManager::getSingleton().createManual(name, group, TEX_TYPE_2D,
				width, height, 1, 0, getFormat(mChannels), TU_DEFAULT, this);
		}

		CoverageMap::~CoverageMap()
		{
			delete[] mData;
			TextureManager::getSingleton().remove(mTexture->getName());
		}

		void CoverageMap::loadResource(Resource*)
		{
			// the texture has requested to (re)load, we just copy our edit buffer
			// into the texture
			mTexture->createInternalResources();
			updateTexture();
		}

		void CoverageMap::updateTexture()
		{
			// write the edit buffer into the texture's pixel buffer
			HardwarePixelBufferSharedPtr buffer = mTexture->getBuffer();
			PixelBox pixelBox (mWidth, mHeight, 1, getFormat(mChannels), mData);
			Image::Box imageBox (0, 0, mWidth, mHeight);
			buffer->blitFromMemory(pixelBox, imageBox);
		}

		const String& CoverageMap::getName() const
		{
			return mTexture->getName();
		}

		uchar CoverageMap::getValue(uint x, uint y, uint c) const
		{
			return mData[(y*mWidth + x)*mChannels + c];
		}

		void CoverageMap::setValue(uint x, uint y, uint c, uchar val)
		{
			mData[(y*mWidth + x)*mChannels + c] = val;
		}


		PixelFormat CoverageMap::getFormat(int channels)
		{
			switch (channels)
			{
			case 1: return PF_BYTE_A;
			case 2: return PF_BYTE_LA;
			case 3: return PF_BYTE_RGB;
			case 4: return PF_BYTE_RGBA;
			case -1: return PF_BYTE_A;
			case -2: return PF_BYTE_LA;
			case -3: return PF_BYTE_BGR;
			case -4: return PF_BYTE_BGRA;
			default: return PF_UNKNOWN;
			}
		}

		int CoverageMap::getChannels(PixelFormat format)
		{
			switch (format)
			{
			case PF_BYTE_A: return 1;
			case PF_BYTE_LA: return 2;
			case PF_BYTE_RGB: return 3;
			case PF_BYTE_BGR: return -3;
			case PF_BYTE_RGBA: return 4;
			case PF_BYTE_BGRA: return -4;
			default: return 0;
			}
		}

		void CoverageMap::loadFromImage(const Image& image)
		{
			if (image.getWidth() != mWidth || image.getHeight() != mHeight)
				OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Given image doesn't conform to the used width and height.", "CoverageMap::loadFromImage");
			if (image.getFormat() != getFormat(mChannels) && image.getFormat() != getFormat(-mChannels))
				OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Given image is of invalid pixel format.", "CoverageMap::loadFromImage");

			memcpy(mData, image.getData(), image.getSize());
			if (getChannels(image.getFormat()) <= -3)
			{
				// need RGB(A), but given BGR(A) so invert
				for (uint i = 0; i < mSize; i += mChannels)
					swap(mData[i], mData[i+2]);
			}

			updateTexture();
		}

		void CoverageMap::saveToImage(Image& image)
		{
			image.loadDynamicImage(mData, mWidth, mHeight, 1, getFormat(mChannels));
		}



		struct SplattingImpl
		{
			String baseName;
			String group;
			uint width, height;
			uint channels;
			uint numTextures;
			uint numMaps;

			typedef vector<Impl::CoverageMap*> MapList;
			MapList maps;

			void paint(uint texture, uint x, uint y, float edit)
			{
				uint map = texture / channels;
				uint channel = texture % channels;
				int val = maps[map]->getValue(x, y, channel) + (int) (255.0 * edit);
				if (val > 255)
					val = 255;
				if (val < 0)
					val = 0;
				maps[map]->setValue(x, y, channel, val);

				balance(x, y, texture, val);
			}

			void balance(uint x, uint y, uint texture, int val)
			{
				// this method ensures that the values at (x, y) of all channels in all maps sum up to 255,
				// otherwise the terrain will get over- or underlighted at that position
				int sum = 0;
				for (uint i = 0; i < numMaps; ++i)
				{
					for (uint j = 0; j < channels; ++j)
					{
						// skip the texture we painted with, otherwise we'd be
						// undoing the change
						if (i * channels + j == texture)
							continue;
						sum += maps[i]->getValue(x, y, j);
					}
				}

				if (sum == 0)
				{
					// all other textures are 0, so set selected texture to full value
					maps[texture/channels]->setValue(x, y, texture%channels, 255);
					return;
				}

				// reduce/add all other channels as necessary
				int diff = sum - (255 - val);
				for (uint i = 0; i < numMaps; ++i)
				{
					for (uint j = 0; j < channels; ++j)
					{
						// skip the texture we painted with, otherwise we'd be
						// undoing the change
						if (i * channels + j == texture)
							continue;
						uchar v = maps[i]->getValue(x, y, j);
						v -= (uchar) floor(0.5 + diff * (float(v) / sum));
						maps[i]->setValue(x, y, j, v);
					}
				}
			}

			void updateTextures()
			{
				// update all map textures
				for (uint i = 0; i < numMaps; ++i)
					maps[i]->updateTexture();
			}

		};

	}




	SplattingManager::SplattingManager(const string& baseName, const string& group, uint width, uint height, uint channels)
	{
		if (channels < 1 || channels > 4)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Number of channels per texture must be between 1 and 4", "SplattingManager::SplattingManager");

		mImpl = new Impl::SplattingImpl();
		mImpl->baseName = baseName;
		mImpl->group = group;
		mImpl->width = width;
		mImpl->height = height;
		mImpl->channels = channels;
		mImpl->numTextures = 0;
		mImpl->numMaps = 0;
	}

	SplattingManager::~SplattingManager()
	{
		for (uint i = 0; i < mImpl->numMaps; ++i)
			delete mImpl->maps[i];
		delete mImpl;
	}

	void SplattingManager::setNumTextures(uint numTextures)
	{
		// the number of maps needed depends on the number of channels per texture
		if (numTextures == 0)
			setNumMaps(0);
		else
			setNumMaps((numTextures-1) / mImpl->channels + 1);
	}

	uint SplattingManager::getNumTextures() const
	{
		return mImpl->numTextures;
	}

	void SplattingManager::setNumMaps(uint numMaps)
	{
		mImpl->numMaps = numMaps;
		mImpl->numTextures = mImpl->numMaps * mImpl->channels;

		// add maps if we don't have enough
		for (size_t i = mImpl->maps.size(); i < mImpl->numMaps; ++i)
		{
			mImpl->maps.push_back(new Impl::CoverageMap(mImpl->baseName+StringConverter::toString(i),
				mImpl->group, mImpl->width, mImpl->height, mImpl->channels, i != 0));
		}

		// remove maps if there are too many
		for (size_t i = mImpl->maps.size(); i > mImpl->numMaps; --i)
		{
			delete *mImpl->maps.rbegin();
			mImpl->maps.pop_back();
		}
	}

	uint SplattingManager::getNumMaps() const
	{
		return mImpl->numMaps;
	}

	NameList SplattingManager::getMapTextureNames() const
	{
		NameList names;
		for (size_t i = 0; i < mImpl->maps.size(); ++i)
			names.push_back(mImpl->maps[i]->getName());
		return names;
	}

	void SplattingManager::loadMapFromImage(uint mapNum, const Image& image)
	{
		mImpl->maps[mapNum]->loadFromImage(image);
	}

	void SplattingManager::saveMapToImage(uint mapNum, Image& image)
	{
		mImpl->maps[mapNum]->saveToImage(image);
	}


	void SplattingManager::paint(uint textureNum, int x, int y, const Brush& brush, float intensity)
	{
		// positions given are supposed to be the mid of the brush
		// so adjust accordingly
		x -= (int)brush.getWidth()/2;
		y -= (int)brush.getHeight()/2;

		// iterate over all fields of the brush array and apply them to the map textures
		// if they lie within the bounds
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			int posX = x + (int)i;
			if (posX < 0 || posX >= (int)mImpl->width)
				continue;
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				int posY = y + (int)j;
				if (posY < 0 || posY >= (int)mImpl->height)
					continue;

				mImpl->paint(textureNum, (uint)posX, (uint)posY, brush.at(i, j) * intensity);
			}
		}

		// finally, update the textures
		mImpl->updateTextures();
	}

	// add by 王宏张 2007-7-25
	// 对一个被选区域进行贴图
	void SplattingManager::paint(uint textureNum, const Selection& selection, float intensity)
	{
		for(Selection::const_iterator iter = selection.begin() ; iter != selection.end() ; iter ++)
		{
			uint iPosX = (uint)iter->first.getX() + selection.getPosition().getX();
			uint iPosY = (uint)iter->first.getY() + selection.getPosition().getY();
			// add by 王宏张 2007-9-2
			// 错误检测
			if (iPosX >= (uint)mImpl->width)
				continue;
			if (iPosY >= (uint)mImpl->height)
				continue;

			mImpl->paint(textureNum, iPosX, iPosY, iter->second * intensity);
		}
		// finally, update the textures
		mImpl->updateTextures();
	}


	void SplattingManager::createColourMap(Image& image, const ColourList& colours)
	{
		if (colours.size() > mImpl->numTextures)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Given more colours than texture channels available.", __FUNCTION__);

		uchar* data = new uchar[mImpl->width*mImpl->height*3];

		for (size_t y = 0; y < mImpl->height; ++y)
		{
			for (size_t x = 0; x < mImpl->width; ++x)
			{
				ColourValue val (0, 0, 0);
				for (size_t i = 0; i < colours.size(); ++i)
				{
					uint m = i / mImpl->channels;
					uint t = i % mImpl->channels;
					val += colours[i] * (float(mImpl->maps[m]->getValue(x, y, t)) / 255);
				}

				size_t pos = (x + y * mImpl->width) * 3;
				data[pos+0] = uchar(255*val.r);
				data[pos+1] = uchar(255*val.g);
				data[pos+2] = uchar(255*val.b);
			}
		}

		image.loadDynamicImage(data, mImpl->width, mImpl->height, 1, PF_BYTE_RGB, true);
	}


	Image createMinimap(const Image& colourMap, const Image& lightMap)
	{
		if (colourMap.getWidth() != lightMap.getWidth() || colourMap.getHeight() != lightMap.getHeight())
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Images must have the same dimensions.", __FUNCTION__);

		uchar* data = new uchar[colourMap.getWidth()*colourMap.getHeight()*3];

		for (size_t y = 0; y < colourMap.getWidth(); ++y)
		{
			for (size_t x = 0; x < colourMap.getHeight(); ++x)
			{
				ColourValue val = const_cast<Image&>(colourMap).getColourAt(x, y, 0) * const_cast<Image&>(lightMap).getColourAt(x, y, 0);
				size_t pos = (x + y*colourMap.getWidth()) * 3;
				data[pos+0] = uchar(255*val.r);
				data[pos+1] = uchar(255*val.g);
				data[pos+2] = uchar(255*val.b);
			}
		}

		Image image;
		image.loadDynamicImage(data, colourMap.getWidth(), colourMap.getHeight(), 1, PF_BYTE_RGB, true);
		return image;
	}
}
