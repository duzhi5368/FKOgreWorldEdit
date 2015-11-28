#include "StdAfx.h"
#include "CameraObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "CameraObjectVisible.h"
namespace Ogre
{

	const String CameraObjectFactory::FACTORY_TYPE = "CameraObject";

	// 获得该工厂的类型名称
	const String &CameraObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *CameraObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		CameraObject *obj = new CameraObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void CameraObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	CameraObject::CameraObject(const String &name)
		: NullObject(name)
		, m_camera(0)
	{
	}

	CameraObject::~CameraObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void CameraObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// 创建实体
		m_camera = static_cast<Camera*>(m_scene->getOgreSceneManager()->createCamera(getFullName()));
		m_camera->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// 挂接上去
		m_node->attachObject(m_camera);
	}

	// 销毁对象实体
	void CameraObject::destroy()
	{
		if(m_camera)
		{
			// 销毁模型实体
			m_scene->getOgreSceneManager()->destroyCamera(m_camera);
			m_camera = 0;
		}
		NullObject::destroy();
	}

	// 鼠标点击测试
	// @x 鼠标x屏幕坐标（0~1）
	// @y 鼠标y屏幕坐标（0~1）
	// @pos 点击相交的坐标
	// @return 是否相交了
	bool CameraObject::hitTest(float x , float y , Vector3 &pos)
	{
		if(!m_camera)
		{
			return false;
		}
		else
		{
			return m_scene->collide(m_camera->getCameraToViewportRay(x , y) , true ,true , pos);
		}
	}


	// 枚举所有属性，以树的形式
	void CameraObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Camera , PT_GROUP , "摄像机设置" , "摄像机设置" , true , false , false));
		properties.back().addChild(Property(ID_PolygonMode , PT_LONG , "几何渲染模式" , "几何渲染模式" , true , true , true));
		properties.back().addChild(Property(ID_LodBias , PT_REAL , "LOD偏移" , "LOD偏移" , true , true , true));
		properties.back().addChild(Property(ID_NearClipDistance , PT_REAL , "近裁面" , "近裁面" , true , true , true));
		properties.back().addChild(Property(ID_FarClipDistance , PT_REAL , "远裁面" , "远裁面" , true , true , true));
		properties.back().addChild(Property(ID_FOVy , PT_REAL , "FOVy角度" , "FOVy角度" , true , true , true));
		properties.back().addChild(Property(ID_AspectRatio , PT_REAL , "窗口比率" , "窗口比率" , true , true , true));
		properties.back().addChild(Property(ID_ProjectionType , PT_LONG , "投影方式" , "投影方式" , true , true , true));
		properties.back().addChild(Property(ID_OrthoWindow , PT_VECTOR2 , "正交投影窗口大小" , "正交投影窗口大小" , true , true , true));
	}

	// 设置属性
	bool CameraObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Camera:							// 摄像机设置
			{
				return true;
			}
			break;
		case ID_PolygonMode:						// 几何渲染模式
			{
				m_camera->setPolygonMode((PolygonMode)any_cast<long>(value));
				return true;
			}
			break;
		case ID_LodBias:							// LOD偏移
			{
				m_camera->setLodBias(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_NearClipDistance:				// 近裁面
			{
				m_camera->setNearClipDistance(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_FarClipDistance:					// 远裁面
			{
				m_camera->setFarClipDistance(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_FOVy:							// FOVy角度
			{
				m_camera->setFOVy(Degree(any_cast<Real>(value)));
				return true;
			}
			break;
		case ID_AspectRatio:						// 窗口比率
			{
				m_camera->setAspectRatio(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_ProjectionType:					// 投影方式
			{
				m_camera->setProjectionType((ProjectionType)any_cast<long>(value));
				return true;
			}
			break;
		case ID_OrthoWindow:						// 正交投影窗口大小
			{
				Vector2 window(any_cast<Vector2>(value));
				m_camera->setOrthoWindow(window.x , window.y);
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any CameraObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Camera:							// 摄像机设置
			{
				return Any();
			}
			break;
		case ID_PolygonMode:						// 几何渲染模式
			{
				return Any((long)m_camera->getPolygonMode());
			}
			break;
		case ID_LodBias:							// LOD偏移
			{
				return Any(m_camera->getLodBias());
			}
			break;
		case ID_NearClipDistance:				// 近裁面
			{
				return Any(m_camera->getNearClipDistance());
			}
			break;
		case ID_FarClipDistance:					// 远裁面
			{
				return Any(m_camera->getFarClipDistance());
			}
			break;
		case ID_FOVy:							// FOVy角度
			{
				return Any(m_camera->getFOVy().valueDegrees());
			}
			break;
		case ID_AspectRatio:						// 窗口比率
			{
				return Any(m_camera->getAspectRatio());
			}
			break;
		case ID_ProjectionType:					// 投影方式
			{
				return Any((long)m_camera->getProjectionType());
			}
			break;
		case ID_OrthoWindow:						// 正交投影窗口大小
			{
				return Any(Vector2(m_camera->getOrthoWindowWidth() , m_camera->getOrthoWindowHeight()));
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* CameraObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* CameraObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *CameraObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new CameraObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

};