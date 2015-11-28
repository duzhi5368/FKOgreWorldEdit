#pragma once
#include "EntityObject.h"
#include "PFSMController.h"
namespace Ogre
{
	class CharacterObject;
	class TriggerObject;
	// 场景物件的对象工厂
	class DISPLAYLAYER_API CharacterObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// 获得该工厂的类型名称
		virtual const String &getType() const;
		// 工厂对象的创建方法
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// 工厂对象的销毁方法
		virtual void destroy(ISceneObject *obj);
	};


	enum PFSMSTATE
	{
		// 这个枚举的第一个值
		CHARACTER_STATE_START,

		// 移动层
		FSM_MOVING,
		// 停止移动
		STATE_MOVING_STOP,
		// 正在移动
		STATE_MOVING,

		// 姿态层
		FSM_POSE,
		// 站立姿态
		STATE_POSE_STAND,
		// 步行姿态
		STATE_POSE_WALK,
		// 跑步姿态
		STATE_POSE_RUN,

		// 动作层
		FSM_ACTION,
		// 空闲动作
		STATE_ACTION_IDLE,
		// 攻击动作
		STATE_ACTION_ATTACK,

		// 淡入淡出层
		FSM_FADE,
		// 停止淡入淡出
		STATE_FADE_STOP,
		// 渐隐
		STATE_FADE_OUT,
		// 渐现
		STATE_FADE_IN,
		// 渐隐渐现
		STATE_FADE_INOUT,

		// 这个枚举的最后一个值，以后继承的就从这里开始
		CHARACTER_STATE_END,
	};
	// 人物动画信息
	typedef std::map<String , AnimationInfo> MapAnimationInfo;

	// 人物动作回调
	class CharacterCallback
	{
	public:
		// 移动开始
		virtual void onMovingStart(){};
		// 移动结束
		virtual void onMovingEnd(){};

		// 移动停止开始
		virtual void onMovingStopStart(){};
		// 移动停止结束
		virtual void onMovingStopEnd(){};

		// 休闲动作开始
		virtual void onActionIdleStart(){};
		// 休闲动作结束
		virtual void onActionIdleEnd(){};

		// 攻击动作开始
		virtual void onActionAttackStart(){};
		// 攻击动作结束
		virtual void onActionAttackEnd(){};


		// 进入了一个触发器
		// @triggerObject 被进入的触发器
		// @player 进入的玩家
		virtual void onEnterTrigger(TriggerObject *triggerObject , CharacterObject *player){};

		// 从一个触发器出去
		// @triggerObject 玩家从这个触发器出去
		// @player 出去的玩家
		virtual void onLeaveTrigger(TriggerObject *triggerObject , CharacterObject *player){};
	};

	enum CharacterBlendType
	{
		UPPER_BLEND = USER_BLEND + 1,
		LEFT_HAND_BLEND,
		RIGHT_HAND_BLEND,
		HEAD_BLEND,
	};

	// 人物对象
	class DISPLAYLAYER_API CharacterObject : public EntityObject
		, public TriggerListener
		, public Node::Listener
	{
	public:
		CharacterObject(const String &name);
		virtual ~CharacterObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return CharacterObjectFactory::FACTORY_TYPE;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();

		// 帧侦听器的每帧执行
		virtual bool frameStarted( const FrameEvent& evt );

		// 步行到这个位置
		void walkTo(const Vector3 &worldPos , float velocity);
		// 跑步到这个位置
		void runTo(const Vector3 &worldPos , float velocity);
		// 移动到这个位置（不改变姿态）
		void moveTo(const Vector3 &worldPos , float velocity);

		// 向前步行，以velocity的速度移动distance距离
		void walk(float velocity , float distance = FLT_MAX);
		// 向前跑步，以velocity的速度移动distance距离
		void run(float velocity , float distance = FLT_MAX);
		// 向前移动，以velocity的速度移动distance距离
		void move(float velocity , float distance = FLT_MAX);

		// 向前步行，先转身向worldPos坐标，然后以velocity的速度移动distance距离
		void walk(float velocity , const Vector3 &worldPos , float distance = FLT_MAX);
		// 向前跑步，先转身向worldPos坐标，然后以velocity的速度移动distance距离
		void run(float velocity , const Vector3 &worldPos , float distance = FLT_MAX);
		// 向前移动，先转身向worldPos坐标，然后以velocity的速度移动distance距离
		void move(float velocity , const Vector3 &worldPos , float distance = FLT_MAX);

		// 向前步行，以velocity的速度向相对于自身方向旋转rotate的方向移动distance距离
		void walk(float velocity , const Quaternion &orientation , float distance = FLT_MAX);
		// 向前跑步，以velocity的速度向相对于自身方向旋转rotate的方向移动distance距离
		void run(float velocity , const Quaternion &orientation , float distance = FLT_MAX);
		// 向前移动，以velocity的速度向相对于自身方向旋转rotate的方向移动distance距离
		void move(float velocity , const Quaternion &orientation , float distance = FLT_MAX);

		// 停止移动，站立
		void stand();

		// 转向到这个方向
		void turnTo(const Quaternion &rotate);
		// 转向指向这个位置的方向（世界坐标）
		void turnTo(const Vector3 &targetPoint);
		// 相对于原来的方向转向一定方向
		void turn(const Quaternion &rotate);

		// 休闲动作
		void idle();
		// 攻击动作
		void attack();

		// 渐现
		void fadeIn(float fadeSpeed);
		// 渐隐
		void fadeOut(float fadeSpeed);
		// 渐隐渐现
		void fadeInOut(float fadeSpeed);
		// 立刻停止渐隐渐现
		void stopFade();

		// 设置人物动作回调
		void setCallback(CharacterCallback *callback)
		{
			m_callback = callback;
		}
		CharacterCallback *getCallback()
		{
			return m_callback;
		}

		// 设置人物动画信息
		void setAnimationInfo(const MapAnimationInfo &mapAnimationInfo)
		{
			m_mapAnimationInfo = mapAnimationInfo;
		}

		// 获得人物动画信息
		MapAnimationInfo &getAnimationInfo()
		{
			return m_mapAnimationInfo;
		}

		// 获得正在移动的方向
		const Quaternion &getMovingDirection()const
		{
			return m_movingDirection;
		}

		// 进入了一个触发器
		// @entity 被进入的触发器
		// @player 进入的玩家
		virtual void onEnter(ITriggerEntity *entity , ITriggerPlayer *player);

		// 从一个触发器出去
		// @entity 玩家从这个触发器出去
		// @player 出去的玩家
		virtual void onLeave(ITriggerEntity *entity , ITriggerPlayer *player);

		// 节点更新时顺便更新到触发器里
		virtual void nodeUpdated(const Node*);

		// 开启触发器
		virtual void setEnableTrigger(bool enable);
		// 是否开启了触发器
		virtual bool isEnableTrigger()const;
	protected:
		// 并行有限状态机
		PFSMController<CharacterObject> m_fsmController;
		// 初始化并行状态机
		virtual void setupPFSM();

		// 初始化动画
		virtual void setupAnimation();

		// 移动开始
		void onMovingEnter(uint32 state);
		// 正在移动
		void onMovingState(float timeSinceLastFrame);
		// 移动结束
		void onMovingLeave(uint32 state);

		// 移动处理
		void moving(float timeSinceLastFrame);

		// 移动停止完毕开始
		void onMovingStopEnter(uint32 state);
		// 正在停止完毕移动
		void onMovingStopState(float timeSinceLastFrame);
		// 移动停止完毕结束
		void onMovingStopLeave(uint32 state);

		// 站立姿态
		void onPoseStandEnter(uint32 state);
		// 步行姿态
		void onPoseWalkEnter(uint32 state);
		// 跑步姿态
		void onPoseRunEnter(uint32 state);

		// 空闲动作开始
		void onActionIdleEnter(uint32 state);
		// 空闲动作结束
		void onActionIdleLeave(uint32 state);
		// 攻击动作进入
		void onActionAttackEnter(uint32 state);
		// 攻击动作结束
		void onActionAttackLeave(uint32 state);
		// 攻击动作执行中
		void onActionAttackState(float timeSinceLastFrame);

		// 渐隐
		void onFadeOutState(float timeSinceLastFrame);
		// 渐现
		void onFadeInState(float timeSinceLastFrame);
		// 渐隐渐现
		void onFadeInOutState(float timeSinceLastFrame);

		// 移动的方向
		Quaternion m_movingDirection;
		// 移动的速度
		float m_movingVelocity;
		// 开始移动的位置
		Vector3 m_moveStartPosition;
		// 需要移动的距离
		float m_movingTargetDistance;
		// 移动的目的地，仅当需要移动距离限制的时候才有用（MoveTo的时候）
		Vector3 m_movingTarget;

		// 每秒的淡入淡出率
		float m_fadeSpeed;

		// 人物动作回调
		CharacterCallback *m_callback;

		// 人物动画信息
		MapAnimationInfo m_mapAnimationInfo;

		// 玩家触发器
		ITriggerPlayer *m_triggerPlayer;
	};


};