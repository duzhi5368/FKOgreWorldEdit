#pragma once
#include "ETSelection.h"
#include "ETTerrain.h"
#include "SelectionRenderer.h"
#include "IDecalSystem.h"

namespace Ogre
{

	// 贴花渲染的选取
	class DecalSelectionRenderer : public SelectionRenderer
	{
	public:
		DecalSelectionRenderer(Selection *pSelection , TerrainBrush *brush);
		virtual ~DecalSelectionRenderer(void);

		// 添加选区
		virtual void addSelPre(ushort x, ushort z, const Brush &brush, float intensity){};
		virtual void addSelPost(ushort x, ushort z, const Brush &brush, float intensity){};
		// 按笔刷清除选区
		virtual void clearSelPre(ushort x, ushort z, const Brush &brush){};
		virtual void clearSelPost(ushort x, ushort z, const Brush &brush){};
		// add by 王宏张 2007-7-27
		// 清空数据
		virtual void clearPre(){};
		virtual void clearPost(){};
		// add by 王宏张 2007-7-27
		// 设置位置
		virtual void setPositionPre(const GridVector2 &pos){};
		virtual void setPositionPost(const GridVector2 &pos);

		virtual void updateBrush();

	protected:
		// 贴花实例
		IDecal *m_decal;
		MaterialPtr m_material;
		TexturePtr m_texture;
	};

};