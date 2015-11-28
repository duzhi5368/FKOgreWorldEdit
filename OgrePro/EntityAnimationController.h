#pragma once
#include "IEntityAnimationController.h"
#include "AnimationStateController.h"
#include "PlayingAnimationInfo.h"

namespace Ogre
{
	class EntityAnimationController;
	// 骨骼选择器
	class OGREPRO_API BoneSelector : public Animation::AnimationCallback
	{
	public:
		BoneSelector(uint blendType)
			: m_blendType(blendType)
		{

		}
		// 获得混合类型
		uint getBlendType()const
		{
			return m_blendType;
		}
		// 是否影响这个骨头
		virtual bool isApplyBone(Bone *bone) = 0;
	protected:
		uint m_blendType;
	};

	// 骨骼选择器
	class OGREPRO_API NameBoneSelector : public BoneSelector
	{
	public:
		NameBoneSelector(uint blendType , EntityAnimationController *controller , const String &preName = StringUtil::BLANK)
			: BoneSelector(blendType)
			, m_preName(preName)
			, m_controller(controller)
		{

		}
		// 当apply骨骼的时候选择是否apply这个骨骼
		// 在执行的过程中，可以动态修改参数
		virtual bool applyBone(Bone *&bone , Real &weight , Real &scale);
		// 是否影响这个骨头
		virtual bool isApplyBone(Bone *bone);

	protected:
		// 前置名称为这个的才播放
		String m_preName;
		// 动画控制器
		EntityAnimationController *m_controller;
	};

	// 正在播放的动画信息
	struct OGREPRO_API PlayingAnimation
	{
		PlayingAnimation()
			: selector(0)
		{};
		PlayingAnimation(BoneSelector *sel)
			: selector(sel)
		{

		}
		// 最终的动画
		PlayingAnimationInfoPtr aniDest;
		// 正在混合的动画
		PlayingAnimationInfoPtr aniBlending;
		// 骨骼选择器
		BoneSelector *selector;

		// 清空动画状态
		void setNull()
		{
			aniDest.setNull();
			aniBlending.setNull();
		}
	};

	// 针对模型实体的骨骼动画的播放控制器实现
	class OGREPRO_API EntityAnimationController : public IEntityAnimationController
	{
	public:
		EntityAnimationController();
		virtual ~EntityAnimationController(void);

		// 创建
		void create(Entity *entity);
		// 销毁
		void destroy();
		// 停止一个骨骼动画
		virtual void stop(const AnimationInfo &aniInfo);

		// 播放一个骨骼动画
		virtual void play(const AnimationInfo &aniInfo);
		// 播放一个骨骼动画
		virtual void play(const String &name
			, uint blend = ALL_REPLACE
			, uint count = 0
			, float blendtime = 1.0f
			, POISTION_FORMAT pfStart = PF_SECOND
			, float posStart = 0.0f
			, POISTION_FORMAT pfEnd = PF_SECOND
			, float posEnd = FLT_MAX
			, float speed = 1.0f
			)
		{
			play(AnimationInfo(name , blend , count , blendtime , pfStart , posStart , pfEnd , posEnd , speed));
		}
		// 设置播放速度
		virtual void setSpeed(float speed);
		// 获得播放速度
		virtual float getSpeed()const;

		// 设置是否暂停
		virtual void setPaused(bool pause);
		// 是否暂停
		virtual bool isPaused();


		// 获得某个动画是否播放结束了
		virtual bool hasEnded(const AnimationInfo &aniInfo) const;

		// 更新动画状态
		virtual void update(float timeSinceLastFrame);

		// 是否已经初始化
		virtual bool isNull()const
		{
			return m_entity == 0;
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

		// 注册一个骨骼选择器
		virtual void registerBoneSelector(uint blendType , const String &preName)
		{
			m_playing[blendType] = PlayingAnimation(new NameBoneSelector(blendType , this , preName));
		}

		// 注册一个骨骼选择器
		virtual void registerBoneSelector(BoneSelector *sel)
		{
			m_playing[sel->getBlendType()] = PlayingAnimation(sel);
		}


		typedef std::map<uint , PlayingAnimation> MapPlayingAnimation;
		typedef MapIterator<MapPlayingAnimation> PlayingAnimationIterator;
		// 获得动画列表
		PlayingAnimationIterator getPlayingAnimationIterator()
		{
			return PlayingAnimationIterator(m_playing.begin() , m_playing.end());
		}

		// 根据名称获得正在播放的动画
		PlayingAnimationInfoPtr getPlayingAnimation(const AnimationInfo &aniInfo);
	protected:
		// 销毁所有骨骼选择器
		void destroyAllBoneSelector();
		// 获得动画
		PlayingAnimation *getPlayingAnimation(uint blendType);
		const PlayingAnimation *getPlayingAnimation(uint blendType)const;
		// 控制的模型实体
		Entity *m_entity;
		// 当前播放的动作
		MapPlayingAnimation m_playing;
		// 播放速度
		float m_speed;
		// 暂停
		bool m_paused;
		// 播放回调
		IEntityAnimationCallback *m_callback;
		// 清除所有播放状态
		void clearState();
		// 清除一个播放状态
		void clearState(const AnimationInfo &aniInfo);
	};
};