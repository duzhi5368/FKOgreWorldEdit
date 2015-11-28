// ============================================================
// 工程项目	:	MMORPG
// 文件名	:	ETTerrainAction.h
// 创建人	:	王宏张
// 说明		:	地形行为
// 创建时间	:	2007-8-20 15:31:25
// ============================================================

#ifndef __ETTERRAINACTION_H__
#define __ETTERRAINACTION_H__

#include "TerrainEditorDll.h"
#include "TerrainBrush.h"

namespace Ogre
{
	class SelectionRenderer;
	class Selection;

	class TERRAINEDITOR_API TerrainAction : public TerrainBrushListener
	{
	public:
		TerrainAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
			: mSelectionRenderer(pSelectionRenderer)
			, mSelection(pSelection)
			, mBrush(pBrush)
			, mTerrain(pTerrain)
		{
			if(mBrush)
			{
				mBrush->addListener(this);
			}
		}
		virtual ~TerrainAction(void)
		{
			if(mBrush)
			{
				mBrush->removeListener(this);
			}
		}
		// add by 王宏张 2007-7-23
		// 动态更改保存的数据
		void setSelection(Selection *pSelection)
		{
			mSelection = pSelection;
		}
		// add by 王宏张 2007-7-23
		// 动态更改保存的数据
		void setSelectionRenderer(SelectionRenderer *pSelectionRenderer)
		{
			mSelectionRenderer = pSelectionRenderer;
		}
		// 视口更新
		virtual void preViewportUpdate(const RenderTargetViewportEvent& evt){};

		
	protected:
		// 选区渲染器
		SelectionRenderer *mSelectionRenderer;
		// 选择了的数据保存到这里
		Selection *mSelection;
		// 当前选择的笔刷
		TerrainBrush *mBrush;
		// 地形系统
		ETTerrain* mTerrain;
	};
}
#endif