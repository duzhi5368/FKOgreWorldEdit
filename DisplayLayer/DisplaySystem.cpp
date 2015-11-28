#include "StdAfx.h"
#include "DisplaySystem.h"
#include "ISceneSystem.h"
#include "IScene.h"
#include "CameraObject.h"
#include "IDecalSystem.h"
#include "CollisionSystem.h"

namespace Ogre
{

	// 单件模式实现
	template<> DisplaySystem* Singleton<DisplaySystem>::ms_Singleton = 0;
	DisplaySystem* DisplaySystem::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	DisplaySystem& DisplaySystem::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	DisplaySystem::DisplaySystem(void)
		: m_root(0)
		, m_inited(false)
		, m_mainCamera(0)
		, m_mainSceneMgr(0)
		, m_mainWindow(0)
		, m_mainViewport(0)
		, m_mainScene(0)
		, m_collisionWorld(0)
		, m_triggerWorld(0)
	{
		// Provide a nice cross platform solution for locating the configuration files
		// On windows files are searched for in the current working directory, on OS X however
		// you must provide the full path, the helper function macBundlePath does this for us.
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		m_resourcePath = macBundlePath() + "/Contents/Resources/";
#else
		m_resourcePath = "";
#endif

	}

	DisplaySystem::~DisplaySystem(void)
	{
	}


	// 初始化显示层系统
	bool DisplaySystem::init(NameValuePairList *params)
	{
		if(m_inited)
			return true;

		if(params)
		{
			m_params = *params;
		}

		// 初始化默认参数
		if(m_params["TITLE"].empty())
		{
			m_params["TITLE"] = "没设置标题";
		}

		String pluginsPath;
		// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
		pluginsPath = m_resourcePath + "plugins.cfg";
#endif

		// 设置Unicode转换的代码页
		setlocale(LC_ALL , "");

		// 创建Ogre系统
		m_root = new Root(pluginsPath, 
			m_resourcePath + "ogre.cfg", m_resourcePath + "Ogre.log");

		// 设置资源路径
		setupResourcesLocation();

		// 配置窗口
		if (!createRenderWindow())
		{
			return false;
		}

		// 选择创景管理器
		createSceneManager();
		// 创建默认相机
		createCamera();
		// 创建默认视口
		createViewports();

		// Set default mipmap level (NB some APIs ignore this)
		TextureManager::getSingleton().setDefaultNumMipmaps(5);

		// Create any resource listeners (for loading screens)
		createResourceListener();
		// 载入所有资源
		loadResources();

		// 初始化贴花系统
		IDecalSystem::init(m_mainSceneMgr);

		m_inited = true;

		return true;
	}
	// 销毁显示层系统
	void DisplaySystem::shutdown()
	{
		if(!m_inited)
			return;
		// 关闭贴花系统
		IDecalSystem::shutdown();

		ISceneSystem::getSingleton().destroyScene(m_mainScene);
		// 关闭场景系统
		ISceneSystem::shutdown();

		// 销毁碰撞世界
		ICollisionSystem::getSingleton().destroyWorld(m_collisionWorld);
		m_collisionWorld = 0;
		// 关闭碰撞系统
		ICollisionSystem::shutdown();

		// 销毁触发器世界
		ITriggerSystem::getSingleton().destroyWorld(m_triggerWorld);
		m_triggerWorld = 0;
		// 关闭触发器系统
		ITriggerSystem::shutdown();


		// 销毁Ogre系统
		OGRE_DELETE(m_root);

		m_inited = false;
	}

	// 开始渲染
	void DisplaySystem::startRendering()
	{
		m_root->startRendering();
	}
	// 渲染一帧
	void DisplaySystem::renderOneFrame()
	{
		m_root->renderOneFrame();
	}

	/** Configures the application - returns false if the user chooses to abandon configuration. */
	bool DisplaySystem::createRenderWindow(void)
	{
		// 如果不存在配置，就让用户选择配置
		if(!m_root->restoreConfig())
		{
			if(!m_root->showConfigDialog())
				return false;
		}


		if(!m_params["WINDOW"].empty())
		{
			// 初始化Ogre系统
			m_root->initialise(false);
			NameValuePairList misc;
			misc["externalWindowHandle"] = m_params["WINDOW"];

			HWND hWnd = (HWND)StringConverter::parseLong(misc["externalWindowHandle"]);
			RECT rect;
			::GetClientRect(hWnd , &rect);
			// 使用外部窗口创建渲染窗口
			m_mainWindow = m_root->createRenderWindow(m_params["TITLE"]
				, rect.right - rect.left
				, rect.bottom - rect.top
				, false
				, &misc);
		}
		else
		{
			// 初始化Ogre系统
			m_mainWindow = m_root->initialise(true , m_params["TITLE"]);
		}
		if(m_mainWindow == 0)
			return false;
		return true;
	}

