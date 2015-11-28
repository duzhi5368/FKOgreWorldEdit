// ============================================================
// 文件名	:	IWorldEditor.h
// 创建人	:	王宏张
// 说明		:	编辑器插件访问世界编辑器的接口
// 创建时间	:	2008-1-20 22:31:47
// ============================================================
#pragma once
#include "IPluginManager.h"
#include "IMainFrame.h"
#include "IChildView.h"
#include "IPlugin.h"
#include "ISettingManager.h"
#include "IMapManager.h"
#include "IClipboardManager.h"
namespace Ogre
{
	// 编辑器插件访问世界编辑器的接口
	class IWorldEditor
	{
	public:

		IWorldEditor(void)
		{
		}

		virtual ~IWorldEditor(void)
		{
		}

		// 获得设置管理器
		virtual ISettingManager *getSettingManager() = 0;
		// 获得插件管理器
		virtual IPluginManager *getPluginManager() = 0;
		// 获得显示层
		virtual DisplaySystem *getDisplaySystem() = 0;
		// 获得主窗口
		virtual IMainFrame *getMainFrame() = 0;
		// 获得地图管理器
		virtual IMapManager *getMapManager() = 0;
		// 获得剪切板管理器
		virtual IClipboardManager *getClipboardManager() = 0;

	};
};