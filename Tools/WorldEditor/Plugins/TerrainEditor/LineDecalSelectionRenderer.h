#pragma once

#include "ETSelection.h"
#include "ETTerrain.h"
#include "MovableText.h"
#include "SelectionRenderer.h"
namespace Ogre
{
	// add by 王宏张 2007-7-27
	// 实现线框渲染的选区渲染器
	class LineDecalSelectionRenderer : public SelectionRenderer
		, public SimpleRenderable
	{
	public:
		LineDecalSelectionRenderer(Selection *pSelection , TerrainBrush *brush);
		virtual ~LineDecalSelectionRenderer(void);

		// 添加选区
		virtual void addSelPre(Ogre::ushort x, Ogre::ushort z, const Brush &brush, float intensity){};
		virtual void addSelPost(Ogre::ushort x, Ogre::ushort z, const Brush &brush, float intensity)
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// 按笔刷清除选区
		virtual void clearSelPre(Ogre::ushort x, Ogre::ushort z, const Brush &brush){};
		virtual void clearSelPost(Ogre::ushort x, Ogre::ushort z, const Brush &brush)
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// 清空数据
		virtual void clearPre(){};
		virtual void clearPost()
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// 设置位置
		virtual void setPositionPre(const GridVector2 &pos){};
		virtual void setPositionPost(const GridVector2 &pos)
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// 更新数据，一般由TerrainAction回调
		virtual void redraw()
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		virtual void getRenderOperation(RenderOperation& op)
		{
			// 重画
			if(m_needRedraw)
			{
				redraw();
				m_needRedraw = false;
			}
			SimpleRenderable::getRenderOperation(op);
		}
		virtual void updateBrush()
		{
			m_needRedraw = true;
		}


		Real getSquaredViewDepth(const Camera *cam) const;
		Real getBoundingRadius() const;
	protected:
		void updateVertexBuffer();
		void updateIndexBuffer();

		const Quaternion &getWorldOrientation() const;
		const Vector3 &getWorldPosition() const;

		// add by 王宏张 2007-8-10
		// 保存被选中的网格排序后的索引
		typedef std::map<GridVector2 , ushort> MapGridVector2;
		MapGridVector2 mGridPos;

	};
};