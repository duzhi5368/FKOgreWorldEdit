#include "StdAfx.h"
#include "StringConverterEx.h"
namespace Ogre
{

	StringConverterEx g_StringConverterEx;

	StringConverterEx::StringConverterEx()
	{
		// 设置Unicode转换的代码页
		setlocale(LC_ALL , "");
	}

	StringEx StringConverterEx::toString(const AnsiString &strMBS)
	{
#ifdef _UNICODE
		return toWideString(strMBS.c_str() , strMBS.size());
#else
		return strMBS;
#endif
	};

	StringEx StringConverterEx::toString(const WideString &strWCS)
	{
#ifdef _UNICODE
		return strWCS;
#else
		return toAnsiString(strWCS.c_str() , strWCS.size());
#endif
	};


	StringEx StringConverterEx::toString(const char *szMBS)
	{
		if(szMBS == 0)
			return StringEx();
#ifdef _UNICODE
		size_t sizeLen = strlen(szMBS);
		return toWideString(szMBS , sizeLen);
#else
		return szMBS;
#endif
	}

	StringEx StringConverterEx::toString(const wchar_t *wzWCS)
	{
		if(wzWCS == 0)
			return StringEx();
#ifndef _UNICODE
		size_t sizeLen = wcslen(wzWCS);
		return toAnsiString(wzWCS , sizeLen);
#else
		return wzWCS;
#endif
	}



	StringEx StringConverterEx::toString(const char *szMBS , size_t sizeLen)
	{
		if(szMBS == 0)
			return StringEx();
#ifdef _UNICODE
		return toWideString(szMBS , sizeLen);
#else
		return szMBS;
#endif
	}

	StringEx StringConverterEx::toString(const wchar_t *wzWCS , size_t sizeLen)
	{
		if(wzWCS == 0)
			return StringEx();
#ifndef _UNICODE
		return toAnsiString(wzWCS , sizeLen);
#else
		return wzWCS;
#endif
	}



	WideString StringConverterEx::toWideString(const StringEx &strValue)
	{
#ifdef _UNICODE
		return strValue;
#else
		return toWideString(strValue.c_str() , strValue.size());
#endif
	}

	AnsiString StringConverterEx::toAnsiString(const StringEx &strValue)
	{
#ifdef _UNICODE
		return toAnsiString(strValue.c_str() , strValue.size());
#else
		return strValue;
#endif
	}


	WideString StringConverterEx::toWideString(const char *szMBS , size_t sizeLen)
	{
		if(szMBS == 0)
			return L"";
		wchar_t *pBuf = new wchar_t[sizeLen + 1];
		memset(pBuf , 0 , sizeof(wchar_t) * (sizeLen + 1));
		mbstowcs(pBuf , szMBS , sizeLen + 1);
		WideString strRet(pBuf);
		delete[] pBuf;
		return strRet;
	}

	AnsiString StringConverterEx::toAnsiString(const wchar_t *wzWCS , size_t sizeLen)
	{
		if(wzWCS == 0)
			return "";
		size_t sizeALen = sizeLen * 2 + 1;
		char *pBuf = new char[sizeALen];
		memset(pBuf , 0 , sizeof(char) * (sizeALen));
		wcstombs(pBuf , wzWCS , sizeALen);
		AnsiString strRet(pBuf);
		delete[] pBuf;
		return strRet;
	}


	WideChar StringConverterEx::toWideChar(const AnsiChar cAnsiChar)
	{
		return cAnsiChar;
	}
	WideChar StringConverterEx::toWideChar(const AnsiChar pAnsiChar[2])
	{
		WideChar cWideChar(0);
		MultiByteToWideChar( 0, 0, pAnsiChar, 2, &cWideChar, 1); //转成宽字节
		return cWideChar;
	}

	bool StringConverterEx::toAnsiChar(const WideChar cWideChar , AnsiChar *cAnsiChar)
	{
		return WideCharToMultiByte(CP_ACP , 0 , &cWideChar , 1 , cAnsiChar , 2 , 0 , 0) != 0;
	}

	Vector2 StringConverterEx::parseVector2(const String& val)
	{
		// Split on space
		std::vector<String> vec = StringUtil::split(val);

		if (vec.size() != 2)
		{
			return Vector2::ZERO;
		}
		else
		{
			return Vector2(StringConverter::parseReal(vec[0]),StringConverter::parseReal(vec[1]));
		}

	}

	// 包围盒转换
	String StringConverterEx::toString(const AxisAlignedBox &box)
	{
		return StringConverter::toString(box.getMinimum()) + " " + StringConverter::toString(box.getMaximum());
	}

	AxisAlignedBox StringConverterEx::parseAxisAlignedBox(const String& val)
	{
		// Split on space
		std::vector<String> vec = StringUtil::split(val);

		if (vec.size() != 6)
		{
			return AxisAlignedBox();
		}
		else
		{
			return AxisAlignedBox(StringConverter::parseReal(vec[0])
				, StringConverter::parseReal(vec[1])
				, StringConverter::parseReal(vec[2])
				, StringConverter::parseReal(vec[3])
				, StringConverter::parseReal(vec[4])
				, StringConverter::parseReal(vec[5])
				);
		}

	}



	// NameValuePairList转换
	String StringConverterEx::toString(const NameValuePairList &value)
	{
		String ret;
		// 所有名称和值
		for(NameValuePairList::const_iterator iter = value.begin() ; iter != value.end() ; iter ++)
		{
			String sub = iter->first + "\t\n " + iter->second + "\t\n ";
			ret += sub;
		}
		return ret;
	}

	NameValuePairList StringConverterEx::parseNameValuePairList(const String& val)
	{
		// Split on space
		std::vector<String> vec = StringUtil::split(val);

		NameValuePairList value;
		if(vec.size() % 2 != 0)
			return value;
		for(size_t i = 0 ; i < vec.size() ; i += 2)
		{
			value[vec[i]] = vec[i + 1];
		}
		return value;
	}



}