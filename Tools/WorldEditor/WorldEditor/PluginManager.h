// ============================================================
// 文件名	:	PluginManager.h
// 创建人	:	王宏张
// 说明		:	插件管理器实现
// 创建时间	:	2008-1-22 7:43:06
// ============================================================
#pragma once
#include "IPluginManager.h"

namespace Ogre
{
	// 插件管理器实现
	class PluginManager : public IPluginManager
		, public Singleton<PluginManager>
	{
	public:
		PluginManager(void);
		virtual ~PluginManager(void);
		// 单间模式接口
		static PluginManager& getSingleton(void);
		static PluginManager* getSingletonPtr(void);


		// 显示插件管理器对话框
		virtual void showPluginManagerDlg();

		// 根据目录名载入所有插件，返回载入了的插件数量
		virtual size_t loadPlugins(const String &pathName);

		// 根据文件名载入一个插件
		virtual IPlugin *loadPlugin(const String &fileName);

		// 卸载一个插件
		virtual void unloadPlugin(IPlugin *plugin);
		// 根据名称卸载一个插件
		virtual void unloadPlugin(const String &name);

		// 卸载所有插件
		virtual void unloadAllPlugins();


		// 根据名称获得一个插件
		virtual IPlugin *getPlugin(const String &name);

		// 获得插件列表
		virtual PluginIterator getPluginIterator();

		// 通知有个渲染目标添加了
		virtual void _notifyRenderTargetAdded(RenderTarget *target);
		// 通知有个渲染目标删除了
		virtual void _notifyRenderTargetRemove(RenderTarget *target);
	protected:
		// 初始化所有插件
		void initAllPlugins();

		MapPlugin m_mapPlugin;

		typedef std::map<String , DynLib*> MapDynLib;
		MapDynLib m_mapDynLib;

		typedef std::list<RenderTarget*> ListRenderTarget;
		ListRenderTarget m_listRenderTarget;
	};
};