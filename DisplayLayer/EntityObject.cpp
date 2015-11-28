#include "StdAfx.h"
#include "EntityObject.h"
#include "IScene.h"
#include "EntityMaterialInstance.h"
#include "EntityObjectVisible.h"
namespace Ogre
{

	const String EntityObjectFactory::FACTORY_TYPE = "EntityObject";

	// 获得该工厂的类型名称
	const String &EntityObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *EntityObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		EntityObject *obj = new EntityObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void EntityObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	EntityObject::EntityObject(const String &name)
		: NullObject(name)
		, m_entity(0)
		, m_alpha(1.0f)
		, m_alphaController(0)
		, m_collisionEntity(0)
		, m_collisionType(CT_NONE)
	{
	}

	EntityObject::~EntityObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void EntityObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		if(params && params->find("mesh") != params->end())
		{
			setMeshFile(params->find("mesh")->second);
		}
	}

	// 销毁对象实体
	void EntityObject::destroy()
	{
		destroyMesh();
		NullObject::destroy();
	}

	// 帧侦听器的每帧执行
	bool EntityObject::frameStarted( const FrameEvent& evt )
	{
		// TODO:将来要优化，不能用frameStarted每帧都去更新动画状态
		// 因为当摄像机看不到这个Object的时候就没必要更新骨骼动画了。
		// 更新动画控制器
		if(!m_animationController.isNull())
		{
			m_animationController.update(evt.timeSinceLastFrame);
		}
		return true;
	}

	// 设置是否显示
	void EntityObject::setVisible(bool visible)
	{
		NullObject::setVisible(visible);
		if(m_sceneObjectVisible)
		{
			m_sceneObjectVisible->showBorder(m_sceneObjectVisible->isShowBorder());
		}
	}
	// 设置碰撞模型类型
	void EntityObject::setCollisionShapeType(CollisionShapeType type)
	{
		m_collisionEntity->setCollisionShapeType(type);
	}

	// 设置碰撞模型
	void EntityObject::setCollisionShapeName(const String &name)
	{
		m_collisionEntity->setCollisionShapeName(name);
	}

	// 设置透明度
	void EntityObject::setAlpha(float alpha)
	{
		m_alpha = std::max(std::min(alpha , 1.0f) , 0.0f);
		if(m_alpha == 1.0f)
		{
			if(m_alphaController)
			{
				m_alphaController->setTransparency(1.0f);
				OGRE_DELETE(m_alphaController);
			}
			return;
		}
		if(m_alphaController == 0)
		{
			m_alphaController = new EntityMaterialInstance(m_entity);
		}
		// 设置透明度
		m_alphaController->setTransparency(m_alpha);
	}
	// 获得透明度
	float EntityObject::getAlpha()const
	{
		return m_alpha;
	}

	// 把一个场景物件挂接到骨头上
	void EntityObject::attachObjectToBone(const String &boneName,
		ISceneObject *obj,
		const Quaternion &offsetOrientation,
		const Vector3 &offsetPosition)
	{
		if(obj->getParent())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"场景对象 '" + obj->getName() + "'已挂接在'" + obj->getParent()->getName() + "'上", 
				"EntityObject::attachObjectToBone");
		}
		// 从节点解除挂接
		static_cast<NullObject*>(obj)->getOgreSceneNode()->detachObject(obj->getOgreMovableObject());
		// 挂接到骨头
		TagPoint* tagPoint = m_entity->attachObjectToBone(boneName
			, obj->getOgreMovableObject()
			, offsetOrientation
			, offsetPosition);
		// 保存到列表
		m_childObjectList[obj->getName()] = obj;
	}
	// 把一个场景物件从骨头取消挂接
	void EntityObject::detachObjectFromBone(ISceneObject *obj)
	{
		detachObjectFromBone(obj->getName());
	}
	void EntityObject::detachObjectFromBone(const String &objName)
	{
		ChildObjectList::iterator iter = m_childObjectList.find(objName);
		if(iter == m_childObjectList.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"场景对象 '" + objName + "'没挂接在'" + getName() + "'上", 
				"EntityObject::detachObjectFromBone");
		}
		m_entity->detachObjectFromBone(objName);
		// 挂接回自己的节点
		static_cast<NullObject*>(iter->second)->getOgreSceneNode()->attachObject(iter->second->getOgreMovableObject());
		m_childObjectList.erase(iter);
	}
	// 把所有场景物件从骨头取消挂接
	void EntityObject::detachAllObjectsFromBone()
	{
		while(m_childObjectList.size() != 0)
		{
			detachObjectFromBone(m_childObjectList.begin()->first);
		}
	}
	// 获得已挂接到骨头的场景物件
	EntityObject::ChildObjectListIterator EntityObject::getAttachedObjectIterator(void)
	{
		return ChildObjectListIterator(m_childObjectList.begin() , m_childObjectList.end());
	}

	// 使用这个Mesh来重构Entity
	bool EntityObject::setMeshFile(const String &meshFileName)
	{
		if(m_meshFileName == meshFileName)
			return true;
		destroyMesh();
		// 创建实体
		m_entity = m_scene->getOgreSceneManager()->createEntity(getFullName() , meshFileName);
		
		m_entity->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// 挂接上去
		m_node->attachObject(m_entity);
		// 创建动画控制器
		m_animationController.create(m_entity);
		Root::getSingleton().addFrameListener(this);
		m_meshFileName = meshFileName;

		if(m_scene->getCollisionWorld())
		{
			m_collisionEntity = m_scene->getCollisionWorld()->createEntityCollisionEntity(m_entity);
		}

		return true;
	}
	// 获得Mesh文件名
	const String &EntityObject::getMeshFile()const
	{
		return m_meshFileName;
	}
	// 销毁mesh
	void EntityObject::destroyMesh()
	{
		OGRE_DELETE(m_sceneObjectVisible);
		// 销毁碰撞实体
		if(m_collisionEntity)
		{
			m_scene->getCollisionWorld()->destroyCollisionEntity(m_collisionEntity);
			m_collisionEntity = 0;
		}
		// 销毁模型实体
		if(m_entity)
		{
			detachAllObjectsFromBone();
			Root::getSingleton().removeFrameListener(this);
			// 销毁透明控制器
			OGRE_DELETE(m_alphaController);
			// 销毁动画控制器
			m_animationController.destroy();
			// 销毁模型实体
			m_scene->getOgreSceneManager()->destroyEntity(m_entity);
			m_entity = 0;
			m_meshFileName.clear();
		}
	}


	// 枚举所有属性，以树的形式
	void EntityObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Entity , PT_GROUP , "模型设置" , "模型设置" , true , false , false));
		properties.back().addChild(Property(ID_MeshFile , PT_STRING , "模型文件" , "模型文件" , true , true , true));
		properties.back().addChild(Property(ID_Alpha , PT_REAL , "透明度" , "透明度" , true , true , true));
		properties.back().addChild(Property(ID_CollisionShapeType , PT_UINT , "碰撞模型类型" , "碰撞模型类型" , true , true , true));
		properties.back().addChild(Property(ID_CollisionShapeName , PT_STRING , "碰撞模型名称" , "碰撞模型名称" , true , true , true));
	}

	// 设置属性
	bool EntityObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Entity:		// 一般设置
			{
				return true;
			}
			break;
		case ID_MeshFile:	// 模型文件
			{
				setMeshFile(any_cast<String>(value));
				return true;
			}
			break;
		case ID_Alpha:	// 透明度
			{
				setAlpha(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_CollisionShapeType:	// 碰撞模型类型
			{
				setCollisionShapeType((CollisionShapeType)any_cast<uint>(value));
				return true;
			}
			break;
		case ID_CollisionShapeName:	// 碰撞模型名称
			{
				setCollisionShapeName(any_cast<String>(value));
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any EntityObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Entity:		// 模型设置
			{
				return Any();
			}
			break;
		case ID_MeshFile:	// 模型文件
			{
				return Any(getMeshFile());
			}
			break;
		case ID_Alpha:		// 透明度
			{
				return Any((Real)getAlpha());
			}
			break;
		case ID_CollisionShapeType:	// 碰撞模型类型
			{
				return Any((uint)getCollisionShapeType());
			}
			break;
		case ID_CollisionShapeName:	// 碰撞模型名称
			{
				return Any(getCollisionShapeName());
			}
			break;
	default:
			return Any();
			break;
		}
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* EntityObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* EntityObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}


	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *EntityObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new EntityObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
};
