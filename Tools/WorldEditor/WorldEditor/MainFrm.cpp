// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "resource.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "SettingManager.h"
#include "PluginManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame
#define WM_TIMER_RENDER (WM_USER + 10000)

#define PLUGIN_BUTTON_START (WM_USER + 1000)
#define PLUGIN_BUTTON_END (PLUGIN_BUTTON_START + 100)

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_SETTING, &CMainFrame::OnViewSetting)
	ON_COMMAND(ID_VIEW_PLUGIN_MANAGER, &CMainFrame::OnViewPluginManager)
	ON_COMMAND(ID_MAP_NEW, &CMainFrame::OnMapNew)
	ON_COMMAND(ID_MAP_OPEN, &CMainFrame::OnMapOpen)
	ON_COMMAND(ID_MAP_SAVE, &CMainFrame::OnMapSave)
	ON_COMMAND(ID_MAP_SAVE_AS, &CMainFrame::OnMapSaveAs)
	ON_COMMAND_RANGE(PLUGIN_BUTTON_START , PLUGIN_BUTTON_END , &CMainFrame::OnPluginButton)
	ON_UPDATE_COMMAND_UI_RANGE(PLUGIN_BUTTON_START , PLUGIN_BUTTON_END , &CMainFrame::OnUpdatePluginButton)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_worldEditor(0)
, m_mouseLocked(false)
, m_menuBar(0)
, m_toolBar(0)
{
	m_worldEditor = new Ogre::WorldEditor(this);
}

CMainFrame::~CMainFrame()
{
	OGRE_DELETE(m_worldEditor);
}

// 创建一个摄像机
Camera *CMainFrame::createCamera(const String &name)
{
	// 如果存在，就直接返回此名字的摄像机
	if(m_worldEditor->getDisplaySystem()->getMainSceneMgr()->hasCamera(name))
	{
		return m_worldEditor->getDisplaySystem()->getMainSceneMgr()->getCamera(name);
	}
	Camera *camera = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->createCamera(name);
	camera->setNearClipDistance(0.1);
	camera->setFarClipDistance(1000);
	return camera;
}

// 创建一个摄像机
Camera *CMainFrame::createCamera(CAMERA_TYPE cameraType)
{
	if(cameraType >= CT_MAX)
		return 0;
	String name("CameraType_" + StringConverter::toString(cameraType));
	Camera *camera = createCamera(name);

	Vector3 cameraPos(500,500,500);
	Vector3 cameraDir(0,0,0);

	switch(cameraType)
	{
	case CT_TOP :			// 顶视图摄像机
		cameraDir.y = -1;
		break;
	case CT_BOTTOM :		// 底视图摄像机
		cameraDir.y = 1;
		break;
	case CT_LEFT :			// 左视图摄像机
		cameraDir.x = 1;
		break;
	case CT_RIGHT :			// 右视图摄像机
		cameraDir.x = -1;
		break;
	case CT_FRONT :			// 前视图摄像机
		cameraDir.z = -1;
		break;
	case CT_BACK :			// 后视图摄像机
		cameraDir.z = 1;
		break;
	case CT_PERSPECTIVE :	// 标准透视摄像机
		camera->setPosition(10,20,30);
		camera->lookAt(0,0,0);
		return camera;
		break;
	}
	camera->setPosition(cameraPos * -cameraDir);
	camera->setOrientation(MathEx::DirectionToQuaternion(cameraDir));
	camera->setProjectionType(PT_ORTHOGRAPHIC);
	camera->setOrthoWindow(10,10);

	return camera;
}

