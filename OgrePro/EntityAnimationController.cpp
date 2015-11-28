#include "StdAfx.h"
#include "EntityAnimationController.h"
#include "PlayingAnimationInfo.h"
namespace Ogre
{
	// 混合骨骼选择器
	class AllBlendBoneSelector : public BoneSelector
	{
	public:
		AllBlendBoneSelector(EntityAnimationController *controller)
			: BoneSelector(ALL_BLEND)
			, m_controller(controller)
		{
		}
		// 当apply骨骼的时候选择是否apply这个骨骼
		// 在执行的过程中，可以动态修改参数
		virtual bool applyBone(Bone *&bone , Real &weight , Real &scale)
		{
			const String &name = bone->getName();
			EntityAnimationController::PlayingAnimationIterator iter = m_controller->getPlayingAnimationIterator();
			// 影响本骨头的动画数量
			size_t numAnimation = 1;
			while(iter.hasMoreElements())
			{
				PlayingAnimation &ani = iter.getNext();
				// 不检查自己，防止死循环
				if(ani.selector)
				{
					if(ani.selector->getBlendType() == m_blendType)
					{
						continue;
					}
					// 如果有任何一个子动作用到这个骨骼，就不参与全身骨骼混合
					if(!ani.aniBlending.isNull() || !ani.aniDest.isNull())
					{
						if(ani.selector->isApplyBone(bone))
						{
							return false;
						}
						// 统计有多少个子动画在播放
						numAnimation++;
					}
				}
			}
			// 如果有子动画在播放，就递增权重播放
			if(numAnimation > 1)
			{
				weight *= numAnimation;
			}
			return true;
		}
		// 是否影响这个骨头
		virtual bool isApplyBone(Bone *bone)
		{
			return true;
		}

	protected:
		EntityAnimationController *m_controller;
	};

	// 是否影响这个骨头
	bool NameBoneSelector::isApplyBone(Bone *bone)
	{
		const String &name = bone->getName();
		return StringUtil::startsWith(name , m_preName , false);
	}

	// 当apply骨骼的时候选择是否apply这个骨骼
	// 在执行的过程中，可以动态修改参数
	bool NameBoneSelector::applyBone(Bone *&bone , Real &weight , Real &scale)
	{
		if(isApplyBone(bone))
		{
			weight = 1.0f;
			return true;
		}
		else
		{
			return false;
		}
	}


	EntityAnimationController::EntityAnimationController()
		: m_entity(0)
		, m_speed(1.0f)
		, m_paused(true)
		, m_callback(0)
	{
	}

	EntityAnimationController::~EntityAnimationController(void)
	{
		destroy();
	}

	// 销毁所有骨骼选择器
	void EntityAnimationController::destroyAllBoneSelector()
	{
		for(MapPlayingAnimation::iterator iter = m_playing.begin() ; iter != m_playing.end() ; iter ++)
		{
			iter->second.setNull();
			delete iter->second.selector;
		}
		m_playing.clear();
	}

	// 创建
	void EntityAnimationController::create(Entity *entity)
	{
		// 先销毁
		destroy();
		// 注册默认的混合器
		registerBoneSelector(new AllBlendBoneSelector(this));

		// 保存实体
		m_entity = entity;
		// 清除重置所有权重和播放状态
		AnimationStateSet *sets = m_entity->getAllAnimationStates();
		if(sets)
		{
			AnimationStateIterator it = sets->getAnimationStateIterator();
			while(it.hasMoreElements())
			{
				AnimationState *anim = it.getNext();
				anim->setEnabled(false);
				anim->setWeight(0);
				anim->setTimePosition(0);
			}
		}
	}

	// 销毁
	void EntityAnimationController::destroy()
	{
		destroyAllBoneSelector();
	}

	// 清除所有播放状态
	void EntityAnimationController::clearState()
	{
		for(MapPlayingAnimation::iterator iter = m_playing.begin() ; iter != m_playing.end() ; iter ++)
		{
			iter->second.setNull();
		}
	}

	// 清除一个播放状态
	void EntityAnimationController::clearState(const AnimationInfo &aniInfo)
	{
		switch(aniInfo.blendType)
		{
		case ALL_REPLACE:
			{
				clearState();
			}
			break;
		case ALL_BLEND:
			{
				PlayingAnimation &ani = *getPlayingAnimation(ALL_BLEND);
				if(!ani.aniBlending.isNull())
				{
					ani.aniDest = ani.aniBlending;
					// 强制完成混合
					ani.aniDest->forceFinishBlend();
					ani.aniBlending.setNull();
				}
			}
			break;
		default:
			{
				getPlayingAnimation(aniInfo.blendType)->setNull();
			}
			break;
		}
	}

