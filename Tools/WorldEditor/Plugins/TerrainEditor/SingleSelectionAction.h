#pragma once
#include "TerrainAction.h"
#include "ETSelection.h"
#include "ETTerrain.h"
#include "SelectionRenderer.h"
#include "TerrainBrush.h"

namespace Ogre
{

	// 单选类
	class SingleSelectionAction : public TerrainAction
		, public IChildView::Listener
	{
	public:
		SingleSelectionAction(ETTerrain* pTerrain
			, SelectionRenderer *pSelectionRenderer
			, Selection *pSelection 
			, TerrainBrush *pBrush
			);
		virtual ~SingleSelectionAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// 视口更新
		virtual void preViewportUpdate(const RenderTargetViewportEvent& evt);

		virtual void updateBrush();

	};
};