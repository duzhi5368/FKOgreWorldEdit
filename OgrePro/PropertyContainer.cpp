#include "StdAfx.h"
#include "PropertyContainer.h"
#include "StringConverterEx.h"
#include "FileHandleDataStreamEx.h"

namespace Ogre
{
	PropertyContainer::Property::Property(){};
	PropertyContainer::Property::Property(uint inID , PropertyType inType , const String &inName , const String &inDesc , bool inShowable , bool inEditable , bool inSaveable)
		: id(inID)
		, name(inName)
		, showable(inShowable)
		, editable(inEditable)
		, type(inType)
		, desc(inDesc)
		, saveable(inSaveable)
		, parent(0)
	{

	};


	// 枚举所有属性，以列表的形式
	void PropertyContainer::enumPropertiesList(MapProperty &properties)const
	{
		// 用来列举所有属性的Visitor
		struct ListProperty : public PropertyVisitor
		{
			ListProperty(MapProperty &properties)
				: m_properties(properties)
			{

			}
			virtual bool visit(const Property &property)
			{
				m_properties[property.id] = Property(property.id
					, property.type
					, property.name
					, property.desc
					, property.showable
					, property.editable
					, property.saveable);
				return true;
			};
		private:
			MapProperty &m_properties;
		};
		ListProperty visitor(properties);
		visitProperties(&visitor);
	};