	void DisplaySystem::createSceneManager(void)
	{
		// Create the SceneManager, in this case a generic one
		m_mainSceneMgr = m_root->createSceneManager(ST_GENERIC, "MainScene");
		// Set ambient light
		m_mainSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

		// 初始化碰撞系统
		ITriggerSystem::init();
		// 初始化触发器系统
		ICollisionSystem::init(m_mainSceneMgr);
		// 创建一个默认的碰撞世界
		m_collisionWorld = ICollisionSystem::getSingleton().createWorld("DefaultCollisionWorld");
		// 创建一个默认的触发器世界
		m_triggerWorld = ITriggerSystem::getSingleton().createWorld("DefaultTriggerWorld");
		// 初始化场景系统
		ISceneSystem::init(m_mainSceneMgr);
		ISceneSystem::getSingleton().setOgreSceneManager(m_mainSceneMgr);
		ISceneSystem::getSingleton().setCollisionWorld(m_collisionWorld);
		ISceneSystem::getSingleton().setTriggerWorld(m_triggerWorld);


		m_mainScene = ISceneSystem::getSingleton().createScene("MainScene");
	}

	void DisplaySystem::createCamera(void)
	{
		// Create the camera
		m_mainCamera = static_cast<CameraObject*>(m_mainScene->getRootObject()->createChildObject("CameraObject"));

		// Position it at 500 in Z direction
		m_mainCamera->setPosition(Vector3(0,100,-1));
		// Look back along -Z
		m_mainCamera->lookAt(Vector3(0,0,0));
		m_mainCamera->getOgreCamera()->setNearClipDistance(0.1);
		m_mainCamera->getOgreCamera()->setFarClipDistance(1000);

	}

	void DisplaySystem::createViewports(void)
	{
		// Create one viewport, entire window
		m_mainViewport = m_mainWindow->addViewport(m_mainCamera->getOgreCamera());
		m_mainViewport->setBackgroundColour(ColourValue(0,0,1));

		// Alter the camera aspect ratio to match the viewport
		m_mainCamera->getOgreCamera()->setAspectRatio(
			Real(m_mainViewport->getActualWidth()) / Real(m_mainViewport->getActualHeight()));
	}

	/// Method which will define the source of resources (other than current folder)
	void DisplaySystem::setupResourcesLocation(void)
	{
		// Load resource paths from config file
		ConfigFile cf;
		cf.load(m_resourcePath + "resources.cfg");

		// Go through all sections & settings in the file
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				String path =  i->second;
				StringVector sv = StringUtil::split(path , "," , 1);
				bool recursive = false;
				archName = sv[0];
				// 分析是否用循环子目录的文件系统
				if(sv.size() > 1)
				{
					recursive = (sv[1] == "recursive");
				}
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				// OS X does not set the working directory relative to the app,
				// In order to make things portable on OS X we need to provide
				// the loading with it's own bundle path location
				ResourceGroupManager::getSingleton().addResourceLocation(
					String(macBundlePath() + "/" + archName), typeName, secName , recursive);
#else
				ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName , recursive);
#endif
			}
		}
	}

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	void DisplaySystem::createResourceListener(void)
	{

	}

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	void DisplaySystem::loadResources(void)
	{
		// Initialise, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	}

	// 获得一个颜色材质
	const String &DisplaySystem::getColorMaterial(const ColourValue &color)
	{
		String name("AutoColor_" + StringConverter::toString(color));
		if(MaterialManager::getSingleton().resourceExists(name))
		{
			return MaterialManager::getSingleton().getByName(name)->getName();
		}
		MaterialPtr mat = MaterialManager::getSingleton().create(name , ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mat->getTechnique(0)->getPass(0)->setDiffuse(ColourValue::Black);
		mat->getTechnique(0)->getPass(0)->setSelfIllumination(color);
		mat->getTechnique(0)->getPass(0)->setAmbient(ColourValue::Black);
		return mat->getName();
	}
	// 主摄像机
	Camera* DisplaySystem::getMainCamera()const
	{
		return m_mainCamera->getOgreCamera();
	}

	// 获得碰撞系统
	ICollisionSystem *DisplaySystem::getCollisionSystem()const
	{
		return ICollisionSystem::getSingletonPtr();
	}
};