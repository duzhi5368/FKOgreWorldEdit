#ifndef __ETOPTIONS_H__
#define __ETOPTIONS_H__

#include "ETPrerequisites.h"

namespace Ogre
{
    struct Options
    {
      ushort tileSize;

      unsigned int maxPixelError;
      float factorC;

      bool useLODMorph;
      float lodMorphStart;
      std::string lodMorphParam;
    };
}

#endif
