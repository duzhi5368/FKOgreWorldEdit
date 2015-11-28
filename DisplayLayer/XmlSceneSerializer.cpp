#include "StdAfx.h"
#include "XMLSceneSerializer.h"
#include "IScene.h"
namespace Ogre
{

	XMLSceneSerializer::XMLSceneSerializer(void)
	{
	}

	XMLSceneSerializer::~XMLSceneSerializer(void)
	{
	}


	// 导入场景
	bool XMLSceneSerializer::importScene(DataStreamPtr& stream, IScene* scene)
	{
		m_scene = scene;
		// 打开XML文件
		if(!openXML(stream))
			return false;
		// 遍历所有Scene
		for (TiXmlElement* element = m_xmlDoc->RootElement()->FirstChildElement("Scene");
			element != 0; element = element->NextSiblingElement("Scene"))
		{
			readScene(element);
		}
		return true;
	}


	// 读取场景
	void XMLSceneSerializer::readScene(TiXmlElement *parent)
	{
		ISceneObject *obj = m_scene->getRootObject();
		readSceneObjectChilds(parent , obj);
	}
	// 读取所有子对象
	void XMLSceneSerializer::readSceneObjectChilds(TiXmlElement *element , ISceneObject *parent)
	{
		// 遍历所有SceneObject
		for (TiXmlElement* childElement = element->FirstChildElement("SceneObject");
			childElement != 0; childElement = childElement->NextSiblingElement("SceneObject"))
		{
			readSceneObject(childElement , parent);
		}
	}

	// 读取场景对象
	void XMLSceneSerializer::readSceneObject(TiXmlElement *element , ISceneObject *parent)
	{
		String name = getAttribute(element , "Name");
		String type = getAttribute(element , "Type");
		NameValuePairList params(StringConverterEx::parseNameValuePairList(getAttribute(element , "CreationParams")));
		// 创建场景对象
		ISceneObject *obj = parent->createChildObject(name , type , &params);
		// 遍历所有属性
		for (TiXmlElement* propertyElement = element->FirstChildElement("Property");
			propertyElement != 0; propertyElement = propertyElement->NextSiblingElement("Property"))
		{
			// 读取值
			uint propertyId = StringConverter::parseUnsignedInt(getAttribute(propertyElement , "Id"));
			PropertyContainer::PropertyType propertyType = (PropertyContainer::PropertyType)StringConverter::parseUnsignedInt(getAttribute(propertyElement , "Type"));
			String propertyValue = getAttribute(propertyElement , "Value");
			if(propertyId)
			{
				obj->setProperty(propertyId , PropertyContainer::toAny(propertyValue , propertyType));
			}
		}
		// 读取所有子对象
		TiXmlElement *childsElement = element->FirstChildElement("Childs");
		if(childsElement)
		{
			readSceneObjectChilds(childsElement , obj);
		}
	}

	// 导出场景
	bool XMLSceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		m_scene = scene;

		LogManager::getSingleton().logMessage("XMLSceneSerializer writing XMLScene data to " + filename + "...");

		// 新建xml文件
		if(!newXML("Scenes"))
		{
			return false;
		}
		TiXmlElement *scenesElement = m_xmlDoc->RootElement();

		LogManager::getSingleton().logMessage("Writing XMLScene data...");
		writeScene(scenesElement);
		LogManager::getSingleton().logMessage("XMLScene data exported.");

		saveXML(filename);
		LogManager::getSingleton().logMessage("XMLSceneSerializer export successful.");

		closeXML();

		return true;
	}
	// 写入场景
	void XMLSceneSerializer::writeScene(TiXmlElement *parent)
	{
		// 开始包
		TiXmlElement *sceneElement = newElement(parent , "Scene");
		ISceneObject *obj = m_scene->getRootObject();
		writeSceneObjectChilds(sceneElement , obj);
	}
	// 写入场景对象
	void XMLSceneSerializer::writeSceneObject(TiXmlElement *parent , ISceneObject *obj)
	{
		const PropertyContainer::MapProperty *properties;
		properties = obj->getPropertiesList();
		// 开始包
		TiXmlElement *element = newElement(parent , "SceneObject");
		// 写入名称
		setAttribute(element , "Name" , obj->getName());
		// 写入类型
		setAttribute(element , "Type" , obj->getType());
		// 写入创建参数
		setAttribute(element , "CreationParams" , obj->getCreationParams());

		// 写入所有属性
		for(PropertyContainer::MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.saveable)
			{
				TiXmlElement *propertyElement = newElement(element , "Property");
				setAttribute(propertyElement , "Name" , iter->second.name);
				setAttribute(propertyElement , "Id" , StringConverter::toString(iter->second.id));
				setAttribute(propertyElement , "Type" , StringConverter::toString(iter->second.type));
				setAttribute(propertyElement , "Value" , PropertyContainer::toString(obj->getProperty(iter->second.id) , iter->second.type));
			}
		}
		// 写入所有子对象
		TiXmlElement *childs = newElement(element , "Childs");
		writeSceneObjectChilds(childs , obj);
	}

	// 写入子场景对象
	void XMLSceneSerializer::writeSceneObjectChilds(TiXmlElement *parent , ISceneObject *obj)
	{
		ISceneObject::ChildObjectIterator iter = obj->getChildObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *objChild = iter.getNext();
			writeSceneObject(parent , objChild);
		}
	}
};