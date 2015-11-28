#pragma once
#include "NullObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API AnimationTrackObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// 获得该工厂的类型名称
		virtual const String &getType() const;
		// 工厂对象的创建方法
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// 工厂对象的销毁方法
		virtual void destroy(ISceneObject *obj);
	};

	// 模型实体对象
	class DISPLAYLAYER_API AnimationTrackObject : public NullObject
	{
	public:
		AnimationTrackObject(const String &name);
		virtual ~AnimationTrackObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return AnimationTrackObjectFactory::FACTORY_TYPE;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();
		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return 0;
		}

		// 设置属性
		virtual bool setProperty(uint id , const Any &value);
		// 获取属性值
		virtual Any getProperty(uint id)const;
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const;

		// 获得场景对象的可见对象，用来在编辑器里选择
		virtual ISceneObjectVisible *getSceneObjectVisible();


		// 一个关键帧
		struct KeyFrame
		{
			KeyFrame()
				: m_length(1.0f)
				, m_translate(Vector3::ZERO)
				, m_scale(Vector3::UNIT_SCALE)
				, m_rotate(Quaternion::IDENTITY)
			{

			}
			// 关键帧的动画长度，单位为秒
			float m_length;
			// 当前的位移
			Vector3 m_translate;
			// 当前的缩放
			Vector3 m_scale;
			// 当前的旋转
			Quaternion m_rotate;
		};
		typedef std::deque<KeyFrame> ListKeyFrame;
		typedef VectorIterator<ListKeyFrame> KeyFrameIterator;
		typedef ConstVectorIterator<ListKeyFrame> ConstKeyFrameIterator;
		// 获得关键帧列表
		KeyFrameIterator getKeyFrameIterator()
		{
			return KeyFrameIterator(m_listKeyFrame.begin() , m_listKeyFrame.end());
		}
		// 获得关键帧列表
		ConstKeyFrameIterator getConstKeyFrameIterator()const
		{
			return ConstKeyFrameIterator(m_listKeyFrame.begin() , m_listKeyFrame.end());
		}

		// 添加一个关键帧到最后，返回关键帧的索引
		size_t addKeyFrameToBack(float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// 添加一个关键帧到最前，返回关键帧的索引
		size_t addKeyFrameToFront(float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// 在一个关键帧之后插入一个关键帧
		size_t insertKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// 修改一个关键帧
		void modifyKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale = Vector3::UNIT_SCALE, const Quaternion &rotate = Quaternion::IDENTITY);
		// 根据索引删除一个关键帧
		void removeKeyFrame(size_t index);
		// 获得关键帧的数量
		size_t getKeyFrameCount()const;
		// 根据索引获得一个关键帧
		const KeyFrame &getKeyFrame(size_t index)const;


		// 设置是否循环
		void setLoop(bool loop);
		// 获取是否循环
		bool getLoop()const;

		// 插值方式
		enum InterpolationMode
		{
			// 线性插值
			IM_LINEAR,
			// 样条插值
			IM_SPLINE
		};

		// 旋转插值方式
		enum RotationInterpolationMode
		{
			// 线性插值
			RIM_LINEAR,
			// 球型插值
			RIM_SPHERICAL,
			// 不插值
			RIM_NONE
		};
		// 设置插值方式
		void setInterpolationMode(InterpolationMode im);
		// 获得插值方式
		InterpolationMode getInterpolationMode(void) const;

		// 设置旋转插值方式
		void setRotationInterpolationMode(RotationInterpolationMode im);
		// 获得旋转插值方式
		RotationInterpolationMode getRotationInterpolationMode(void) const;


		// 轨迹控制器
		class IAnimationTrackController
		{
		public:
			IAnimationTrackController(){};
			virtual ~IAnimationTrackController(){};
			// 返回轨迹动画对象
			virtual AnimationTrackObject *getAnimationTrackObject()const = 0; 
			// 返回轨迹上的物件对象
			virtual ISceneObject *getTrackObject()const = 0;

			// 播放轨迹动画，设置开始到结束的动画关键帧索引
			virtual void playByIndex(size_t startIndex = 0 , size_t endIndex = 0xFFFFFFFF) = 0;
			// 播放轨迹动画，设置开始到结束的动画时间
			virtual void playByTime(float startTime = 0.0f , float endTime = FLT_MAX) = 0;

			// 暂停
			virtual void setPaused(bool pause) = 0;
			// 是否暂停
			virtual bool getPaused()const = 0;

			// 获得动画长度
			virtual float getLength()const = 0;

			// 设置是否循环
			virtual void setLoop(bool loop) = 0;
			// 获取是否循环
			virtual bool getLoop()const = 0;

			// 设置插值方式
			virtual void setInterpolationMode(InterpolationMode im) = 0;
			// 获得插值方式
			virtual InterpolationMode getInterpolationMode(void) const = 0;

			// 设置旋转插值方式
			virtual void setRotationInterpolationMode(RotationInterpolationMode im) = 0;
			// 获得旋转插值方式
			virtual RotationInterpolationMode getRotationInterpolationMode(void) const = 0;


			// 是否已经播放完毕
			virtual bool hasEnded() const = 0;

			// 设置当前播放位置，使用时间来设置
			virtual void setPlayPosByTime(float timePos) = 0;
			// 设置当前播放位置，使用关键帧索引来设置
			virtual void setPlayPosByIndex(size_t index) = 0;

			// 获得当前播放的位置，单位为秒
			virtual float getTimePosition()const = 0;

			// 设置走路速度
			virtual void setSpeed(float speed) = 0;
			// 获得走路速度
			virtual float getSpeed()const = 0;
		};
		// 让一个场景对象延这条轨迹跑动，返回轨迹控制器
		IAnimationTrackController* createTrack(ISceneObject *trackObject);
		// 销毁跟踪轨迹
		void destroyTrack(IAnimationTrackController *controller);

		typedef std::list<IAnimationTrackController*> ListAnimationTrackController;
		typedef VectorIterator<ListAnimationTrackController> AnimationTrackControllerIterator;
		typedef ConstVectorIterator<ListAnimationTrackController> ConstAnimationTrackControllerIterator;
		// 获得所有轨迹控制器
		AnimationTrackControllerIterator getAnimationTrackControllerIterator()
		{
			return AnimationTrackControllerIterator(m_controllers.begin() , m_controllers.end());
		}
		// 获得所有轨迹控制器
		ConstAnimationTrackControllerIterator getConstAnimationTrackControllerIterator()
		{
			return ConstAnimationTrackControllerIterator(m_controllers.begin() , m_controllers.end());
		}
	protected:
		// 通知其重建所有动画轨迹控制器
		void _notifyRebuild();

		// 销毁所有轨迹控制器
		void destroyAllControllers();
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// 当前动画轨迹包含的所有关键帧
		ListKeyFrame m_listKeyFrame;

		// 轨迹控制器列表
		ListAnimationTrackController m_controllers;

		// 是否循环
		bool m_loop;
		// 插值方式
		InterpolationMode m_interpolationMode;
		// 旋转插值方式
		RotationInterpolationMode m_rotationInterpolationMode;
	};
};