	// 获得动画
	PlayingAnimation *EntityAnimationController::getPlayingAnimation(uint blendType)
	{
		MapPlayingAnimation::iterator iter = m_playing.find(blendType);
		if(iter == m_playing.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"找不到BlendType为 '" + StringConverter::toString(blendType) + "' 的混合类型", 
				"EntityAnimationController::getPlayingAnimation");
			return 0;
		}
		return &iter->second;
	}

	// 获得动画
	const PlayingAnimation *EntityAnimationController::getPlayingAnimation(uint blendType)const
	{
		MapPlayingAnimation::const_iterator iter = m_playing.find(blendType);
		if(iter == m_playing.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"找不到BlendType为 '" + StringConverter::toString(blendType) + "' 的混合类型", 
				"EntityAnimationController::getPlayingAnimation");
			return 0;
		}
		return &iter->second;
	}
	// 播放一个骨骼动画
	void EntityAnimationController::play(const AnimationInfo &aniInfo)
	{
		// 删除之前的动画
		clearState(aniInfo);
		// 获得动画状态
		AnimationState *state = m_entity->getAnimationState(aniInfo.aniName);
		if(state == 0)
			return;
		PlayingAnimationInfoPtr info(new PlayingAnimationInfo(state , aniInfo));
		// 设置播放回调
		info->setCallback(m_callback);

		PlayingAnimation *ani = 0;
		if(aniInfo.blendType == ALL_REPLACE)
		{
			ani = getPlayingAnimation(ALL_BLEND);
		}
		else
		{
			ani = getPlayingAnimation(aniInfo.blendType);
		}

		if(!ani->aniDest.isNull())
		{
			ani->aniDest->setDestWeight(0.0f , info->getBlendTime());
		}
		if(!ani->aniBlending.isNull())
		{
			ani->aniDest = ani->aniBlending;
		}

		if(ani->aniDest.isNull())
		{
			ani->aniDest = info;
			ani->aniDest->setDestWeight(1.0f , 0.0f);
			// 使用自己的选择器
			ani->aniDest->setAnimationStateCallback(ani->selector);
		}
		else
		{
			ani->aniBlending = info;
			ani->aniBlending->setDestWeight(1.0f , info->getBlendTime());
			// 使用自己的选择器
			ani->aniBlending->setAnimationStateCallback(ani->selector);
		}

		m_paused = false;
	}

	// 设置播放速度
	void EntityAnimationController::setSpeed(float speed)
	{
		m_speed = speed;
	}

	// 获得播放速度
	float EntityAnimationController::getSpeed()const
	{
		return m_speed;
	}

	// 设置是否暂停
	void EntityAnimationController::setPaused(bool pause)
	{
		m_paused = pause;
	}

	// 是否暂停
	bool EntityAnimationController::isPaused()
	{
		return m_paused;
	}

	// 更新动画状态
	void EntityAnimationController::update(float timeSinceLastFrame)
	{
		if(m_paused)
			return;
		// 根据速度计算时间
		float timeSince = timeSinceLastFrame * m_speed;
		for(MapPlayingAnimation::iterator iter = m_playing.begin() ; iter != m_playing.end() ; iter ++)
		{
			PlayingAnimation &ani = iter->second;
			// 更新全身姿态
			if(!ani.aniDest.isNull())
			{
				ani.aniDest->update(timeSince);
				if(ani.aniDest->hasEnded())
				{
					// 非全身动作播放完毕，就停止播放之
					if(iter->first != ALL_BLEND)
					{
						ani.aniDest.setNull();
					}
					// 全身动作，就调用停止回调，以后就不回调了
					else
					{
						if(ani.aniDest->getCallback())
						{
							ani.aniDest->getCallback()->onPlayEnd(ani.aniDest->getAnimationInfo());
							ani.aniDest->setCallback(0);
						}
					}
				}

			}
			// 更新过度姿态
			if(!ani.aniBlending.isNull())
			{
				ani.aniBlending->update(timeSince);
				// 全身过度动作过度完毕，就切换到全身姿态
				if(ani.aniBlending->hasBlendEnded())
				{
					ani.aniDest = ani.aniBlending;
					ani.aniBlending.setNull();
					ani.aniDest->forceFinishBlend();
				}
			}
		}
	}

	// 获得某个动画是否播放结束了
	bool EntityAnimationController::hasEnded(const AnimationInfo &aniInfo) const
	{
		const PlayingAnimation &ani = *getPlayingAnimation(aniInfo.blendType);
		PlayingAnimationInfoPtr aniPtr(ani.aniBlending.isNull() ? ani.aniDest : ani.aniBlending);
		if(aniPtr.isNull())
			return true;
		if(aniPtr->getAnimationName() == aniInfo.aniName)
		{
			return aniPtr->hasEnded();
		}
		else
		{
			return true;
		}
	}


	// 停止一个骨骼动画
	void EntityAnimationController::stop(const AnimationInfo &aniInfo)
	{
		
	}
	// 根据名称获得正在播放的动画
	PlayingAnimationInfoPtr EntityAnimationController::getPlayingAnimation(const AnimationInfo &aniInfo)
	{
		MapPlayingAnimation::iterator iter = m_playing.find(aniInfo.blendType);
		if(iter == m_playing.end())
		{
			return PlayingAnimationInfoPtr((PlayingAnimationInfo*)0);
		}
		if(!iter->second.aniBlending.isNull())
		{
			if(iter->second.aniBlending->getAnimationName() == aniInfo.aniName)
			{
				return iter->second.aniBlending;
			}
		}
		if(!iter->second.aniDest.isNull())
		{
			if(iter->second.aniDest->getAnimationName() == aniInfo.aniName)
			{
				return iter->second.aniDest;
			}
		}
		return PlayingAnimationInfoPtr((PlayingAnimationInfo*)0);
	}

	// 获得单位为秒的位置
	inline float convertPositionToSecond(const AnimationState *aniState , uchar format , float pos)
	{
		float posSecond;
		switch(format & 0xF)
		{
		case PF_SECOND:
			posSecond = pos;
			break;
		case PF_MILLISECOND:
			posSecond = pos / 1000.0f;
			break;
		case PF_PERCENT:
			posSecond = aniState->getLength() * pos;
			break;
		default:
			posSecond = pos;
			break;
		}

		switch(format & 0xF0)
		{
		case PF_BEGINPOS:
			return posSecond;
			break;
		case PF_CURPOS:
			return aniState->getTimePosition() + posSecond;
			break;
		case PF_ENDPOS:
			return aniState->getLength() + posSecond;
			break;
		default:
			return posSecond;
			break;
		}
	}
};