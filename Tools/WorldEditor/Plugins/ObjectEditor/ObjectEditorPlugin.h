// ============================================================
// 文件名	:	ObjectEditorPlugin.h
// 创建人	:	王宏张
// 说明		:	物件编辑插件实现
// 创建时间	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "DlgPlaceSceneObject.h"
#include "DlgEditTools.h"
#include "DlgTrackTools.h"
#include "DlgSceneObjectProperty.h"
#include "DlgObjectList.h"
namespace Ogre
{
	class ObjectAction;
	class OBJECTEDITOR_API ObjectEditorPlugin : public IPlugin 
		, public IMainFrame::Listener
		, public IChildView::Listener
		, public IMapManager::Listener
		, public IScene::Listener
		, public Singleton<ObjectEditorPlugin>
	{
	public:
		ObjectEditorPlugin(HINSTANCE hInstance);
		virtual ~ObjectEditorPlugin(void);
		// 单间模式接口
		static ObjectEditorPlugin& getSingleton(void);
		static ObjectEditorPlugin* getSingletonPtr(void);


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

		// 获得插件描述
		virtual const Description &getDescription()const;

		// 查询一些数据，然后返回到result中
		virtual bool query(const String &name , const NameValuePairList *params , void *result);
		// 主窗口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// 浮动窗口通知
		LRESULT onDockingPaneNotify(WPARAM wParam, LPARAM lParam);


		// 获得世界编辑器接口
		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}

		// 是否支持此物件的动态编辑
		bool isEditable(ISceneObject *obj)const;
		// 从一个MO获得一个可编辑的场景物件
		ISceneObject* getEditableSceneObject(MovableObject *obj)const;

		// 设置当前的物件操作器
		void setObjectAction(ObjectAction *objectAction);

		// 射线碰撞检测，仅返回第一个发生碰撞的信息（如果没有地形，就碰撞0平面）
		// @ray 射线
		// @collideWorld 是否跟世界碰撞（如：地形）
		// @collideObject 是否跟物件碰撞
		// @intersection 输出交点
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection);


		// 新建场景
		virtual void onNewScene(IScene *scene);
		// 载入一个场景
		virtual void onPreLoadScene(const String &fileName ,IScene *scene);
		// 销毁场景
		virtual void onDestroyScene(IScene *scene);

		// 当创建一个场景对象后调用
		virtual void onCreateSceneObject(ISceneObject *obj);

		// 选中一个场景对象
		virtual void onSelectObject(ISceneObject *obj);
		// 反选一个场景对象
		virtual void onDeselectObject(ISceneObject *obj);

		// 获取轨迹编辑器
		DlgTrackTools *getDlgTrackTools()
		{
			return &m_DlgTrackTools;
		}
	protected:
		// 支持编辑的SceneObject类型
		std::list<String> m_editableSceneObjectType;

		bool m_inited;
		bool m_activated;

		// 插件描述
		Description m_description;

		// 世界编辑器接口
		IWorldEditor *m_worldEditor;

		DlgPlaceSceneObject m_PlaceSceneObject;
		DlgEditTools m_DlgEditTools;
		DlgTrackTools m_DlgTrackTools;
		DlgSceneObjectProperty m_DlgObjectProperty;
		DlgObjectList m_DlgObjectList;

		// 当前的物件操作器
		ObjectAction *m_objectAction;
	};

};