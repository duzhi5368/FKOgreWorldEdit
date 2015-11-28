// ============================================================
// 文件名	:	IClipboardManager.h
// 创建人	:	王宏张
// 说明		:	剪切板管理器的接口
// 创建时间	:	2008-2-29 22:31:47
// ============================================================
#pragma once

namespace Ogre
{
	// 剪切板管理器的接口
	class IClipboardManager
	{
	public:

		IClipboardManager(void)
		{
		}

		virtual ~IClipboardManager(void)
		{
		}

		// 把数据保存到剪切板
		virtual void save(const void *dataPtr , size_t dataSize , const String &clipboardFormat) = 0;
		// 从剪贴板读取数据
		virtual size_t load(void *dataPtr , const String &clipboardFormat) = 0;
		// 查看剪贴板里的数据大小
		virtual size_t getSize(const String &clipboardFormat) = 0;

		// 把数据保存到剪切板
		virtual void save(const void *dataPtr , size_t dataSize , uint clipboardFormat) = 0;
		// 从剪贴板读取数据
		virtual size_t load(void *dataPtr , uint clipboardFormat) = 0;
		// 查看剪贴板里的数据大小
		virtual size_t getSize(uint clipboardFormat) = 0;


		// 把字符串保存到剪切板
		virtual void save(const String &text) = 0;
		// 从剪贴板读取字符串
		virtual String load() = 0;


	};
};