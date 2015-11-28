#include "StdAfx.h"
#include "TrackKeyFrameObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "DlgTrackTools.h"
namespace Ogre
{

#define ID_KeyFrameLength (ID_EntityObject_End - 1)
	const String TrackKeyFrameObjectFactory::FACTORY_TYPE = "TrackKeyFrameObject";

	// 获得该工厂的类型名称
	const String &TrackKeyFrameObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *TrackKeyFrameObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		TrackKeyFrameObject *obj = new TrackKeyFrameObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void TrackKeyFrameObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	TrackKeyFrameObject::TrackKeyFrameObject(const String &name)
		: EntityObject(name)
		, m_length(1.0f)
		, m_scale(Vector3::UNIT_SCALE)
	{
	}

	TrackKeyFrameObject::~TrackKeyFrameObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void TrackKeyFrameObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		EntityObject::create(scene , params);

		setScale(m_scale);

	}

	// 销毁对象实体
	void TrackKeyFrameObject::destroy()
	{

		EntityObject::destroy();
	}

	// 把数据更新到关键帧动画里去
	void TrackKeyFrameObject::updateToKeyFrame()
	{
		// 只有保存着动画指针的才可以
		KeyObjectData data = safe_cast<KeyObjectData>(getUserAny());
		if(!data.isNull())
		{
			const AnimationTrackObject::KeyFrame &keyFrame = data.m_object->getKeyFrame(data.m_index);
			data.m_object->modifyKeyFrame(data.m_index , m_length , getPosition() , getScale() , getOrientation());
		}
	}

	// 把数据从关键帧动画里更新到这里
	void TrackKeyFrameObject::updateFromKeyFrame()
	{
		// 只有保存着动画指针的才可以
		KeyObjectData data = safe_cast<KeyObjectData>(getUserAny());
		if(!data.isNull())
		{
			const AnimationTrackObject::KeyFrame &keyFrame = data.m_object->getKeyFrame(data.m_index);
			m_node->setPosition(keyFrame.m_translate);
			m_scale = keyFrame.m_scale;
			EntityObject::setScale(m_scale * (1.0f / m_entity->getMesh()->getBoundingSphereRadius()));
			m_node->setOrientation(keyFrame.m_rotate);
			m_length = keyFrame.m_length;
		}
	}

	// 设置相对坐标
	void TrackKeyFrameObject::setPosition(const Vector3 &pos)
	{
		EntityObject::setPosition(pos);
		updateToKeyFrame();
	}

	// 设置相对缩放
	void TrackKeyFrameObject::setScale(const Vector3 &scale)
	{
		m_scale = scale;
		EntityObject::setScale(scale * (1.0f / m_entity->getMesh()->getBoundingSphereRadius()));
		updateToKeyFrame();
	}

	// 获得相对坐标
	const Vector3 &TrackKeyFrameObject::getScale()const
	{
		return m_scale;
	}

	// 设置相对方位
	void TrackKeyFrameObject::setOrientation(const Quaternion &orientation)
	{
		EntityObject::setOrientation(orientation);
		updateToKeyFrame();
	}

	// 望向某个世界坐标，这个函数会在内部设置世界方位
	void TrackKeyFrameObject::lookAt(const Vector3& targetPoint)
	{
		EntityObject::lookAt(targetPoint);
		updateToKeyFrame();
	}
	// 根据当前缩放值缩放
	void TrackKeyFrameObject::scale(const Vector3& scale)
	{
		EntityObject::scale(scale);
		m_scale *= scale;
		updateToKeyFrame();
	}
	// 根据当前位置移动
	void TrackKeyFrameObject::translate(const Vector3& distance)
	{
		EntityObject::translate(distance);
		updateToKeyFrame();
	}
	// 根据当前方位旋转
	void TrackKeyFrameObject::rotate(const Quaternion& rotate)
	{
		EntityObject::rotate(rotate);
		updateToKeyFrame();
	}

	// 枚举所有属性，以树的形式
	void TrackKeyFrameObject::enumPropertiesTree(VTProperty &properties)const
	{
		EntityObject::enumPropertiesTree(properties);
		properties.front().addChild(Property( ID_KeyFrameLength , PT_REAL , "关键帧长度" , "关键帧长度，单位是秒" , true , true , true));
	}

	// 设置属性
	bool TrackKeyFrameObject::setProperty(uint id , const Any &value)
	{
		switch(id)
		{
		case ID_KeyFrameLength:							// 关键帧长度
			{
				m_length = any_cast<Real>(value);
				updateToKeyFrame();
				return true;
			}
			break;
		default:
			return EntityObject::setProperty(id , value);
			break;
		}
	}
	// 获取属性值
	Any TrackKeyFrameObject::getProperty(uint id)const
	{
		switch(id)
		{
		case ID_KeyFrameLength:							// 关键帧长度
			{
				return Any(m_length);
			}
			break;
		default:
			return EntityObject::getProperty(id);
			break;
		}
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* TrackKeyFrameObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* TrackKeyFrameObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

};