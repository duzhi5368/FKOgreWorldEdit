#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// 提升或降低地形高度
	class RaiseUpDownAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		RaiseUpDownAction(bool raiseUp , ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~RaiseUpDownAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);

	protected:
		bool m_canAction;
		bool m_up;
	};
};