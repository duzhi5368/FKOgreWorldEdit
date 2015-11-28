#ifndef __ETPREREQUISITES_H__
#define __ETPREREQUISITES_H__

#include <Ogre.h>
#include <CollisionSystem.h>
#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32 )
#   ifdef ET_MANAGER_EXPORTS
#       define ET_EXPORT __declspec(dllexport)
#   else
#       if defined( __MINGW32__ )
#           define ET_EXPORT
#       else
#    		define ET_EXPORT __declspec(dllimport)
#       endif
#   endif
#elif defined ( OGRE_GCC_VISIBILITY )
#    define ET_EXPORT  __attribute__ ((visibility("default")))
#else
#   define ET_EXPORT
#endif

// 混合图数量
#define SPLATTING_TEXTURE_NUM 6
// 一个透明度图的颜色通道数量
#define COVERAGE_CHANNEL_NUM 4
// 混合图的透明度图数量，4个颜色通道，每个通道存储着每个混合贴图的alpha值，在地形编辑的时候会动态改变
#define COVERAGE_TEXTURE_NUM ((SPLATTING_TEXTURE_NUM + COVERAGE_CHANNEL_NUM - 1) / COVERAGE_CHANNEL_NUM)

#endif
