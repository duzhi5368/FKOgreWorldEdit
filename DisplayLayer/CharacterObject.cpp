#include "StdAfx.h"
#include "CharacterObject.h"
#include "IScene.h"
#include "ITriggerPlayer.h"
#include "TriggerObject.h"
namespace Ogre
{

	const String CharacterObjectFactory::FACTORY_TYPE = "CharacterObject";

	// 获得该工厂的类型名称
	const String &CharacterObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *CharacterObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		ISceneObject *obj = new CharacterObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void CharacterObjectFactory::destroy(ISceneObject *obj)
	{
		obj->destroy();
		OGRE_DELETE(obj);
	}

	CharacterObject::CharacterObject(const String &name)
		: EntityObject(name)
		, m_movingTargetDistance(FLT_MAX)
		, m_moveStartPosition(Vector3::ZERO)
		, m_movingTarget(Vector3::ZERO)
		, m_movingVelocity(0)
		, m_fadeSpeed(0)
		, m_callback(0)
		, m_triggerPlayer(0)
	{
		m_canCollide = false;
		m_mapAnimationInfo["idle"] = AnimationInfo("idle" , ALL_BLEND , 0 , 0.1f);
		m_mapAnimationInfo["walk"] = AnimationInfo("walk" , ALL_BLEND , 0 , 0.1f);
		m_mapAnimationInfo["run"] = AnimationInfo("run" , ALL_BLEND , 0 , 0.1f);
		m_mapAnimationInfo["attack"] = AnimationInfo("attack" , UPPER_BLEND , 1 , 0.1f);
	}

	CharacterObject::~CharacterObject(void)
	{
		destroy();
	}

	void CharacterObject::setupAnimation()
	{
		// 注册上身骨骼
		m_animationController.registerBoneSelector(UPPER_BLEND , "BU");
		m_animationController.registerBoneSelector(LEFT_HAND_BLEND , "BUL");
		m_animationController.registerBoneSelector(RIGHT_HAND_BLEND , "BUR");
		m_animationController.registerBoneSelector(HEAD_BLEND , "BUH");
	}
	// 设置并行状态机
	void CharacterObject::setupPFSM()
	{
		//添加状态机
		m_fsmController.setCallBackClass(this);

		// 移动层
		m_fsmController.addFSM(FSM_MOVING);
		// 停止移动完毕
		m_fsmController.addState(FSM_MOVING , STATE_MOVING_STOP , &CharacterObject::onMovingStopEnter , &CharacterObject::onMovingStopLeave , &CharacterObject::onMovingStopState);
		// 正在移动
		m_fsmController.addState(FSM_MOVING , STATE_MOVING , &CharacterObject::onMovingEnter , &CharacterObject::onMovingLeave , &CharacterObject::onMovingState);
		// 默认移动
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING_STOP);

		// 姿态层
		m_fsmController.addFSM(FSM_POSE);
		// 站立姿态
		m_fsmController.addState(FSM_POSE , STATE_POSE_STAND , &CharacterObject::onPoseStandEnter);
		// 步行姿态
		m_fsmController.addState(FSM_POSE , STATE_POSE_WALK , &CharacterObject::onPoseWalkEnter);
		// 跑步姿态
		m_fsmController.addState(FSM_POSE , STATE_POSE_RUN , &CharacterObject::onPoseRunEnter);
		// 默认姿态
		m_fsmController.changeState(FSM_POSE , STATE_POSE_STAND);

		// 动作层
		m_fsmController.addFSM(FSM_ACTION);
		// 空闲动作
		m_fsmController.addState(FSM_ACTION , STATE_ACTION_IDLE , &CharacterObject::onActionIdleEnter , &CharacterObject::onActionIdleLeave);
		// 攻击动作
		m_fsmController.addState(FSM_ACTION , STATE_ACTION_ATTACK , &CharacterObject::onActionAttackEnter , &CharacterObject::onActionAttackLeave , &CharacterObject::onActionAttackState);
		// 默认动作
		m_fsmController.changeState(FSM_ACTION , STATE_ACTION_IDLE);

