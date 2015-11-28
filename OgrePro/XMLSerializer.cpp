#include "StdAfx.h"
#include "XMLSerializer.h"
#include "StringConverterEx.h"

namespace Ogre
{
	XMLSerializer::XMLSerializer(void)
	{
	}

	XMLSerializer::~XMLSerializer(void)
	{
		closeXML();
	}
	// 新建一个XML文件
	bool XMLSerializer::newXML(const String &rootElementName)
	{
		closeXML();
		m_xmlDoc.bind(new TiXmlDocument());
		TiXmlElement element(rootElementName.c_str());
		m_xmlDoc->InsertEndChild(element);
		return true;
	}

	// 打开XML文件
	bool XMLSerializer::openXML(DataStreamPtr& stream)
	{
		closeXML();
		m_xmlDoc.bind(new TiXmlDocument());
		// 分析XML文件
		size_t iSize = stream->size();
		char *pBuf = new char[iSize+1];
		memset(pBuf, 0, iSize+1);
		stream->read(pBuf, iSize);
		stream.setNull();
		m_xmlDoc->Parse(pBuf);
		OGRE_DELETE_ARRAY(pBuf);

		// 检查XML文件是否读取成功
		if(m_xmlDoc->Error())
		{
			m_xmlDoc.setNull();

			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"打开XML文件失败",
				"XMLSerializer::openXML");
			return false;
		}
		return true;
	}
	// 保存XML文件
	bool XMLSerializer::saveXML(const String& fileName)
	{
		if(m_xmlDoc.isNull())
			return false;
		return m_xmlDoc->SaveFile(fileName.c_str());
	}
	// 关闭XML文件
	void XMLSerializer::closeXML()
	{
		m_xmlDoc.setNull();
	}

	// 获得颜色属性，对应xml节点里的r、g、b属性，0~1表示最暗和最亮
	ColourValue XMLSerializer::getColorAttribute(TiXmlElement *element)
	{
		return ColourValue(StringConverter::parseReal(getAttribute(element , "r"))
			, StringConverter::parseReal(getAttribute(element , "g"))
			, StringConverter::parseReal(getAttribute(element , "b"))
			, element->Attribute("a") ? StringConverter::parseReal(getAttribute(element , "a")) : 1.0f
			);
	}
	// 获得Vector3，对应xml节点的x/y/z属性
	Vector3 XMLSerializer::getVector3Attribute(TiXmlElement *element)
	{
		return Vector3(StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// 获得Quatation，对应xml节点的x/y/z/w属性
	Quaternion XMLSerializer::getQuaternionAttribute(TiXmlElement *element)
	{
		return Quaternion(StringConverter::parseReal(getAttribute(element , "w"))
			, StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// 获得字符串属性
	String XMLSerializer::getAttribute(TiXmlElement *element , const char *name)
	{
		if(!element)
			return StringUtil::BLANK;
		const char *value = element->Attribute(name);
		if(!value)
			return StringUtil::BLANK;
		else
			return value;
	}



	// 新建一个节点
	TiXmlElement *XMLSerializer::newElement(TiXmlElement *parent , const String &name)
	{
		TiXmlElement element(name.c_str());
		return (TiXmlElement*)parent->InsertEndChild(element);
	}

	// 设置属性
	void XMLSerializer::setAttribute(TiXmlElement *element , const String &name , const String &value)
	{
		element->SetAttribute(name.c_str() , value.c_str());
	}

	void XMLSerializer::setAttribute(TiXmlElement *element , const String &name , const NameValuePairList &value)
	{
		setAttribute(element , name , StringConverterEx::toString(value));
	}

};