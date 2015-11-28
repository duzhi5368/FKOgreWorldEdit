#include "StdAfx.h"
#include "TerrainEditorPlugin.h"
#include "DecalSelectionRenderer.h"
#include "SingleSelectionAction.h"
#include "LineDecalSelectionRenderer.h"
#include "ETTerrain.h"
#include "TerrainBrush.h"
#include "SelectionRenderer.h"
#include "TerrainAction.h"
#include "SelectTerrainAction.h"

namespace Ogre
{
	// 单件模式实现
	template<> TerrainEditorPlugin* Singleton<TerrainEditorPlugin>::ms_Singleton = 0;
	TerrainEditorPlugin* TerrainEditorPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	TerrainEditorPlugin& TerrainEditorPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	TerrainEditorPlugin::TerrainEditorPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_selectionRenderer(0)
		, m_selection(0)
		, m_activatedTerrain(0)
		, m_terrainAction(0)
		, m_brush(0)
	{
		m_description.name = "地形编辑器插件";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "渲染场景原点平面和坐标轴";
		m_description.publish = "王宏张 2008-1-26";
		m_description.version = 1;
	}

	TerrainEditorPlugin::~TerrainEditorPlugin(void)
	{
	}


	// 初始化插件，传进来的worldEditor是世界编辑器的接口
	bool TerrainEditorPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;
		m_brush = new TerrainBrush;
		m_brush->loadBrushFromFile("圆形_软.tga");
		m_worldEditor->getMapManager()->addListener(this);

		m_worldEditor->getMainFrame()->addListener(this);
		CXTPDockingPane* paneBrush = m_worldEditor->getMainFrame()->createPane(DlgTerrainBrush::IDD , "笔刷设置" , CRect(0 , 0 , 230 , 230) , xtpPaneDockRight);
		CXTPDockingPane* paneHeight = m_worldEditor->getMainFrame()->createPane(DlgTerrainHeight::IDD , "地形工具" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneBrush);
		CXTPDockingPane* panePaint = m_worldEditor->getMainFrame()->createPane(DlgTerrainPaint::IDD , "地表贴图" , CRect(0 , 200 , 230 , 430) , xtpPaneDockBottom , paneHeight);

		m_worldEditor->getMainFrame()->createPluginToolBarButton("地形插件" , this);