// 创建子视图
IChildView* CMainFrame::createChildView(int row , int col , const String &name , Camera *camera)
{
	CChildView *view;
	view = (CChildView *)m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(row , col));
	if(view)
	{
		m_ListChildView.remove(view);
		m_wndSplitter.DeleteView(row , col);
		view = 0;
	}
	// 创建顶视图
	if (!m_wndSplitter.CreateView(row , col , RUNTIME_CLASS(CChildView),
		CSize(0, 0), 0))
	{
		TRACE0("Failed to create CChildView\n");
		return false;
	}
	view = (CChildView*)m_wndSplitter.GetPane(row , col);
	view->SetWindowText(name.c_str());
	m_ListChildView.push_back(view);
	view->setCamera(camera);
	// 添加所有侦听器
	for(ListChildViewListener::iterator iter = m_listChildViewListener.begin() ; iter != m_listChildViewListener.end() ; iter ++)
	{
		view->addListener(iter->first , iter->second);
	}
	return view;
}

// 创建子视图
IChildView* CMainFrame::createChildView(int row , int col , const String &name , CAMERA_TYPE cameraType)
{
	// 创建摄像机
	Camera *camera = createCamera(cameraType);
	if(camera == 0)
		return false;
	// 创建视图
	IChildView *view = createChildView(row , col , name , camera);
	if(view == 0)
	{
		return false;
	}
	static_cast<CChildView*>(view)->setCameraType(cameraType);
	return view;
}

