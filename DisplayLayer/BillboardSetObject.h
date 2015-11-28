#pragma once
#include "NullObject.h"
namespace Ogre
{
	// 场景物件的对象工厂
	class DISPLAYLAYER_API BillboardSetObjectFactory : public ISceneObjectFactory
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

	// 公告板对象
	class DISPLAYLAYER_API BillboardSetObject : public NullObject
		, private MovableObject::Listener
	{
	public:
		BillboardSetObject(const String &name);
		virtual ~BillboardSetObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return BillboardSetObjectFactory::FACTORY_TYPE;
		}
		BillboardSet *getOgreBillboardSet()const
		{
			return m_billboardSet;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();
		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_billboardSet;
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
	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// Ogre实体
		BillboardSet *m_billboardSet;

		// 旋转信息，表示从一个角度夹角中使用一个指定的纹理
		struct RotateInfo
		{
			// 开始角度
			Radian m_begin;
			// 结束角度
			Radian m_end;
			// 使用这个材质
			String m_materialName;
			// 纹理坐标
			FloatRect m_textureCoord;
		};
		typedef std::vector<RotateInfo> ListRotateInfo;
		ListRotateInfo m_listRotateInfo;
	private:
		// 根据摄像机方向改变billboard的纹理
		virtual bool objectRendering(const MovableObject *mo, const Camera *camera);
	};


};