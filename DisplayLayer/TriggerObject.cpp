#include "StdAfx.h"
#include "TriggerObject.h"

namespace Ogre
{
	TriggerObject::TriggerObject(const String &name)
		: NullObject(name)
		, m_condition(true)
	{
	}

	TriggerObject::~TriggerObject(void)
	{
	}

	// 枚举所有属性，以树的形式
	void TriggerObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Trigger , PT_GROUP , "触发器设置" , "触发器设置" , true , false , false));
		properties.back().addChild(Property( ID_TriggerCondition , PT_BOOL , "满足条件" , "是否已经满足条件触发这个触发器，只有当条件满足时才可以触发" , true , true , true));
		properties.back().addChild(Property( ID_TriggerOnEnterEvent , PT_STRING , "进入事件" , "当满足条件后进入触发器，就会触发这个事件" , true , true , true));
		properties.back().addChild(Property( ID_TriggerOnLeaveEvent , PT_STRING , "退出事件" , "当触发了进入事件后退出触发器，就会触发这个事件" , true , true , true));
	}

	// 设置属性
	bool TriggerObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Trigger:							// 触发器设置
			{
				return true;
			}
			break;
		case ID_TriggerCondition:							// 满足条件
			{
				setCondition(any_cast<bool>(value));
				return true;
			}
			break;
		case ID_TriggerOnEnterEvent:							// 进入事件
			{
				setOnEnterEvent(any_cast<String>(value));
				return true;
			}
			break;
		case ID_TriggerOnLeaveEvent:							// 退出事件
			{
				setOnLeaveEvent(any_cast<String>(value));
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any TriggerObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Trigger:							// 触发器设置
			{
				return Any();
			}
			break;
		case ID_TriggerCondition:							// 满足条件
			{
				return Any(getCondition());
			}
			break;
		case ID_TriggerOnEnterEvent:							// 进入事件
			{
				return Any(getOnEnterEvent());
			}
			break;
		case ID_TriggerOnLeaveEvent:							// 退出事件
			{
				return Any(getOnLeaveEvent());
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* TriggerObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* TriggerObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

};