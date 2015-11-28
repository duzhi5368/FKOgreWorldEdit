// ============================================================
// 文件名	:	DisplaySystem.h
// 创建人	:	王宏张
// 说明		:	显示层系统的实现
// 创建时间	:	2008-1-20 20:51:58
// ============================================================

#pragma once
namespace Ogre
{
	class IScene;
	class CameraObject;
	class ICollisionSystem;
	class ICollisionWorld;
	// 显示层系统
	class DISPLAYLAYER_API DisplaySystem : public Singleton<DisplaySystem>
	{
	public:
		DisplaySystem(void);
		virtual ~DisplaySystem(void);
		// 单间模式接口
		static DisplaySystem& getSingleton(void);
		static DisplaySystem* getSingletonPtr(void);

		// 初始化显示层系统
		// params是创建时的参数，可用参数如下
		// 参数名WINDOW，值是HWND的字符串值，表示创建Ogre窗口在这个HWND指定的窗口上，默认由Ogre自动创建
		// 参数名TITLE，值是字符串，表示创建的Ogre窗口的标题，默认值是"没设置标题"
		virtual bool init(NameValuePairList *params = 0);
		// 销毁显示层系统
		virtual void shutdown();

		// 是否已经初始化
		virtual bool isInited()const
		{
			return m_inited;
		}

		// 开始渲染
		virtual void startRendering();

		// 渲染一帧
		virtual void renderOneFrame();

		// Ogre系统
		Root *getRoot()const
		{
			return m_root;
		}

		// 主摄像机
		Camera* getMainCamera()const;
		// 主摄像机
		CameraObject* getMainCameraObject()const
		{
			return m_mainCamera;
		}
		// 主场景管理器
		SceneManager* getMainSceneMgr()const
		{
			return m_mainSceneMgr;
		}
		// 主场景
		IScene* getMainScene()const
		{
			return m_mainScene;
		}
		// 主渲染窗口
		RenderWindow* getMainWindow()const
		{
			return m_mainWindow;
		}
		// 主视口
		Viewport* getMainViewport()const
		{
			return m_mainViewport;
		}

		// 资源路径
		const String &getResourcePath()const
		{
			return m_resourcePath;
		}

		// 显示层参数
		const NameValuePairList &getParams()const
		{
			return m_params;
		}

		// 获得碰撞系统
		ICollisionSystem *getCollisionSystem()const;

		// 获得碰撞世界
		ICollisionWorld *getCollisionWorld()const
		{
			return m_collisionWorld;
		}

		// 获得一个颜色材质
		const String &DisplaySystem::getColorMaterial(const ColourValue &color);
	protected:
		// 创建主渲染窗口，并初始化Ogre，期间会填充m_mainWindow
		virtual bool createRenderWindow(void);
		// 创建场景管理器，期间会填充m_mainSceneMgr
		virtual void createSceneManager(void);
		// 创建摄像机，期间会填充m_mainCamera
		virtual void createCamera(void);
		// 创建视口，期间会填充m_mainViewport
		virtual void createViewports(void);
		// 设置资源路径，期间会调用一系列ResourceGroupManager::getSingleton().addResourceLocation();
		virtual void setupResourcesLocation(void);
		// 创建资源载入侦听器，用来显示载入资源进度用
		virtual void createResourceListener(void);
		// 载入所有资源，期间会调用ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		virtual void loadResources(void);

		// 系统是否已经启动
		bool m_inited;
		// Ogre系统
		Root *m_root;

		// 主摄像机
		CameraObject* m_mainCamera;
		// 主场景
		SceneManager* m_mainSceneMgr;
		// 主渲染窗口
		RenderWindow* m_mainWindow;
		// 主视口
		Viewport* m_mainViewport;

		// 主场景
		IScene *m_mainScene;

		// 碰撞世界
		ICollisionWorld *m_collisionWorld;

		// 触发器世界
		ITriggerWorld *m_triggerWorld;

		// 资源路径
		String m_resourcePath;

		// 显示层参数
		NameValuePairList m_params;
	};
};