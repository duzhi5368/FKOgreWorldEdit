// ============================================================
// 文件名	:	CameraControllerPlugin.h
// 创建人	:	王宏张
// 说明		:	摄像机控制器实现
// 创建时间	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "OgreTextAreaOverlayElement.h"
namespace Ogre
{
	class CameraControllerPlugin : public IPlugin 
		, public IChildView::Listener
		, public Singleton<CameraControllerPlugin>
	{
	public:
		CameraControllerPlugin(HINSTANCE hInstance);
		virtual ~CameraControllerPlugin(void);
		// 单间模式接口
		static CameraControllerPlugin& getSingleton(void);
		static CameraControllerPlugin* getSingletonPtr(void);


		// 初始化插件，传进来的worldEditor是世界编辑器的接口
		virtual bool init(IWorldEditor *worldEditor);
		// 关闭插件
		virtual void shutdown();
		// 是否已经初始化
		virtual bool isInited()const;

		// 激活插件
		virtual void activate();
		// 反激活插件
		virtual void deactivate();
		// 是否已经激活
		virtual bool isActivate()const;

		// 视口更新
		virtual void preViewportUpdate(const RenderTargetViewportEvent& evt);
		virtual void postViewportUpdate(const RenderTargetViewportEvent& evt);

		// 获得插件描述
		virtual const Description &getDescription()const;

		// 查询一些数据，然后返回到result中
		virtual bool query(const String &name , const NameValuePairList *params , void *result);
		// 主窗口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// 鼠标移动
		virtual bool onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

		// 获得世界编辑器接口
		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}
		// 移动摄像机
		void moveCameraRel(IChildView *view , float x , float y , float z);
		// 旋转摄像机，沿着某点旋转
		void rotateCameraAroundPosition(IChildView *view , const Vector3 &aroundPosition , const Radian &x , const Radian &y);
		// 旋转摄像机，沿视口中心旋转
		void rotateCameraAroundViewportCenter(IChildView *view , const Radian &x , const Radian &y);
	protected:

		bool m_inited;
		bool m_activated;
		
		// 插件描述
		Description m_description;

		// 世界编辑器接口
		IWorldEditor *m_worldEditor;
	};

};