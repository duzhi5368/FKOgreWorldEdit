#pragma once
#include "AnimationTrackObject.h"
namespace Ogre
{
	// 动画轨迹控制器
	class DISPLAYLAYER_API AnimationTrackController : public AnimationTrackObject::IAnimationTrackController
		, public FrameListener
	{
	public:
		AnimationTrackController(AnimationTrackObject *animationTrackObject , ISceneObject *trackObject);
		virtual ~AnimationTrackController();
		// 返回轨迹动画对象
		virtual AnimationTrackObject *getAnimationTrackObject()const;
		// 返回轨迹上的物件对象
		virtual ISceneObject *getTrackObject()const;

		// 播放轨迹动画，设置开始到结束的动画关键帧索引
		virtual void playByIndex(size_t startIndex = 0 , size_t endIndex = 0xFFFFFFFF);
		// 播放轨迹动画，设置开始到结束的动画时间
		virtual void playByTime(float startTime = 0.0f , float endTime = FLT_MAX);

		// 暂停
		virtual void setPaused(bool pause);
		// 是否暂停
		virtual bool getPaused()const;

		// 设置是否循环
		virtual void setLoop(bool loop);
		// 获取是否循环
		virtual bool getLoop()const;

		// 获得动画长度
		virtual float getLength()const
		{
			return m_length;
		}

		// 设置插值方式
		virtual void setInterpolationMode(AnimationTrackObject::InterpolationMode im);
		// 获得插值方式
		virtual AnimationTrackObject::InterpolationMode getInterpolationMode(void) const;
		
		// 设置旋转插值方式
		virtual void setRotationInterpolationMode(AnimationTrackObject::RotationInterpolationMode im);
		// 获得旋转插值方式
		virtual AnimationTrackObject::RotationInterpolationMode getRotationInterpolationMode(void) const;

		// 设置当前播放位置，使用时间来设置
		virtual void setPlayPosByTime(float timePos);
		// 设置当前播放位置，使用关键帧索引来设置
		virtual void setPlayPosByIndex(size_t index);

		// 获得当前播放的位置，单位为秒
		virtual float getTimePosition()const;

		// 是否已经播放完毕
		virtual bool hasEnded() const;

		// 设置播放速度
		virtual void setSpeed(float speed);
		// 获得播放速度
		virtual float getSpeed()const;

		// 通知其重建整个动画轨迹控制器
		void _notifyRebuild()
		{
			m_needRebuild = true;
		}
		// 重建整个动画轨迹控制器
		void _rebuild();

		// 每帧更新动画
		virtual bool frameStarted(const FrameEvent& evt);

		// 获取Ogre动画
		Animation *getAnimation()const
		{
			return m_animation;
		}
		// 获取Ogre动画轨迹
		NodeAnimationTrack *getNodeAnimationTrack()const
		{
			return m_animationTrack;
		}
		// 获取Ogre动画状态
		AnimationState *getAnimationState()const
		{
			return m_animationState;
		}
	protected:
		// 获得所在索引的关键帧相对于开始时间的偏移
		float getIndexTimePosition(size_t index);
		// 销毁动画控制器
		void destroy();
		// 轨迹动画对象
		AnimationTrackObject *m_animationTrackObject;
		// 轨迹上的物件对象
		ISceneObject *m_trackObject;
		// 播放速度
		float m_speed;
		// 是否暂停状态
		bool m_paused;
		// 是否需要重建整个动画轨迹控制器
		bool m_needRebuild;
		// 开始播放的时间
		float m_startTime;
		// 结束播放的时间
		float m_endTime;
		// 动画长度
		float m_length;

		// 是否循环
		bool m_loop;
		// 插值方式
		AnimationTrackObject::InterpolationMode m_interpolationMode;
		// 旋转插值方式
		AnimationTrackObject::RotationInterpolationMode m_rotationInterpolationMode;

		// Ogre动画
		Animation *m_animation;
		// Ogre动画轨迹
		NodeAnimationTrack *m_animationTrack;
		// Ogre动画状态
		AnimationState *m_animationState;
		// Ogre场景管理器
		SceneManager *m_sceneMgr;
	};
};