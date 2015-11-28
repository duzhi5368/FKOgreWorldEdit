#include "StdAfx.h"
#include "ObjectEditorPlugin.h"
#include "DlgPlaceSceneObject.h"
#include "DlgEditTools.h"
#include "DlgTrackTools.h"
#include "DlgObjectProperty.h"

#include "ISceneObjectVisible.h"

#include "ObjectSelectAction.h"
#include "ObjectDragAction.h"
#include "ObjectMoveAction.h"
#include "ObjectRotateAction.h"
#include "ObjectScaleAction.h"
#include "ObjectDeleteAction.h"
#include "ObjectCloneAction.h"
#include "ObjectPlaceFloorAction.h"

#include "TrackKeyFrameObject.h"

namespace Ogre
{
	// 单件模式实现
	template<> ObjectEditorPlugin* Singleton<ObjectEditorPlugin>::ms_Singleton = 0;
	ObjectEditorPlugin* ObjectEditorPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	ObjectEditorPlugin& ObjectEditorPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	ObjectEditorPlugin::ObjectEditorPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_objectAction(0)
	{
		m_description.name = "物件编辑插件";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "场景物件的摆放编辑";
		m_description.publish = "王宏张 2008-2-24";
		m_description.version = 1;

		m_editableSceneObjectType.push_back(NullObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(EntityObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(CameraObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(ParticleObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(EnvironmentObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(BoxTriggerObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(SphereTriggerObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(AnimationTrackObjectFactory::FACTORY_TYPE);
	}

	ObjectEditorPlugin::~ObjectEditorPlugin(void)
	{
	}


	// 初始化插件，传进来的worldEditor是世界编辑器的接口
	bool ObjectEditorPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;

		m_worldEditor->getMainFrame()->addListener(this);


		CXTPDockingPane* paneInsert = m_worldEditor->getMainFrame()->createPane(DlgPlaceSceneObject::IDD , "物件摆放" , CRect(0 , 0 , 230 , 230) , xtpPaneDockRight);
		CXTPDockingPane* paneTools = m_worldEditor->getMainFrame()->createPane(DlgEditTools::IDD , "物件工具箱" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneInsert);
		CXTPDockingPane* paneTrackTools = m_worldEditor->getMainFrame()->createPane(DlgTrackTools::IDD , "轨迹编辑器" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneInsert);
		CXTPDockingPane* paneProperty = m_worldEditor->getMainFrame()->createPane(DlgObjectProperty::IDD , "物件属性" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneInsert);
		CXTPDockingPane* paneList = m_worldEditor->getMainFrame()->createPane(DlgObjectList::IDD , "物件列表" , CRect(0 , 0 , 230 , 230) , xtpPaneDockRight , paneInsert);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->AttachPane(paneList , paneInsert);
		paneInsert->SetFocus();

		getWorldEditor()->getMapManager()->addListener(&m_DlgObjectProperty);
		getWorldEditor()->getMapManager()->addListener(&m_DlgObjectList);
		getWorldEditor()->getMapManager()->addListener(this);
		// 添加场景侦听器
		if(m_worldEditor->getMapManager()->getScene())
		{
			m_worldEditor->getMapManager()->getScene()->addListener(this);
		}

		m_worldEditor->getMainFrame()->createPluginToolBarButton("物件插件" , this);

		// 添加轨迹关键帧的对象工厂
		ISceneSystem::getSingleton().addFactory(new TrackKeyFrameObjectFactory);

		m_activated = true;
		deactivate();
		m_inited = true;
		return m_inited;
	}

	// 关闭插件
	void ObjectEditorPlugin::shutdown()
	{
		if(!m_inited)
			return;

		deactivate();
		m_worldEditor->getMainFrame()->destroyPluginToolBarButton("物件插件");
		getWorldEditor()->getMapManager()->removeListener(&m_DlgObjectProperty);
		getWorldEditor()->getMapManager()->removeListener(&m_DlgObjectList);
		getWorldEditor()->getMapManager()->removeListener(this);
		m_worldEditor->getMainFrame()->removeListener(this);
		m_worldEditor->getMainFrame()->destroyPane(DlgPlaceSceneObject::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgTrackTools::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgEditTools::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgObjectProperty::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgObjectList::IDD);
		// 删除场景侦听器
		if(m_worldEditor->getMapManager()->getScene())
		{
			m_worldEditor->getMapManager()->getScene()->removeListener(this);
		}

		// 删除轨迹关键帧的对象工厂
		ISceneSystem::getSingleton().removeFactory(TrackKeyFrameObjectFactory::FACTORY_TYPE);

		m_inited = false;
	}


	// 设置当前的物件操作器
	void ObjectEditorPlugin::setObjectAction(ObjectAction *objectAction)
	{
		OGRE_DELETE(m_objectAction);
		m_objectAction = objectAction;
	}

	// 是否已经初始化
	bool ObjectEditorPlugin::isInited()const
	{
		return m_inited;
	}

	// 激活插件
	void ObjectEditorPlugin::activate()
	{
		if(m_activated)
			return;
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgPlaceSceneObject::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgEditTools::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgObjectProperty::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgObjectList::IDD);
		m_worldEditor->getMainFrame()->addChildViewListener(this);
		m_activated = true;
	}

	// 反激活插件
	void ObjectEditorPlugin::deactivate()
	{
		if(!m_activated)
			return;
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTrackTools::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgPlaceSceneObject::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgEditTools::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgObjectProperty::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgObjectList::IDD);
		m_DlgTrackTools.deactivate();
		OGRE_DELETE(m_objectAction);
		m_worldEditor->getMainFrame()->removeChildViewListener(this);
		m_activated = false;
	}

	// 是否已经激活
	bool ObjectEditorPlugin::isActivate()const
	{
		return m_activated;
	}

	// 获得插件描述
	const IPlugin::Description &ObjectEditorPlugin::getDescription()const
	{
		return m_description;
	}

	// 主窗口消息
	bool ObjectEditorPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if(message == WM_KEYUP)
		{
			switch(wParam)
			{
			case 'S':
				{
					setObjectAction(0);
					setObjectAction(new ObjectSelectAction());
				}
				break;
			case 'D':
			case VK_DELETE:
				{
					ObjectDeleteAction action;
					action.execute();
				}
				break;
			case 'C':
				{
					ObjectCloneAction action;
					action.execute();
				}
				break;
			case 'T':
				{
					ObjectPlaceFloorAction action;
					action.execute();
				}
				break;
			case 'A':
				{
					setObjectAction(0);
					setObjectAction(new ObjectDragAction());
				}
				break;
			case 'M':
				{
					setObjectAction(0);
					setObjectAction(new ObjectMoveAction());
				}
				break;
			case 'R':
				{
					setObjectAction(0);
					setObjectAction(new ObjectRotateAction());
				}
				break;
			case 'L':
				{
					setObjectAction(0);
					setObjectAction(new ObjectScaleAction());
				}
				break;
			}
		}
		return true;
	}
	// 查询一些数据，然后返回到result中
	bool ObjectEditorPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// 浮动窗口通知
	LRESULT ObjectEditorPlugin::onDockingPaneNotify(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == XTP_DPN_SHOWWINDOW)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

			if (!pPane->IsValid())
			{
				switch (pPane->GetID())
				{
				case DlgPlaceSceneObject::IDD:
					{
						if (m_PlaceSceneObject.GetSafeHwnd() == 0)
						{
							m_PlaceSceneObject.Create(DlgPlaceSceneObject::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_PlaceSceneObject);
					}
					break;
				case DlgEditTools::IDD:
					{
						if (m_DlgEditTools.GetSafeHwnd() == 0)
						{
							m_DlgEditTools.Create(DlgEditTools::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgEditTools);
					}
					break;
				case DlgTrackTools::IDD:
					{
						if (m_DlgTrackTools.GetSafeHwnd() == 0)
						{
							m_DlgTrackTools.Create(DlgTrackTools::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTrackTools);
					}
					break;
				case DlgObjectProperty::IDD:
					{
						if (m_DlgObjectProperty.GetSafeHwnd() == 0)
						{
							m_DlgObjectProperty.Create(DlgObjectProperty::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgObjectProperty);
					}
					break;
				case DlgObjectList::IDD:
					{
						if (m_DlgObjectList.GetSafeHwnd() == 0)
						{
							m_DlgObjectList.Create(DlgObjectList::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgObjectList);
					}
					break;
				default:
					return FALSE;
					break;
				}
			}
			return TRUE;
		}
		return FALSE;
	}

	// 视口更新
	void ObjectEditorPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
	}


	// 是否支持此物件的动态编辑
	bool ObjectEditorPlugin::isEditable(ISceneObject *obj)const
	{
		return std::find(m_editableSceneObjectType.begin() , m_editableSceneObjectType.end() , obj->getType()) != m_editableSceneObjectType.end()
			&& ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->hasSceneObject(obj->getName());
	}
	// 从一个MO获得一个可编辑的场景物件
	ISceneObject* ObjectEditorPlugin::getEditableSceneObject(MovableObject *obj)const
	{
		ISceneObject *sceneObject = obj_cast(obj->getUserAny());
		if(!sceneObject || !isEditable(sceneObject))
			return 0;
		if(sceneObject->getType() == CameraObjectFactory::FACTORY_TYPE && obj->getMovableType() == "Camera")
			return 0;
		return sceneObject;
	}

	// 新建场景
	void ObjectEditorPlugin::onNewScene(IScene *scene)
	{
		scene->addListener(this);
	}
	// 载入一个场景
	void ObjectEditorPlugin::onPreLoadScene(const String &fileName ,IScene *scene)
	{
		scene->addListener(this);
	}
	// 销毁场景
	void ObjectEditorPlugin::onDestroyScene(IScene *scene)
	{
		m_DlgTrackTools.deactivate();
		scene->removeListener(this);
		OGRE_DELETE(m_objectAction);
	};


	// 射线碰撞检测，仅返回第一个发生碰撞的信息（如果没有地形，就碰撞0平面）
	// @ray 射线
	// @collideWorld 是否跟世界碰撞（如：地形）
	// @collideObject 是否跟物件碰撞
	// @intersection 输出交点
	bool ObjectEditorPlugin::collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection)
	{
		// 使用场景碰撞检测
		if(!m_worldEditor->getMapManager()->getScene()->collide(ray , true , false , intersection))
		{
			// 如果没有碰撞到场景，则使用0平面正面碰撞
			std::pair<bool, Real> result = Math::intersects(ray , Plane(Vector3::UNIT_Y , 0));
			if(result.first)
			{
				intersection = ray.getPoint(result.second);
				return true;
			}
			else
			{
				// 如果正面碰撞不到，就碰撞反面
				std::pair<bool, Real> result = Math::intersects(ray , Plane(Vector3::NEGATIVE_UNIT_Y , 0));
				if(result.first)
				{
					intersection = ray.getPoint(result.second);
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return true;
		}
	}


	// 当创建一个场景对象后调用
	void ObjectEditorPlugin::onCreateSceneObject(ISceneObject *obj)
	{
		if(isEditable(obj))
		{
			obj->getSceneObjectVisible()->showEntity(true);
		}
	}


	// 选中一个场景对象
	void ObjectEditorPlugin::onSelectObject(ISceneObject *obj)
	{
		//// 如果单独寻中了一个动画轨迹对象，则激活编辑这个轨迹
		//if(obj->getType() == AnimationTrackObjectFactory::FACTORY_TYPE)
		//{
		//	m_DlgTrackTools.activate(static_cast<AnimationTrackObject*>(obj));
		//}
	}
	// 反选一个场景对象
	void ObjectEditorPlugin::onDeselectObject(ISceneObject *obj)
	{
	}

};