	// 遍历访问所有属性
	void PropertyContainer::visitProperties(PropertyVisitor *visitor)
	{
		VTProperty properties;
		enumPropertiesTree(properties);
		visitProperties(visitor , properties);
	}
	// 遍历访问所有属性
	void PropertyContainer::visitProperties(PropertyVisitor *visitor)const
	{
		VTProperty properties;
		enumPropertiesTree(properties);
		visitProperties(visitor , properties);
	}
	// 遍历访问一个属性数组
	void PropertyContainer::visitProperties(PropertyVisitor *visitor , const VTProperty &properties)
	{
		for(VTProperty::const_iterator iter = properties.begin() ; iter != properties.end() ; iter ++)
		{
			visitor->visit(*iter);
			visitProperties(visitor , iter->childs);
		}
	}
	// 设置属性
	bool PropertyContainer::setPropertyByName(const String &name , const String &value)
	{
		const MapProperty *properties = getPropertiesList();
		// 遍历所有属性
		for(MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.name == name)
			{
				return setProperty(iter->second.id , toAny(value , iter->second.type));
			}
		}
		return false;
	}



	// 获取属性值
	String PropertyContainer::getPropertyByName(const String &name)const
	{
		const MapProperty *properties = getPropertiesList();
		// 遍历所有属性
		for(MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.name == name)
			{
				return toString(getProperty(iter->second.id) , iter->second.type);
			}
		}
		return StringUtil::BLANK;
	}

	// 设置属性
	bool PropertyContainer::setPropertyByName(const String &name , const Any &value)
	{
		const MapProperty *properties = getPropertiesList();
		// 遍历所有属性
		for(MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.name == name)
			{
				return setProperty(iter->second.id , value);
			}
		}
		return false;
	}
	// 获取属性值
	Any PropertyContainer::getProperty(const String &name)const
	{
		const MapProperty *properties = getPropertiesList();
		// 遍历所有属性
		for(MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.name == name)
			{
				return getProperty(iter->second.id);
			}
		}
		return Any();
	}

	// 根据属性名称获取属性ID
	uint PropertyContainer::getPropertyID(const String &name)
	{
		const MapProperty *properties = getPropertiesList();
		// 遍历所有属性
		for(MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.name == name)
			{
				return iter->second.id;
			}
		}
		return 0;
	}
	// 根据属性ID获取属性名称
	String PropertyContainer::getPropertyName(uint id)
	{
		const MapProperty *properties = getPropertiesList();
		MapProperty::const_iterator iter = properties->find(id);
		if(iter == properties->end())
			return StringUtil::BLANK;
		return iter->second.name;
	}

	// 设置属性
	bool PropertyContainer::setPropertyByID(uint id , const String &value)
	{
		const MapProperty *properties = getPropertiesList();
		MapProperty::const_iterator iter = properties->find(id);
		if(iter == properties->end())
			return false;
		return setProperty(id , toAny(value , iter->second.type));
	}
	// 获取属性值
	String PropertyContainer::getPropertyByID(uint id)const
	{
		const MapProperty *properties = getPropertiesList();
		MapProperty::const_iterator iter = properties->find(id);
		if(iter == properties->end())
			return StringUtil::BLANK;
		return toString(getProperty(id) , iter->second.type);
	}

	// 把Any值转换到String类型
	String PropertyContainer::toString(const Any &any , PropertyType type)
	{
		switch(type)
		{
		case PT_GROUP:
			{
				return StringUtil::BLANK;
			}
			break;
		case PT_BOOL:
			{
				return StringConverter::toString(any_cast<bool>(any));
			}
			break;
		case PT_REAL:
			{
				return StringConverter::toString(any_cast<Real>(any));
			}
			break;
		case PT_INT:
			{
				return StringConverter::toString(any_cast<int>(any));
			}
			break;
		case PT_UINT:
			{
				return StringConverter::toString(any_cast<uint>(any));
			}
			break;
		case PT_SHORT:
			{
				return StringConverter::toString(any_cast<short>(any));
			}
			break;
		case PT_USHORT:
			{
				return StringConverter::toString(any_cast<ushort>(any));
			}
			break;
		case PT_LONG:
			{
				return StringConverter::toString(any_cast<long>(any));
			}
			break;
		case PT_ULONG:
			{
				return StringConverter::toString(any_cast<ulong>(any));
			}
			break;
		case PT_STRING:
			{
				return any_cast<String>(any);
			}
			break;
		case PT_VECTOR2:
			{
				return StringConverter::toString(any_cast<Vector2>(any));
			}
			break;
		case PT_VECTOR3:
			{
				return StringConverter::toString(any_cast<Vector3>(any));
			}
			break;
		case PT_MATRIX3:
			{
				return StringConverter::toString(any_cast<Matrix3>(any));
			}
			break;
		case PT_MATRIX4:
			{
				return StringConverter::toString(any_cast<Matrix4>(any));
			}
			break;
		case PT_QUATERNION:
			{
				return StringConverter::toString(any_cast<Quaternion>(any));
			}
			break;
		case PT_COLOURVALUE:
			{
				return StringConverter::toString(any_cast<ColourValue>(any));
			}
			break;
		case PT_AXISALIGNEDBOX:
			{
				return StringConverterEx::toString(any_cast<AxisAlignedBox>(any));
			}
			break;
		default:
			return StringUtil::BLANK;
			break;
		}
	}
	// 把String类型根据类型转换成Any
	Any PropertyContainer::toAny(const String &value , PropertyType type)
	{
		switch(type)
		{
		case PT_GROUP:
			{
				return Any();
			}
			break;
		case PT_BOOL:
			{
				return Any(StringConverter::parseBool(value));
			}
			break;
		case PT_REAL:
			{
				return Any(StringConverter::parseReal(value));
			}
			break;
		case PT_INT:
			{
				return Any((int)StringConverter::parseInt(value));
			}
			break;
		case PT_UINT:
			{
				return Any((uint)StringConverter::parseInt(value));
			}
			break;
		case PT_SHORT:
			{
				return Any((short)StringConverter::parseInt(value));
			}
			break;
		case PT_USHORT:
			{
				return Any((ushort)StringConverter::parseInt(value));
			}
			break;
		case PT_LONG:
			{
				return Any((long)StringConverter::parseLong(value));
			}
			break;
		case PT_ULONG:
			{
				return Any((ulong)StringConverter::parseLong(value));
			}
			break;
		case PT_STRING:
			{
				return Any(value);
			}
			break;
		case PT_VECTOR2:
			{
				return Any(StringConverterEx::parseVector2(value));
			}
			break;
		case PT_VECTOR3:
			{
				return Any(StringConverter::parseVector3(value));
			}
			break;
		case PT_MATRIX3:
			{
				return Any(StringConverter::parseMatrix3(value));
			}
			break;
		case PT_MATRIX4:
			{
				return Any(StringConverter::parseMatrix4(value));
			}
			break;
		case PT_QUATERNION:
			{
				return Any(StringConverter::parseQuaternion(value));
			}
			break;
		case PT_COLOURVALUE:
			{
				return Any(StringConverter::parseColourValue(value));
			}
			break;
		case PT_AXISALIGNEDBOX:
			{
				return Any(StringConverterEx::parseAxisAlignedBox(value));
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// 把当前属性集中所有属性拷贝到target中
	void PropertyContainer::cloneProperties(PropertyContainer *target)
	{
		const MapProperty *properties = getPropertiesList();
		// 遍历所有属性
		for(MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			target->setProperty(iter->second.id , getProperty(iter->second.id));
		}
	}


	// 写入Any值到文件file
	void PropertyContainer::writeAny(FILE* file , const Any &any , PropertyType type)
	{
		switch(type)
		{
		case PT_GROUP:
			{
			}
			break;
		case PT_BOOL:
			{
				bool value(any_cast<bool>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_REAL:
			{
				Real value(any_cast<Real>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_INT:
			{
				int value(any_cast<int>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_UINT:
			{
				uint value(any_cast<uint>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_SHORT:
			{
				short value(any_cast<short>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_USHORT:
			{
				ushort value(any_cast<ushort>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_LONG:
			{
				long value(any_cast<long>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_ULONG:
			{
				ulong value(any_cast<ulong>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_STRING:
			{
				String value(any_cast<String>(any));
				fputs(value.c_str(), file);
				// Write terminating newline char
				fputc('\n', file);
			}
			break;
		case PT_VECTOR2:
			{
				Vector2 value(any_cast<Vector2>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_VECTOR3:
			{
				Vector3 value(any_cast<Vector3>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_MATRIX3:
			{
				Matrix3 value(any_cast<Matrix3>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_MATRIX4:
			{
				Matrix4 value(any_cast<Matrix4>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_QUATERNION:
			{
				Quaternion value(any_cast<Quaternion>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_COLOURVALUE:
			{
				ColourValue value(any_cast<ColourValue>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		case PT_AXISALIGNEDBOX:
			{
				AxisAlignedBox value(any_cast<AxisAlignedBox>(any));
				fwrite((void* const)&value, sizeof(value), 1, file);
			}
			break;
		default:
			break;
		}
	}
	// 从文件file读取一个Any值
	Any PropertyContainer::readAny(FILE* file , PropertyType type)
	{
		DataStreamPtr stream(new FileHandleDataStreamEx(file , false));
		return readAny(stream , type);
	}
	// 从数据流stream读取一个Any值
	Any PropertyContainer::readAny(DataStreamPtr &stream , PropertyType type)
	{
		switch(type)
		{
		case PT_GROUP:
			{
				return Any();
			}
			break;
		case PT_BOOL:
			{
				bool value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_REAL:
			{
				Real value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_INT:
			{
				int value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_UINT:
			{
				uint value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_SHORT:
			{
				short value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_USHORT:
			{
				ushort value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_LONG:
			{
				long value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_ULONG:
			{
				ulong value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_STRING:
			{
				String value = stream->getLine(false);
				return Any(value);
			}
			break;
		case PT_VECTOR2:
			{
				Vector2 value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_VECTOR3:
			{
				Vector3 value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_MATRIX3:
			{
				Matrix3 value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_MATRIX4:
			{
				Matrix4 value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_QUATERNION:
			{
				Quaternion value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_COLOURVALUE:
			{
				ColourValue value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		case PT_AXISALIGNEDBOX:
			{
				AxisAlignedBox value;
				stream->read(&value , sizeof(value));
				return Any(value);
			}
			break;
		default:
			return Any();
			break;
		}
	}
};