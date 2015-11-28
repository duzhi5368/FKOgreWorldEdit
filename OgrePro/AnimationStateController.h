#pragma once
#include "IAnimationStateController.h"
namespace Ogre
{
	// 动画状态控制器的实现（只能对单个动画操作）
	class OGREPRO_API AnimationStateController : public IAnimationStateController
	{
	public:
		AnimationStateController();
		virtual ~AnimationStateController(void);

		// 创建
		void create(const String &name , AnimationState* state);
		void create(AnimationState* state);
		// 销毁
		void destroy();

		// 获得动画控制器名称
		virtual const String& getName() const;
		// 获得动画名称
		virtual const String& getAnimationName() const;
		// 获得正在播放的位置
		virtual float getTimePosition(void) const;
		// 设置播放位置
		virtual void setTimePosition(float timePos);
		// 获得动画长度
		virtual float getLength() const;

		// 获得是否播放结束了
		virtual bool hasEnded(void) const;

		// 获得是否正在播放
		virtual bool isPlaying(void) const;
		// 播放
		virtual void play();
		// 暂停
		virtual void pause();
		// 停止
		virtual void stop();
		// 设置是否循环
		virtual void setLoop(bool loop);
		// 获得是否循环
		virtual bool getLoop(void) const;

		// 设置播放速度
		virtual void setSpeed(float speed);
		// 获得播放速度
		virtual float getSpeed()const;

		// 更新动画状态
		virtual void update(float timeSinceLastFrame);
		// 是否已经初始化
		virtual bool isNull()const
		{
			return m_state == 0;
		}
	protected:
		// 动画状态
		AnimationState *m_state;
		// 播放速度
		float m_speed;
		// 控制器名称
		String m_name;
	};
};