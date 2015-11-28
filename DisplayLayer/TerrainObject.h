#pragma once
#include "NullObject.h"
namespace Ogre
{
	class ETTerrain;
	// 场景物件的对象工厂
	class DISPLAYLAYER_API TerrainObjectFactory : public ISceneObjectFactory
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

	// 地形对象
	class DISPLAYLAYER_API TerrainObject : public NullObject
	{
	public:
		TerrainObject(const String &name);
		virtual ~TerrainObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return TerrainObjectFactory::FACTORY_TYPE;
		}
		ETTerrain *getOgreTerrain()const
		{
			return m_terrain;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();
		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const;


		// 设置属性
		virtual bool setProperty(uint id , const Any &value);
		// 获取属性值
		virtual Any getProperty(uint id)const;
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const;

		// 获得文件名
		const String &getFileName()const
		{
			return m_fileName;
		}
		// 保存地形到文件
		void save(const String &fileName);
	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// 地形
		ETTerrain *m_terrain;

		// 地形文件名
		String m_fileName;
	};


};