#pragma once
#include "TerrainAction.h"
namespace Ogre
{
	// 选择地形
	class SelectTerrainAction : public TerrainAction
		, public IChildView::Listener
	{
	public:
		SelectTerrainAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~SelectTerrainAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	};
};