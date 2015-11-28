/******************************************************************
** 文件名:	DecalSystem.h
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:30
** 描  述:	网格贴花的实现，创建指定大小的网格贴花，然后自动调整网格的位置
******************************************************************/
#pragma once
#include "SimpleDecal.h"
namespace Ogre
{
	class GridDecal : public SimpleDecal
	{
	public:
		GridDecal(SceneManager *mgr , const String &name);
		virtual ~GridDecal();

		// 获得贴花的类型
		virtual const DecalType getType()const
		{
			return DT_NORMAL;
		}

		// 设置横向网格数量
		void setWidthGridCount(uint gridCount);
		// 设置竖向网格数量
		void setHeightGridCount(uint gridCount);

		// 获得横向网格数量
		uint getWidthGridCount()const
		{
			return m_widthGridCount;
		}
		
		// 获得竖向网格数量
		uint getHeightGridCount()const
		{
			return m_heightGridCount;
		}

	protected:
		// 当属性改变了，需要重构贴花的时候
		virtual void updateDecal();
		// 贴花对象
		ManualObject *m_decal;
		// 贴花平面
		Plane m_decalPlane;

		// 涉及到贴花的属性被改变了
		enum GRID_DECAL_DIRTY_FLAGS
		{
			GRID_DECAL_DIRTY_WIDTH_GRID_COUNT = 1 << (SimpleDecal::DIRTY_MAX_COUNT + 0),
			GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT = 1 << (SimpleDecal::DIRTY_MAX_COUNT + 1),
			GRID_DECAL_DIRTY_MAX_COUNT		= 7,
		};
		// 横向网格数量
		uint m_widthGridCount;
		// 竖向网格数量
		uint m_heightGridCount;

		// 保存着所有网格的地形碰撞数据
		Vector3 *m_gridPositions;
	};

};