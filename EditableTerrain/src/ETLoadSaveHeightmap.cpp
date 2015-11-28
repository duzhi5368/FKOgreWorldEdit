#include "ETPrerequisites.h"
#include "ETTerrainInfo.h"

#include <OgreDataStream.h>
#include <OgreImage.h>
#include <OgreException.h>

#include <iostream>


using namespace std;

namespace Ogre
{

	void loadHeightmapFromFloatRawData(TerrainInfo* info, DataStreamPtr& stream, size_t width, size_t height)
	{
		size_t size = width*height;
		float *data = new float[size];

		stream->seek(0);
		size_t readSize = stream->read((void*)data, (size_t)size * sizeof(float));

		assert(readSize == size * sizeof(float));

		info->setHeightmap(width, height, data , true);
	}

	void saveHeightmapToFloatRawData(TerrainInfo* info, ostream& stream)
	{
		stream.write((char*)info->getHeightmapData(), (std::streamsize)(info->getWidth() * info->getHeight() * sizeof(float)));
	}
}
