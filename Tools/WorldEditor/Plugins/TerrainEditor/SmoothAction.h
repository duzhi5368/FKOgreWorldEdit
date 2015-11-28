#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// 地形平滑过渡
	class SmoothAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		SmoothAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~SmoothAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		bool m_canAction;
	};
};