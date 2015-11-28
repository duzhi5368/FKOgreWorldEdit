#include "StdAfx.h"
#include "FileHandleDataStreamEx.h"

namespace Ogre
{
	//-----------------------------------------------------------------------
	FileHandleDataStreamEx::FileHandleDataStreamEx(FILE* handle , bool autoCloseFile)
		: FileHandleDataStream(handle)
		, m_autoCloseFile(autoCloseFile)
	{
	}
	//-----------------------------------------------------------------------
	FileHandleDataStreamEx::FileHandleDataStreamEx(const String& name, FILE* handle , bool autoCloseFile)
		: FileHandleDataStream(name , handle)
		, m_autoCloseFile(autoCloseFile)
	{
	}
	//-----------------------------------------------------------------------
	FileHandleDataStreamEx::~FileHandleDataStreamEx()
	{
		if(m_autoCloseFile)
		{
			close();
		}
	}
	//-----------------------------------------------------------------------
};