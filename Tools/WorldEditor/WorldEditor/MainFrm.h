// ============================================================
// 文件名	:	MainFrm.h
// 创建人	:	王宏张
// 说明		:	主窗口实现
// 创建时间	:	2008-1-23 8:34:45
// ============================================================
#pragma once
#include "IMainFrame.h"
#include "TListener.h"

namespace Ogre
{
	class WorldEditor;
};
class CChildView;

using namespace Ogre;

class CMainFrame : public CXTPFrameWnd
	, public IMainFrame
	, public TListener<IMainFrame::Listener>
{

public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

	// Attributes
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// Implementation
public:


	// 添加主窗口侦听器
	virtual void addListener(Listener *listener , bool toBack = true)
	{
		TListener<IMainFrame::Listener>::addListener(listener , toBack);
	}
	// 删除主窗口侦听器
	virtual void removeListener(Listener *listener)
	{
		TListener<IMainFrame::Listener>::removeListener(listener);
	}

	// 添加子窗口侦听器
	virtual void addChildViewListener(IChildView::Listener *listener , bool toBack = true)
	{
		ListChildViewListener::iterator iter = m_listChildViewListener.find(listener);
		if(iter != m_listChildViewListener.end())
		{
			m_listChildViewListener.erase(iter);
		}
		m_listChildViewListener[listener] = toBack;
		for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
		{
			(*iter)->addListener(listener , toBack);
		}

	}
	// 删除子窗口侦听器
	virtual void removeChildViewListener(IChildView::Listener *listener)
	{
		ListChildViewListener::iterator iter = m_listChildViewListener.find(listener);
		if(iter != m_listChildViewListener.end())
		{
			m_listChildViewListener.erase(iter);
		}
		for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
		{
			(*iter)->removeListener(listener);
		}

	}

	// 创建插件工具栏的按钮
	virtual int createPluginToolBarButton(const String &strName , IPlugin *plugin);
	// 销毁插件工具栏的按钮
	virtual void destroyPluginToolBarButton(const String &strName);

	// 获得窗口指针
	virtual CWnd* getWnd();
	// 创建一个摄像机
	virtual Camera *createCamera(const String &name);
	// 获得一个摄像机
	virtual Camera *createCamera(CAMERA_TYPE cameraType);
	// 创建子视图
	virtual IChildView* createChildView(int row , int col , const String &name , Camera *camera);
	// 创建子视图
	virtual IChildView* createChildView(int row , int col , const String &name , CAMERA_TYPE cameraType);

	// 锁住并隐藏鼠标
	virtual void lockMouse(bool bLock);
	// 鼠标是否已经锁住了
	virtual bool isMouseLocked();
	// 保证鼠标锁定在锁定时的位置
	virtual void keepLockMouse();
	// 获得子窗口列表
	virtual ChildViewIterator getChildViewIterator();

	// 获得命令bars，用来操作工具栏、菜单的接口
	virtual CXTPCommandBars* getCommandBars();
	// 获得状态栏接口
	virtual CXTPStatusBar* getStatusBar();
	// 获得浮动窗口管理器
	virtual CXTPDockingPaneManager* getDockingPaneManager();

	// 函 数 名 : createPane
	// 注    释 : 创建一个浮动窗口
	// 返回类型 : virtual CXTPDockingPane* 
	// 参    数 : UINT nID									窗口资源ID
	// 参    数 : CRect rc									区域
	// 参    数 : XTPDockingPaneDirection direction			浮动方向
	// 参    数 : CXTPDockingPaneBase* pNeighbour = NULL	靠近的窗口
	virtual CXTPDockingPane* createPane(UINT nID, const String &title , CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour = NULL);
	// 销毁浮动窗口
	virtual void destroyPane(UINT nID);
	// 函 数 名 : createSubMenu
	// 注    释 : 创建一个子菜单
	// 返回类型 : virtual CXTPMenuBar* 
	// 参    数 : UINT nID				菜单资源ID
	// 参    数 : UINT nSubMenu = 0		第几个子菜单
	// 参    数 : UINT nBefore = -1		是否在之前插入
	virtual CXTPMenuBar* createSubMenu(UINT nID , UINT nSubMenu = 0 , UINT nBefore = -1);
	// 销毁子菜单
	virtual void destroySubMenu(CXTPMenuBar* pMenuBar , UINT nSubMenu = 0);

	// 函 数 名 : *createToolBar
	// 注    释 : 创建工具栏
	// 返回类型 : virtual CXTPToolBar 
	// 参    数 : UINT nID									工具栏的资源ID
	// 参    数 : const CString &strText					工具栏的标题
	// 参    数 : XTPBarPosition barPosition = xtpBarTop	工具栏的位置
	virtual CXTPToolBar *createToolBar(UINT nID , const CString &strText , XTPBarPosition barPosition = xtpBarTop);
	// 销毁工具栏
	virtual void destroyToolBar(UINT nID);

	// 显示打开文件对话框
	virtual String showOpenDialog(bool openFileDialog , const String &title, const String &filter , const String &defaultExt);
	// 显示打开图片文件对话框
	virtual String showOpenImageDialog(const String &title);

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPaneManager m_paneManager;
	CXTSplitterWnd  m_wndSplitter;
	CXTPMenuBar *m_menuBar;
	CXTPToolBar *m_toolBar;
	// 视图列表
	ListChildView m_ListChildView;
	// 编辑器系统
	WorldEditor *m_worldEditor;
	// 场景文件名
	String m_sceneFileName;


	// 鼠标是否已经锁住
	bool m_mouseLocked;
	// 锁住的位置
	POINT m_lockMousePos;

	typedef std::map<IChildView::Listener* , bool> ListChildViewListener;
	// 侦听器列表
	ListChildViewListener m_listChildViewListener;

	typedef std::map<UINT , IPlugin*> PluginButtonList;
	// 插件按钮列表
	PluginButtonList m_pluginButtonList;

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewSetting();
	afx_msg void OnViewPluginManager();
	afx_msg void OnMapNew();
	afx_msg void OnMapOpen();
	afx_msg void OnMapSave();
	afx_msg void OnMapSaveAs();
	afx_msg void OnPluginButton(UINT nID);
	afx_msg void OnUpdatePluginButton(CCmdUI* pCmdUI);
};

