#pragma once
#include "ITriggerEntity.h"
namespace Ogre
{
	// 盒子触发器基类
	class IBoxTriggerEntity : public ITriggerEntity
	{
	public:

		IBoxTriggerEntity(void)
		{
		}

		virtual ~IBoxTriggerEntity(void)
		{
		}

		// 设置盒子
		virtual void setBox(const AxisAlignedBox &box) = 0;
		// 获得盒子
		virtual const AxisAlignedBox &getBox() = 0;
	};

};