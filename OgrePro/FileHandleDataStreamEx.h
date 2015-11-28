#pragma once

namespace Ogre
{

	// 加强的FileHandleDataStream
	class OGREPRO_API FileHandleDataStreamEx : public FileHandleDataStream
	{
	public:
		/// Create stream from a C file handle
		FileHandleDataStreamEx(FILE* handle , bool autoCloseFile);
		/// Create named stream from a C file handle
		FileHandleDataStreamEx(const String& name, FILE* handle , bool autoCloseFile);
		~FileHandleDataStreamEx();
	protected:
		// 自动关闭文件
		bool m_autoCloseFile;
	};
};