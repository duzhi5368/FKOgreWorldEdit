#pragma once

namespace Ogre
{
	class IScene;
	// 动画状态控制器（只能对单个动画操作）
	class IAnimationStateController
	{
	public:

		IAnimationStateController(void)
		{
		}

		virtual ~IAnimationStateController(void)
		{
		}
		// 获得动画控制器名称
		virtual const String& getName() const = 0;
		// 获得动画名称
		virtual const String& getAnimationName() const = 0;
		// 获得正在播放的位置
		virtual float getTimePosition(void) const = 0;
		// 设置播放位置
		virtual void setTimePosition(float timePos) = 0;
		// 获得动画长度
		virtual float getLength() const = 0;

		// 获得是否播放结束了
		virtual bool hasEnded(void) const = 0;

		// 获得是否正在播放
		virtual bool isPlaying(void) const = 0;
		// 播放
		virtual void play() = 0;
		// 暂停
		virtual void pause() = 0;
		// 停止
		virtual void stop() = 0;
		// 设置是否循环
		virtual void setLoop(bool loop) = 0;
		// 获得是否循环
		virtual bool getLoop(void) const = 0;

		// 设置播放速度
		virtual void setSpeed(float speed) = 0;
		// 获得播放速度
		virtual float getSpeed()const = 0;

		// 更新动画状态
		virtual void update(float timeSinceLastFrame) = 0;

		// 是否已经初始化
		virtual bool isNull()const = 0;

	};
};