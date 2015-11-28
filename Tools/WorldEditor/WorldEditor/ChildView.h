// View1.h : header file
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

#if !defined(AFX_VIEW1_H__DB5DD4BB_239B_4D1D_8478_3CA079B61C83__INCLUDED_)
#define AFX_VIEW1_H__DB5DD4BB_239B_4D1D_8478_3CA079B61C83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView view
#include "WorldEditor.h"
#include "IChildView.h"
#include "TListener.h"
using namespace Ogre;
class CChildView : public CView
	, public IChildView
	, public RenderTargetListener
	, public TListener<IChildView::Listener>
{
protected:
	CChildView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView)

	// Attributes
public:

	// Operations
public:


	// 添加子视口侦听器
	virtual void addListener(Listener *listener , bool toBack = true)
	{
		TListener<IChildView::Listener>::addListener(listener , toBack);
	}
	// 删除子视口侦听器
	virtual void removeListener(Listener *listener)
	{
		TListener<IChildView::Listener>::removeListener(listener);
	}


	// 获得窗口指针
	virtual CWnd* getWnd();
	// 获得窗口大小
	virtual CRect getWndRect();

	// 设置这个视口的摄像机
	virtual void setCamera(Camera *camera);
	// 获得这个视口的摄像机
	virtual Camera *getCamera()const;

	// 获得摄像机挂接的节点
	virtual SceneNode *getMoveCameraSceneNode()const
	{
		return m_moveCameraNode;
	}
	// 获得摄像机挂接的节点
	virtual SceneNode *getRotateCameraSceneNode()const
	{
		return m_rotateCameraNode;
	}


	// 获得渲染窗口
	virtual RenderWindow *getRenderWindow()const;

	// 获得视口
	virtual Viewport *getViewport()const;

	// 设置摄像机类型
	virtual void setCameraType(CAMERA_TYPE type);
	// 获得摄像机类型
	virtual CAMERA_TYPE getCameraType()const;


	// 获得最近一次WM_MOUSEMOVE时鼠标移动了多少距离
	virtual const CPoint &getMouseMoveDistance()
	{
		return m_moveDistance;
	}
	virtual void preViewportUpdate(const RenderTargetViewportEvent& evt);
	virtual void postViewportUpdate(const RenderTargetViewportEvent& evt);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

	// 初始化
	void init();
	// 销毁
	void destroy();

	// Implementation
protected:
	virtual ~CChildView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 渲染窗口
	RenderWindow *m_window;
	// 渲染视口
	Viewport *m_viewport;
	// 摄像机
	Camera *m_camera;
	// 摄像机类型
	CAMERA_TYPE m_cameraType;

	// 摄像机挂接的节点
	SceneNode *m_moveCameraNode;
	// 摄像机挂接的节点
	SceneNode *m_rotateCameraNode;


	// 鼠标最后的位置
	CPoint m_lastPosition;
	// 鼠标移动的距离
	CPoint m_moveDistance;


	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW1_H__DB5DD4BB_239B_4D1D_8478_3CA079B61C83__INCLUDED_)


