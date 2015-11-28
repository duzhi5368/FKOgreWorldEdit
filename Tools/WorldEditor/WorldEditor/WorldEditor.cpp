#include "StdAfx.h"
#include "WorldEditor.h"
#include "PluginManager.h"
#include "SettingManager.h"
#include "MapManager.h"
#include "ClipboardManager.h"

namespace Ogre
{
	// 单件模式实现
	template<> WorldEditor* Singleton<WorldEditor>::ms_Singleton = 0;
	WorldEditor* WorldEditor::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	WorldEditor& WorldEditor::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}


	WorldEditor::WorldEditor(IMainFrame *mainFrm)
		: m_pluginManager(0)
		, m_inited(false)
		, m_displaySystem(0)
		, m_mainFrm(mainFrm)
		, m_settingManager(0)
		, m_mapManager(0)
		, m_clipboardManager(0)
	{
	}

	WorldEditor::~WorldEditor(void)
	{
	}

	// 初始化编辑器系统
	void WorldEditor::init(HWND hWnd)
	{
		if(m_inited)
			return;

		// 检查设置管理器
		m_settingManager = new SettingManager;
		// 创建显示系统
		m_displaySystem = new DisplaySystem;
		// 在第一次Paint的时候就创建Ogre系统
		if(!m_displaySystem->isInited())
		{
			Ogre::NameValuePairList params;
			params["WINDOW"] = Ogre::StringConverter::toString((long)hWnd);
			m_displaySystem->init(&params);
			// 设置成灰色背景
			m_displaySystem->getMainViewport()->setBackgroundColour(ColourValue(0.5,0.5,0.5));
			m_displaySystem->getMainCamera()->setPosition(Vector3(0 , 100 , -100));
			m_displaySystem->getMainCamera()->lookAt(Vector3::ZERO);
		}
		// 建立地图管理器
		m_mapManager = new MapManager;
		// 创建一个新场景
		m_mapManager->newScene();
		// 建立剪切板管理器
		m_clipboardManager = new ClipboardManager;
		// 建立插件系统
		m_pluginManager = new PluginManager;
		// 载入此目录下所有插件
		m_pluginManager->loadPlugins(SettingManager::getSingleton().getString("插件路径" , ".\\WorldEditorPlugins" , "插件"));


		// 创建一个平行光
		Light *light = getMapManager()->getScene()->getOgreSceneManager()->createLight("TerrainLight");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(1 , -1 , -1).normalisedCopy());
		light->setDiffuseColour(ColourValue(0.8,0.8,0.8));
		light->setVisible(true);
		light->setCastShadows(true);
		getMapManager()->getScene()->getOgreSceneManager()->setAmbientLight(ColourValue(0.5,0.5,0.5));

		m_inited = true;
	}

	// 销毁编辑器系统
	void WorldEditor::shutdown()
	{
		if(!m_inited)
			return;

		// 销毁平行光
		getMapManager()->getScene()->getOgreSceneManager()->destroyLight("TerrainLight");

		m_pluginManager->_notifyRenderTargetRemove(m_displaySystem->getMainWindow());
		// 卸载所有插件
		m_pluginManager->unloadAllPlugins();
		OGRE_DELETE(m_pluginManager);
		OGRE_DELETE(m_mapManager);
		OGRE_DELETE(m_clipboardManager);

		// 销毁显示层
		m_displaySystem->shutdown();
		OGRE_DELETE(m_displaySystem);

		// 销毁设置管理器
		OGRE_DELETE(m_settingManager);

		m_inited = false;
	}

	// 获得显示层
	DisplaySystem *WorldEditor::getDisplaySystem()
	{
		return DisplaySystem::getSingletonPtr();
	}

	// 获得插件管理器
	IPluginManager *WorldEditor::getPluginManager()
	{
		return m_pluginManager;
	}

	// 获得主窗口
	IMainFrame *WorldEditor::getMainFrame()
	{
		return m_mainFrm;
	}
	// 获得设置管理器
	ISettingManager *WorldEditor::getSettingManager()
	{
		return m_settingManager;
	}
	// 获得地图管理器
	IMapManager *WorldEditor::getMapManager()
	{
		return m_mapManager;
	}
	// 获得剪切板管理器
	IClipboardManager *WorldEditor::getClipboardManager()
	{
		return m_clipboardManager;
	}

};