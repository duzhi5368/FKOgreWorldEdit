#pragma once
namespace Ogre
{
	class PlayingAnimationInfo;
	typedef SharedPtr<PlayingAnimationInfo> PlayingAnimationInfoPtr;

	// 混合类型
	enum ANI_BLEND_TYPE
	{
		ALL_REPLACE = 0,// 全身替换，只播放这个动画
		ALL_BLEND,      // 全身混合，保持当前的所有动画，然后混合这个动画
		USER_BLEND,		// 自定义混合的由这里开始
	};
	// 位置格式
	enum POISTION_FORMAT
	{
		// 以下3个是位置的度量单位格式，3个中只能选一个
		PF_SECOND		= 1 << 1,		// 单位是秒
		PF_MILLISECOND	= 1 << 2,		// 单位是毫秒，千分之一秒
		PF_PERCENT		= 1 << 3,		// 百分比

		// 以下3个是位置的相对位置，3个中只能选一个
		PF_BEGINPOS		= 1 << 4,		// 从动画开头位置算起
		PF_CURPOS		= 1 << 5,		// 从动画当前位置算起
		PF_ENDPOS		= 1 << 6,		// 从动画末尾位置算起
	};

	// 获得单位为秒的位置
	OGREPRO_API float convertPositionToSecond(const AnimationState *aniState , uchar format , float pos);

	// 动画信息
	struct OGREPRO_API AnimationInfo
	{
		AnimationInfo()
			: blendType(ALL_REPLACE)
			, playCount(0)
			, blendTime(1.3f)
			, positionFormatStart(PF_SECOND | PF_BEGINPOS)
			, positionFormatEnd(PF_SECOND | PF_BEGINPOS)
			, positionStart(0)
			, positionEnd(FLT_MAX)
		{

		}
		AnimationInfo(const String &name
			, uint blend
			, uint count = 0
			, float blendtime = 1.0f
			, uchar pfStart = PF_SECOND | PF_BEGINPOS
			, float posStart = 0.0f
			, uchar pfEnd = PF_SECOND | PF_BEGINPOS
			, float posEnd = FLT_MAX
			, float speed = 1.0f
			)
			: aniName(name)
			, blendType(blend)
			, playCount(count)
			, blendTime(blendtime)
			, positionFormatStart(pfStart)
			, positionStart(posStart)
			, positionFormatEnd(pfEnd)
			, positionEnd(posEnd)
			, playSpeed(speed)
		{

		};
		// 动画名称
		String				aniName;
		// 混合类型
		uint			    blendType;
		// 播放次数
		uint				playCount;
		// 从别的动画过度到这个动画所需的时间
		float				blendTime;
		// 开始播放位置的单位格式
		uchar				positionFormatStart;
		// 开始播放位置
		float				positionStart;
		// 结束播放位置的单位格式
		uchar				positionFormatEnd;
		// 结束播放位置
		float				positionEnd;
		// 播放速度，1表示标准速度，2表示2速，以此类推，负数为倒叙播放，positionStart和positionEnd不变
		float				playSpeed;

		// 获得单位为秒的开始播放位置
		float getPositionStartAsSecond(AnimationState *aniState)
		{
			return convertPositionToSecond(aniState , positionFormatStart , positionStart);
		}

		// 获得单位为秒的结束播放位置
		float getPositionEndAsSecond(AnimationState *aniState)
		{
			return convertPositionToSecond(aniState , positionFormatEnd , positionEnd);
		}
	};

	// 模型实体动画回调
	class IEntityAnimationCallback
	{
	public:
		// 播放完毕回调
		virtual void onPlayEnd(const AnimationInfo &aniInfo) = 0;
		// 播放完了一次，在循环动画中，每播放完一次都会回调到这里
		virtual void onPlayOnce(const AnimationInfo &aniInfo) = 0;
	};

	// 针对模型实体的骨骼动画的播放控制器
	class IEntityAnimationController
	{
	public:

		IEntityAnimationController(void)
		{
		}

		virtual ~IEntityAnimationController(void)
		{
		}

		// 停止一个骨骼动画
		virtual void stop(const AnimationInfo &aniInfo) = 0;
		// 播放一个骨骼动画
		virtual void play(const AnimationInfo &aniInfo) = 0;
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
			) = 0;
		// 设置播放速度
		virtual void setSpeed(float speed) = 0;
		// 获得播放速度
		virtual float getSpeed()const = 0;

		// 设置是否暂停
		virtual void setPaused(bool pause) = 0;
		// 是否暂停
		virtual bool isPaused() = 0;

		// 获得某个动画是否播放结束了
		virtual bool hasEnded(const AnimationInfo &aniInfo) const = 0;

		// 更新动画状态
		virtual void update(float timeSinceLastFrame) = 0;

		// 是否已经初始化
		virtual bool isNull()const = 0;

		// 设置回调
		virtual void setCallback(IEntityAnimationCallback *callback) = 0;
		// 获取回调
		virtual IEntityAnimationCallback* getCallback() = 0;

		// 根据名称获得正在播放的动画
		virtual PlayingAnimationInfoPtr getPlayingAnimation(const AnimationInfo &aniInfo) = 0;

		// 注册一个骨骼选择器
		virtual void registerBoneSelector(uint blendType , const String &preName) = 0;
	};

};