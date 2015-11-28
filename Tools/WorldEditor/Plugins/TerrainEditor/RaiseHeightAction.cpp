#include "StdAfx.h"
#include "RaiseHeightAction.h"
#include "TerrainEditorPlugin.h"

namespace Ogre
{
	RaiseHeightAction::RaiseHeightAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
		: SingleSelectionAction(pTerrain , pSelectionRenderer , pSelection , pBrush)
	{

	}
	RaiseHeightAction::~RaiseHeightAction(void)
	{

	}

	// 子视口消息
	bool RaiseHeightAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
			if(wParam == VK_LBUTTON)
			{
				mTerrain->getTerrainImpl()->deform(*mSelection , (float)view->getMouseMoveDistance().y * 0.0001f * mBrush->getStrength());
				mSelectionRenderer->needRedraw();
				return true;
			}
			break;
		}
		return SingleSelectionAction::onWndMsg(view , message , wParam , lParam , pResult);
	}
}; 