		m_activated = true;
		deactivate();
		m_inited = true;
		return m_inited;
	}

	// 激活以便编辑这个地形
	void TerrainEditorPlugin::activateTerrain(ETTerrain *terrain)
	{
		if(m_activatedTerrain == terrain)
			return;
		deactivateTerrain();
		m_activatedTerrain = terrain;
		m_activatedTerrain->getTerrainImpl()->getParentSceneNode()->showBoundingBox(true);
		m_selection = new Selection(m_activatedTerrain);
		m_selectionRenderer = new LineDecalSelectionRenderer(m_selection , m_brush);
		m_DlgTerrainPaint.refreshAllTexture();
		addTerrainBlockerPass(terrain);

		setTerrainAction(new SelectTerrainAction(getActivatedTerrain()
			, getSelectionRenderer()
			, getSelection() 
			, getTerrainBrush()
			));

	}
	// 反激活地形
	void TerrainEditorPlugin::deactivateTerrain()
	{
		OGRE_DELETE(m_terrainAction);
		OGRE_DELETE(m_selectionRenderer);
		OGRE_DELETE(m_selection);
		if(m_activatedTerrain)
		{
			m_activatedTerrain->getTerrainImpl()->getParentSceneNode()->showBoundingBox(false);
			m_activatedTerrain = 0;
		}
	}

	// 关闭插件
	void TerrainEditorPlugin::shutdown()
	{
		if(!m_inited)
			return;
		m_worldEditor->getMainFrame()->destroyPluginToolBarButton("地形插件");
		m_worldEditor->getMapManager()->removeListener(this);

		m_worldEditor->getMainFrame()->destroyPane(DlgTerrainBrush::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgTerrainHeight::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgTerrainPaint::IDD);
		deactivateTerrain();
		OGRE_DELETE(m_brush);

		m_worldEditor->getMainFrame()->removeListener(this);
		m_inited = false;
	}

	// 是否已经初始化
	bool TerrainEditorPlugin::isInited()const
	{
		return m_inited;
	}

	// 激活插件
	void TerrainEditorPlugin::activate()
	{
		if(m_activated)
			return;
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTerrainBrush::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTerrainHeight::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTerrainPaint::IDD);
		m_worldEditor->getMainFrame()->addChildViewListener(this);

		setTerrainAction(new SelectTerrainAction(getActivatedTerrain()
			, getSelectionRenderer()
			, getSelection() 
			, getTerrainBrush()
			));

		m_activated = true;

	}

	// 反激活插件
	void TerrainEditorPlugin::deactivate()
	{
		if(!m_activated)
			return;
		deactivateTerrain();
		m_worldEditor->getMainFrame()->removeChildViewListener(this);

		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTerrainBrush::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTerrainHeight::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTerrainPaint::IDD);

		m_activated = false;
	}

	// 是否已经激活
	bool TerrainEditorPlugin::isActivate()const
	{
		return m_activated;
	}
	// 实用这个地形操作器
	void TerrainEditorPlugin::setTerrainAction(TerrainAction *action)
	{
		OGRE_DELETE(m_terrainAction);
		m_terrainAction = action;
	}
	// 获得插件描述
	const IPlugin::Description &TerrainEditorPlugin::getDescription()const
	{
		return m_description;
	}

	// 查询一些数据，然后返回到result中
	bool TerrainEditorPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// 视口更新
	void TerrainEditorPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		if(m_terrainAction)
		{
			m_terrainAction->preViewportUpdate(evt);
		}
	}
	// 浮动窗口通知
	LRESULT TerrainEditorPlugin::onDockingPaneNotify(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == XTP_DPN_SHOWWINDOW)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

			if (!pPane->IsValid())
			{
				switch (pPane->GetID())
				{
				case DlgTerrainBrush::IDD:
					{
						if (m_DlgTerrainBrush.GetSafeHwnd() == 0)
						{
							m_DlgTerrainBrush.Create(DlgTerrainBrush::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTerrainBrush);
					}
					break;
				case DlgTerrainHeight::IDD:
					{
						if (m_DlgTerrainHeight.GetSafeHwnd() == 0)
						{
							m_DlgTerrainHeight.Create(DlgTerrainHeight::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTerrainHeight);
					}
					break;
				case DlgTerrainPaint::IDD:
					{
						if (m_DlgTerrainPaint.GetSafeHwnd() == 0)
						{
							m_DlgTerrainPaint.Create(DlgTerrainPaint::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTerrainPaint);
					}
					break;
				default:
					return FALSE;
					break;
				}
			}
			return TRUE;
		}
		return FALSE;
	}

	// 子视口消息
	bool TerrainEditorPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONUP:
			{
				//short xPos = GET_X_LPARAM(lParam); 
				//short yPos = GET_Y_LPARAM(lParam); 
				//CRect rect;
				//::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				//Vector3 pos;
				//Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
				//	, yPos / (float)rect.Height());
				//if(ISceneSystem::getSingleton().rayQuary(ray , 
				//	pos))
				//{
				//	pos.length();
				//}
			}
			break;
		case WM_MOUSEMOVE:
			{
			}
			break;
		case WM_CHAR:
			switch(wParam)
			{
			case '[':
				m_DlgTerrainBrush.m_sliSize.SetPos(m_DlgTerrainBrush.m_sliSize.GetPos() - 1);
				break;
			case ']':
				m_DlgTerrainBrush.m_sliSize.SetPos(m_DlgTerrainBrush.m_sliSize.GetPos() + 1);
				break;
			}
			break;
		}
		return true;
	}

	// 销毁场景
	void TerrainEditorPlugin::onDestroyScene(IScene *scene)
	{
		deactivateTerrain();
	}

	// 保存场景
	void TerrainEditorPlugin::onPreSaveScene(const String &fileName ,IScene *scene)
	{
		// 找到所有地形，保存之
		IScene::SceneObjectIterator iter = scene->getSceneObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
			{
				TerrainObject *terrain = static_cast<TerrainObject*>(obj);

				String outBasename;
				String outExtention; 
				String outPath;
				StringUtil::splitFullFilename(fileName , outBasename , outExtention , outPath);
				removeTerrainBlockerPass(terrain->getOgreTerrain());
				terrain->save(outBasename);
				addTerrainBlockerPass(terrain->getOgreTerrain());
			}
		}
	}

	// 载入一个场景
	void TerrainEditorPlugin::onPostLoadScene(const String &fileName ,IScene *scene)
	{
		// 找到所有地形，激活之
		IScene::SceneObjectIterator iter = scene->getSceneObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
			{
				TerrainObject *terrain = static_cast<TerrainObject*>(obj);
				activateTerrain(terrain->getOgreTerrain());
			}
		}
	}

	// 新建场景
	void TerrainEditorPlugin::onNewScene(IScene *scene)
	{
		// 找到所有地形，激活之
		IScene::SceneObjectIterator iter = scene->getSceneObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
			{
				TerrainObject *terrain = static_cast<TerrainObject*>(obj);
				activateTerrain(terrain->getOgreTerrain());
			}
		}
	}

	// 为地形添加一个障碍区域的显示Pass
	void TerrainEditorPlugin::addTerrainBlockerPass(ETTerrain *terrain)
	{
		MaterialPtr mat = terrain->getTerrainImpl()->getMaterial();
		Technique *tech = mat->getTechnique(0);
		// 看有没有这个Blocker的Pass，如果有就不用添加了
		for(size_t i = 0 ; i < tech->getNumPasses() ; i ++)
		{
			if(tech->getPass(i)->getName() == "Blocker")
				return;
		}
		Pass *blockerPass = mat->getTechnique(0)->createPass();
		blockerPass->setName("Blocker");
		// 使用透明渲染
		blockerPass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
		blockerPass->setVertexProgram("ET/Programs/VSLodMorph2");
		//blockerPass->setPolygonMode(PM_WIREFRAME);
		//blockerPass->setLightingEnabled(false);
		blockerPass->setCullingMode(CULL_ANTICLOCKWISE);
		//blockerPass->setDepthCheckEnabled(false);
		// 创建个贴图
		TextureUnitState *state = blockerPass->createTextureUnitState(terrain->getBlockerTexture()->getName());
		state->setTextureFiltering(TFO_NONE);

	}
	// 把地形的障碍区域显示Pass移除
	void TerrainEditorPlugin::removeTerrainBlockerPass(ETTerrain *terrain)
	{
		MaterialPtr mat = terrain->getTerrainImpl()->getMaterial();
		Technique *tech = mat->getTechnique(0);
		// 看有没有这个Blocker的Pass，如果有就删除之
		for(size_t i = 0 ; i < tech->getNumPasses() ; i ++)
		{
			if(tech->getPass(i)->getName() == "Blocker")
			{
				tech->removePass(i);
				return;
			}
		}
	}

	// 为当前激活的地形添加一个障碍区域的显示Pass
	void TerrainEditorPlugin::addTerrainBlockerPass()
	{
		if(m_activatedTerrain)
		{
			addTerrainBlockerPass(m_activatedTerrain);
		}
	}
	// 把当前激活的地形的障碍区域显示Pass移除
	void TerrainEditorPlugin::removeTerrainBlockerPass()
	{
		if(m_activatedTerrain)
		{
			removeTerrainBlockerPass(m_activatedTerrain);
		}
	}


};