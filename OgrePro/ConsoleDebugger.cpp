#include "StdAfx.h"
#include "ConsoleDebugger.h"
#include <conio.h>
#include <errno.h>
namespace ConsoleDebugger
{

	void allocConsole()
	{   
		char   NewTitle[1024];   
		if   (AllocConsole())   
		{   
			strcpy(NewTitle,"ConsoleDebugger");   
			SetConsoleTitle(NewTitle);   
		}
	}

	void freeConsole()
	{
		FreeConsole();
	}

	void   print(const   char   *fmt,   ...)   
	{   
		va_list   args;   
		int   count;   
		char   buf[1024];   
		va_start(args,fmt);   
		count   =   vsprintf(buf,fmt,args);   
		printStr(buf);   
		va_end(args);   
	}   


	void   printStr(const char   *str)   
	{   
		CONSOLE_SCREEN_BUFFER_INFO   console_info;   
		WORD   attrib[1024];   
		char   data[1024];   
		DWORD   wl;   
		size_t   i,len;   
		sprintf(data,"s\n",str);   
		len   =   strlen(data);   
		for   (i=0;i<len;i++)   attrib[i]   =   15;   
		HANDLE console   =   GetStdHandle(STD_OUTPUT_HANDLE);   
		GetConsoleScreenBufferInfo(console,&console_info);   
		WriteConsoleOutputAttribute(console,attrib,(DWORD)len,console_info.dwCursorPosition,&wl);   
		_cputs((LPCTSTR)str);   
	}   
};