#include "StdAfx.h"
#include "AnimationTrackController.h"
#include "IScene.h"
namespace Ogre
{
	AnimationTrackController::AnimationTrackController(AnimationTrackObject *animationTrackObject , ISceneObject *trackObject)
		: m_speed(1.0f)
		, m_paused(true)
		, m_animationTrackObject(animationTrackObject)
		, m_trackObject(trackObject)
		, m_animation(0)
		, m_animationTrack(0)
		, m_animationState(0)
		, m_sceneMgr(0)
		, m_startTime(0.0f)
		, m_endTime(0.0f)
		, m_needRebuild(true)
		, m_loop(false)
		, m_length(0.0f)
		, m_interpolationMode(AnimationTrackObject::IM_LINEAR)
		, m_rotationInterpolationMode(AnimationTrackObject::RIM_LINEAR)
	{
		m_loop = m_animationTrackObject->getLoop();
		m_interpolationMode = m_animationTrackObject->getInterpolationMode();
		m_rotationInterpolationMode = m_animationTrackObject->getRotationInterpolationMode();

		m_sceneMgr = m_animationTrackObject->getScene()->getOgreSceneManager();
		Root::getSingleton().addFrameListener(this);
		// 一开始就重建所有
		_rebuild();
	}
	AnimationTrackController::~AnimationTrackController()
	{
		Root::getSingleton().removeFrameListener(this);
		destroy();
	}
	// 返回轨迹动画对象
	AnimationTrackObject *AnimationTrackController::getAnimationTrackObject()const
	{
		return m_animationTrackObject;
	}
	// 返回轨迹上的物件对象
	ISceneObject *AnimationTrackController::getTrackObject()const
	{
		return m_trackObject;
	}

	// 播放轨迹动画，设置开始到结束的动画关键帧索引
	void AnimationTrackController::playByIndex(size_t startIndex , size_t endIndex)
	{
		float startTime = getIndexTimePosition(startIndex);
		float endTime = getIndexTimePosition(endIndex);
		playByTime(startTime , endTime);
	}
	// 播放轨迹动画，设置开始到结束的动画时间
	void AnimationTrackController::playByTime(float startTime , float endTime)
	{
		m_startTime = startTime;
		m_endTime = endTime;
		m_animationState->setTimePosition(m_startTime);
		m_animationState->setEnabled(true);
		m_paused = false;
	}
	// 获得所在索引的关键帧相对于开始时间的偏移
	float AnimationTrackController::getIndexTimePosition(size_t index)
	{
		float aniLength = 0.0f;
		AnimationTrackObject::ConstKeyFrameIterator iterator = m_animationTrackObject->getConstKeyFrameIterator();
		size_t indexCount = 0;
		while(iterator.hasMoreElements())
		{
			const AnimationTrackObject::KeyFrame& keyFrame = iterator.getNext();
			if(indexCount >= index)
				break;
			aniLength += keyFrame.m_length;
		}
		return aniLength;
	}

	// 是否已经播放完毕
	bool AnimationTrackController::hasEnded() const
	{
		return m_animationState->getTimePosition() == m_endTime && !m_loop;
	}

	// 暂停
	void AnimationTrackController::setPaused(bool pause)
	{
		m_paused = pause;
	}
	// 是否暂停
	bool AnimationTrackController::getPaused()const
	{
		return m_paused;
	}

	// 设置当前播放位置，使用时间来设置
	void AnimationTrackController::setPlayPosByTime(float timePos)
	{

	}
	// 设置当前播放位置，使用关键帧索引来设置
	void AnimationTrackController::setPlayPosByIndex(size_t index)
	{

	}

	// 获得当前播放的位置，单位为秒
	float AnimationTrackController::getTimePosition()const
	{
		return m_animationState ? m_animationState->getTimePosition() : 0.0f;
	}

	// 设置播放速度
	void AnimationTrackController::setSpeed(float speed)
	{
		m_speed = speed;
	}
	// 获得播放速度
	float AnimationTrackController::getSpeed()const
	{
		return m_speed;
	}

