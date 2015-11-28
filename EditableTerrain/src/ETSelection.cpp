#include "ETPrerequisites.h"
#include "ETSelection.h"
namespace Ogre
{
	Selection::Selection(ETTerrain *terrain)
		: mPosition(0.0f , 0.0f)
		, mMin(65535 , 65535)
		, mMax(0 , 0)
		, m_terrain(terrain)
	{
	}

	Selection::~Selection(void)
	{
	}

	void Selection::clear()
	{
				
		// add by 王宏张 2007-7-27
		// 回调到侦听器
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearPre();
		}
		MapSelection::clear();
		mMin.setX(65535);
		mMin.setY(65535);
		mMax.setX(0);
		mMax.setY(0);
		// add by 王宏张 2007-8-10
		// 不用清理位置
		//mPosition.setX(0);
		//mPosition.setY(0);
		
		// add by 王宏张 2007-7-27
		// 回调到侦听器
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearPost();
		}
	}

	void Selection::addSel(ushort x, ushort z, const Brush &brush, float intensity)
	{

		// add by 王宏张 2007-7-27
		// 回调到侦听器
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->addSelPre(x , z , brush , intensity);
		}


		// 获得笔刷起始位置
		x -= (ushort)brush.getWidth()/2;
		z -= (ushort)brush.getHeight()/2;

		// 遍历笔刷的每个点,从而获得每个点的 x索引值,z索引值,权重值 最后保存在mSeletedPoint中
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			ushort posX = x + (ushort)i;
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				ushort posZ = z + (ushort)j;
				
				// add by 王宏张 2007-7-27
				// 计算最小与最大区域
				mMin.setX(std::min(mMin.getX() , posX));
				mMin.setY(std::min(mMin.getY() , posZ));
				
				mMax.setX(std::max(mMax.getX() , posX));
				mMax.setY(std::max(mMax.getY() , posZ));

				// 查找重复的点,如果重复则不加入
				MapSelection::iterator it = find(GridVector2(posX,posZ));
				if(it == end())
				{
					insert(MapSelection::value_type(GridVector2(posX,posZ)
						, brush.at(i,j) * intensity));
				}
				else
				{
					it->second += brush.at(i,j) * intensity;
					// add by 王宏张 2007-7-23
					// 最大1
					if(it->second > 1.0f)
					{
						it->second = 1.0f;
					}
					else if(it->second < -1.0f)
					{
						it->second = -1.0f;
					}
				}
			}
		}

		
		// add by 王宏张 2007-7-27
		// 回调到侦听器
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->addSelPost(x , z , brush , intensity);
		}
	}

	void Selection::clearSel(ushort x, ushort z, const Brush &brush)
	{
		
		// add by 王宏张 2007-7-27
		// 回调到侦听器
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearSelPre(x , z , brush);
		}
		// 获得笔刷起始位置
		x -= (ushort)brush.getWidth()/2;
		z -= (ushort)brush.getHeight()/2;

		// 遍历笔刷的每个点
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			int posX = x + (ushort)i;
		
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				int posZ = z + (ushort)j;
				
				// 查找笔刷重合的点,如果重合则从mSeletedPoint中清除
				MapSelection::iterator it = find(GridVector2(posX,posZ));
				if(it != end())
				{
					erase(it);
				}
			}
		}
		// add by 王宏张 2007-7-27
		// 更新最小最大区域
		updateMinMax();

				
		// add by 王宏张 2007-7-27
		// 回调到侦听器
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearSelPost(x , z , brush);
		}
	}

	void Selection::updateMinMax()
	{
		mMin.setX(65535);
		mMin.setY(65535);
		mMax.setX(0);
		mMax.setY(0);
		for(iterator iter = begin() ; iter != end() ; iter ++)
		{
			// add by 王宏张 2007-7-27
			// 计算最小与最大区域
			mMin.setX(std::min(mMin.getX() , iter->first.getX()));
			mMin.setY(std::min(mMin.getY()  , iter->first.getY()));
			
			mMax.setX(std::max(mMax.getX()  , iter->first.getX()));
			mMax.setY(std::max(mMax.getY()  , iter->first.getY()));
		}
	}
}