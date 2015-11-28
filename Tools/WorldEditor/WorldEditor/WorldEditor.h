// ============================================================
// 文件名	:	WorldEditor.h
// 创建人	:	王宏张
// 说明		:	编辑器插件访问世界编辑器的接口实现
// 创建时间	:	2008-1-20 23:29:11
// ============================================================
#pragma once
#include "IWorldEditor.h"
namespace Ogre
{
	// 编辑器插件访问世界编辑器的接口实现
	class WorldEditor : public IWorldEditor
		, public Singleton<WorldEditor>
	{
	public:
		WorldEditor(IMainFrame *mainFrm);
		virtual ~WorldEditor(void);
		// 单间模式接口
		static WorldEditor& getSingleton(void);
		static WorldEditor* getSingletonPtr(void);

		// 初始化编辑器系统
		void init(HWND hWnd);

		// 销毁编辑器系统
		void shutdown();

		// 是否已经初始化
		bool isInited()const
		{
			return m_inited;
		}

		// 获得显示层
		virtual DisplaySystem *getDisplaySystem();
		// 获得插件管理器
		virtual IPluginManager *getPluginManager();
		// 获得主窗口
		virtual IMainFrame *getMainFrame();
		// 获得设置管理器
		virtual ISettingManager *getSettingManager();
		// 获得地图管理器
		virtual IMapManager *getMapManager();
		// 获得剪切板管理器
		virtual IClipboardManager *getClipboardManager();

	protected:
		bool m_inited;

		// 插件管理器
		IPluginManager *m_pluginManager;
		// 显示系统
		DisplaySystem *m_displaySystem;
		// 设置管理器
		ISettingManager *m_settingManager;
		// 地图管理器
		IMapManager *m_mapManager;
		// 剪切板管理器
		IClipboardManager *m_clipboardManager;
		// 主窗口
		IMainFrame *m_mainFrm;


	};

};