#include "StdAfx.h"
#include "CameraControllerPlugin.h"

namespace Ogre
{
	// 单件模式实现
	template<> CameraControllerPlugin* Singleton<CameraControllerPlugin>::ms_Singleton = 0;
	CameraControllerPlugin* CameraControllerPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	CameraControllerPlugin& CameraControllerPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	CameraControllerPlugin::CameraControllerPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
	{
		m_description.name = "摄像机控制器插件";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "控制摄像机在世界中浏览";
		m_description.publish = "王宏张 2008-3-7";
		m_description.version = 1;
	}

	CameraControllerPlugin::~CameraControllerPlugin(void)
	{
	}


	// 初始化插件，传进来的worldEditor是世界编辑器的接口
	bool CameraControllerPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;

		m_worldEditor->getMainFrame()->addChildViewListener(this , false);
		activate();
		m_inited = true;
		return m_inited;
	}

	// 主窗口消息
	bool CameraControllerPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			// 如果按ALT，就屏蔽其他操作
			if(GetAsyncKeyState(VK_MENU))
			{
				return false;
			}
			break;
		}
		return true;
	}


	// 关闭插件
	void CameraControllerPlugin::shutdown()
	{
		if(!m_inited)
			return;
		m_worldEditor->getMainFrame()->removeChildViewListener(this);
		m_inited = false;
	}

	// 是否已经初始化
	bool CameraControllerPlugin::isInited()const
	{
		return m_inited;
	}

	// 激活插件
	void CameraControllerPlugin::activate()
	{
		if(m_activated)
			return;
		m_activated = true;
	}

	// 反激活插件
	void CameraControllerPlugin::deactivate()
	{
		if(!m_activated)
			return;
		m_activated = false;
	}

	// 是否已经激活
	bool CameraControllerPlugin::isActivate()const
	{
		return m_activated;
	}

	// 获得插件描述
	const IPlugin::Description &CameraControllerPlugin::getDescription()const
	{
		return m_description;
	}

	// 查询一些数据，然后返回到result中
	bool CameraControllerPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// 视口更新
	void CameraControllerPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{

	}

	// 视口更新
	void CameraControllerPlugin::postViewportUpdate(const RenderTargetViewportEvent& evt)
	{

	}

	// 鼠标移动
	bool CameraControllerPlugin::onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		// 如果按ALT，就开始摄像机操作
		if(GetAsyncKeyState(VK_MENU))
		{
			// 按下中间键
			switch(wParam)
			{
			case MK_MBUTTON:
				{
					moveCameraRel(view , moveDistance.x * 0.1f , -moveDistance.y * 0.1f , 0);
					return false;
				}
				break;
			case MK_RBUTTON:
				{
					moveCameraRel(view , 0 , 0 , -moveDistance.y * 0.1f + moveDistance.x * 0.1f);
					return false;
				}
				break;
			case MK_LBUTTON:
				{
					Vector3 center;
					//if(m_worldEditor->getMapManager()->getSelectedObjectsAvgCenter(center))
					//{
					//	rotateCameraAroundPosition(view , center , Degree(moveDistance.x * 0.5) , Degree(moveDistance.y * 0.5));
					//}
					//else
					{
						rotateCameraAroundViewportCenter(view , Degree(moveDistance.x * 0.5) , Degree(moveDistance.y * 0.5));
					}
					return false;
				}
				break;
			};
		}
		return true;
	}

	// 移动摄像机
	void CameraControllerPlugin::moveCameraRel(IChildView *view , float x , float y , float z)
	{
		Camera *camera = view->getCamera();
		// 正交投影特殊处理
		if(camera->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			camera->moveRelative(Vector3(x , y , 0));
			if(camera->getOrthoWindowWidth() + z > 0
				&& camera->getOrthoWindowHeight() + z > 0)
			{
				camera->setOrthoWindow(camera->getOrthoWindowWidth() + z
					, camera->getOrthoWindowHeight() + z
					);
			}
		}
		else
		{
			Vector3 rel(x , y , z);
			rel = view->getCamera()->getDerivedOrientation() * rel;
			view->getMoveCameraSceneNode()->translate(rel);
		}
	}
	// 旋转摄像机，沿着某点旋转
	void CameraControllerPlugin::rotateCameraAroundPosition(IChildView *view , const Vector3 &aroundPosition , const Radian &x , const Radian &y)
	{
		// 正交投影不允许旋转
		if(view->getCamera()->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			return;
		}
		SceneNode *rotateNode = view->getRotateCameraSceneNode();
		SceneNode *moveNode = view->getMoveCameraSceneNode();

		Vector3 posCamera = (rotateNode->getWorldPosition() - aroundPosition);

		moveNode->setPosition(aroundPosition);
		rotateNode->setPosition(posCamera);

		rotateCameraAroundViewportCenter(view , x , y);


	}

	// 旋转摄像机，沿视口中心旋转
	void CameraControllerPlugin::rotateCameraAroundViewportCenter(IChildView *view , const Radian &x , const Radian &y)
	{
		Camera *camera = view->getCamera();
		// 正交投影不允许旋转
		if(camera->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			return;
		}
		SceneNode *rotateNode = view->getRotateCameraSceneNode();
		camera->yaw(x);
		camera->pitch(y);
	}

};