		// 淡入淡出层
		m_fsmController.addFSM(FSM_FADE);
		// 停止淡入淡出
		m_fsmController.addState(FSM_FADE , STATE_FADE_STOP);
		// 渐隐
		m_fsmController.addState(FSM_FADE , STATE_FADE_OUT , 0 , 0 , &CharacterObject::onFadeOutState);
		// 渐现
		m_fsmController.addState(FSM_FADE , STATE_FADE_IN , 0 , 0 , &CharacterObject::onFadeInState);
		// 渐隐渐现
		m_fsmController.addState(FSM_FADE , STATE_FADE_INOUT , 0 , 0 , &CharacterObject::onFadeInOutState);
		// 默认淡入淡出
		m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
	}

	// 创建对象实体
	void CharacterObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();

		EntityObject::create(scene , params);


		// 初始化动画
		setupAnimation();
		
		// 设置默认动画
		if(params && params->find("idle") != params->end())
		{
			m_mapAnimationInfo["idle"].aniName = params->find("idle")->second;
		}
		// 设置并行状态机
		setupPFSM();
	}

	// 销毁对象实体
	void CharacterObject::destroy()
	{
		if(m_triggerPlayer)
		{
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_triggerPlayer);
			m_triggerPlayer = 0;
		}
		m_fsmController.clearFSM();

		EntityObject::destroy();
	}

	// 开启触发器
	void CharacterObject::setEnableTrigger(bool enable)
	{
		if(m_triggerPlayer && !enable)
		{
			m_node->setListener(0);
			m_scene->getTriggerWorld()->destroyTriggerEntity(m_triggerPlayer);
			m_triggerPlayer = 0;
		}
		if(!m_triggerPlayer && enable)
		{
			// 创建触发器玩家对象
			m_triggerPlayer = m_scene->getTriggerWorld()->createTriggerPlayer(getFullName());
			m_triggerPlayer->setBox(m_entity->getBoundingBox());
			m_triggerPlayer->addListener(this);
			m_triggerPlayer->setUserAny(Any(static_cast<ISceneObject*>(this)));
			m_node->setListener(this);
			nodeUpdated(m_node);
		}
	}
	// 是否开启了触发器
	bool CharacterObject::isEnableTrigger()const
	{
		return m_triggerPlayer != 0;
	}

	// 节点更新时顺便更新到触发器里
	void CharacterObject::nodeUpdated(const Node *node)
	{
		if(m_triggerPlayer)
		{
			m_triggerPlayer->setPosition(getWorldPosition());
			m_triggerPlayer->setScale(getWorldScale());
			m_triggerPlayer->setOrientation(getWorldOrientation());
			m_triggerPlayer->updateTrigger();
		}
	}

	// 帧侦听器的每帧执行
	bool CharacterObject::frameStarted( const FrameEvent& evt )
	{
		// 每帧更新并行状态机
		m_fsmController.update(evt.timeSinceLastFrame);
		// 更新动画
		if(!m_animationController.isNull())
		{
			m_animationController.update(evt.timeSinceLastFrame);
		}

		return true;
	}

	// 步行到这个位置
	void  CharacterObject::walkTo(const Vector3 &worldPos , float velocity)
	{
		moveTo(worldPos , velocity);
		//TODO:如果要慢慢转身再移动，那么这里需要做一个转身状态机
		// 转向向前方向
		setWorldOrientation(m_movingDirection);
		// 切换到走路姿态
		m_fsmController.changeState(FSM_POSE , STATE_POSE_WALK);
	}

	// 跑步到这个位置
	void  CharacterObject::runTo(const Vector3 &worldPos , float velocity)
	{
		// 如果移动距离太少，就不理
		if((worldPos - getWorldPosition()).length() < 0.0001f)
			return;
		moveTo(worldPos , velocity);
		// TODO:如果要慢慢转身再移动，那么这里需要做一个转身状态机
		// 立刻转向向前方向
		setWorldOrientation(m_movingDirection);
		// 切换到跑步姿态
		m_fsmController.changeState(FSM_POSE , STATE_POSE_RUN);
	}

	// 移动到这个位置（不改变姿态）
	void CharacterObject::moveTo(const Vector3 &worldPos , float velocity)
	{
		// 预防除0出错，移动距离太小就不管
		if(velocity == 0.0f)
			return;
		// 起点
		m_moveStartPosition = getWorldPosition();
		// 速度
		m_movingVelocity = velocity;

		// 计算四元数方向
		Vector3 dir(worldPos - getWorldPosition());
		float distance = dir.normalise();
		m_movingDirection = MathEx::DirectionToQuaternion(dir);

		// 移动到减速前的点就调用stand
		m_movingTargetDistance = distance;
		// 计算目的地
		m_movingTarget = worldPos;
		// 切换到移动状态
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING);
	}

	// 转向到这个方向
	void CharacterObject::turnTo(const Quaternion &rotate)
	{
		setWorldOrientation(rotate);
		m_movingDirection = rotate;
	}

	// 转向指向这个位置的方向（世界坐标）
	void CharacterObject::turnTo(const Vector3 &targetPoint)
	{
		turnTo(MathEx::PositionToQuaternion(getWorldPosition() , targetPoint));
	}

	// 相对于原来的方向转向一定方向
	void CharacterObject::turn(const Quaternion &rotate)
	{
		turnTo(getWorldOrientation() * rotate);
	}
	// 向前步行
	void CharacterObject::walk(float velocity , float distance)
	{
		move(velocity , distance);
		// 切换到走路姿态
		m_fsmController.changeState(FSM_POSE , STATE_POSE_WALK);
	}

	// 向前跑步
	void CharacterObject::run(float velocity , float distance)
	{
		move(velocity , distance);
		// 切换到跑步姿态
		m_fsmController.changeState(FSM_POSE , STATE_POSE_RUN);
	}

	// 向前移动
	void CharacterObject::move(float velocity , float distance)
	{
		// 预防除0出错
		if(velocity == 0.0f)
			return;
		// 计算方向
		m_movingDirection = getWorldOrientation();
		m_movingVelocity = velocity;
		// 限制距离
		m_movingTargetDistance = FLT_MAX;
		// 起点
		m_moveStartPosition = getWorldPosition();
		// 切换到移动状态
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING);
	}
	// 向前步行
	void CharacterObject::walk(float velocity , const Vector3 &worldPos , float distance)
	{
		turnTo(worldPos);
		walk(velocity , distance);
	}
	// 向前跑步
	void CharacterObject::run(float velocity , const Vector3 &worldPos , float distance)
	{
		turnTo(worldPos);
		run(velocity , distance);
	}
	// 向前移动
	void CharacterObject::move(float velocity , const Vector3 &worldPos , float distance)
	{
		turnTo(worldPos);
		move(velocity , distance);
	}

	// 向前步行，以velocity的速度向相对于自身方向旋转orientation的方向移动distance距离
	void CharacterObject::walk(float velocity , const Quaternion &orientation , float distance)
	{
		// 先保存旋转
		Quaternion rotateOld(getOrientation());
		// 旋转
		rotate(orientation);
		// 向这个方向走
		walk(velocity , distance);
		// 恢复旋转
		setOrientation(rotateOld);
	}

	// 向前跑步，以velocity的速度向相对于自身方向旋转orientation的方向移动distance距离
	void CharacterObject::run(float velocity , const Quaternion &orientation , float distance)
	{
		// 先保存旋转
		Quaternion rotateOld(getOrientation());
		// 旋转
		rotate(orientation);
		// 向这个方向走
		run(velocity , distance);
		// 恢复旋转
		setOrientation(rotateOld);
	}

	// 向前移动，以velocity的速度向相对于自身方向旋转orientation的方向移动distance距离
	void CharacterObject::move(float velocity , const Quaternion &orientation , float distance )
	{
		// 先保存旋转
		Quaternion rotateOld(getOrientation());
		// 旋转
		rotate(orientation);
		// 向这个方向走
		move(velocity , distance);
		// 恢复旋转
		setOrientation(rotateOld);
	}


	// 停止移动，站立
	void CharacterObject::stand()
	{
		// 设置目的速度为0
		m_movingVelocity = 0.0f;
		// 起点
		m_moveStartPosition = getWorldPosition();
		m_movingTargetDistance = 0.0f;
		// 计算目的地
		m_movingTarget = m_moveStartPosition;
		// 停止移动
		m_fsmController.changeState(FSM_MOVING , STATE_MOVING_STOP);
	}
	// 休闲动作
	void CharacterObject::idle()
	{
		m_fsmController.changeState(FSM_ACTION , STATE_ACTION_IDLE);
	}

	// 攻击动作
	void CharacterObject::attack()
	{
		m_fsmController.changeState(FSM_ACTION , STATE_ACTION_ATTACK);
	}

	// 移动开始
	void CharacterObject::onMovingEnter(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onMovingStart();
		}
	}

	// 正在移动
	void CharacterObject::onMovingState(float timeSinceLastFrame)
	{
		// 处理移动
		moving(timeSinceLastFrame);
		// 要检查
		if(m_movingTargetDistance < FLT_MAX)
		{
			// 达目的地，允许一点点误差
			if((getWorldPosition() - m_moveStartPosition).length() >= (m_movingTargetDistance - 0.0001f))
			{
				// 直接改变位置到目的地，预防他跳了过去
				setWorldPosition(m_movingTarget);
				// 停止移动
				stand();
				// 不移动了
				return;
			}
		}
	};
	// 移动处理
	void CharacterObject::moving(float timeSinceLastFrame)
	{
		// 根据时间平移，-Z为正方向
		translate(m_movingDirection * Vector3(0 , 0 , -m_movingVelocity * timeSinceLastFrame));
	}

	// 移动结束
	void CharacterObject::onMovingLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onMovingEnd();
		}
	}


	// 移动停止完毕开始
	void CharacterObject::onMovingStopEnter(uint32 state)
	{
		// 站立姿势
		m_fsmController.changeState(FSM_POSE , STATE_POSE_STAND);
	}

	// 正在移动停止完毕
	void CharacterObject::onMovingStopState(float timeSinceLastFrame)
	{
	};

	// 移动完毕结束
	void CharacterObject::onMovingStopLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onMovingStopEnd();
		}
	}



	// 站立姿态
	void CharacterObject::onPoseStandEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["idle"]);
	};

	// 步行姿态
	void CharacterObject::onPoseWalkEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["walk"]);
	};

	// 跑步姿态
	void CharacterObject::onPoseRunEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["run"]);
	};


	// 空闲动作
	void CharacterObject::onActionIdleEnter(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onActionIdleStart();
		}
	};

	// 空闲动作
	void CharacterObject::onActionIdleLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onActionIdleEnd();
		}
	};
	// 攻击动作
	void CharacterObject::onActionAttackEnter(uint32 state)
	{
		m_animationController.play(m_mapAnimationInfo["attack"]);
		if(m_callback)
		{
			m_callback->onActionAttackStart();
		}
	};
	// 攻击动作
	void CharacterObject::onActionAttackLeave(uint32 state)
	{
		if(m_callback)
		{
			m_callback->onActionAttackEnd();
		}
	};

	// 攻击动作
	void CharacterObject::onActionAttackState(float timeSinceLastFrame)
	{
		// 如果已经播放完毕，就切换回休闲动作
		if(m_animationController.hasEnded(m_mapAnimationInfo["attack"]))
		{
			m_fsmController.changeState(FSM_ACTION , STATE_ACTION_IDLE);
		}
	};


	// 渐隐
	void CharacterObject::onFadeOutState(float timeSinceLastFrame)
	{
		setAlpha(getAlpha() - m_fadeSpeed * timeSinceLastFrame);
		// 如果小于0，就停止渐隐
		if(getAlpha() <= 0.0f)
		{
			m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
		}
	};

	// 渐现
	void CharacterObject::onFadeInState(float timeSinceLastFrame)
	{
		setAlpha(getAlpha() + m_fadeSpeed * timeSinceLastFrame);
		// 如果大于1，就停止渐现
		if(getAlpha() >= 1.0f)
		{
			m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
		}
	};

	// 渐隐渐现
	void CharacterObject::onFadeInOutState(float timeSinceLastFrame)
	{
		setAlpha(getAlpha() + m_fadeSpeed * timeSinceLastFrame);
		// 如果到达最亮或最暗，就反转
		if(getAlpha() >= 1.0f || getAlpha() <= 0.0f)
		{
			m_fadeSpeed = -m_fadeSpeed;
		}
	};

	// 渐现
	void  CharacterObject::fadeIn(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
		m_fsmController.changeState(FSM_FADE , STATE_FADE_IN);
	}

	// 渐隐
	void  CharacterObject::fadeOut(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
		m_fsmController.changeState(FSM_FADE , STATE_FADE_OUT);
	}

	// 渐隐渐现
	void  CharacterObject::fadeInOut(float fadeSpeed)
	{
		m_fadeSpeed = fadeSpeed;
		m_fsmController.changeState(FSM_FADE , STATE_FADE_INOUT);
	}

	// 立刻停止渐隐渐现
	void CharacterObject::stopFade()
	{
		m_fsmController.changeState(FSM_FADE , STATE_FADE_STOP);
	}


	// 进入了一个触发器
	// @entity 被进入的触发器
	// @player 进入的玩家
	void CharacterObject::onEnter(ITriggerEntity *entity , ITriggerPlayer *player)
	{
		ISceneObject *triggerObject = obj_cast(entity->getUserAny());
		ISceneObject *playerObject = obj_cast(player->getUserAny());
		if(m_callback && triggerObject && playerObject)
		{
			m_callback->onEnterTrigger(static_cast<TriggerObject*>(triggerObject) , static_cast<CharacterObject*>(playerObject));
		}
	}

	// 从一个触发器出去
	// @entity 玩家从这个触发器出去
	// @player 出去的玩家
	void CharacterObject::onLeave(ITriggerEntity *entity , ITriggerPlayer *player)
	{
		ISceneObject *triggerObject = obj_cast(entity->getUserAny());
		ISceneObject *playerObject = obj_cast(player->getUserAny());
		if(m_callback && triggerObject && playerObject)
		{
			m_callback->onLeaveTrigger(static_cast<TriggerObject*>(triggerObject) , static_cast<CharacterObject*>(playerObject));
		}
	}
};