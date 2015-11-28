#include "StdAfx.h"
#include "AnimationStateController.h"

namespace Ogre
{
	AnimationStateController::AnimationStateController()
		: m_state(0)
		, m_speed(1.0f)
	{
	}

	AnimationStateController::~AnimationStateController(void)
	{
		destroy();
	}

	// 创建
	void AnimationStateController::create(const String &name , AnimationState* state)
	{
		destroy();
		m_state = state;
		m_name = name;
	}

	// 创建
	void AnimationStateController::create(AnimationState* state)
	{
		create(StringUtil::BLANK , state);
	}
	// 销毁
	void AnimationStateController::destroy()
	{
	}


	// 获得动画控制器名称
	const String& AnimationStateController::getName() const
	{
		return m_name;
	}
	// 获得动画名称
	const String& AnimationStateController::getAnimationName() const
	{
		return m_state->getAnimationName();
	}

	// 获得正在播放的位置
	float AnimationStateController::getTimePosition(void) const
	{
		return m_state->getTimePosition();
	}

	// 设置播放位置
	void AnimationStateController::setTimePosition(float timePos)
	{
		m_state->setTimePosition(timePos);
	}

	// 获得动画长度
	float AnimationStateController::getLength() const
	{
		return m_state->getLength();
	}

	// 获得是否播放结束了
	bool AnimationStateController::hasEnded(void) const
	{
		return m_state->hasEnded();
	}


	// 获得是否正在播放
	bool AnimationStateController::isPlaying(void) const
	{
		return m_state->getEnabled();
	}

	// 播放
	void AnimationStateController::play()
	{
		m_state->setEnabled(true);
	}

	// 暂停
	void AnimationStateController::pause()
	{
		m_state->setEnabled(false);
	}

	// 停止
	void AnimationStateController::stop()
	{
		m_state->setEnabled(false);
		m_state->setTimePosition(0.0f);
	}

	// 设置是否循环
	void AnimationStateController::setLoop(bool loop)
	{
		m_state->setLoop(loop);
	}

	// 获得是否循环
	bool AnimationStateController::getLoop(void) const
	{
		return m_state->getLoop();
	}

	// 更新动画状态
	void AnimationStateController::update(float timeSinceLastFrame)
	{
		m_state->addTime(timeSinceLastFrame * m_speed);
	}
	// 设置播放速度
	void AnimationStateController::setSpeed(float speed)
	{
		m_speed = speed;
	}
	// 获得播放速度
	float AnimationStateController::getSpeed()const
	{
		return m_speed;
	}

};