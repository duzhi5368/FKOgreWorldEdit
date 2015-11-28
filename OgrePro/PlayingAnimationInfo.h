#pragma once
#include "IEntityAnimationController.h"
namespace Ogre
{
	// 正在播放的动画信息
	class OGREPRO_API PlayingAnimationInfo
	{
	public:
		PlayingAnimationInfo(AnimationState *state , const AnimationInfo &info);
		virtual ~PlayingAnimationInfo();
		// 清除动画信息
		void clear();
		// 更新动画
		void update(float timeSinceLastFrame);
		// 是否已经混合动画完毕
		bool hasBlendEnded()
		{
			return m_aniState->hasEnded() || (m_aniState->getWeight() == m_destWeight || m_destBlendTime == 0.0f);
		}
		// 是否已经播放完毕
		bool hasEnded()
		{
			// 达到次数，就完毕
			return (m_aniInfo.playCount > 0 && m_playedCount >= m_aniInfo.playCount);
		}

		// 设置目标权重
		void setDestWeight(float weight , float blendTime)
		{
			m_sourceWeight = m_aniState->getWeight();
			m_destWeight = weight;
			m_destBlendTime = blendTime;
			m_destBlendTimeLeft = blendTime;
			if(blendTime == 0.0f)
			{
				m_aniState->setWeight(m_destWeight);
				//printf("最终%s : %.2f \n" , m_aniState->getAnimationName().c_str() , m_aniState->getWeight());
			}
		}
		// 强制完成混合
		void forceFinishBlend()
		{
			m_aniState->setEnabled(true);
			m_aniState->setWeight(m_destWeight);
			m_destBlendTime = 0.0f;
		}
		// 设置回调
		virtual void setCallback(IEntityAnimationCallback *callback)
		{
			m_callback = callback;
		}
		// 获取回调
		virtual IEntityAnimationCallback* getCallback()
		{
			return m_callback;
		}

		// 获取切换到这个动画所需的时间
		float getBlendTime()
		{
			return m_aniInfo.blendTime;
		}

		// 获得动画长度
		float getAnimationLength()
		{
			return m_aniState->getLength();
		}

		// 获得当前播放位置，单位是秒
		float getTimePosition()
		{
			return m_aniState->getTimePosition();
		}

		// 动画名称
		const String &getAnimationName()const
		{
			return m_aniInfo.aniName;
		}

		// 设置动作混合时回调
		void setAnimationStateCallback(Animation::AnimationCallback *callback)
		{
			m_aniState->setCallback(callback);
		}

		// 获得动画信息
		const AnimationInfo& getAnimationInfo()const
		{
			return m_aniInfo;
		}

		// 获得动作混合时回调
		Animation::AnimationCallback* getAnimationStateCallback() const { return m_aniState->getCallback(); } 

	protected:
		// 当前播放的动画
		AnimationState *m_aniState;
		// 目标权重
		float m_destWeight;
		// 开始权重
		float m_sourceWeight;
		// 动画信息
		AnimationInfo m_aniInfo;
		// 剩下的转换时间
		float m_destBlendTimeLeft;
		// 从现在权重转换到目标权重所需的时间
		float m_destBlendTime;
		// 已经播放的次数
		uint m_playedCount;
		// 播放回调
		IEntityAnimationCallback *m_callback;

	};

};