#pragma once
#include "NullObject.h"

namespace Ogre
{

	// 触发器对象基类
	class DISPLAYLAYER_API TriggerObject : public NullObject
	{
	public:

		TriggerObject(const String &name);

		virtual ~TriggerObject(void);

		// 获得触发器实例
		virtual ITriggerEntity *getTriggerEntity()const = 0;


		// 设置是否满足条件
		virtual void setCondition(bool value)
		{
			m_condition = value;
		}
		// 返回是否满足条件
		virtual bool getCondition()const
		{
			return m_condition;
		}

		// 设置进入触发器后触发的事件
		virtual void setOnEnterEvent(const String &event)
		{
			m_onEnterEvent = event;
		}
		// 返回进入触发器后触发的事件
		virtual const String &getOnEnterEvent()const
		{
			return m_onEnterEvent;
		}

		// 设置离开触发器后触发的事件
		virtual void setOnLeaveEvent(const String &event)
		{
			m_onLeaveEvent = event;
		}
		// 返回离开触发器后触发的事件
		virtual const String &getOnLeaveEvent()const
		{
			return m_onLeaveEvent;
		}

		// 设置属性
		virtual bool setProperty(uint id , const Any &value);
		// 获取属性值
		virtual Any getProperty(uint id)const;
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const;
	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// 是否满足条件
		bool m_condition;
		// 进入触发器后触发的事件
		String m_onEnterEvent;
		// 离开触发器后触发的事件
		String m_onLeaveEvent;
	};

};