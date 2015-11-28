#include "StdAfx.h"
#include "SelectTerrainAction.h"
#include "TerrainEditorPlugin.h"

namespace Ogre
{
	SelectTerrainAction::SelectTerrainAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
		: TerrainAction(pTerrain , pSelectionRenderer , pSelection , pBrush)
	{
		TerrainEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
	}
	SelectTerrainAction::~SelectTerrainAction(void)
	{
		TerrainEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// 子视口消息
	bool SelectTerrainAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
			if(wParam == VK_LBUTTON)
			{
				short xPos = GET_X_LPARAM(lParam); 
				short yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				Vector3 pos;
				Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
					, yPos / (float)rect.Height());
				RayCheckInfo info;
				if(TerrainEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->collide(ray , true , false , info))
				{
					// 如果是地形，就激活之
					if(info.obj && info.obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
					{
						TerrainEditorPlugin::getSingleton().activateTerrain(static_cast<TerrainObject*>(info.obj)->getOgreTerrain());
					}
				}
				return true;
			}
			break;
		}
		return true;
	}
}; 