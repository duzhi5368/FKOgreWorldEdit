// ============================================================
// 文件名	:	AxisGridPlugin.h
// 创建人	:	王宏张
// 说明		:	网格插件实现
// 创建时间	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "AxisRTT.h"
#include "OgreTextAreaOverlayElement.h"
namespace Ogre
{
	class AxisGridPlugin : public IPlugin 
		, public IChildView::Listener
		, public Singleton<AxisGridPlugin>
	{
	public:
		AxisGridPlugin(HINSTANCE hInstance);
		virtual ~AxisGridPlugin(void);
		// 单间模式接口
		static AxisGridPlugin& getSingleton(void);
		static AxisGridPlugin* getSingletonPtr(void);


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
		// 获得世界编辑器接口
		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}

	protected:

		// 创建一个网格对象
		ManualObject* createGrid(float width , float height , float interval);
		// 创建坐标轴标识
		ManualObject* createAxis();
		// 创建坐标轴界面
		void createAxisOverlay();
		// 创建统计界面
		void createStatOverlay();

		bool m_inited;
		bool m_activated;
		
		// 网格对象
		ManualObject *m_gridObject;
		SceneNode *m_gridNode;
		// 坐标轴对象
		Overlay* m_axisOverlay;
		OverlayElement* m_axisElement;
		AxisRTT * m_axisRTT;
		// 统计栏
		TextAreaOverlayElement *m_statElement;

		// 插件描述
		Description m_description;


		// 世界编辑器接口
		IWorldEditor *m_worldEditor;
	};

};