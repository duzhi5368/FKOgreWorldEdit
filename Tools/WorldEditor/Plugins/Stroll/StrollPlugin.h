// ============================================================
// 文件名	:	StrollPlugin.h
// 创建人	:	王宏张
// 说明		:	漫步场景插件实现
// 创建时间	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
namespace Ogre
{
	class StrollPlugin : public IPlugin 
		, public IChildView::Listener
		, public FrameListener
		, public CharacterCallback
		, public IMapManager::Listener
		, public Singleton<StrollPlugin>
	{
	public:
		StrollPlugin(HINSTANCE hInstance);
		virtual ~StrollPlugin(void);
		// 单间模式接口
		static StrollPlugin& getSingleton(void);
		static StrollPlugin* getSingletonPtr(void);


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


		// 每帧更新人物走路状态
		bool frameStarted( const FrameEvent& evt );

		// 进入了一个触发器
		// @triggerObject 被进入的触发器
		// @player 进入的玩家
		virtual void onEnterTrigger(TriggerObject *triggerObject , CharacterObject *player);

		// 从一个触发器出去
		// @triggerObject 玩家从这个触发器出去
		// @player 出去的玩家
		virtual void onLeaveTrigger(TriggerObject *triggerObject , CharacterObject *player);

		// 销毁场景
		virtual void onDestroyScene(IScene *scene);
	protected:

		bool m_inited;
		bool m_activated;
		
		// 插件描述
		Description m_description;


		// 世界编辑器接口
		IWorldEditor *m_worldEditor;

		// 用来漫游场景的玩家
		CharacterObject *m_character;
		// 玩家后面的摄像机
		CameraObject *m_camera;

		// 自动把玩家拉到地面上
		bool m_autoPlaceFloor;


		enum CharacterState
		{
			CS_TurnLeft = 1 << 1,
			CS_TurnRight = 1 << 2,
			CS_Forward = 1 << 3,
			CS_Backward = 1 << 4,
		};
		// 玩家状态，组合状态
		uint m_state;
	};

};