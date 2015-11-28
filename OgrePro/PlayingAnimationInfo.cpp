#include "StdAfx.h"
#include "PlayingAnimationInfo.h"
namespace Ogre
{
	PlayingAnimationInfo::PlayingAnimationInfo(AnimationState *state , const AnimationInfo &info)
		: m_aniState(state)
		, m_destWeight(0.0f)
		, m_destBlendTime(info.blendTime)
		, m_aniInfo(info)
		, m_callback(0)
		, m_sourceWeight(0.0f)
		, m_playedCount(0)
	{
		m_aniState->setLoop(false);
		// 全部转换成秒为单位，并标准化到动画时间范围内
		m_aniInfo.positionStart = std::max<float>(m_aniInfo.getPositionStartAsSecond(m_aniState) , 0.0f);
		m_aniInfo.positionEnd = std::min<float>(m_aniInfo.getPositionEndAsSecond(m_aniState) , m_aniState->getLength());
		m_aniInfo.positionFormatStart = PF_SECOND;
		m_aniInfo.positionFormatEnd = PF_SECOND;

		// 从头播放
		// 正向播放
		if(m_aniInfo.playSpeed > 0)
		{
			m_aniState->setTimePosition(m_aniInfo.positionStart);
		}
		// 反向播放
		else if(m_aniInfo.playSpeed < 0)
		{
			m_aniState->setTimePosition(m_aniInfo.positionEnd);
		}
		m_aniState->setEnabled(true);
		m_aniState->setWeight(0.0f);
	}

	PlayingAnimationInfo::~PlayingAnimationInfo()
	{
		clear();
	};
	// 清除动画信息
	void PlayingAnimationInfo::clear()
	{
		if(m_callback)
		{
			m_callback->onPlayEnd(m_aniInfo);
		}
		m_aniState->setEnabled(false);
		m_aniState->setWeight(0.0f);
	}

	// 更新动画
	void PlayingAnimationInfo::update(float timeSinceLastFrame)
	{
		// 更新权重
		float weight = m_aniState->getWeight();
		// 如果时间还没消逝完毕，就步进
		if(m_destBlendTime > 0.0f)
		{
			// 时间慢慢消逝
			m_destBlendTime -= timeSinceLastFrame;
			// 步进
			if(weight > m_destWeight)
			{
				weight -= (m_sourceWeight - m_destWeight) * (timeSinceLastFrame / m_destBlendTimeLeft);
				weight = std::max(weight , 0.0f);
			}
			else if(weight < m_destWeight)
			{
				weight += (m_destWeight - m_sourceWeight) * (timeSinceLastFrame / m_destBlendTimeLeft);
				weight = std::min(weight , 1.0f);
			}
			m_aniState->setWeight(weight);

			// printf("%s : %.2f \n" , m_aniState->getAnimationName().c_str() , m_aniState->getWeight());
		}
		else if(weight != m_destWeight)
		{
			weight = m_destWeight;
			m_destBlendTime = 0.0f;
			m_aniState->setWeight(weight);

			// printf("最终权重 %s : %.2f \n" , m_aniState->getAnimationName().c_str() , m_aniState->getWeight());
		}
		// 计算准备要addTime的时间
		float timePass = timeSinceLastFrame * m_aniInfo.playSpeed;
		// 如果已经播放完毕，就不更新时间了
		if(timePass == 0 || hasEnded())
		{
			return;
		}

		// 更新动画时间
		m_aniState->addTime(timePass);

		Real pos = m_aniState->getTimePosition();
		// 正向播放
		if(m_aniInfo.playSpeed > 0)
		{
			// 播放完一次
			if(pos >= m_aniInfo.positionEnd)
			{
				m_playedCount += (uint)(abs(timePass / (m_aniInfo.positionEnd - m_aniInfo.positionStart)) + 0.9999f);
				// 回调一下
				if(m_callback)
				{
					m_callback->onPlayOnce(m_aniInfo);
				}
				if(hasEnded())
					return;
				m_aniState->setTimePosition(m_aniInfo.positionStart);
			}
		}
		// 反向播放
		else if(m_aniInfo.playSpeed < 0)
		{
			// 播放完了一次
			if(pos <= m_aniInfo.positionStart)
			{
				m_playedCount += (uint)(abs(timePass / (m_aniInfo.positionEnd - m_aniInfo.positionStart)) + 0.9999f);
				// 回调一下
				if(m_callback)
				{
					m_callback->onPlayOnce(m_aniInfo);
				}
				if(hasEnded())
					return;
				m_aniState->setTimePosition(m_aniInfo.positionEnd);
			}
		}
	}
};