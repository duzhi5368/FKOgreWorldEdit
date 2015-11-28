// ============================================================
// 工程项目	:	MMORPG
// 文件名	:	ETSelectionRenderer.h
// 创建人	:	王宏张
// 说明		:	被选区域渲染器
// 创建时间	:	2007-7-27 14:11:49
// ============================================================

#ifndef __ETSELECTIONRENDERER_H__
#define __ETSELECTIONRENDERER_H__

#include "TerrainEditorDll.h"
#include "ETSelection.h"
#include "ETTerrain.h"
#include "TerrainBrush.h"
namespace Ogre
{

	// add by 王宏张 2007-7-27
	// 被选区域渲染器
	class TERRAINEDITOR_API SelectionRenderer : public SelectionListener
		, public TerrainBrushListener
	{
	public:
		SelectionRenderer(Selection *pSelection , TerrainBrush *brush)
			: m_selection(pSelection)
			, m_brush(brush)
			, m_needRedraw(true)
		{
			m_selection->addSelectionListener(this);
			m_brush->addListener(this);
		}

		virtual ~SelectionRenderer(void)
		{
			m_brush->removeListener(this);
			m_selection->removeSelectionListener(this);
		}

		// 根据数据重画
		virtual void redraw(){};
		// 需要重画
		void needRedraw(){m_needRedraw = true;}

	protected:
		// 选区
		Selection *m_selection;

		TerrainBrush *m_brush;

		bool m_needRedraw;

	};	
}
#endif