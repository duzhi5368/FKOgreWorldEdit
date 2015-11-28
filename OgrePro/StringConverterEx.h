#pragma once

namespace Ogre
{
#ifdef _UNICODE
	typedef std::wstring StringEx;
#else
	typedef std::string StringEx;
#endif
	typedef wchar_t WideChar;
	typedef char AnsiChar;
	typedef std::string AnsiString;
	typedef std::wstring WideString;

	// 字符串加强操作器，用来从ansi和unicode之间自由的转换
	class OGREPRO_API StringConverterEx
	{
	public:
		StringConverterEx();
		static StringEx toString(const AnsiString &strMBS);
		static StringEx toString(const WideString &strWCS);
		static StringEx toString(const char *szMBS);
		static StringEx toString(const wchar_t *wzWCS);
		static StringEx toString(const char *szMBS , size_t sizeLen);
		static StringEx toString(const wchar_t *wzWCS , size_t sizeLen);
		static WideString toWideString(const char *szMBS , size_t sizeLen);
		static WideString toWideString(const StringEx &strValue);
		static AnsiString toAnsiString(const wchar_t *wzWCS , size_t sizeLen);
		static AnsiString toAnsiString(const StringEx &strValue);

		static WideChar toWideChar(const AnsiChar cAnsiChar);
		static WideChar toWideChar(const AnsiChar pAnsiChar[2]);

		static bool toAnsiChar(const WideChar cWideChar , AnsiChar *cAnsiChar);

		static const StringEx &parseString(const StringEx &str){return str;};

		// 包围盒转换
		static String toString(const AxisAlignedBox &box);
		static AxisAlignedBox parseAxisAlignedBox(const String& val);
		// NameValuePairList转换
		static String toString(const NameValuePairList &value);
		static NameValuePairList parseNameValuePairList(const String& val);

		// Vector2转换
		static Vector2 parseVector2(const String& val);
	};

}