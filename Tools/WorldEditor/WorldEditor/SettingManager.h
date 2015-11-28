// ============================================================
// 文件名 : SettingManager.h
// 创建人 : 王宏张
// 说明 : 设置管理器实现
// 创建时间 : 2008-1-26 10:03:34
// ============================================================
#pragma once
#include "ISettingManager.h"
#include "TinyXMLInclude.h"

class CIniFile;
namespace Ogre
{

	// 设置的值类型
	enum SettingType
	{
		ST_String,
		ST_Binary,
		ST_Bool,
		ST_Double,
		ST_Float,
		ST_Char,
		ST_Short,
		ST_Int,
		ST_Long,
		ST_Int64,
		ST_UChar,
		ST_UShort,
		ST_UInt,
		ST_ULong,
		ST_Vector2,
		ST_Vector3,
		ST_ColourValue,
		ST_Matrix3,
		ST_Matrix4,
		ST_Quaternion,
		ST_Radian,
		ST_Degree,
		ST_StringVector,

		ST_MAX,
	};
	// 设置管理器实现
	class SettingManager : public ISettingManager
		, public Singleton<SettingManager>
	{
	public:
		SettingManager(void);
		virtual ~SettingManager(void);
		// 单间模式接口
		static SettingManager& getSingleton(void);
		static SettingManager* getSingletonPtr(void);

		// 显示设置对话框
		virtual void showSettingDlg();
		// 马上保存数据
		virtual void flush();


		// 注    释 : get系列函数是获取设置的函数，如果指定的设置字段名不存在，会用默认值填充之
		// 返回类型 : 设置的数据
		// 参    数 : name			设置名
		// 参    数 : defaultValue	默认值
		// 参    数 : group			分组名


		virtual String getString(const String& name, const String& defaultValue = StringUtil::BLANK, const String& group = StringUtil::BLANK);
		virtual void getBinary(const String& name, uchar** ppData, uint* pBytes, const String& group = StringUtil::BLANK);

		virtual bool getBool(const String& name, bool defaultValue = false, const String& group = StringUtil::BLANK);
		virtual double getDouble(const String& name, double defaultValue = 0.0, const String& group = StringUtil::BLANK);
		virtual float getFloat(const String& name, float defaultValue = 0.0f, const String& group = StringUtil::BLANK);

		virtual char getChar(const String& name, char defaultValue = 0, const String& group = StringUtil::BLANK);
		virtual short getShort(const String& name, short defaultValue = 0, const String& group = StringUtil::BLANK);
		virtual int getInt(const String& name, int defaultValue = 0, const String& group = StringUtil::BLANK);
		virtual long getLong(const String& name, long defaultValue = 0, const String& group = StringUtil::BLANK);
		virtual __int64 getInt64(const String& name, __int64 defaultValue = 0, const String& group = StringUtil::BLANK);

		virtual uchar getUChar(const String& name, uchar defaultValue = 0, const String& group = StringUtil::BLANK);
		virtual ushort getUShort(const String& name, ushort defaultValue = 0, const String& group = StringUtil::BLANK);
		virtual uint getUInt(const String& name, uint defaultValue = 0, const String& group = StringUtil::BLANK);
		virtual ulong getULong(const String& name, ulong defaultValue = 0, const String& group = StringUtil::BLANK);

		virtual Vector2 getVector2(const String& name, const Vector2& defaultValue = Vector2::ZERO, const String& group = StringUtil::BLANK);
		virtual Vector3 getVector3(const String& name, const Vector3& defaultValue = Vector3::ZERO, const String& group = StringUtil::BLANK);
		virtual ColourValue getColourValue(const String& name, const ColourValue& defaultValue = ColourValue::White, const String& group = StringUtil::BLANK);
		virtual Matrix3 getMatrix3(const String& name, const Matrix3& defaultValue = Matrix3::IDENTITY, const String& group = StringUtil::BLANK);
		virtual Matrix4 getMatrix4(const String& name, const Matrix4& defaultValue = Matrix4::IDENTITY, const String& group = StringUtil::BLANK);
		virtual Quaternion getQuaternion(const String& name, const Quaternion& defaultValue = Quaternion::IDENTITY, const String& group = StringUtil::BLANK);
		virtual Radian getRadian(const String& name, const Radian &defaultValue = Radian(0.0f), const String& group = StringUtil::BLANK);
		virtual Degree getDegree(const String& name, const Degree &defaultValue = Degree(0.0f), const String& group = StringUtil::BLANK);
		virtual StringVector getStringVector(const String& name, const StringVector &defaultValue = StringVector(), const String& group = StringUtil::BLANK);

		// 枚举所有设置的名称
		virtual StringVector enumSettingName(const String& group = StringUtil::BLANK);


		// 注    释 : set系列函数是保存设置的函数
		// 参    数 : name		设置名
		// 参    数 : value			设置数据
		// 参    数 : group	分组名

		virtual void setString(const String& name, const String& value, const String& group = StringUtil::BLANK);
		virtual void setBinary(const String& name, uchar* data, uint size, const String& group = StringUtil::BLANK);

		virtual void setBool(const String& name, bool value, const String& group = StringUtil::BLANK);
		virtual void setDouble(const String& name, double value, const String& group = StringUtil::BLANK);
		virtual void setFloat(const String& name, float value, const String& group = StringUtil::BLANK);

		virtual void setChar(const String& name, char value, const String& group = StringUtil::BLANK);
		virtual void setShort(const String& name, short value, const String& group = StringUtil::BLANK);
		virtual void setInt(const String& name, int value, const String& group = StringUtil::BLANK);
		virtual void setLong(const String& name, long value, const String& group = StringUtil::BLANK);
		virtual void setInt64(const String& name, __int64 value, const String& group = StringUtil::BLANK);

		virtual void setUChar(const String& name, uchar value, const String& group = StringUtil::BLANK);
		virtual void setUShort(const String& name, ushort value, const String& group = StringUtil::BLANK);
		virtual void setUInt(const String& name, uint value, const String& group = StringUtil::BLANK);
		virtual void setULong(const String& name, ulong value, const String& group = StringUtil::BLANK);

		virtual void setVector2(const String& name, const Vector2& value , const String& group = StringUtil::BLANK);
		virtual void setVector3(const String& name, const Vector3& value , const String& group = StringUtil::BLANK);
		virtual void setColourValue(const String& name, const ColourValue& value , const String& group = StringUtil::BLANK);
		virtual void setMatrix3(const String& name, const Matrix3& value , const String& group = StringUtil::BLANK);
		virtual void setMatrix4(const String& name, const Matrix4& value , const String& group = StringUtil::BLANK);
		virtual void setQuaternion(const String& name, const Quaternion& value , const String& group = StringUtil::BLANK);
		virtual void setRadian(const String& name, const Radian &value , const String& group = StringUtil::BLANK);
		virtual void setDegree(const String& name, const Degree &value , const String& group = StringUtil::BLANK);
		virtual void setStringVector(const String& name, const StringVector &value , const String& group = StringUtil::BLANK);

		// 用来保存设置值
		TiXmlDocumentPtr &getSetting()
		{
			return m_setting;
		}

	protected:
		inline String getString(SettingType st , const String& name, const String& defaultValue = StringUtil::BLANK, const String& group = StringUtil::BLANK);
		inline void setString(SettingType st , const String& name, const String& value, const String& group = StringUtil::BLANK);


		// 默认分组
		String m_defaultGroup;
		
		// 用来保存设置值
		TiXmlDocumentPtr m_setting;

	};
};