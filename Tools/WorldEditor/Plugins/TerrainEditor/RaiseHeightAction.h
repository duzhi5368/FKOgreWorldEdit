#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// 调整地形高度
	class RaiseHeightAction : public SingleSelectionAction
	{
	public:
		RaiseHeightAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~RaiseHeightAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	};
};