// 获得窗口指针
CWnd* CMainFrame::getWnd()
{
	return this;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{


	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	m_menuBar = pCommandBars->SetMenu(
		_T("菜单栏"), IDR_MAINFRAME);
	if(m_menuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	m_toolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!m_toolBar || !m_toolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	//LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);


	// Load the previous state for docking panes.
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//if (layoutNormal.Load(_T("NormalLayout")))
	//{
	//	m_paneManager.SetLayout(&layoutNormal);
	//}

	// 在第一次Paint的时候就创建Ogre系统
	m_worldEditor->init(GetSafeHwnd());
	// 禁用主视口
	m_worldEditor->getDisplaySystem()->getMainWindow()->setActive(false);

	// 渲染计时器
	SetTimer(WM_TIMER_RENDER , 1 , 0);

	const int xMax = 2;
	const int yMax = 2;
	// 创建2行2列
	if (!m_wndSplitter.CreateStatic(this, xMax, yMax))
	{
		TRACE0("Failed to create splitter window\n");
		return FALSE;
	}

	// 创建顶视图
	createChildView(0 , 0 , "顶视图" , CT_TOP);
	createChildView(0 , 1 , "前视图" , CT_FRONT);
	createChildView(1 , 0 , "左视图" , CT_LEFT);
	createChildView(1 , 1 , "透视" , CT_PERSPECTIVE);

	// 自动分割
	CRect r;
	GetClientRect(&r);
	m_wndSplitter.SetRowInfo( 0, r.Height()/2, 0 );
	m_wndSplitter.SetColumnInfo( 0, r.Width()/2, 0 );

	// 现在只显示透视试图
	m_wndSplitter.HideRow(0);
	m_wndSplitter.HideColumn(0);

	// 执行所有侦听器
	ExecuteListener1(IMainFrame::Listener , onCreate , lpCreateStruct);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
	{
		((CChildView*)*iter)->SetFocus();
	}
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 执行所有侦听器
	for(size_t i = 0 ; i < m_listListener.size() ; i ++)
	{
		if(m_listListener[i]->onCmdMsg(nID , nCode , pExtra , pHandlerInfo))
			return TRUE;
	}

	ListChildView listChildView(m_ListChildView);
	// 回调到子窗口
	for(ListChildView::iterator iter = listChildView.begin() ; iter != listChildView.end() ; iter ++)
	{
		if (((CChildView*)*iter)->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnClose()
{

	// 执行所有侦听器
	ExecuteListener(IMainFrame::Listener , onClose);

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}


void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the keyboard page to the customize dialog.
		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
		dlg.AddPage(&pageKeyboard);
		pageKeyboard.AddCategories(IDR_MAINFRAME);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	// 执行所有侦听器
	ExecuteListener2(IMainFrame::Listener , onDockingPaneNotify , wParam , lParam);

	return FALSE;
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// 捕获所有Ogre错误
	try
	{
		// 执行所有侦听器
		for(size_t i = 0 ; i < m_listListener.size() ; i ++)
		{
			if(!m_listListener[i]->onWndMsg(message , wParam , lParam , pResult))
				return TRUE;
		}


		return CXTPFrameWnd::OnWndMsg(message , wParam , lParam , pResult);
	}
	catch( Exception& e )
	{
		MessageBox(e.getFullDescription().c_str(), "抛出了一个异常", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		return FALSE;
	}
}

void CMainFrame::OnDestroy()
{
	CXTPFrameWnd::OnDestroy();
	// 先销毁所有子窗口
	for(ListChildView::iterator iter = m_ListChildView.begin() ; iter != m_ListChildView.end() ; iter ++)
	{
		((CChildView*)*iter)->DestroyWindow();
	}
	m_ListChildView.clear();
	m_wndSplitter.DestroyWindow();
	// 销毁显示层
	m_worldEditor->shutdown();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == WM_TIMER_RENDER)
	{
		if(m_worldEditor->isInited())
		{
			m_worldEditor->getDisplaySystem()->renderOneFrame();
		}
		return;
	}
	CXTPFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	keepLockMouse();
	__super::OnMouseMove(nFlags, point);
}
// 保证鼠标锁定在锁定时的位置
void CMainFrame::keepLockMouse()
{
	// 如果已经被锁定，就锁定鼠标位置
	if(isMouseLocked())
	{
		SetCursorPos(m_lockMousePos.x , m_lockMousePos.y);
	}
}
// 锁住并隐藏鼠标
void CMainFrame::lockMouse(bool bLock)
{
	if(m_mouseLocked == bLock)
	{
		return;
	}
	m_mouseLocked = bLock;
	if(m_mouseLocked)
	{
		ShowCursor(false);
		GetCursorPos(&m_lockMousePos);
	}
	else
	{
		SetCursorPos(m_lockMousePos.x , m_lockMousePos.y);
		ShowCursor(true);
	}
}

// 鼠标是否已经锁住了
bool CMainFrame::isMouseLocked()
{
	return m_mouseLocked;
};
// 获得子窗口列表
IMainFrame::ChildViewIterator CMainFrame::getChildViewIterator()
{
	return ChildViewIterator(m_ListChildView.begin() , m_ListChildView.end());
}


// 获得命令bars，用来操作工具栏、菜单的接口
CXTPCommandBars* CMainFrame::getCommandBars()
{
	return GetCommandBars();
}

// 获得状态栏接口
CXTPStatusBar* CMainFrame::getStatusBar()
{
	return &m_wndStatusBar;
}

// 获得浮动窗口管理器
CXTPDockingPaneManager* CMainFrame::getDockingPaneManager()
{
	return &m_paneManager;
}


// 函 数 名 : createPane
// 注    释 : 创建一个浮动窗口
// 返回类型 : CXTPDockingPane* 
// 参    数 : UINT nID									窗口资源ID
// 参    数 : CRect rc									区域
// 参    数 : XTPDockingPaneDirection direction			浮动方向
// 参    数 : CXTPDockingPaneBase* pNeighbour = NULL	靠近的窗口
CXTPDockingPane* CMainFrame::createPane(UINT nID, const String &title , CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour)
{

	CXTPDockingPaneBase* pDockPane = pNeighbour;
	//if(pDockPane == 0)
	//{
	//	// 附加到第一个pane
	//	CXTPDockingPaneInfoList& paneList = m_paneManager.GetPaneList();
	//	if(paneList.GetCount())
	//	{
	//		pDockPane = paneList.GetHead();
	//	}
	//}
	// 创建面板
	CXTPDockingPane* pPane = m_paneManager.CreatePane(nID, rc, direction, pDockPane);
	if(pPane)
	{
		pPane->SetTitle(title.c_str());
		//if(pDockPane)
		//{
		//	m_paneManager.DockPane(pPane , xtpPaneDockBottom , pDockPane);
		//}
		//CString strTitle;
		//strTitle.LoadString(nID);
		//// 获得面板菜单
		//CXTPControl *pControl = GetViewPaneControls()->Add(xtpControlButton, ID_VIEW_PANE_FIRST + GetViewPaneControls()->GetCount(), _T(""));
		//// 设置文字
		//pControl->SetCaption(strTitle);
		//pControl->SetIconId(nID);
	}
	return pPane;
}
// 销毁浮动窗口
void CMainFrame::destroyPane(UINT nID)
{
	m_paneManager.DestroyPane(nID);
}

// 函 数 名 : createSubMenu
// 注    释 : 创建一个子菜单
// 返回类型 : CXTPMenuBar* 
// 参    数 : UINT nID				菜单资源ID
// 参    数 : UINT nSubMenu = 0		第几个子菜单
// 参    数 : UINT nBefore = -1		是否在之前插入
CXTPMenuBar* CMainFrame::createSubMenu(UINT nID , UINT nSubMenu , UINT nBefore)
{
	// 新建菜单
	CXTPMenuBar* pMenuBar(static_cast<CXTPMenuBar*>(CXTPMenuBar::CreateObject()));
	pMenuBar->LoadMenuBar(nID);

	// 添加菜单
	CXTPControl *pControl = pMenuBar->GetControl(nSubMenu);
	pControl = m_menuBar->GetControls()->Add(pControl , 0 , "" , nBefore);
	// 启用菜单
	pControl->SetEnabled(TRUE);

	return pMenuBar;
}

// 销毁子菜单
void CMainFrame::destroySubMenu(CXTPMenuBar* pMenuBar , UINT nSubMenu)
{
	CXTPControl *pControl = pMenuBar->GetControl(nSubMenu);
	if(pControl)
	{
		//getMenuBar()->GetControls()->Remove(pControl);
		// add by 王宏张 2007-8-22
		// 如何清除pMenuBar的内存？
	}

}

// 函 数 名 : *createToolBar
// 注    释 : 创建工具栏
// 返回类型 : CXTPToolBar 
// 参    数 : UINT nID									工具栏的资源ID
// 参    数 : const CString &strText					工具栏的标题
// 参    数 : XTPBarPosition barPosition = xtpBarTop	工具栏的位置
CXTPToolBar *CMainFrame::createToolBar(UINT nID , const CString &strText , XTPBarPosition barPosition)
{
	// 创建工具条
	CXTPToolBar* pObjectBar = (CXTPToolBar*)getCommandBars()->Add(strText, xtpBarTop);
	if (!pObjectBar ||
		!pObjectBar->LoadToolBar(nID))
	{
		TRACE0("Failed to create toolbar\n");
		return 0;
	}
	CXTPCommandBars* pCommandBars = GetCommandBars();
	CXTPCommandBar *bar = pCommandBars->GetAt(pCommandBars->GetCount() - 1);
	DockRightOf(pObjectBar , (CXTPToolBar*)bar);
	return pObjectBar;
}
// 销毁工具栏
void CMainFrame::destroyToolBar(UINT nID)
{

}

void CMainFrame::OnViewSetting()
{
	SettingManager::getSingleton().showSettingDlg();
}

void CMainFrame::OnViewPluginManager()
{
	PluginManager::getSingleton().showPluginManagerDlg();
}


// 显示打开文件对话框
String CMainFrame::showOpenDialog(bool openFileDialog , const String &title, const String &filter , const String &defaultExt)
{
	String filePathName;
	CFileDialog dlgFile(openFileDialog , defaultExt.c_str() , NULL , OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT , filter.c_str() , this);

	CString fileName;
	dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(1024 * 1024);
	dlgFile.GetOFN().nMaxFile = 1024 * 1024;
	char szDirectoryp[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH , szDirectoryp);

	INT_PTR nResult = dlgFile.DoModal();
	if(nResult == IDOK)
	{
		//若有选取到文件，则返回文件名称
		filePathName = dlgFile.GetPathName();
	}
	fileName.ReleaseBuffer();
	::SetCurrentDirectory(szDirectoryp);
	return filePathName;
}

// 显示打开图片文件对话框
String CMainFrame::showOpenImageDialog(const String &title)
{
	return showOpenDialog(true , title , "*.png;*.bmp;*.jpg;*.dds;*.tga|*.png;*.bmp;*.jpg;*.dds;*.tga|.*|*.*||" , ".tga");
}

void CMainFrame::OnMapNew()
{
	WorldEditor::getSingleton().getMapManager()->newScene();
	m_sceneFileName.clear();
}

void CMainFrame::OnMapOpen()
{
	String fileName = showOpenDialog(true , "打开场景" , "*.osm;*.sne;*.xml|*.osm;*.sne;*.xml||" , ".sne");
	if(!fileName.empty())
	{
		String outBaseName , outPath;
		StringUtil::splitFilename(fileName , outBaseName , outPath);
		WorldEditor::getSingleton().getMapManager()->loadScene(outBaseName);
		m_sceneFileName = fileName;
	}
}

void CMainFrame::OnMapSave()
{
	if(!m_sceneFileName.empty())
	{
		WorldEditor::getSingleton().getMapManager()->saveScene(m_sceneFileName);
	}
	else
	{
		OnMapSaveAs();
	}
}

void CMainFrame::OnMapSaveAs()
{
	String fileName = showOpenDialog(false , "保存场景" , "*.sne|*.sne|*.xml|*.xml||" , ".sne");
	if(!fileName.empty())
	{
		if(WorldEditor::getSingleton().getMapManager()->saveScene(fileName))
		{
			m_sceneFileName = fileName;
		}
	}
}


// 创建插件工具栏的按钮
int CMainFrame::createPluginToolBarButton(const String &strName , IPlugin *plugin)
{
	CXTPControl *control = m_toolBar->GetControls()->Add(xtpControlCheckBox , m_toolBar->GetControls()->GetCount() + PLUGIN_BUTTON_START);
	control->SetCaption(strName.c_str());
	m_pluginButtonList[control->GetID()] = plugin;
	return control->GetID();
}

// 销毁插件工具栏的按钮
void CMainFrame::destroyPluginToolBarButton(const String &strName)
{
	CXTPControls *controls = m_toolBar->GetControls();
	for(int i = 0 ; i < controls->GetCount() ; i ++)
	{
		if(String(controls->GetAt(i)->GetCaption()) == strName)
		{
			m_pluginButtonList.erase(m_pluginButtonList.find(controls->GetAt(i)->GetID()));
			controls->Remove(i);
			return;
		}
	}
}

void CMainFrame::OnPluginButton(UINT nID)
{
	PluginButtonList::iterator iter = m_pluginButtonList.find(nID);
	if(iter != m_pluginButtonList.end())
	{
		IPlugin *plugin = iter->second;
		for(PluginButtonList::iterator iter = m_pluginButtonList.begin(); iter != m_pluginButtonList.end() ; iter ++)
		{
			// 只激活选中的
			if(iter->second == plugin && !iter->second->isActivate())
			{
				iter->second->activate();
			}
			else
			{
				iter->second->deactivate();
			}
		}
	}
}

void CMainFrame::OnUpdatePluginButton(CCmdUI* pCmdUI)
{
	PluginButtonList::iterator iter = m_pluginButtonList.find(pCmdUI->m_nID);
	if(iter != m_pluginButtonList.end())
	{
		IPlugin *plugin = iter->second;
		pCmdUI->SetCheck(plugin->isActivate());
	}
}
