#pragma once
#include "ITriggerEntity.h"
namespace Ogre
{

	// 球体触发器基类
	class ISphereTriggerEntity : public ITriggerEntity
	{
	public:

		ISphereTriggerEntity(void)
		{
		}

		virtual ~ISphereTriggerEntity(void)
		{
		}

		// 设置球的半径
		virtual void setRadius(Real radius) = 0;
		// 获得球的半径
		virtual Real getRadius()const = 0;
	};

};