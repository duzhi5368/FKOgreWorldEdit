#include "StdAfx.h"
#include "PluginManager.h"
#include "IPlugin.h"
#include "OgreDynLibManager.h"
#include "OgreDynLib.h"
#include "WorldEditor.h"
#include "DlgPluginManager.h"
#include "SettingManager.h"

namespace Ogre
{
	// 单件模式实现
	template<> PluginManager* Singleton<PluginManager>::ms_Singleton = 0;
	PluginManager* PluginManager::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	PluginManager& PluginManager::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	typedef IPlugin* (*DLL_GET_PLUGIN)();

	PluginManager::PluginManager(void)
	{
	}

	PluginManager::~PluginManager(void)
	{
	}

	// 根据目录名载入所有插件，返回载入了的插件数量
	size_t PluginManager::loadPlugins(const String &pathName)
	{
		String strOldEnv(_tgetenv("Path"));
		// 把插件目录加入到环境变量，以便可以让系统载入DLL时找到插件间依赖的DLL
		if(_tputenv(("Path=" + strOldEnv + ";" + pathName).c_str()) == -1)
			return 0;
		CFileFind finder;
		BOOL bWorking = finder.FindFile((pathName + "\\*.dll").c_str());
		size_t count = 0;
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			// 载入一个插件
			if(loadPlugin((LPCSTR)finder.GetFilePath()) != 0)
			{
				count ++;
			}
		}
		// 初始化所有插件
		initAllPlugins();

		return count;
	}
	// 初始化所有插件
	void PluginManager::initAllPlugins()
	{
		// 自动载入的列表
		StringVector autoLoads = SettingManager::getSingleton().getStringVector("自动载入列表" , StringVector() , "插件");
		typedef std::list<IPlugin*> ListPlugin;
		ListPlugin loads;
		ListPlugin endLoads;
		// 所有载入的插件
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			IPlugin *plugin = iter->second;
			// 只有需要自动载入才load
			if(std::count(autoLoads.begin() , autoLoads.end() , plugin->getDescription().fileName) > 0)
			{
				loads.push_back(plugin);
				endLoads.push_back(plugin);
			}
		}
		// 排序
		for(ListPlugin::iterator iter = loads.begin() ; iter != loads.end() ; iter ++)
		{
			IPlugin *plugin = (*iter);
			const IPlugin::Description &desc = plugin->getDescription();
			// 循环所有依赖列表
			for(size_t i = 0 ; i < desc.dependentPlugins.size() ; i ++)
			{
				ListPlugin::iterator dependentPlugin = loads.end();
				// 找这个依赖项是否已经载入
				for(ListPlugin::iterator iterFind = loads.begin() ; iterFind != loads.end() ; iterFind ++)
				{
					if((*iterFind)->getDescription().name == desc.dependentPlugins[i])
					{
						dependentPlugin = iterFind;
						break;
					}
				}
				// 如果没载入，那么本插件就不用载入了
				if(dependentPlugin == loads.end())
				{
					endLoads.remove(plugin);
					break;
				}
				// 否则，就把本插件插入到这个依赖项的后面
				else
				{
					ListPlugin::iterator iterDependent = std::find(endLoads.begin() , endLoads.end() , *dependentPlugin);
					endLoads.splice(++std::find(endLoads.begin() , endLoads.end() , *dependentPlugin)
						, endLoads
						, std::find(endLoads.begin() , endLoads.end() , plugin)
						);
				}
			}
		}

		// 按顺序载入
		for(ListPlugin::iterator iter = endLoads.begin() ; iter != endLoads.end() ; iter ++)
		{
			(*iter)->init(WorldEditor::getSingletonPtr());
		}
	}

	// 根据文件名载入一个插件
	IPlugin *PluginManager::loadPlugin(const String &fileName)
	{
		// 载入DLL
		DynLib* lib = DynLibManager::getSingleton().load(fileName);
		if(!lib)
		{
			return 0;
		}
		// 获得启动函数
		DLL_GET_PLUGIN pFunc = (DLL_GET_PLUGIN)lib->getSymbol("getPlugin");
		if(!pFunc)
		{
			DynLibManager::getSingleton().unload(lib);
			return 0;
		}

		// 启动之
		IPlugin *plugin = pFunc();
		// 启动失败就返回
		if(!plugin)
		{
			DynLibManager::getSingleton().unload(lib);
			return 0;
		}
		// 保存指针
		m_mapDynLib[plugin->getDescription().name] = lib;
		m_mapPlugin[plugin->getDescription().name] = plugin;
		return plugin;
	}

	// 卸载一个插件
	void PluginManager::unloadPlugin(IPlugin *plugin)
	{
		unloadPlugin(plugin->getDescription().name);
	}
	// 根据名称卸载一个插件
	void PluginManager::unloadPlugin(const String &name)
	{
		// 关闭插件
		MapPlugin::iterator iter = m_mapPlugin.find(name);
		if(iter == m_mapPlugin.end())
			return;
		// 删除侦听器
		for(ListRenderTarget::iterator iterTarget = m_listRenderTarget.begin() ; iterTarget != m_listRenderTarget.end() ; iterTarget ++)
		{
			(*iterTarget)->removeListener(iter->second);
		}
		iter->second->shutdown();
		m_mapPlugin.erase(iter);

		// 卸载DLL
		MapDynLib::iterator iterDynLib = m_mapDynLib.find(name);
		if(iterDynLib == m_mapDynLib.end())
			return;
		DynLibManager::getSingleton().unload(iterDynLib->second);
		m_mapDynLib.erase(iterDynLib);
	}

	// 卸载所有插件
	void PluginManager::unloadAllPlugins()
	{
		// 关闭所有插件
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			iter->second->shutdown();
		}
		m_mapPlugin.clear();
		// 卸载DLL
		for(MapDynLib::iterator iterDynLib = m_mapDynLib.begin() ; iterDynLib != m_mapDynLib.end() ; iterDynLib ++)
		{
			DynLibManager::getSingleton().unload(iterDynLib->second);
		}
		m_mapDynLib.clear();
	}

	// 根据名称获得一个插件
	IPlugin *PluginManager::getPlugin(const String &name)
	{
		MapPlugin::iterator iter = m_mapPlugin.find(name);
		if(iter == m_mapPlugin.end())
			return 0;
		return iter->second;
	}

	// 获得插件列表
	PluginManager::PluginIterator PluginManager::getPluginIterator()
	{
		return PluginIterator(m_mapPlugin.begin() , m_mapPlugin.end());
	}


	// 通知有个渲染目标添加了
	void PluginManager::_notifyRenderTargetAdded(RenderTarget *target)
	{
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			if(iter->second->isInited())
			{
				target->addListener(iter->second);
			}
		}		
		m_listRenderTarget.push_back(target);
	}
	// 通知有个渲染目标删除了
	void PluginManager::_notifyRenderTargetRemove(RenderTarget *target)
	{
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			if(iter->second->isInited())
			{
				target->removeListener(iter->second);
			}
		}
		m_listRenderTarget.remove(target);
	}


	// 显示插件管理器对话框
	void PluginManager::showPluginManagerDlg()
	{
		CDlgPluginManager dlg(WorldEditor::getSingleton().getMainFrame()->getWnd());
		dlg.DoModal();
	}

};