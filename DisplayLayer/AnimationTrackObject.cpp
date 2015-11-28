#include "StdAfx.h"
#include "AnimationTrackObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "AnimationTrackObjectVisible.h"
#include "AnimationTrackController.h"
namespace Ogre
{

	const String AnimationTrackObjectFactory::FACTORY_TYPE = "AnimationTrackObject";

	// 获得该工厂的类型名称
	const String &AnimationTrackObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *AnimationTrackObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		AnimationTrackObject *obj = new AnimationTrackObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void AnimationTrackObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	AnimationTrackObject::AnimationTrackObject(const String &name)
		: NullObject(name)
		, m_loop(false)
		, m_interpolationMode(IM_LINEAR)
		, m_rotationInterpolationMode(RIM_LINEAR)
	{
	}

	AnimationTrackObject::~AnimationTrackObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void AnimationTrackObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
	}

	// 销毁对象实体
	void AnimationTrackObject::destroy()
	{
		NullObject::destroy();
		destroyAllControllers();
	}


	// 枚举所有属性，以树的形式
	void AnimationTrackObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_AnimationTrack , PT_GROUP , "动画轨迹" , "动画轨迹" , true , false , false));
		properties.back().addChild(Property( ID_AnimationLoop , PT_BOOL , "是否循环" , "是否循环" , true , true , true));
		properties.back().addChild(Property( ID_AnimationInterpolationMode , PT_UINT , "插值模式" , "插值模式" , true , true , true));
		properties.back().addChild(Property( ID_AnimationRotationInterpolationMode , PT_UINT , "旋转插值模式" , "旋转插值模式" , true , true , true));
		properties.back().addChild(Property( ID_AnimationKeyFrames , PT_GROUP , "关键帧列表" , "关键帧列表" , false , false , false));
		VTProperty &keyFrames = properties.back().childs.back().childs;

		uint count = 0;
		// 添加所有关键帧进去
		for(ListKeyFrame::const_iterator iter = m_listKeyFrame.begin() ; iter != m_listKeyFrame.end() ; iter ++)
		{
			
			keyFrames.push_back(Property( ID_AnimationKeyFrames_Begin + count , PT_GROUP , "关键帧" + StringConverter::toString(count / 5 + 1) , "关键帧" + StringConverter::toString(count / 5 + 1) , false , false , false));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_REAL , "帧动画长度" + StringConverter::toString(count / 5 + 1) , "帧动画长度，单位是秒" , false , true , true));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_VECTOR3 , "帧位移" + StringConverter::toString(count / 5 + 1) , "帧位移" , false , true , true));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_VECTOR3 , "帧缩放" + StringConverter::toString(count / 5 + 1) , "帧缩放" , false , true , true));
			count ++;
			keyFrames.back().addChild(Property( ID_AnimationKeyFrames_Begin + count, PT_VECTOR3 , "帧旋转" + StringConverter::toString(count / 5 + 1) , "帧旋转" , false , true , true));
			count ++;
		}
	}

	// 设置属性
	bool AnimationTrackObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_AnimationTrack:							// 动画轨迹
			{
				return true;
			}
			break;
		case ID_AnimationKeyFrames:							// 关键帧列表
			{
				return true;
			}
			break;
		case ID_AnimationLoop:							// 是否循环
			{
				setLoop(any_cast<bool>(value));
				return true;
			}
			break;
		case ID_AnimationInterpolationMode:							// 插值模式
			{
				setInterpolationMode((InterpolationMode)any_cast<uint>(value));
				return true;
			}
			break;
		case ID_AnimationRotationInterpolationMode:							// 旋转插值模式
			{
				setRotationInterpolationMode((RotationInterpolationMode)any_cast<uint>(value));
				return true;
			}
			break;
		default:
			// 关键帧
			if(id >= ID_AnimationKeyFrames_Begin && id <= ID_AnimationKeyFrames_End)
			{
				size_t index = (id - ID_AnimationKeyFrames_Begin) / 5;
				KeyFrame *keyFrame = 0;
				// 如果超过了大小，就创建一个新的
				if(index >= m_listKeyFrame.size())
				{
					KeyFrame frame;
					m_listKeyFrame.push_back(frame);
					keyFrame = &m_listKeyFrame.back();
				}
				// 否则就取一个旧的
				else
				{
					keyFrame = &m_listKeyFrame[index];
				}
				switch((id - ID_AnimationKeyFrames_Begin) % 5)
				{
					// 关键帧
				case 0:
					return true;
					break;
					// 动画长度
				case 1:
					keyFrame->m_length = any_cast<Real>(value);
					break;
					// 位移
				case 2:
					keyFrame->m_translate = any_cast<Vector3>(value);
					break;
					// 缩放
				case 3:
					keyFrame->m_scale = any_cast<Vector3>(value);
					break;
					// 旋转
				case 4:
					Vector3 rotate(any_cast<Vector3>(value));
					rotate.x = Math::DegreesToRadians(rotate.x);
					rotate.y = Math::DegreesToRadians(rotate.y);
					rotate.z = Math::DegreesToRadians(rotate.z);
					keyFrame->m_rotate = MathEx::EularToQuaternion(rotate);
					break;

				}
				// 通知重建
				_notifyRebuild();
				return true;
			}
			return false;
			break;
		}
	}
	// 获取属性值
	Any AnimationTrackObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_AnimationTrack:							// 动画轨迹
			{
				return Any();
			}
			break;
		case ID_AnimationKeyFrames:							// 关键帧列表
			{
				return Any();
			}
			break;
		case ID_AnimationLoop:							// 是否循环
			{
				return Any(getLoop());
			}
			break;
		case ID_AnimationInterpolationMode:							// 插值模式
			{
				return Any((uint)getInterpolationMode());
			}
			break;
		case ID_AnimationRotationInterpolationMode:							// 旋转插值模式
			{
				return Any((uint)getRotationInterpolationMode());
			}
			break;
		default:
			// 关键帧
			if(id >= ID_AnimationKeyFrames_Begin && id <= ID_AnimationKeyFrames_End)
			{
				size_t index = (id - ID_AnimationKeyFrames_Begin) / 5;
				const KeyFrame *keyFrame = 0;
				// 如果超过了大小，就创建一个新的
				if(index >= m_listKeyFrame.size())
				{
					return Any();
				}
				// 否则就取一个旧的
				else
				{
					keyFrame = &m_listKeyFrame[index];
				}
				switch((id - ID_AnimationKeyFrames_Begin) % 5)
				{
					// 关键帧
				case 0:
					return Any();
					break;
					// 动画长度
				case 1:
					return Any(keyFrame->m_length);
					break;
					// 位移
				case 2:
					return Any(keyFrame->m_translate);
					break;
					// 缩放
				case 3:
					return Any(keyFrame->m_scale);
					break;
					// 旋转
				case 4:
					Vector3 rotate(MathEx::QuaternionToEular(keyFrame->m_rotate));
					rotate.x = Math::RadiansToDegrees(rotate.x);
					rotate.y = Math::RadiansToDegrees(rotate.y);
					rotate.z = Math::RadiansToDegrees(rotate.z);
					return Any(rotate);
					break;
				}
				return Any();
			}
			return Any();
			break;
		}
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* AnimationTrackObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		s_properties.clear();
		enumPropertiesTree(s_properties);
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* AnimationTrackObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		s_properties.clear();
		enumPropertiesList(s_properties);
		return &s_properties;
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *AnimationTrackObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new AnimationTrackObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
	// 添加一个关键帧到最后，返回关键帧的索引
	size_t AnimationTrackObject::addKeyFrameToBack(float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame keyFrame;
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		m_listKeyFrame.push_back(keyFrame);
		_notifyRebuild();
		return m_listKeyFrame.size() - 1;
	}

	// 添加一个关键帧到最前，返回关键帧的索引
	size_t AnimationTrackObject::addKeyFrameToFront(float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame keyFrame;
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		m_listKeyFrame.push_front(keyFrame);
		_notifyRebuild();
		return 0;
	}

	// 在一个关键帧之后插入一个关键帧
	size_t AnimationTrackObject::insertKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame keyFrame;
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		m_listKeyFrame.insert(m_listKeyFrame.begin() + index + 1 , keyFrame);
		_notifyRebuild();
		return index + 1;
	}

	// 修改一个关键帧
	void AnimationTrackObject::modifyKeyFrame(size_t index , float length , const Vector3 &translate , const Vector3 &scale , const Quaternion &rotate)
	{
		KeyFrame &keyFrame = m_listKeyFrame[index];
		keyFrame.m_length = length;
		keyFrame.m_translate = translate;
		keyFrame.m_scale = scale;
		keyFrame.m_rotate = rotate;
		_notifyRebuild();
	}

	// 根据索引删除一个关键帧
	void AnimationTrackObject::removeKeyFrame(size_t index)
	{
		m_listKeyFrame.erase(m_listKeyFrame.begin() + index);
		_notifyRebuild();
	}

	// 获得关键帧的数量
	size_t AnimationTrackObject::getKeyFrameCount()const
	{
		return m_listKeyFrame.size();
	}

	// 根据索引获得一个关键帧
	const AnimationTrackObject::KeyFrame &AnimationTrackObject::getKeyFrame(size_t index)const
	{
		return m_listKeyFrame[index];
	}

	// 让一个场景对象延这条轨迹跑动，返回轨迹控制器
	AnimationTrackObject::IAnimationTrackController* AnimationTrackObject::createTrack(ISceneObject *trackObject)
	{
		AnimationTrackController *controller = new AnimationTrackController(this , trackObject);
		m_controllers.push_back(controller);
		return controller;
	}
	// 销毁跟踪轨迹
	void AnimationTrackObject::destroyTrack(IAnimationTrackController *controller)
	{
		m_controllers.remove(controller);
		delete controller;
	}

	// 通知其重建所有动画轨迹控制器
	void AnimationTrackObject::_notifyRebuild()
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			static_cast<AnimationTrackController*>(*iter)->_notifyRebuild();
		}
		// 重建可见对象
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// 销毁所有轨迹控制器
	void AnimationTrackObject::destroyAllControllers()
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			delete (*iter);
		}
		m_controllers.clear();
	}

	// 设置是否循环
	void AnimationTrackObject::setLoop(bool loop)
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			(*iter)->setLoop(loop);
		}
		m_loop = loop;
		// 重建可见对象
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// 获取是否循环
	bool AnimationTrackObject::getLoop()const
	{
		return m_loop;
	}

	// 设置插值方式
	void AnimationTrackObject::setInterpolationMode(InterpolationMode im)
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			(*iter)->setInterpolationMode(im);
		}
		m_interpolationMode = im;
		// 重建可见对象
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// 获得插值方式
	AnimationTrackObject::InterpolationMode AnimationTrackObject::getInterpolationMode(void) const
	{
		return m_interpolationMode;
	}

	// 设置旋转插值方式
	void AnimationTrackObject::setRotationInterpolationMode(RotationInterpolationMode im)
	{
		for(ListAnimationTrackController::iterator iter = m_controllers.begin() ; iter != m_controllers.end() ; iter ++)
		{
			(*iter)->setRotationInterpolationMode(im);
		}
		m_rotationInterpolationMode = im;
		// 重建可见对象
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->_notifyRebuild();
		}
	}
	// 获得旋转插值方式
	AnimationTrackObject::RotationInterpolationMode AnimationTrackObject::getRotationInterpolationMode(void) const
	{
		return m_rotationInterpolationMode;
	}

};