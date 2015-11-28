#include "StdAfx.h"
#include "RaiseUpDownAction.h"
#include "TerrainEditorPlugin.h"

namespace Ogre
{
	RaiseUpDownAction::RaiseUpDownAction(bool raiseUp , ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
		: SingleSelectionAction(pTerrain , pSelectionRenderer , pSelection , pBrush)
		, m_canAction(false)
		, m_up(raiseUp)
	{
		Root::getSingleton().addFrameListener(this);
	}
	RaiseUpDownAction::~RaiseUpDownAction(void)
	{
		Root::getSingleton().removeFrameListener(this);
	}

	// 子视口消息
	bool RaiseUpDownAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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

	bool RaiseUpDownAction::frameStarted(const FrameEvent& evt)
	{
		if(m_canAction)
		{
			mTerrain->getTerrainImpl()->deform(*mSelection , (m_up ? 1.0f : -1.0f) * evt.timeSinceLastFrame * mBrush->getStrength() * 0.01f);
			mSelectionRenderer->needRedraw();
		}
		return true;
	}

}; 