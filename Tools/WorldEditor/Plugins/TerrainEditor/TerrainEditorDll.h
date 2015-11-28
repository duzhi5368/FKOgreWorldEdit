#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DISPLAYLAYER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DISPLAYLAYER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TERRAINEDITOR_EXPORTS
#define TERRAINEDITOR_API __declspec(dllexport)
#else
#define TERRAINEDITOR_API __declspec(dllimport)
#endif
