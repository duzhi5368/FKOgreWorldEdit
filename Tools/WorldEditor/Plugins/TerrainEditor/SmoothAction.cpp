#include "StdAfx.h"
#include "SmoothAction.h"
#include "TerrainEditorPlugin.h"

namespace Ogre
{
	SmoothAction::SmoothAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
		: SingleSelectionAction(pTerrain , pSelectionRenderer , pSelection , pBrush)
		, m_canAction(false)
	{
		Root::getSingleton().addFrameListener(this);

	}
	SmoothAction::~SmoothAction(void)
	{
		Root::getSingleton().removeFrameListener(this);

	}

	// 子视口消息
	bool SmoothAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
			m_canAction = true;
			break;
		case WM_LBUTTONUP:
			m_canAction = false;
			break;
		}
		return SingleSelectionAction::onWndMsg(view , message , wParam , lParam , pResult);
	}

	bool SmoothAction::frameStarted(const FrameEvent& evt)
	{
		if(m_canAction)
		{
			mTerrain->getTerrainImpl()->smooth(*mSelection , evt.timeSinceLastFrame);
			mSelectionRenderer->needRedraw();
		}
		return true;
	}

}; 