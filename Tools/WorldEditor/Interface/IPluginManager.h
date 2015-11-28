// ============================================================
// 文件名	:	IPluginManager.h
// 创建人	:	王宏张
// 说明		:	插件管理器的接口
// 创建时间	:	2008-1-20 22:31:47
// ============================================================
#pragma once

namespace Ogre
{
	class IPlugin;
	// 插件管理器的接口
	class IPluginManager
	{
	public:

		IPluginManager(void)
		{
		}

		virtual ~IPluginManager(void)
		{
		}

		// 显示插件管理器对话框
		virtual void showPluginManagerDlg() = 0;

		// 根据目录名（支持通配符，如c:\\plugins\\*.dll）载入所有插件，返回载入了的插件数量
		virtual size_t loadPlugins(const String &pathName) = 0;

		// 根据文件名载入一个插件
		virtual IPlugin *loadPlugin(const String &fileName) = 0;

		// 卸载一个插件
		virtual void unloadPlugin(IPlugin *plugin) = 0;
		// 根据名称卸载一个插件
		virtual void unloadPlugin(const String &name) = 0;

		// 卸载所有插件
		virtual void unloadAllPlugins() = 0;
		
		// 根据名称获得一个插件
		virtual IPlugin *getPlugin(const String &name) = 0;
		
		typedef std::map<String , IPlugin*> MapPlugin;
		typedef MapIterator<MapPlugin> PluginIterator;
		// 获得插件列表
		virtual PluginIterator getPluginIterator() = 0;

		// 通知有个渲染目标添加了
		virtual void _notifyRenderTargetAdded(RenderTarget *target) = 0;
		// 通知有个渲染目标删除了
		virtual void _notifyRenderTargetRemove(RenderTarget *target) = 0;
	};
};