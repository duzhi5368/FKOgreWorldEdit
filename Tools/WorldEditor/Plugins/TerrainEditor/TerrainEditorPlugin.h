// ============================================================
// 文件名	:	TerrainEditorPlugin.h
// 创建人	:	王宏张
// 说明		:	网格插件实现
// 创建时间	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "DlgTerrainBrush.h"
#include "DlgTerrainHeight.h"
#include "DlgTerrainPaint.h"
#include "TerrainEditorDll.h"

namespace Ogre
{
	class ETTerrain;
	class Selection;
	class SelectionRenderer;
	class TerrainAction;
	class TerrainBrush;

	class TERRAINEDITOR_API TerrainEditorPlugin : public IPlugin 
		, public IChildView::Listener
		, public IMainFrame::Listener
		, public IMapManager::Listener
		, public Singleton<TerrainEditorPlugin>
	{
	public:
		TerrainEditorPlugin(HINSTANCE hInstance);
		virtual ~TerrainEditorPlugin(void);
		// 单间模式接口
		static TerrainEditorPlugin& getSingleton(void);
		static TerrainEditorPlugin* getSingletonPtr(void);

		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);


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
		// 浮动窗口通知
		virtual LRESULT onDockingPaneNotify(WPARAM wParam, LPARAM lParam);

		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}

		// 激活以便编辑这个地形
		void activateTerrain(ETTerrain *terrain);
		// 反激活地形
		void deactivateTerrain();

		// 实用这个地形操作器
		void setTerrainAction(TerrainAction *action);

		TerrainAction* getTerrainAction()
		{
			return m_terrainAction;
		}

		Selection* getSelection()
		{
			return m_selection;
		}

		SelectionRenderer* getSelectionRenderer()
		{
			return m_selectionRenderer;
		}

		TerrainBrush* getTerrainBrush()
		{
			return m_brush;
		}

		ETTerrain* getActivatedTerrain()
		{
			return m_activatedTerrain;
		}

		// 销毁场景
		virtual void onDestroyScene(IScene *scene);
		// 保存场景
		virtual void onPreSaveScene(const String &fileName ,IScene *scene);
		// 载入一个场景
		virtual void onPostLoadScene(const String &fileName ,IScene *scene);
		// 新建场景
		virtual void onNewScene(IScene *scene);

		// 为当前激活的地形添加一个障碍区域的显示Pass
		void addTerrainBlockerPass();
		// 把当前激活的地形的障碍区域显示Pass移除
		void removeTerrainBlockerPass();

	protected:

		// 为地形添加一个障碍区域的显示Pass
		void addTerrainBlockerPass(ETTerrain *terrain);
		// 把地形的障碍区域显示Pass移除
		void removeTerrainBlockerPass(ETTerrain *terrain);


		bool m_inited;
		bool m_activated;

		// 插件描述
		Description m_description;

		// 世界编辑器接口
		IWorldEditor *m_worldEditor;

		DlgTerrainBrush m_DlgTerrainBrush;
		DlgTerrainHeight m_DlgTerrainHeight;
		DlgTerrainPaint m_DlgTerrainPaint;

		// 选择了的数据保存到这里
		Selection *m_selection;
		// 选区渲染器
		SelectionRenderer *m_selectionRenderer;
		// 对选区进行的动作
		TerrainAction *m_terrainAction;

		// 当前选择的笔刷
		TerrainBrush *m_brush;
		// 地形系统
		ETTerrain* m_activatedTerrain;

	};

};