#include "StdAfx.h"
#include "SettingManager.h"
#include "DlgSetting.h"
#include "WorldEditor.h"
namespace Ogre
{
	// 单件模式实现
	template<> SettingManager* Singleton<SettingManager>::ms_Singleton = 0;
	SettingManager* SettingManager::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	SettingManager& SettingManager::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	SettingManager::SettingManager(void)
		: m_defaultGroup("一般设置")
	{
		m_setting.bind(new TiXmlDocument("Setting.xml"));

		// 分析XML文件
		m_setting->LoadFile(TIXML_ENCODING_UTF8);

		// 如果文件读取失败，就创建个新的
		if(m_setting->Error())
		{
			m_setting->ClearError();
			m_setting->Clear();
			TiXmlElement element( "Setting" );
			m_setting->InsertEndChild(element);
		}
	}

	SettingManager::~SettingManager(void)
	{
		// 保存文件
		flush();
		m_setting.setNull();
	}

	// 显示设置对话框
	void SettingManager::showSettingDlg()
	{
		CDlgSetting dlg(WorldEditor::getSingleton().getMainFrame()->getWnd());
		dlg.DoModal();
	}
	// 马上保存数据
	void SettingManager::flush()
	{
		// 保存文件
		m_setting->SaveFile();
	}

	// 注    释 : SettingManager::get系列函数是获取设置的函数，如果指定的设置字段名不存在，会用默认值填充之
	// 返回类型 : 设置的数据
	// 参    数 : name			设置名
	// 参    数 : defaultValue	默认值
	// 参    数 : group			分组名

	String SettingManager::getString(SettingType st , const String& name, const String& defaultValue, const String& group)
	{
		String groupReal(group.empty() ? m_defaultGroup : group);
		TiXmlElement* elementGroup = m_setting->RootElement()->FirstChildElement(groupReal.c_str());
		// 创建分组
		if(elementGroup == 0)
		{
			TiXmlElement element( groupReal.c_str() );
			elementGroup = (TiXmlElement*)m_setting->RootElement()->InsertEndChild(element);
		}
		TiXmlElement* elementValue = elementGroup->FirstChildElement(name.c_str());
		// 创建值
		if(elementValue == 0)
		{
			TiXmlElement element( name.c_str() );
			elementValue = (TiXmlElement*)elementGroup->InsertEndChild(element);
			elementValue->SetAttribute("value" , defaultValue.c_str());
		}
		elementValue->SetAttribute("type" , st);
		return elementValue->Attribute("value");
	}

	String SettingManager::getString(const String& name, const String& defaultValue, const String& group)
	{
		return getString(ST_String , name , defaultValue , group);
	}

