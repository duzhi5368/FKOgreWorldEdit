#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// 抹平地形
	class AvgFlattenAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		AvgFlattenAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~AvgFlattenAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		bool m_canAction;
	};
};