// ============================================================
// 文件名	:	IChildView.h
// 创建人	:	王宏张
// 说明		:	子视口管理器
// 创建时间	:	2008-1-23 8:31:03
// ============================================================

#pragma once
#include "IMainFrame.h"
namespace Ogre
{
	// 摄像机类型
	enum CAMERA_TYPE
	{
		CT_TOP,			// 顶视图摄像机
		CT_BOTTOM,		// 底视图摄像机
		CT_LEFT,		// 左视图摄像机
		CT_RIGHT,		// 右视图摄像机
		CT_FRONT,		// 前视图摄像机
		CT_BACK,		// 后视图摄像机
		CT_PERSPECTIVE,	// 标准透视摄像机
		CT_USER,		// 用户自定义摄像机
		CT_MAX,			// 最多的摄像机类型
	};
	// 子视口
	class IChildView
	{
	public:

		IChildView(void)
		{
		}

		virtual ~IChildView(void)
		{
		}

		// 子视口的事件侦听器
		struct Listener
		{
			// 初始化
			virtual void onInit(IChildView *view){}
			// 销毁
			virtual void onDestroy(IChildView *view){}
			// 窗口消息，返回true则表示处理这个命令的消息，相应按钮就会被启用，并且不执行后面的侦听器。false则禁用相应按钮
			virtual bool onCmdMsg(IChildView *view , UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo){return false;};
			// 子视口消息，返回false则不执行后面的侦听器
			virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){return true;};
			// 鼠标移动，返回false则不执行后面的侦听器
			virtual bool onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){return true;};

			// 准备更新窗口，返回false则不执行后面的侦听器
			virtual bool preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt){return true;};
			// 更新完毕窗口，返回false则不执行后面的侦听器
			virtual bool postViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt){return true;};
		};

		// 添加子视口侦听器
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// 删除子视口侦听器
		virtual void removeListener(Listener *listener) = 0;


		// 获得窗口指针
		virtual CWnd* getWnd() = 0;
		// 获得窗口大小
		virtual CRect getWndRect() = 0;

		// 设置这个视口的摄像机
		virtual void setCamera(Camera *camera) = 0;
		// 获得这个视口的摄像机
		virtual Camera *getCamera()const = 0;

		// 获得摄像机挂接的节点
		virtual SceneNode *getMoveCameraSceneNode()const = 0;
		// 获得摄像机挂接的节点
		virtual SceneNode *getRotateCameraSceneNode()const = 0;
		// 获得渲染窗口
		virtual RenderWindow *getRenderWindow()const = 0;
		// 获得视口
		virtual Viewport *getViewport()const = 0;

		// 获得摄像机类型
		virtual CAMERA_TYPE getCameraType()const = 0;

		// 获得最近一次WM_MOUSEMOVE时鼠标移动了多少距离
		virtual const CPoint &getMouseMoveDistance() = 0;


	};
};