	void SettingManager::getBinary(const String& name, uchar** ppData, uint* pBytes, const String& group)
	{
		String str(getString(ST_Binary , name , StringUtil::BLANK , group));
		*ppData = NULL;
		*pBytes = 0;
		if(str.empty())
		{
			return;
		}
		ASSERT(str.size() % 2 == 0);
		INT_PTR nLen = str.size();
		*pBytes = UINT(nLen)/2;
		*ppData = new uchar[*pBytes];
		for (int i=0;i<nLen;i+=2)
		{
			(*ppData)[i/2] = (uchar)(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}
	}


	bool SettingManager::getBool(const String& name, bool defaultValue, const String& group)
	{
		return StringConverter::parseBool(getString(ST_Bool , name , StringConverter::toString(defaultValue) , group));
	}

	double SettingManager::getDouble(const String& name, double defaultValue, const String& group)
	{
		return (double)StringConverter::parseReal(getString(ST_Double , name , StringConverter::toString((Real)defaultValue) , group));
	}

	float SettingManager::getFloat(const String& name, float defaultValue, const String& group)
	{
		return (float)StringConverter::parseReal(getString(ST_Float , name , StringConverter::toString(defaultValue) , group));
	}


	char SettingManager::getChar(const String& name, char defaultValue, const String& group)
	{
		return (char)StringConverter::parseInt(getString(ST_Char , name , StringConverter::toString(defaultValue) , group));
	}

	short SettingManager::getShort(const String& name, short defaultValue, const String& group)
	{
		return (short)StringConverter::parseInt(getString(ST_Short , name , StringConverter::toString(defaultValue) , group));
	}

	int SettingManager::getInt(const String& name, int defaultValue, const String& group)
	{
		return (int)StringConverter::parseInt(getString(ST_Int , name , StringConverter::toString(defaultValue) , group));
	}

	long SettingManager::getLong(const String& name, long defaultValue, const String& group)
	{
		return (long)StringConverter::parseLong(getString(ST_Long , name , StringConverter::toString(defaultValue) , group));
	}

	__int64 SettingManager::getInt64(const String& name, __int64 defaultValue, const String& group)
	{
		TCHAR strBuffer[MAX_PATH];
		_ui64tot(defaultValue, strBuffer, 10);

		String str(getString(ST_Int64 , name , strBuffer , group));

		ULONGLONG nResult;
		if (_stscanf(str.c_str(), _T("%I64u"), &nResult) != 1)
			return 0;

		return nResult;
	}


	uchar SettingManager::getUChar(const String& name, uchar defaultValue, const String& group)
	{
		return (uchar)StringConverter::parseLong(getString(ST_UChar , name , StringConverter::toString(defaultValue) , group));
	}

	ushort SettingManager::getUShort(const String& name, ushort defaultValue, const String& group)
	{
		return (ushort)StringConverter::parseLong(getString(ST_UShort , name , StringConverter::toString(defaultValue) , group));
	}

	uint SettingManager::getUInt(const String& name, uint defaultValue, const String& group)
	{
		return (uint)StringConverter::parseLong(getString(ST_UInt , name , StringConverter::toString(defaultValue) , group));
	}

	ulong SettingManager::getULong(const String& name, ulong defaultValue, const String& group)
	{
		return (ulong)StringConverter::parseLong(getString(ST_ULong , name , StringConverter::toString(defaultValue) , group));
	}



	Vector2 SettingManager::getVector2(const String& name, const Vector2& defaultValue , const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			StringVector strings(StringConverter::parseStringVector(str));
			if(strings.size() != 2)
			{
				return defaultValue;
			}
			else
			{
				return Vector2(
					StringConverter::parseReal(strings[0])
					, StringConverter::parseReal(strings[1])
					);
			}
		}
	}

