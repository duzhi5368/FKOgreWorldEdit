#include "StdAfx.h"
#include "StrollPlugin.h"

namespace Ogre
{
	// 单件模式实现
	template<> StrollPlugin* Singleton<StrollPlugin>::ms_Singleton = 0;
	StrollPlugin* StrollPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	StrollPlugin& StrollPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	StrollPlugin::StrollPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_character(0)
		, m_camera(0)
		, m_state(0)
		, m_autoPlaceFloor(true)
	{
		m_description.name = "漫步场景插件";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "以第三人称漫步场景";
		m_description.publish = "王宏张 2008-3-14";
		m_description.version = 1;
	}

	StrollPlugin::~StrollPlugin(void)
	{
	}


	// 初始化插件，传进来的worldEditor是世界编辑器的接口
	bool StrollPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;

		m_worldEditor->getMainFrame()->createPluginToolBarButton("漫步插件" , this);

		m_activated = true;
		deactivate();
		m_inited = true;
		return m_inited;
	}

	// 关闭插件
	void StrollPlugin::shutdown()
	{
		if(!m_inited)
			return;
		deactivate();
		m_worldEditor->getMainFrame()->destroyPluginToolBarButton("漫步插件");

		m_inited = false;
	}

	// 是否已经初始化
	bool StrollPlugin::isInited()const
	{
		return m_inited;
	}

	// 激活插件
	void StrollPlugin::activate()
	{
		if(m_activated)
			return;

		NameValuePairList params;
		params["mesh"] = "R_F_Z_Head_01.mesh";
		// 创建一个漫游玩家
		m_character = static_cast<CharacterObject*>(DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("CharacterObject" , &params));
		// 创建个摄像机挂在人的后面
		m_camera = static_cast<CameraObject*>(DisplaySystem::getSingleton().getMainCameraObject()->clone("CameraObject"));
		m_camera->removeFromParentObject();
		m_character->addChildObject(m_camera);
		m_camera->getOgreSceneNode()->setFixedYawAxis(true);
		// 放到漫游玩家的后上方，并且看着玩家前方
		m_camera->setPosition(Vector3(0 , 3 , 10));
		m_camera->lookAt(Vector3(0 , 2 , 0));

		// 注册侦听玩家
		m_character->setCallback(this);
		// 开启触发器
		m_character->setEnableTrigger(true);

		IMainFrame::ChildViewIterator iter = m_worldEditor->getMainFrame()->getChildViewIterator();
		while(iter.hasMoreElements())
		{
			IChildView *view = iter.getNext();
			// 把透视视图的摄像机设置成当前摄像机
			if(view->getCameraType() == CT_PERSPECTIVE)
			{
				m_character->setWorldPosition(view->getMoveCameraSceneNode()->getWorldPosition() * Vector3(1 , 0 , 1));
				view->getViewport()->setCamera(m_camera->getOgreCamera());
				break;
			}
		}
		// 开始侦听
		m_worldEditor->getMainFrame()->addChildViewListener(this);
		Root::getSingleton().addFrameListener(this);

		m_worldEditor->getMapManager()->addListener(this);

		m_activated = true;
	}

	// 反激活插件
	void StrollPlugin::deactivate()
	{
		if(!m_activated)
			return;

		// 停止侦听
		m_worldEditor->getMapManager()->removeListener(this);
		Root::getSingleton().removeFrameListener(this);
		m_worldEditor->getMainFrame()->removeChildViewListener(this);

		// 恢复摄像机
		IMainFrame::ChildViewIterator iter = m_worldEditor->getMainFrame()->getChildViewIterator();
		while(iter.hasMoreElements())
		{
			IChildView *view = iter.getNext();
			// 把透视视图的摄像机设置成原来的摄像机
			if(view->getCameraType() == CT_PERSPECTIVE)
			{
				view->getViewport()->setCamera(view->getCamera());
				break;
			}
		}

		// 销毁摄像机
		if(m_camera)
		{
			DisplaySystem::getSingleton().getMainScene()->destroySceneObject(m_camera);
			m_camera = 0;
		}
		// 销毁玩家对象
		if(m_character)
		{
			DisplaySystem::getSingleton().getMainScene()->destroySceneObject(m_character);
			m_character = 0;
		}
		m_state = 0;

		m_activated = false;
	}

	// 是否已经激活
	bool StrollPlugin::isActivate()const
	{
		return m_activated;
	}

	// 获得插件描述
	const IPlugin::Description &StrollPlugin::getDescription()const
	{
		return m_description;
	}

	// 查询一些数据，然后返回到result中
	bool StrollPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// 主窗口消息
	bool StrollPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYDOWN:
			switch(wParam)
			{
			case 'W':
				m_state |= CS_Forward;
				m_character->run(5);
				break;
			case 'S':
				m_state |= CS_Backward;
				m_character->walk(-3.0f);
				break;
			case 'A':
				m_state |= CS_TurnLeft;
				break;
			case 'D':
				m_state |= CS_TurnRight;
				break;
			}
			break;
		case WM_KEYUP:
			switch(wParam)
			{
			case 'W':
				m_state &= ~CS_Forward;
				m_character->stand();
				break;
			case 'S':
				m_state &= ~CS_Backward;
				m_character->stand();
				break;
			case 'A':
				m_state &= ~CS_TurnLeft;
				break;
			case 'D':
				m_state &= ~CS_TurnRight;
				break;
			}
			break;
		}
		return true;
	}

	bool StrollPlugin::frameStarted( const FrameEvent& evt )
	{
		if(m_character)
		{
			// 前行
			if(m_state & CS_Forward)
			{
			}
			// 退后
			if(m_state & CS_Backward)
			{
			}

			// 左转
			if(m_state & CS_TurnLeft)
			{
				m_character->turn(MathEx::EularToQuaternion(Vector3(0 
					, Degree(45).valueRadians() * evt.timeSinceLastFrame , 0)));
			}

			// 右转
			if(m_state & CS_TurnRight)
			{
				m_character->turn(MathEx::EularToQuaternion(Vector3(0 
					, - Degree(45).valueRadians() * evt.timeSinceLastFrame , 0)));
			}
		}
		return true;
	}


	// 视口更新
	void StrollPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		if(m_character && m_autoPlaceFloor)
		{
			Vector3 pos = m_character->getWorldPosition();
			RayCheckInfo info;
			Ray ray(pos + Vector3(0 , 1 , 0) , Vector3::NEGATIVE_UNIT_Y);
			if(m_character->getScene()->collide(Ray(pos + Vector3(0 , 1 , 0) , Vector3::NEGATIVE_UNIT_Y) , true , true , info))
			{
				if(info.obj->getType() == BoxTriggerObjectFactory::FACTORY_TYPE 
					|| info.obj->getType() == SphereTriggerObjectFactory::FACTORY_TYPE)
				{
					return;
				}
				m_character->setWorldPosition(ray.getPoint(info.info.distance));
			}
		}
	}

	// 视口更新
	void StrollPlugin::postViewportUpdate(const RenderTargetViewportEvent& evt)
	{
	}

	// 进入了一个触发器
	// @triggerObject 被进入的触发器
	// @player 进入的玩家
	void StrollPlugin::onEnterTrigger(TriggerObject *triggerObject , CharacterObject *player)
	{
		String msg("玩家[" + player->getName() + "]进入了一个触发器[" + triggerObject->getName() + "]\n");
		ConsoleDebugger::printStr(msg.c_str());
		//AfxMessageBox(msg.c_str());
	}

	// 从一个触发器出去
	// @triggerObject 玩家从这个触发器出去
	// @player 出去的玩家
	void StrollPlugin::onLeaveTrigger(TriggerObject *triggerObject , CharacterObject *player)
	{
		String msg("玩家[" + player->getName() + "]从一个触发器[" + triggerObject->getName() + "]出去了\n");
		ConsoleDebugger::printStr(msg.c_str());
		//AfxMessageBox(msg.c_str());
	}

	// 销毁场景
	void StrollPlugin::onDestroyScene(IScene *scene)
	{
		deactivate();
	}

};