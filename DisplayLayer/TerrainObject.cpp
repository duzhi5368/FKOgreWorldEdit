#include "StdAfx.h"
#include "TerrainObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "ETTerrain.h"
namespace Ogre
{

	const String TerrainObjectFactory::FACTORY_TYPE = "TerrainObject";

	// 获得该工厂的类型名称
	const String &TerrainObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *TerrainObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		TerrainObject *obj = new TerrainObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void TerrainObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	TerrainObject::TerrainObject(const String &name)
		: NullObject(name)
		, m_terrain(0)
	{
	}

	TerrainObject::~TerrainObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void TerrainObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// 创建实体
		m_terrain = new ETTerrain();
		m_terrain->init(scene->getOgreSceneManager() , m_scene->getCollisionWorld() , getName());
		m_terrain->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_terrain->setBaseDir("../Data/ET/");
		// 挂接上去
		m_node->attachObject(m_terrain->getTerrainImpl());
	}
	// 获得MovableObject
	MovableObject *TerrainObject::getOgreMovableObject()const
	{
		return m_terrain->getTerrainImpl();
	}

	// 销毁对象实体
	void TerrainObject::destroy()
	{
		OGRE_DELETE(m_terrain);
		NullObject::destroy();
	}

	// 保存地形到文件
	void TerrainObject::save(const String &fileName)
	{
		m_fileName = fileName;
		if(m_terrain)
		{
			m_terrain->saveTerrain(m_fileName);
		}
	}

	// 枚举所有属性，以树的形式
	void TerrainObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Terrain , PT_GROUP , "地形设置" , "地形设置" , true , false , false));
		properties.push_back(Property( ID_TerrainFile , PT_STRING , "地形文件" , "地形文件" , true , true , true));
	}

	// 设置属性
	bool TerrainObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Terrain:		// 地形设置
			{
				return true;
			}
			break;
		case ID_TerrainFile:		// 地形文件
			{
				m_fileName = any_cast<String>(value);
				m_terrain->loadTerrain(m_fileName);
				
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any TerrainObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Terrain:		// 地形设置
			{
				return Any();
			}
			break;
		case ID_TerrainFile:		// 地形文件
			{
				return Any(m_fileName);
			}
			break;
		default:
			return Any();
			break;
		}
	}


	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* TerrainObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* TerrainObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}
};