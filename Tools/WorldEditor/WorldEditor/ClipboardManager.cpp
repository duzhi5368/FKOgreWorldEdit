#include "StdAfx.h"
#include "ClipboardManager.h"
namespace Ogre
{

	ClipboardManager::ClipboardManager(void)
	{
	}

	ClipboardManager::~ClipboardManager(void)
	{
	}

	// 把数据保存到剪切板
	void ClipboardManager::save(const void *dataPtr , size_t dataSize , const String &clipboardFormat)
	{
		save(dataPtr , dataSize , RegisterClipboardFormat(clipboardFormat.c_str()));
	}
	// 从剪贴板读取数据
	size_t ClipboardManager::load(void *dataPtr , const String &clipboardFormat)
	{
		return load(dataPtr , RegisterClipboardFormat(clipboardFormat.c_str()));
	}
	// 查看剪贴板里的数据大小
	size_t ClipboardManager::getSize(const String &clipboardFormat)
	{
		return getSize(RegisterClipboardFormat(clipboardFormat.c_str()));
	}

	// 把数据保存到剪切板
	void ClipboardManager::save(const void *dataPtr , size_t dataSize , uint clipboardFormat)
	{
		if(dataSize == 0) return;

		HANDLE hGlobalMemory = GlobalAlloc(GHND, dataSize); // 分配内存
		LPVOID lpGlobalMemory = GlobalLock(hGlobalMemory); // 锁定内存

		memcpy(lpGlobalMemory , dataPtr , dataSize);

		GlobalUnlock(hGlobalMemory);	//锁定内存块解锁
		HWND hWnd = GetActiveWindow();	//获取安全窗口句柄
		::OpenClipboard(hWnd);			//打开剪贴板
		::EmptyClipboard();				//清空剪贴板

		::SetClipboardData(clipboardFormat, hGlobalMemory);//将内存中的数据放置到剪贴板
		::CloseClipboard();				//关闭剪贴板	
	}

	// 从剪贴板读取数据
	size_t ClipboardManager::load(void *dataPtr , uint clipboardFormat)
	{
		HWND hWnd = ::GetActiveWindow();	// 获取安全窗口句柄
		::OpenClipboard(hWnd);				// 打开剪贴板
		HANDLE hClipMemory = ::GetClipboardData(clipboardFormat);		// 获取剪贴板数据句柄
		size_t dwLength = 0;
		if(hClipMemory)
		{
			dwLength = (size_t)GlobalSize(hClipMemory);				// 返回指定内存区域的当前大小
			if(dwLength > 0)
			{
				LPVOID lpClipMemory = GlobalLock(hClipMemory);	// 锁定内存
				memcpy(dataPtr , lpClipMemory , dwLength);
				GlobalUnlock(hClipMemory);	// 内存解锁
			}
		}
		::CloseClipboard();			// 关闭剪贴板	

		return dwLength;

	}
	// 查看剪贴板里的数据大小
	size_t ClipboardManager::getSize(uint clipboardFormat)
	{
		HWND hWnd = ::GetActiveWindow();	// 获取安全窗口句柄
		::OpenClipboard(hWnd);				// 打开剪贴板
		HANDLE hClipMemory = ::GetClipboardData(clipboardFormat);		// 获取剪贴板数据句柄
		size_t dwLength = 0;
		if(hClipMemory)
		{
			dwLength = (size_t)GlobalSize(hClipMemory);				// 返回指定内存区域的当前大小
		}
		::CloseClipboard();			// 关闭剪贴板	
		return dwLength;
	}
	// 把字符串保存到剪切板
	void ClipboardManager::save(const String &text)
	{
		save(text.c_str() , text.length() + 1 , CF_TEXT);
	}

	// 从剪贴板读取字符串
	String ClipboardManager::load()
	{
		size_t textSize = getSize(CF_TEXT);
		if(textSize == 0)
			return StringUtil::BLANK;
		typedef SharedPtr<char> CharPtr;
		CharPtr text(new char[textSize + 1]);
		text.getPointer()[textSize] = 0;
		load(text.getPointer() , CF_TEXT);
		return String(text.getPointer());
	}
};