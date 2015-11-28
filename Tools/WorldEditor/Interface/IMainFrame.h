// ============================================================
// 文件名	:	IMainFrame.h
// 创建人	:	王宏张
// 说明		:	主窗口管理器
// 创建时间	:	2008-1-23 8:31:03
// ============================================================

#pragma once
#include "IChildView.h"
namespace Ogre
{

	class IChildView;
	class IPlugin;


	// 主窗口管理器
	class IMainFrame
	{
	public:

		IMainFrame(void)
		{
		}

		virtual ~IMainFrame(void)
		{
		}

		// 主窗口的事件侦听器
		struct Listener
		{
			// 创建窗口
			virtual void onCreate(LPCREATESTRUCT lpCreateStruct){};
			// 主窗口销毁
			virtual void onClose(){};
			// 窗口消息，返回true则表示处理这个命令的消息，相应按钮就会被启用，并且不执行后面的侦听器。false则禁用相应按钮
			virtual bool onCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo){return false;};
			// 主窗口消息，返回false则不执行后面的侦听器
			virtual bool onWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){return true;};
			// 浮动窗口通知
			virtual LRESULT onDockingPaneNotify(WPARAM wParam, LPARAM lParam){return 0;};
		};

		// 添加主窗口侦听器
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// 删除主窗口侦听器
		virtual void removeListener(Listener *listener) = 0;

		// 添加子窗口侦听器
		virtual void addChildViewListener(IChildView::Listener *listener , bool toBack = true) = 0;
		// 删除子窗口侦听器
		virtual void removeChildViewListener(IChildView::Listener *listener) = 0;

		// 获得窗口指针
		virtual CWnd* getWnd() = 0;

		// 创建一个摄像机
		virtual Camera *createCamera(const String &name) = 0;
		// 获得一个摄像机
		virtual Camera *createCamera(CAMERA_TYPE cameraType) = 0;
		// 创建子视图
		virtual IChildView* createChildView(int row , int col , const String &name , Camera *camera) = 0;
		// 创建子视图
		virtual IChildView* createChildView(int row , int col , const String &name , CAMERA_TYPE cameraType) = 0;

		// 锁住并隐藏鼠标
		virtual void lockMouse(bool bLock) = 0;
		// 鼠标是否已经锁住了
		virtual bool isMouseLocked() = 0;
		// 保证鼠标锁定在锁定时的位置
		virtual void keepLockMouse() = 0;

		// 函 数 名 : createPane
		// 注    释 : 创建一个浮动窗口
		// 返回类型 : virtual CXTPDockingPane* 
		// 参    数 : UINT nID									窗口资源ID
		// 参    数 : CRect rc									区域
		// 参    数 : XTPDockingPaneDirection direction			浮动方向
		// 参    数 : CXTPDockingPaneBase* pNeighbour = NULL	靠近的窗口
		virtual CXTPDockingPane* createPane(UINT nID, const String &title , CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour = NULL) = 0;
		// 销毁浮动窗口
		virtual void destroyPane(UINT nID) = 0;

		// 函 数 名 : createSubMenu
		// 注    释 : 创建一个子菜单
		// 返回类型 : virtual CXTPMenuBar* 
		// 参    数 : UINT nID				菜单资源ID
		// 参    数 : UINT nSubMenu = 0		第几个子菜单
		// 参    数 : UINT nBefore = -1		是否在之前插入
		virtual CXTPMenuBar* createSubMenu(UINT nID , UINT nSubMenu = 0 , UINT nBefore = -1) = 0;
		// 销毁子菜单
		virtual void destroySubMenu(CXTPMenuBar* pMenuBar , UINT nSubMenu = 0) = 0;

		// 函 数 名 : *createToolBar
		// 注    释 : 创建工具栏
		// 返回类型 : virtual CXTPToolBar 
		// 参    数 : UINT nID									工具栏的资源ID
		// 参    数 : const CString &strText					工具栏的标题
		// 参    数 : XTPBarPosition barPosition = xtpBarTop	工具栏的位置
		virtual CXTPToolBar *createToolBar(UINT nID , const CString &strText , XTPBarPosition barPosition = xtpBarTop) = 0;
		// 销毁工具栏
		virtual void destroyToolBar(UINT nID) = 0;

		// 创建插件工具栏的按钮
		virtual int createPluginToolBarButton(const String &strName , IPlugin *plugin) = 0;
		// 销毁插件工具栏的按钮
		virtual void destroyPluginToolBarButton(const String &strName) = 0;

		// 获得命令bars，用来操作工具栏、菜单的接口
		virtual CXTPCommandBars* getCommandBars() = 0;
		// 获得状态栏接口
		virtual CXTPStatusBar* getStatusBar() = 0;
		// 获得浮动窗口管理器
		virtual CXTPDockingPaneManager* getDockingPaneManager() = 0;

		typedef std::list<IChildView*> ListChildView;
		typedef VectorIterator<ListChildView> ChildViewIterator;
		// 获得子窗口列表
		virtual ChildViewIterator getChildViewIterator() = 0;

		// 显示打开文件对话框
		virtual String showOpenDialog(bool openFileDialog , const String &title, const String &filter , const String &defaultExt) = 0;
		// 显示打开图片文件对话框
		virtual String showOpenImageDialog(const String &title) = 0;

	};
};