// View1.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2007 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChildView.h"
#include "PluginManager.h"
#include "IMainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

IMPLEMENT_DYNCREATE(CChildView, CView)

CChildView::CChildView()
: m_window(0)
, m_camera(0)
, m_viewport(0)
, m_lastPosition(-1 , -1)
, m_moveCameraNode(0)
, m_rotateCameraNode(0)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CView)
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView drawing

void CChildView::OnDraw(CDC* pDC)
{
	if(m_window == 0 && m_camera)
	{
		init();
	}
}
// 初始化
void CChildView::init()
{
	if(m_window == 0 && m_camera)
	{
		NameValuePairList misc;
		misc["externalWindowHandle"] = StringConverter::toString((long)GetSafeHwnd());
		CRect rect;
		GetClientRect(rect);
		// 创建渲染窗口
		m_window = Root::getSingleton().getRenderSystem()->createRenderWindow("Window_" + m_camera->getName()
			, rect.Width()
			, rect.Height()
			, false
			, &misc);
		// 创建视口
		m_viewport = m_window->addViewport(m_camera);
		// 设置成灰色
		m_viewport->setBackgroundColour(ColourValue(0.5,0.5,0.5));

		// 通知插件管理器
		PluginManager::getSingleton()._notifyRenderTargetAdded(m_window);

		m_window->addListener(this);

		// 执行所有侦听器
		ExecuteListener1(IChildView::Listener , onInit , this);
	}
}
// 销毁
void CChildView::destroy()
{
	if(m_window)
	{
		// 执行所有侦听器
		ExecuteListener1(IChildView::Listener , onDestroy , this);

		m_window->removeListener(this);
		// 通知插件管理器
		PluginManager::getSingleton()._notifyRenderTargetRemove(m_window);
		Root::getSingleton().getRenderSystem()->destroyRenderWindow(m_window->getName());
		m_window = 0;
	}
	if(m_moveCameraNode)
	{
		m_moveCameraNode->getCreator()->destroySceneNode(m_moveCameraNode->getName());
		m_moveCameraNode = 0;
	}
	if(m_rotateCameraNode)
	{
		m_rotateCameraNode->getCreator()->destroySceneNode(m_rotateCameraNode->getName());
		m_rotateCameraNode = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChildView diagnostics

#ifdef _DEBUG
void CChildView::AssertValid() const
{
	CView::AssertValid();
}

void CChildView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

void CChildView::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to reduce screen flicker.
	CXTMemDC memDC(&dc, rectClient, GetXtremeColor(COLOR_WINDOW));

	OnPrepareDC(&memDC);
	OnDraw(&memDC);
	if(m_window)
	{
		m_window->update();
	}
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CChildView::OnDestroy()
{
	CView::OnDestroy();
	destroy();
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnTimer(nIDEvent);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(cx <= 10 || cy <= 10)
	{
		MoveWindow(0 , 0 , std::max(10 , cx) , std::max(10 , cy));
	}
	else if(m_window)
	{
		m_window->windowMovedOrResized();
		m_camera->setAspectRatio((float)cx / (float)cy);
	}
}

// 获得窗口指针
CWnd* CChildView::getWnd()
{
	return this;
}

// 获得窗口大小
CRect CChildView::getWndRect()
{
	CRect rect;
	GetClientRect(&rect);
	return rect;
}
// 设置这个视口的摄像机
void CChildView::setCamera(Camera *camera)
{
	m_camera = camera;
	if(m_window)
	{
		m_window->getViewport(0)->setCamera(m_camera);
	}
	m_cameraType = CT_USER;

	if(!m_moveCameraNode)
	{
		m_moveCameraNode = m_camera->_getManager()->getRootSceneNode()->createChildSceneNode();
		m_moveCameraNode->setFixedYawAxis(true);
	}
	if(!m_rotateCameraNode)
	{
		m_rotateCameraNode = m_moveCameraNode->createChildSceneNode();
		m_rotateCameraNode->setFixedYawAxis(true);
	}
	if(m_camera->getParentSceneNode())
	{
		m_camera->getParentSceneNode()->detachObject(m_camera);
	}
	m_rotateCameraNode->attachObject(m_camera);
}

// 获得这个视口的摄像机
Camera *CChildView::getCamera()const
{
	return m_camera;
}

// 获得渲染窗口
RenderWindow *CChildView::getRenderWindow()const
{
	return m_window;
}

// 获得视口
Viewport *CChildView::getViewport()const
{
	if(m_window)
	{
		return m_window->getViewport(0);
	}
	else
	{
		return 0;
	}
}

// 设置摄像机类型
void CChildView::setCameraType(CAMERA_TYPE type)
{
	m_cameraType = type;
}

// 获得摄像机类型
CAMERA_TYPE CChildView::getCameraType()const
{
	return m_cameraType;
}

BOOL CChildView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// 捕获所有Ogre错误
	try
	{
		if(message == WM_MOUSEMOVE)
		{
			CPoint point(GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam));
			// 如果未初始化，就初始化
			if(m_lastPosition.x == -1 && m_lastPosition.y == -1)
			{
				m_lastPosition = point;
			}
			// 相对上次移动了多少
			m_moveDistance = m_lastPosition - point;
			// 保存最后位置
			m_lastPosition = point;
		}
		// 执行所有侦听器
		for(size_t i = 0 ; i < m_listListener.size() ; i ++)
		{
			if(message == WM_MOUSEMOVE)
			{
				if(!m_listListener[i]->onMouseMove(this , m_moveDistance , message , wParam , lParam , pResult))
					break;
			}
			if(!m_listListener[i]->onWndMsg(this , message , wParam , lParam , pResult))
				break;
		}
		switch(message)
		{
		case WM_SHOWWINDOW:
			{
				if(m_window)
				{
					m_window->setActive(wParam != FALSE);
				}
			}
			break;

		}
		return __super::OnWndMsg(message, wParam, lParam, pResult);
	}
	catch( Exception& e )
	{
		MessageBox(e.getFullDescription().c_str(), "抛出了一个异常", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		return FALSE;
	}
}

BOOL CChildView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 执行所有侦听器
	for(size_t i = 0 ; i < m_listListener.size() ; i ++)
	{
		if(m_listListener[i]->onCmdMsg(this , nID , nCode , pExtra , pHandlerInfo))
			return TRUE;
	}

	return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CChildView::preViewportUpdate(const RenderTargetViewportEvent& evt)
{
	// 执行所有侦听器
	ExecuteListener2(IChildView::Listener , preViewportUpdate , this , evt);

}
void CChildView::postViewportUpdate(const RenderTargetViewportEvent& evt)
{
	// 执行所有侦听器
	ExecuteListener2(IChildView::Listener , postViewportUpdate , this , evt);

}