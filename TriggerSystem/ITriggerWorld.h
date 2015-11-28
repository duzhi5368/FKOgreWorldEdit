#pragma once
#include <TListener.h>
#include "ITriggerEntity.h"
namespace Ogre
{


	class IBoxTriggerEntity;
	class ISphereTriggerEntity;
	// 触发器世界
	class ITriggerWorld
		: public TListener<TriggerListener>
	{
	public:
		ITriggerWorld(){};
		virtual ~ITriggerWorld(void){};

		// 获得名称
		virtual const String &getName()const = 0;


		// 创建盒子触发器实例
		virtual IBoxTriggerEntity* createBoxTriggerEntity(const String &name = StringUtil::BLANK) = 0;
		// 创建球体触发器实例
		virtual ISphereTriggerEntity* createSphereTriggerEntity(const String &name = StringUtil::BLANK) = 0;
		// 创建玩家实例
		virtual ITriggerPlayer * createTriggerPlayer(const String &name = StringUtil::BLANK) = 0;
		// 销毁触发器实例
		virtual void destroyTriggerEntity(ITriggerEntity *entity)= 0;
		// 销毁触发器实例
		virtual void destroyTriggerEntity(const String &name , const String &triggerType)= 0;
		// 获得一个触发器实例
		virtual ITriggerEntity* getTriggerEntity(const String &name , const String &triggerType)const = 0;
		// 是否有这个触发器实例
		virtual bool hasTriggerEntity(const String &name , const String &triggerType)const = 0;

		// 销毁所有触发器实例
		virtual void destroyAllTriggerEntity() = 0;

		typedef std::map<String , ITriggerEntity*> MapTriggerEntity;
		typedef MapIterator<MapTriggerEntity> TriggerEntityIterator;
		// 获得所有触发器的列表
		virtual TriggerEntityIterator getTriggerEntityIterator(const String &triggerType) = 0;
	};
};