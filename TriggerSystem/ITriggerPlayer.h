#pragma once
#include "ITriggerEntity.h"

namespace Ogre
{

	// 可以接受触发器事件的玩家
	class ITriggerPlayer : public ITriggerEntity
	{
	public:
		ITriggerPlayer(void){};
		virtual ~ITriggerPlayer(void){};

		// 更新触发器数据，调用这个会导致一次遍历查询，并回调
		virtual void updateTrigger() = 0;

		// 设置玩家的盒子
		virtual void setBox(const AxisAlignedBox &box) = 0;
		// 获得玩家的盒子
		virtual const AxisAlignedBox &getBox() = 0;

		// 与之发生碰撞的触发器
		typedef std::map<String , ITriggerEntity*> MapCollideTriggerEntity;
		typedef VectorIterator<MapCollideTriggerEntity> CollideTriggerEntityIterator;
		// 获得所有与之发生碰撞的触发器
		virtual CollideTriggerEntityIterator getCollideTriggerEntityIterator() = 0;

	};

};