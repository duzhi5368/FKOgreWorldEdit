#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COLLISIONSYSTEM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COLLISIONSYSTEM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef COLLISIONSYSTEM_EXPORTS
#define COLLISIONSYSTEM_API __declspec(dllexport)
#else
#define COLLISIONSYSTEM_API __declspec(dllimport)
#endif

#include <OgrePrerequisites.h>