	Vector3 SettingManager::getVector3(const String& name, const Vector3& defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseVector3(str);
		}
	}

	ColourValue SettingManager::getColourValue(const String& name, const ColourValue& defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseColourValue(str);
		}
	}

	Matrix3 SettingManager::getMatrix3(const String& name, const Matrix3& defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseMatrix3(str);
		}
	}

	Matrix4 SettingManager::getMatrix4(const String& name, const Matrix4& defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseMatrix4(str);
		}
	}

	Quaternion SettingManager::getQuaternion(const String& name, const Quaternion& defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseQuaternion(str);
		}
	}

	Radian SettingManager::getRadian(const String& name, const Radian &defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseAngle(str);
		}
	}

	Degree SettingManager::getDegree(const String& name, const Degree &defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseAngle(str);
		}
	}

	StringVector SettingManager::getStringVector(const String& name, const StringVector &defaultValue, const String& group)
	{
		String str(getString(name , StringUtil::BLANK , group));
		if(str.empty())
		{
			return defaultValue;
		}
		else
		{
			return StringConverter::parseStringVector(str);
		}
	}


	// 枚举所有设置的名称
	StringVector SettingManager::enumSettingName(const String& group)
	{
		String groupReal(group.empty() ? m_defaultGroup : group);
		TiXmlElement* elementGroup = m_setting->RootElement()->FirstChildElement(groupReal.c_str());
		// 创建分组
		if(elementGroup == 0)
		{
			TiXmlElement element( groupReal.c_str() );
			elementGroup = (TiXmlElement*)m_setting->RootElement()->InsertEndChild(element);
		}
		StringVector names;
		for(TiXmlElement* elementValue = elementGroup->FirstChildElement()
			; elementValue != 0 ; elementValue = elementValue->NextSiblingElement())
		{
			names.push_back(elementValue->Value());
		}
		return names;
	}

	// 注    释 : SettingManager::set系列函数是保存设置的函数
	// 参    数 : name		设置名
	// 参    数 : value			设置数据
	// 参    数 : group	分组名

	void SettingManager::setString(SettingType st , const String& name, const String& value, const String& group)
	{
		String groupReal(group.empty() ? m_defaultGroup : group);
		TiXmlElement* elementGroup = m_setting->RootElement()->FirstChildElement(groupReal.c_str());
		// 创建分组
		if(elementGroup == 0)
		{
			TiXmlElement element( groupReal.c_str() );
			elementGroup = (TiXmlElement*)m_setting->RootElement()->InsertEndChild(element);
		}
		TiXmlElement* elementValue = elementGroup->FirstChildElement(name.c_str());
		// 创建值
		if(elementValue == 0)
		{
			TiXmlElement element( name.c_str() );
			elementValue = (TiXmlElement*)elementGroup->InsertEndChild(element);
		}
		elementValue->SetAttribute("type" , st);
		elementValue->SetAttribute("value" , value.c_str());
	}

	void SettingManager::setString(const String& name, const String& value, const String& group)
	{
		setString(ST_String , name , value , group);
	}

	void SettingManager::setBinary(const String& name, uchar* data, uint size, const String& group)
	{

		// convert to string and write out
		String lpsz(size*2 , ' ');
		UINT i;
		for (i = 0; i < size; i++)
		{
			lpsz[i*2] = (TCHAR)((data[i] & 0x0F) + 'A'); //low nibble
			lpsz[i*2+1] = (TCHAR)(((data[i] >> 4) & 0x0F) + 'A'); //high nibble
		}
		setString(ST_Binary , name , lpsz , group);
	}


	void SettingManager::setBool(const String& name, bool value, const String& group)
	{
		setString(ST_Bool , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setDouble(const String& name, double value, const String& group)
	{
		setString(ST_Double , name , StringConverter::toString((Real)value) , group);
	}

	void SettingManager::setFloat(const String& name, float value, const String& group)
	{
		setString(ST_Float , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setChar(const String& name, char value, const String& group)
	{
		setString(ST_Char , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setShort(const String& name, short value, const String& group)
	{
		setString(ST_Short , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setInt(const String& name, int value, const String& group)
	{
		setString(ST_Int , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setLong(const String& name, long value, const String& group)
	{
		setString(ST_Long , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setInt64(const String& name, __int64 value, const String& group)
	{
		TCHAR strBuffer[MAX_PATH];
		_ui64tot(value, strBuffer, 10);
		setString(ST_Int64 , name , strBuffer , group);
	}


	void SettingManager::setUChar(const String& name, uchar value, const String& group)
	{
		setString(ST_UChar , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setUShort(const String& name, ushort value, const String& group)
	{
		setString(ST_UShort , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setUInt(const String& name, uint value, const String& group)
	{
		setString(ST_UInt , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setULong(const String& name, ulong value, const String& group)
	{
		setString(ST_ULong , name , StringConverter::toString(value) , group);
	}


	void SettingManager::setVector2(const String& name, const Vector2& value , const String& group)
	{
		setString(ST_Vector2 , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setVector3(const String& name, const Vector3& value , const String& group)
	{
		setString(ST_Vector3 , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setColourValue(const String& name, const ColourValue& value , const String& group)
	{
		setString(ST_ColourValue , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setMatrix3(const String& name, const Matrix3& value , const String& group)
	{
		setString(ST_Matrix3 , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setMatrix4(const String& name, const Matrix4& value , const String& group)
	{
		setString(ST_Matrix4 , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setQuaternion(const String& name, const Quaternion& value , const String& group)
	{
		setString(ST_Quaternion , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setRadian(const String& name, const Radian &value , const String& group)
	{
		setString(ST_Radian , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setDegree(const String& name, const Degree &value , const String& group)
	{
		setString(ST_Degree , name , StringConverter::toString(value) , group);
	}

	void SettingManager::setStringVector(const String& name, const StringVector &value , const String& group)
	{
		setString(ST_StringVector , name , StringConverter::toString(value) , group);
	}
};