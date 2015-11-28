#include "StdAfx.h"
#include "EnvironmentObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "EnvironmentObjectVisible.h"
namespace Ogre
{

	const String EnvironmentObjectFactory::FACTORY_TYPE = "EnvironmentObject";

	// 获得该工厂的类型名称
	const String &EnvironmentObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *EnvironmentObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		EnvironmentObject *obj = new EnvironmentObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void EnvironmentObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	EnvironmentObject::EnvironmentObject(const String &name)
		: NullObject(name)
		, m_environmentInited(false)
		, m_skyBoxEnabled(false)
		, m_skyBoxRenderDistance(0)
		, m_fogColor(ColourValue::White)
		, m_fogDensity(0)
		, m_fogMode(FOG_NONE)
		, m_fogStart(0)
		, m_fogEnd(0)
		, m_ambientLight(ColourValue::White)
	{
		m_visible = false;
	}

	EnvironmentObject::~EnvironmentObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void EnvironmentObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);

		// 从场景管理器读取环境设置
		_getEnvironment();
	}

	// 销毁对象实体
	void EnvironmentObject::destroy()
	{
		_disableEnvironment();
		NullObject::destroy();
		m_environmentInited = false;
	}

	// 从场景管理器读取环境设置
	void EnvironmentObject::_getEnvironment()
	{
		SceneManager *mgr = m_scene->getOgreSceneManager();
		// 天空盒设置
		m_skyBoxEnabled = mgr->isSkyBoxEnabled();					// 是否开启天空盒
		//m_skyBoxMaterialName;			// 天空盒纹理
		m_skyBoxRenderDistance = 50.0f;			// 渲染距离

		// 雾化设置
		m_fogMode = mgr->getFogMode();						// 雾化方式
		m_fogColor = mgr->getFogColour();					// 雾化颜色
		m_fogDensity = mgr->getFogDensity();						// 雾化密度
		m_fogStart = mgr->getFogStart();						// 雾化开始距离
		m_fogEnd = mgr->getFogEnd();							// 雾化结束距离

		// 光照设置
		m_ambientLight = mgr->getAmbientLight();				// 环境光颜色

	}


	// 枚举所有属性，以树的形式
	void EnvironmentObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Environment , PT_GROUP , "环境设置" , "环境设置" , true , false , false));
		properties.back().addChild(Property( ID_SkyBox , PT_GROUP , "天空盒设置" , "天空盒设置" , true , false , false));
		properties.back().childs.back().addChild(Property(ID_SkyBoxEnabled , PT_BOOL , "是否开启天空盒" , "是否开启天空盒" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_SkyBoxMaterialName , PT_STRING , "天空盒纹理" , "天空盒纹理" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_SkyBoxRenderDistance , PT_REAL , "渲染距离" , "渲染距离" , true , true , true));

		properties.back().addChild(Property( ID_Fog , PT_GROUP , "雾化设置" , "雾化设置" , true , false , false));
		properties.back().childs.back().addChild(Property(ID_FogMode , PT_UINT , "雾化方式" , "雾化方式" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogColor , PT_COLOURVALUE , "雾化颜色" , "雾化颜色" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogExpDensity , PT_REAL , "雾化密度" , "雾化密度" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogStart , PT_REAL , "雾化开始距离" , "雾化开始距离" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogEnd , PT_REAL , "雾化结束距离" , "雾化结束距离" , true , true , true));

		properties.back().addChild(Property( ID_Lighting , PT_GROUP , "光照设置" , "光照设置" , true , false , false));
		properties.back().childs.back().addChild(Property(ID_AmbientLight , PT_COLOURVALUE , "环境光颜色" , "环境光颜色" , true , true , true));

	}

	// 设置属性
	bool EnvironmentObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Environment:						// 环境设置
			{
				return true;
			}
			break;
		case ID_SkyBox:							// 天空盒设置
			{
				return true;
			}
			break;
		case ID_SkyBoxEnabled:					// 是否开启天空盒
			{
				bool skyBoxEnabled = any_cast<bool>(value);
				if(skyBoxEnabled != m_skyBoxEnabled)
				{
					m_skyBoxEnabled = skyBoxEnabled;
					_setSky();
				}
				return true;
			}
			break;
		case ID_SkyBoxMaterialName:				// 天空盒纹理
			{
				String skyBoxMaterialName = any_cast<String>(value);
				if(skyBoxMaterialName != m_skyBoxMaterialName)
				{
					m_skyBoxMaterialName = skyBoxMaterialName;
					_setSky();
				}
				return true;
			}
			break;
		case ID_SkyBoxRenderDistance:			// 渲染距离
			{
				Real skyBoxRenderDistance = any_cast<Real>(value);
				if(skyBoxRenderDistance != m_skyBoxRenderDistance)
				{
					m_skyBoxRenderDistance = skyBoxRenderDistance;
					_setSky();
				}
				return true;
			}
			break;

		case ID_Fog:								// 雾化设置
			{
				return true;
			}
			break;
		case ID_FogMode:							// 雾化方式
			{
				FogMode fogMode = (FogMode)any_cast<uint>(value);
				if(fogMode != m_fogMode)
				{
					m_fogMode = fogMode;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogColor:						// 雾化颜色
			{
				ColourValue fogColor = any_cast<ColourValue>(value);
				if(fogColor != m_fogColor)
				{
					m_fogColor = fogColor;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogExpDensity:					// 雾化密度
			{
				Real fogDensity = any_cast<Real>(value);
				if(fogDensity != m_fogDensity)
				{
					m_fogDensity = fogDensity;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogStart:						// 雾化开始距离
			{
				Real fogStart = any_cast<Real>(value);
				if(fogStart != m_fogStart)
				{
					m_fogStart = fogStart;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogEnd:							// 雾化结束距离
			{
				Real fogEnd = any_cast<Real>(value);
				if(fogEnd != m_fogEnd)
				{
					m_fogEnd = fogEnd;
					_setFog();
				}
				return true;
			}
			break;

		case ID_Lighting:						// 光照设置
			{
				return true;
			}
			break;
		case ID_AmbientLight:					// 环境光颜色
			{
				ColourValue ambientLight = any_cast<ColourValue>(value);
				if(ambientLight != m_ambientLight)
				{
					m_ambientLight = ambientLight;
					_setLighting();
				}
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any EnvironmentObject::getProperty(uint id)const
	{
		switch(id)
		{
		case ID_Environment:						// 环境设置
			{
				return Any();
			}
			break;
		case ID_SkyBox:							// 天空盒设置
			{
				return Any();
			}
			break;
		case ID_SkyBoxEnabled:					// 是否开启天空盒
			{
				return Any(m_skyBoxEnabled);
			}
			break;
		case ID_SkyBoxMaterialName:				// 天空盒纹理
			{
				return Any(m_skyBoxMaterialName);
			}
			break;
		case ID_SkyBoxRenderDistance:			// 渲染距离
			{
				return Any(m_skyBoxRenderDistance);
			}
			break;

		case ID_Fog:								// 雾化设置
			{
				return Any();
			}
			break;
		case ID_FogMode:							// 雾化方式
			{
				return Any((uint)m_fogMode);
			}
			break;
		case ID_FogColor:						// 雾化颜色
			{
				return Any(m_fogColor);
			}
			break;
		case ID_FogExpDensity:					// 雾化密度
			{
				return Any(m_fogDensity);
			}
			break;
		case ID_FogStart:						// 雾化开始距离
			{
				return Any(m_fogStart);
			}
			break;
		case ID_FogEnd:							// 雾化结束距离
			{
				return Any(m_fogEnd);
			}
			break;

		case ID_Lighting:						// 光照设置
			{
				return Any();
			}
			break;
		case ID_AmbientLight:					// 环境光颜色
			{
				return Any(m_ambientLight);
			}
			break;
		default:
			if(id > ID_NullObject_Begin && id < ID_NullObject_End)
				return NullObject::getProperty(id);
			else
				return Any();
			break;
		}
	}
	// 设置是否显示
	void EnvironmentObject::setVisible(bool visible)
	{
		if(m_visible == visible && !m_environmentInited)
			return;

		if(visible)
		{
			_setEnvironment();
			m_environmentInited = true;
		}
		else
		{
			_disableEnvironment();
		}		
	}
	// 设置天空
	void EnvironmentObject::_setSky()
	{
		if(m_visible)
		{
			m_scene->getOgreSceneManager()->setSkyBox(!m_skyBoxMaterialName.empty() && m_skyBoxEnabled , m_skyBoxMaterialName , m_skyBoxRenderDistance);
		}
	}
	// 设置雾化
	void EnvironmentObject::_setFog()
	{
		if(m_visible)
		{
			m_scene->getOgreSceneManager()->setFog(m_fogMode , m_fogColor , m_fogDensity , m_fogStart , m_fogEnd);
		}
	}
	// 设置光照
	void EnvironmentObject::_setLighting()
	{
		if(m_visible)
		{
			m_scene->getOgreSceneManager()->setAmbientLight(m_ambientLight);
		}
	}

	// 禁用环境
	void EnvironmentObject::_disableEnvironment()
	{
		if(m_scene && m_visible)
		{
			m_scene->getOgreSceneManager()->setSkyBox(false , StringUtil::BLANK);
			m_scene->getOgreSceneManager()->setFog(FOG_NONE);
			m_scene->getOgreSceneManager()->setAmbientLight(ColourValue(0.5f , 0.5f , 0.5f));
			m_visible = false;
		}
	}

	// 设置环境
	void EnvironmentObject::_setEnvironment()
	{
		m_visible = true;
		_setFog();
		_setSky();
		_setLighting();
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* EnvironmentObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* EnvironmentObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *EnvironmentObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new EnvironmentObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
};