#pragma once
#include <TListener.h>
namespace Ogre
{
	class ITriggerEntity;
	class ITriggerPlayer;
	// 侦听器
	struct TriggerListener
	{
		// 进入了一个触发器
		// @entity 被进入的触发器
		// @player 进入的玩家
		virtual void onEnter(ITriggerEntity *entity , ITriggerPlayer *player){};

		// 从一个触发器出去
		// @entity 玩家从这个触发器出去
		// @player 出去的玩家
		virtual void onLeave(ITriggerEntity *entity , ITriggerPlayer *player){};
	};

	class ITriggerWorld;
	class ITriggerPlayer;
	// 触发器实体
	class TRIGGERSYSTEM_API ITriggerEntity
		: public TListener<TriggerListener>
	{
	public:
		ITriggerEntity(void){}
		virtual ~ITriggerEntity(void){}


		// 获得名称
		virtual const String &getName()const = 0;
		// 获得类型
		virtual const String &getType()const = 0;

		// 获得触发器世界
		virtual ITriggerWorld *getTriggerWorld()const = 0;

		// 设置相对坐标
		virtual void setPosition(const Vector3 &pos) = 0;
		// 获得相对坐标
		virtual const Vector3 &getPosition()const = 0;

		// 设置相对缩放
		virtual void setScale(const Vector3 &scale) = 0;
		// 获得相对坐标
		virtual const Vector3 &getScale()const = 0;


		// 设置相对方位
		virtual void setOrientation(const Quaternion &orientation) = 0;
		// 获得相对坐标
		virtual const Quaternion &getOrientation()const = 0;

		// 是否与玩家发生了碰撞
		virtual bool collide(ITriggerPlayer *player) = 0;

		// 设置用户变量
		virtual void setUserAny(const Any& anything) { m_userAny = anything; }
		// 获得用户变量
		virtual const Any& getUserAny(void) const { return m_userAny; }

	protected:
		// 用户变量
		Any m_userAny;
	};

	// 把MovableObject::getUserAny的值转换成ITriggerEntity *，转换失败则返回0
	inline ITriggerEntity *trigger_cast(const Any & operand)
	{
		ITriggerEntity ** obj = any_cast<ITriggerEntity*>((Any*)&operand);
		return obj ? *obj : 0;
	};
};