#pragma once
#include "TriggerObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class TrackKeyFrameObjectFactory : public ISceneObjectFactory
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

	// 轨迹关键帧对象
	class TrackKeyFrameObject : public EntityObject
	{
	public:
		TrackKeyFrameObject(const String &name);
		virtual ~TrackKeyFrameObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return TrackKeyFrameObjectFactory::FACTORY_TYPE;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();

		// 设置属性
		virtual bool setProperty(uint id , const Any &value);
		// 获取属性值
		virtual Any getProperty(uint id)const;
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const;



		// 设置相对坐标
		virtual void setPosition(const Vector3 &pos);

		// 设置相对缩放
		virtual void setScale(const Vector3 &scale);
		// 获得相对坐标
		virtual const Vector3 &getScale()const;

		// 设置相对方位
		virtual void setOrientation(const Quaternion &orientation);

		// 望向某个世界坐标，这个函数会在内部设置世界方位
		virtual void lookAt(const Vector3& targetPoint);
		// 根据当前缩放值缩放
		virtual void scale(const Vector3& scale);
		// 根据当前位置移动
		virtual void translate(const Vector3& distance);
		// 根据当前方位旋转
		virtual void rotate(const Quaternion& rotate);

		// 把数据更新到关键帧动画里去
		void updateToKeyFrame();
		// 把数据从关键帧动画里更新到这里
		void updateFromKeyFrame();
	protected:
		// 动画长度
		float m_length;
		// 缩放比率
		Vector3 m_scale;
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
	};


};