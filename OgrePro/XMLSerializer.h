#pragma once
#include "TinyXMLInclude.h"
#include "PropertyContainer.h"

namespace Ogre
{

	// XML序列化器
	class OGREPRO_API XMLSerializer
	{
	public:
		XMLSerializer(void);
		virtual ~XMLSerializer(void);

	protected:
		// XML文档
		TiXmlDocumentPtr m_xmlDoc;

		// 新建一个XML文件
		bool newXML(const String &rootElementName);
		// 打开XML文件
		bool openXML(DataStreamPtr& stream);
		// 保存XML文件
		bool saveXML(const String& fileName);
		// 关闭XML文件
		void closeXML();

		// 获得颜色属性，对应xml节点里的r、g、b属性，0~1表示最暗和最亮
		ColourValue getColorAttribute(TiXmlElement *element);
		// 获得Vector3，对应xml节点的x/y/z属性
		Vector3 getVector3Attribute(TiXmlElement *element);
		// 获得Quatation，对应xml节点的x/y/z/w属性
		Quaternion getQuaternionAttribute(TiXmlElement *element);
		// 获得字符串属性
		String getAttribute(TiXmlElement *element , const char *name);

		// 设置属性
		void setAttribute(TiXmlElement *element , const String &name , const String &value);
		void setAttribute(TiXmlElement *element , const String &name , const NameValuePairList &value);

		// 新建一个节点
		TiXmlElement *newElement(TiXmlElement *parent , const String &name);
	};

};