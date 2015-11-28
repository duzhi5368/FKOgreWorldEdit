#pragma once
#include "NullObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API EnvironmentObjectFactory : public ISceneObjectFactory
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

	// 环境对象
	class DISPLAYLAYER_API EnvironmentObject : public NullObject
	{
	public:
		EnvironmentObject(const String &name);
		virtual ~EnvironmentObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return EnvironmentObjectFactory::FACTORY_TYPE;
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

		// 设置是否显示
		virtual void setVisible(bool visible);


		// 获得场景对象的可见对象，用来在编辑器里选择
		virtual ISceneObjectVisible *getSceneObjectVisible();
	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// 环境是否初始化
		bool m_environmentInited;

		// 从场景管理器读取环境设置
		void _getEnvironment();
		// 设置环境
		void _setEnvironment();
		// 禁用环境
		void _disableEnvironment();
		// 设置天空
		void _setSky();
		// 设置雾化
		void _setFog();
		// 设置光照
		void _setLighting();

		// 天空盒设置
		bool m_skyBoxEnabled;					// 是否开启天空盒
		String m_skyBoxMaterialName;			// 天空盒纹理
		Real m_skyBoxRenderDistance;			// 渲染距离

		// 雾化设置
		FogMode m_fogMode;						// 雾化方式
		ColourValue m_fogColor;					// 雾化颜色
		Real m_fogDensity;						// 雾化密度
		Real m_fogStart;						// 雾化开始距离
		Real m_fogEnd;							// 雾化结束距离

		// 光照设置
		ColourValue m_ambientLight;				// 环境光颜色

	};


};