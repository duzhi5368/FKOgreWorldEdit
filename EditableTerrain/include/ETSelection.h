/*
Create by 王宏张 2007-07-23
地形选取操作
*/
#ifndef __ETSELECTION_H__
#define __ETSELECTION_H__

#include "ETBrush.h"
#include "ETTerrainInfo.h"
#include "GridVector2.h"
namespace Ogre
{


	typedef std::map<GridVector2 , Real> MapSelection;

	

	// add by 王宏张 2007-7-27
	// 选区的侦听器
	struct ET_EXPORT SelectionListener
	{
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
		virtual void setPositionPost(const GridVector2 &pos){};
	};


	class ETTerrain;

	class ET_EXPORT Selection : public MapSelection
	{
	public:

		Selection(ETTerrain *terrain);
		virtual ~Selection(void);

		// 添加选区
		void addSel(ushort x, ushort z, const Brush &brush, float intensity = 1.0f);
		// 按笔刷清除选区
		void clearSel(ushort x, ushort z, const Brush &brush);

		void clear();

		void updateMinMax();

		const GridVector2 &getPosition()const{return mPosition;};
		const GridVector2 &getMinRel()const{return mMin;};
		const GridVector2 &getMaxRel()const{return mMax;};
		const GridVector2 getMin()const{return mMin + mPosition;};
		const GridVector2 getMax()const{return mMax + mPosition;};
		void setPosition(const GridVector2 &pos)
		{
			// add by 王宏张 2007-7-27
			// 回调到侦听器
			for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
			{
				(*iter)->setPositionPre(pos);
			}
			mPosition = pos;
			// add by 王宏张 2007-7-27
			// 回调到侦听器
			for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
			{
				(*iter)->setPositionPost(pos);
			}
		};

		void addSelectionListener(SelectionListener * pListener)
		{
			mVTSelectionListener.push_back(pListener);
		}

		void removeSelectionListener(SelectionListener * pListener)
		{
			for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
			{
				if(*iter == pListener)
				{
					mVTSelectionListener.erase(iter);
					return;
				}
			}
		}
		void removeAllSelectionListener()
		{
			mVTSelectionListener.clear();
		}
		ETTerrain *getTerrain()
		{
			return m_terrain;
		}

	protected:
		// add by 王宏张 2007-7-27
		// 选区区域
		GridVector2 mPosition;

		// add by 王宏张 2007-7-27
		// 选区最小区域
		GridVector2 mMin;
		// add by 王宏张 2007-7-27
		// 选区最大区域
		GridVector2 mMax;

		ETTerrain *m_terrain;

		// add by 王宏张 2007-7-27
		// 侦听所有影响到Selection数据的函数
		typedef std::vector<SelectionListener*> VTSelectionListener;
		VTSelectionListener mVTSelectionListener;
	};

}
#endif
