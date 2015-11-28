#include "StdAfx.h"
#include "AxisGridPlugin.h"

namespace Ogre
{
	// 单件模式实现
	template<> AxisGridPlugin* Singleton<AxisGridPlugin>::ms_Singleton = 0;
	AxisGridPlugin* AxisGridPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	AxisGridPlugin& AxisGridPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	AxisGridPlugin::AxisGridPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_gridObject(0)
		, m_gridNode(0)
		, m_axisOverlay(0)
		, m_axisElement(0)
		, m_axisRTT(0)
		, m_statElement(0)
	{
		m_description.name = "网格坐标系插件";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "渲染场景原点平面和坐标轴";
		m_description.publish = "王宏张 2008-1-26";
		m_description.version = 1;
	}

	AxisGridPlugin::~AxisGridPlugin(void)
	{
	}


	// 初始化插件，传进来的worldEditor是世界编辑器的接口
	bool AxisGridPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;
		// 创建网格对象
		m_gridObject = createGrid(128 , 128 , 1);
		// 显示之
		m_gridNode = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->getRootSceneNode()->createChildSceneNode("GridNode");
		m_gridNode->attachObject(m_gridObject);


		m_axisRTT = new AxisRTT();
		m_axisRTT->init(worldEditor);
		m_axisRTT->setEnabled(true);
		// 创建坐标轴
		createAxisOverlay();
		// 创建统计界面
		createStatOverlay();

		m_worldEditor->getMainFrame()->addChildViewListener(this);

		activate();
		m_inited = true;
		return m_inited;
	}

	// 主窗口消息
	bool AxisGridPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYUP:
			switch(wParam)
			{
			case 'G':
				m_gridObject->setVisible(!m_gridObject->getVisible());
				break;
			case VK_INSERT:
				Profiler::getSingleton().setEnabled(!Profiler::getSingleton().getEnabled());
				break;
			}
			break;
		}
		return true;
	}
	// 关闭插件
	void AxisGridPlugin::shutdown()
	{
		if(!m_inited)
			return;
		m_worldEditor->getMainFrame()->removeChildViewListener(this);

		// 销毁网格对象
		m_worldEditor->getDisplaySystem()->getMainSceneMgr()->destroyManualObject(m_gridObject);
		m_worldEditor->getDisplaySystem()->getMainSceneMgr()->destroySceneNode(m_gridNode->getName());
		m_gridObject = 0;
		m_gridNode = 0;

		// 销毁坐标轴对象
		OverlayManager::getSingleton().destroyOverlayElement(m_statElement);
		OverlayManager::getSingleton().destroyOverlayElement(m_axisElement);
		OverlayManager::getSingleton().destroy(m_axisOverlay);
		m_statElement = 0;
		m_axisOverlay = 0;
		m_axisElement = 0;
		OGRE_DELETE(m_axisRTT);

		m_inited = false;
	}

	// 创建网格
	ManualObject* AxisGridPlugin::createGrid(float width , float height , float interval)
	{
		// 创建网格对象
		static size_t s_index = 0;
		ManualObject *gridObject = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->createManualObject(
			"GridObject" + StringConverter::toString(s_index));
		// 用黑色画中间线
		gridObject->begin(m_worldEditor->getDisplaySystem()->getColorMaterial(ColourValue::Black) , RenderOperation::OT_LINE_LIST);
		gridObject->position(-width , 0 , 0);
		gridObject->position(width , 0 , 0);
		gridObject->position(0 , 0 , -height);
		gridObject->position(0 , 0 , height);
		gridObject->end();
		// 画普通线
		gridObject->begin(m_worldEditor->getDisplaySystem()->getColorMaterial(ColourValue(0.37,0.37,0.37)) , RenderOperation::OT_LINE_LIST);
		for(float x = -width ; x <= width ; x += interval)
		{
			if(abs(x) > 0.0001f)
			{
				gridObject->position(x , 0 , -height);
				gridObject->position(x , 0 , height);
			}
		}
		for(float y = -height ; y <= height ; y += interval)
		{
			if(abs(y) > 0.0001f)
			{
				gridObject->position(-width , 0 , y);
				gridObject->position(width , 0 , y);
			}
		}
		gridObject->end();

		return gridObject;
	}
	// 创建统计界面
	void AxisGridPlugin::createStatOverlay()
	{
		m_statElement = static_cast<TextAreaOverlayElement *>(OverlayManager::getSingleton().createOverlayElement("TextArea" , "StatElement"));
		m_statElement->setMetricsMode(GMM_PIXELS);
		m_statElement->setFontName("english");
		m_statElement->setColour(ColourValue(0.1 , 1 , 0.1));
		m_statElement->setDimensions(200 , 14);
		m_statElement->setLeft(m_axisElement->getWidth());
		m_statElement->setCharHeight(14);
		m_axisOverlay->add2D((OverlayContainer*)m_statElement);
	}
	// 创建坐标轴界面
	void AxisGridPlugin::createAxisOverlay()
	{
		m_axisOverlay = OverlayManager::getSingleton().create("AxisOverlay");
		m_axisElement = OverlayManager::getSingleton().createOverlayElement("Panel" , "AxisElement");
		m_axisElement->setMetricsMode(GMM_PIXELS);
		m_axisElement->setDimensions(64 , 64);
		m_axisElement->setMaterialName(m_axisRTT->getAxisMaterialName());
		m_axisOverlay->add2D((OverlayContainer*)m_axisElement);
		m_axisOverlay->show();

	}
	// 是否已经初始化
	bool AxisGridPlugin::isInited()const
	{
		return m_inited;
	}


	// 激活插件
	void AxisGridPlugin::activate()
	{
		if(m_activated)
			return;
		m_activated = true;
	}

	// 反激活插件
	void AxisGridPlugin::deactivate()
	{
		if(!m_activated)
			return;
		m_activated = false;
	}

	// 是否已经激活
	bool AxisGridPlugin::isActivate()const
	{
		return m_activated;
	}

	// 获得插件描述
	const IPlugin::Description &AxisGridPlugin::getDescription()const
	{
		return m_description;
	}

	// 查询一些数据，然后返回到result中
	bool AxisGridPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// 视口更新
	void AxisGridPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		m_axisRTT->preViewportUpdate(evt);
		m_axisElement->setTop(evt.source->getTarget()->getHeight() - m_axisElement->getHeight());
		m_statElement->setTop(evt.source->getTarget()->getHeight() - m_statElement->getHeight());
	}

	// 视口更新
	void AxisGridPlugin::postViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		RenderTarget *target = evt.source->getTarget();
		char text[255];
		const RenderTarget::FrameStats& stats = target->getStatistics();
		sprintf(text , "FPS:%.2f TRI:%d BATCH:%d" , stats.avgFPS , stats.triangleCount , stats.batchCount);
		m_statElement->setCaption(text);
	}
};