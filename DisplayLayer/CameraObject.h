#pragma once
#include "NullObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API CameraObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API CameraObject : public NullObject
	{
	public:
		CameraObject(const String &name);
		virtual ~CameraObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return CameraObjectFactory::FACTORY_TYPE;
		}
		Camera *getOgreCamera()const
		{
			return m_camera;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();
		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_camera;
		}

		// 鼠标点击测试
		// @x 鼠标x屏幕坐标（0~1）
		// @y 鼠标y屏幕坐标（0~1）
		// @pos 点击相交的坐标
		// @return 是否相交了
		virtual bool hitTest(float x , float y , Vector3 &pos);


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
	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// Ogre实体
		Camera *m_camera;
	};


};