	// 重建整个动画控制器
	void AnimationTrackController::_rebuild()
	{
		if(!m_needRebuild)
			return;
		destroy();

		// 计算动画长度
		m_length = 0.0f;
		{
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_animationTrackObject->getConstKeyFrameIterator();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame& keyFrame = iterator.getNext();
				m_length += keyFrame.m_length;
			}
		}
		// 创建动画
		m_animation = m_sceneMgr->createAnimation(m_animationTrackObject->getFullName() + "_" + m_trackObject->getScene()->getName() + m_trackObject->getName()
			, m_length
			);
		// 使用样条动画
		m_animation->setInterpolationMode((Animation::InterpolationMode)m_interpolationMode);
		m_animation->setRotationInterpolationMode((Animation::RotationInterpolationMode)m_rotationInterpolationMode);
		// 创建节点轨迹动画
		m_animationTrack = m_animation->createNodeTrack(0 , m_trackObject->getOgreSceneNode());
		// 创建所有关键帧
		{
			float timePos = 0.0f;
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_animationTrackObject->getConstKeyFrameIterator();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame& keyFrame = iterator.getNext();
				timePos += keyFrame.m_length;
				TransformKeyFrame *frame = m_animationTrack->createNodeKeyFrame(timePos);
				frame->setTranslate(keyFrame.m_translate);
				frame->setScale(keyFrame.m_scale);
				frame->setRotation(keyFrame.m_rotate);
			}
		}
		// 创建动画状态
		m_animationState = m_sceneMgr->createAnimationState(m_animation->getName());
		// 总是不使用循环系统
		m_animationState->setLoop(false);
		
		// 重建完毕
		m_needRebuild = false;
	}

	// 销毁动画控制器
	void AnimationTrackController::destroy()
	{
		// 销毁动画状态
		if(m_animationState)
		{
			m_sceneMgr->destroyAnimationState(m_animationState->getAnimationName());
			m_animationState = 0;
		}
		// 销毁动画
		if(m_animation)
		{
			m_sceneMgr->destroyAnimation(m_animation->getName());
			m_animation = 0;
			m_animationTrack = 0;
		}
	}

	// 每帧更新动画
	bool AnimationTrackController::frameStarted(const FrameEvent& evt)
	{
		// 看是否需要重建所有
		_rebuild();
		// 如果暂停或未初始化，则不播放
		if(m_paused
			|| m_animationState == 0
			|| !m_animationState->getEnabled())
		{
			return true;
		}
		// 步进时间
		m_animationState->addTime(evt.timeSinceLastFrame * m_speed);
		// 如果超过结束播放时间，就设置到最后去
		if(m_animationState->getTimePosition() >= m_endTime)
		{
			if(!m_loop)
			{
				m_animationState->setTimePosition(m_endTime);
				m_animationState->setEnabled(false);
			}
			else
			{
				m_animationState->setTimePosition(m_startTime);
			}
		}
		// 倒叙播放会出现这种情况
		else if(m_animationState->getTimePosition() <= m_startTime)
		{
			if(!m_loop)
			{
				m_animationState->setTimePosition(m_startTime);
				m_animationState->setEnabled(false);
			}
			else
			{
				m_animationState->setTimePosition(m_endTime);
			}
		}
		return true;
	}


	// 设置是否循环
	void AnimationTrackController::setLoop(bool loop)
	{
		m_loop = loop;
	}
	// 获取是否循环
	bool AnimationTrackController::getLoop()const
	{
		return m_loop;
	}

	// 设置插值方式
	void AnimationTrackController::setInterpolationMode(AnimationTrackObject::InterpolationMode im)
	{
		m_animation->setInterpolationMode((Animation::InterpolationMode)im);
		m_interpolationMode = im;
	}
	// 获得插值方式
	AnimationTrackObject::InterpolationMode AnimationTrackController::getInterpolationMode(void) const
	{
		return m_interpolationMode;
	}

	// 设置旋转插值方式
	void AnimationTrackController::setRotationInterpolationMode(AnimationTrackObject::RotationInterpolationMode im)
	{
		m_animation->setRotationInterpolationMode((Animation::RotationInterpolationMode)im);
		m_rotationInterpolationMode = im;
	}
	// 获得旋转插值方式
	AnimationTrackObject::RotationInterpolationMode AnimationTrackController::getRotationInterpolationMode(void) const
	{
		return m_rotationInterpolationMode;
	}
};