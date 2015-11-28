#include "StdAfx.h"
#include "BillboardSetObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
namespace Ogre
{

	const String BillboardSetObjectFactory::FACTORY_TYPE = "BillboardSetObject";

	// 获得该工厂的类型名称
	const String &BillboardSetObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *BillboardSetObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		BillboardSetObject *obj = new BillboardSetObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void BillboardSetObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	BillboardSetObject::BillboardSetObject(const String &name)
		: NullObject(name)
		, m_billboardSet(0)
	{
	}

	BillboardSetObject::~BillboardSetObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void BillboardSetObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// 创建实体
		m_billboardSet = static_cast<BillboardSet*>(m_scene->getOgreSceneManager()->createBillboardSet(getFullName()));
		m_billboardSet->setUserAny(Any(static_cast<ISceneObject*>(this)));
		m_billboardSet->setListener(this);
		// 挂接上去
		m_node->attachObject(m_billboardSet);
	}

	// 销毁对象实体
	void BillboardSetObject::destroy()
	{
		if(m_billboardSet)
		{
			// 销毁模型实体
			m_scene->getOgreSceneManager()->destroyBillboardSet(m_billboardSet);
			m_billboardSet = 0;
		}
		NullObject::destroy();
	}
	// 根据摄像机方向改变billboard的纹理
	bool BillboardSetObject::objectRendering(const MovableObject *mo, const Camera *camera)
	{
		const Quaternion &camQ = m_billboardSet->getCamQ();
		Vector3 eular(MathEx::QuaternionToEular(camQ));
		

		return true;
	}


	// 枚举所有属性，以树的形式
	void BillboardSetObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
	}

	// 设置属性
	bool BillboardSetObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);

		return false;
	}
	// 获取属性值
	Any BillboardSetObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		return Any();
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* BillboardSetObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* BillboardSetObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *BillboardSetObject::getSceneObjectVisible()
	{
		//if(!m_sceneObjectVisible)
		//{
		//	m_sceneObjectVisible = new BillboardSetObjectVisible(this);
		//}
		return 0;
	}

};