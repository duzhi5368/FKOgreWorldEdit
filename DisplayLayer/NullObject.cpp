#include "StdAfx.h"
#include "NullObject.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "Scene.h"
#include "NullObjectVisible.h"
namespace Ogre
{

	const String NullObjectFactory::FACTORY_TYPE = "NullObject";

	// 获得该工厂的类型名称
	const String &NullObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *NullObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		ISceneObject *obj = new NullObject(name);
		obj->create(scene , params);
		return obj;
	}
	// 工厂对象的销毁方法
	void NullObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}


	NullObject::NullObject(const String &name)
		: m_name(name)
		, m_node(0)
		, m_scene(0)
		, m_parent(0)
		, m_visible(true)
		, m_autoDeleteNode(false)
		, m_canCollide(true)
		, m_sceneObjectVisible(0)
	{
	}

	NullObject::~NullObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void NullObject::create(IScene *scene , const NameValuePairList* params)
	{
		m_scene = scene;

		if(params)
		{
			m_creationParams = *params;
			// 如果指定了场景节点名称，就直接用这个节点
			NameValuePairList::const_iterator iter = params->find("isRootNode");
			if(iter != params->end())
			{
				m_node = m_scene->getOgreSceneManager()->getRootSceneNode();
				m_autoDeleteNode = false;
			}
		}
		// 如果没有指定节点名称，就去创建一个，但是暂时不挂接
		if(!m_node)
		{
			m_node = m_scene->getOgreSceneManager()->createSceneNode(getFullName());
			m_autoDeleteNode = true;
		}
	}

	// 销毁对象实体
	void NullObject::destroy()
	{
		OGRE_DELETE(m_sceneObjectVisible);
		if(m_parent)
		{
			m_parent->removeChildObject(this);
		}
		while(m_mapChildObject.size() != 0)
		{
			removeChildObject(m_mapChildObject.begin()->second);
		}
		if(m_node && m_autoDeleteNode)
		{
			m_scene->getOgreSceneManager()->destroySceneNode(m_node->getName());
			m_node = 0;
		}
		m_creationParams.clear();
	}



	// 获得创建他的场景
	IScene *NullObject::getScene()const
	{
		return m_scene;
	}

	// 重命名对象
	bool NullObject::setName(const String &name)
	{
		if(m_scene->hasSceneObject(name))
			return false;
		static_cast<Scene*>(m_scene)->renameSceneObject(m_name , name);
		m_name = name;
		return true;
	}
	// 获得对象名称
	const String &NullObject::getName()const
	{
		return m_name;
	}
	// 获得在Ogre中的对象名称
	String NullObject::getFullName()const
	{
		return m_scene->getName() + m_name;
	}

	// 设置是否显示
	void NullObject::setVisible(bool visible)
	{
		m_node->setVisible(visible);
		m_visible = visible;
	}
	// 是否显示
	bool NullObject::isVisible()const
	{
		return m_visible;
	}

	// 设置相对坐标
	void NullObject::setPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos);
	}

	// 获得相对坐标
	const Vector3 &NullObject::getPosition()const
	{
		return m_node->getPosition();
	}

	// 设置世界坐标
	void NullObject::setWorldPosition(const Vector3 &pos)
	{
		m_node->setPosition(pos - m_node->getWorldPosition() + m_node->getPosition());
	}

	// 获得世界坐标
	const Vector3 &NullObject::getWorldPosition()const
	{
		return m_node->getWorldPosition();
	}


	// 设置相对缩放
	void NullObject::setScale(const Vector3 &scale)
	{
		m_node->setScale(scale);
	}

	// 获得相对坐标
	const Vector3 &NullObject::getScale()const
	{
		return m_node->getScale();
	}

	// 设置世界缩放
	void NullObject::setWorldScale(const Vector3 &scale)
	{
		const Vector3 &worldScale = m_node->_getDerivedScale();
		const Vector3 &localScale = m_node->getScale();
		Vector3 scaleResult;
		if(worldScale.x == 0.0f)
			scaleResult.x = 0.0f;
		else
			scaleResult.x = scale.x / worldScale.x * localScale.x;
		if(worldScale.y == 0.0f)
			scaleResult.y = 0.0f;
		else
			scaleResult.y = scale.y / worldScale.y * localScale.y;
		if(worldScale.z == 0.0f)
			scaleResult.z = 0.0f;
		else
			scaleResult.z = scale.z / worldScale.z * localScale.z;

		m_node->setScale(scaleResult);
	}

	// 获得世界缩放
	const Vector3 &NullObject::getWorldScale()const
	{
		return m_node->_getDerivedScale();
	}


	// 设置相对方位
	void NullObject::setOrientation(const Quaternion &orientation)
	{
		m_node->setOrientation(orientation);
	}

	// 获得相对坐标
	const Quaternion &NullObject::getOrientation()const
	{
		return m_node->getOrientation();
	}

	// 设置世界方位
	void NullObject::setWorldOrientation(const Quaternion &orientation)
	{
		// 先用世界旋转的逆乘以orientation，得到的结果是从世界旋转到orientation的角旋转偏移
		m_node->setOrientation(m_node->getOrientation() * (m_node->getWorldOrientation().Inverse() * orientation));
	}

	// 获得世界方位
	const Quaternion &NullObject::getWorldOrientation()const
	{
		return m_node->getWorldOrientation();
	}


	// 设置父对象
	void NullObject::setParent(ISceneObject *parent)
	{
		removeFromParentObject();
		parent->addChildObject(this);
	}
	// 获得父对象
	ISceneObject* NullObject::getParent()const
	{
		return m_parent;
	}


	// 望向某个世界坐标，这个函数会在内部设置世界方位
	void NullObject::lookAt(const Vector3& targetPoint)
	{
		m_node->lookAt(targetPoint , Node::TS_WORLD);
	}

	// 根据当前缩放值缩放
	void NullObject::scale(const Vector3& scale)
	{
		m_node->scale(scale);
	}

	// 根据当前位置移动
	void NullObject::translate(const Vector3& distance)
	{
		m_node->translate(distance);
	}

	// 根据当前方位旋转
	void NullObject::rotate(const Quaternion& rotate)
	{
		m_node->rotate(rotate);
	}

	// 把一个场景物件挂接到这里
	void NullObject::addChildObject(ISceneObject *obj)
	{
		if(obj->getParent())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"场景对象 '" + obj->getName() + "'已挂接在'" + obj->getParent()->getName() + "'上", 
				"NullObject::addChildObject");
			return;
		}
		m_mapChildObject[obj->getName()] = obj;

		SceneNode *node = static_cast<NullObject*>(obj)->getOgreSceneNode();
		if(node->getParentSceneNode())
		{
			node->getParentSceneNode()->removeChild(node);
		}
		m_node->addChild(node);
		static_cast<NullObject*>(obj)->m_parent = this;
	}
	// 把一个场景物件从到这里取消挂接
	void NullObject::removeChildObject(ISceneObject *obj)
	{
		if(!hasChildObject(obj->getName()))
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"场景对象 '" + obj->getName() + "'挂接在'" + obj->getParent()->getName() + "'上，但不是挂接在本节点'" + getName() + "'上", 
				"NullObject::removeChildObject");
			return;
		}
		m_mapChildObject.erase(m_mapChildObject.find(obj->getName()));

		SceneNode *node = static_cast<NullObject*>(obj)->getOgreSceneNode();
		if(node->getParentSceneNode())
		{
			node->getParentSceneNode()->removeChild(node);
		}
		static_cast<NullObject*>(obj)->m_parent = 0;
	}

	// 把这个场景物件从父场景附件取消挂接
	void NullObject::removeFromParentObject()
	{
		if(m_parent)
		{
			m_parent->removeChildObject(this);
			m_parent = 0;
		}
	}

	// 获得已附加的场景物件枚举
	ISceneObject::ChildObjectIterator NullObject::getChildObjectIterator()
	{
		return ChildObjectIterator(m_mapChildObject.begin() , m_mapChildObject.end());
	}

	// 根据类型和名称创建一个此场景对象的子场景对象
	ISceneObject *NullObject::createChildObject(const String &name , const String &type, const NameValuePairList* params)
	{
		ISceneObject *obj = m_scene->createSceneObject(name , type , params);
		addChildObject(obj);
		return obj;
	}

	// 根据类型和名称创建一个此场景对象的子场景对象
	ISceneObject *NullObject::createChildObject(const String &type, const NameValuePairList* params)
	{
		ISceneObject *obj = m_scene->createSceneObject(type , params);
		addChildObject(obj);
		return obj;
	}

	// 销毁一个此场景对象的子场景对象
	void NullObject::destroyChildObject(const String &name)
	{
		MapChildObject::iterator iter = m_mapChildObject.find(name);
		if(iter == m_mapChildObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"找不到名称为 '" + name + "' 的子场景对象", 
				"NullObject::destroyChildObject");
			return;
		}
		m_scene->destroySceneObject(name);
	}

	void NullObject::destroyChildObject(ISceneObject *obj)
	{
		destroyChildObject(obj->getName());
	}

	// 获得一个此场景对象的子场景对象
	ISceneObject *NullObject::getChildObject(const String &name)
	{
		MapChildObject::iterator iter = m_mapChildObject.find(name);
		if(iter == m_mapChildObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"找不到名称为 '" + name + "' 的子场景对象", 
				"NullObject::getChildObject");
			return 0;
		}
		return iter->second;
	}

	// 是否存在指定名字的此场景对象的子场景对象
	bool NullObject::hasChildObject(const String &name)const
	{
		return m_mapChildObject.find(name) != m_mapChildObject.end();
	}
	// 用newName作为名称克隆一个新的对象
	ISceneObject *NullObject::clone(const String &newName , IScene *newScene)
	{
		ISceneObject *obj = (newScene ? newScene : m_scene)->createSceneObject(newName , getType() , &m_creationParams);
		cloneData(obj);
		return obj;
	}
	// 用自动生成的名称作为名称克隆一个新的对象
	ISceneObject *NullObject::clone(IScene *newScene)
	{
		return clone((newScene ? newScene : m_scene)->getUniqueName() , newScene);
	}

	// 把当前对象中的数据克隆到target中
	void NullObject::cloneData(ISceneObject *target)
	{
		if(target->getType() != getType())
		{
			OGRE_EXCEPT(Exception::ERR_INVALID_STATE, 
				"target对象类型不一致 '" + target->getType() + "' 不是当前对象的类型 '" + getType() + "'", 
				"NullObject::cloneData");
			return;
		}
		cloneProperties(target);
	}

	// 获得所有属性，以树的形式
	const PropertyContainer::VTProperty* NullObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// 获得所有属性，以列表的形式
	const PropertyContainer::MapProperty* NullObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// 枚举所有属性，以树的形式
	void NullObject::enumPropertiesTree(VTProperty &properties)const
	{
		properties.push_back(Property( ID_General , PT_GROUP , "一般设置" , "一般设置" , true , false , false));
		properties.back().addChild(Property(ID_Type , PT_STRING , "类型" , "类型" , true , false , false));
		properties.back().addChild(Property(ID_Name , PT_STRING , "名称" , "名称" , true , true , false));
		properties.back().addChild(Property(ID_Position , PT_VECTOR3 , "位置" , "位置" , true , true , true));
		properties.back().addChild(Property(ID_Rotate , PT_VECTOR3 , "旋转" , "旋转" , true , true , true));
		properties.back().addChild(Property(ID_Scale , PT_VECTOR3 , "缩放" , "缩放" , true , true , true));
		properties.back().addChild(Property(ID_Visible , PT_BOOL , "可见" , "节点是否可见" , true , true , true));
		properties.back().addChild(Property(ID_Parent , PT_STRING , "父节点" , "父节点" , false , false , true));
		properties.back().addChild(Property(ID_CanCollide , PT_BOOL , "碰撞计算" , "碰撞计算" , true , true , true));
	}

	// 设置是否参加碰撞
	void NullObject::setCanCollide(bool can)
	{
		m_canCollide = can;
	}

	// 设置是否参加碰撞
	bool NullObject::getCanCollide()const
	{
		return m_canCollide;
	}

	// 设置属性
	bool NullObject::setProperty(uint id , const Any &value)
	{
		switch(id)
		{
		case ID_General:		// 一般设置
			{
				return true;
			}
			break;
		case ID_Name:	// 名称
			{
				setName(any_cast<String>(value));
				return true;
			}
			break;
		case ID_Position:	// 位置
			{
				setPosition(any_cast<Vector3>(value));
				return true;
			}
			break;
		case ID_Rotate:		// 旋转
			{
				Vector3 rotate(any_cast<Vector3>(value));
				rotate.x = Math::DegreesToRadians(rotate.x);
				rotate.y = Math::DegreesToRadians(rotate.y);
				rotate.z = Math::DegreesToRadians(rotate.z);
				setOrientation(MathEx::EularToQuaternion(rotate));
				return true;
			}
			break;
		case ID_Scale:		// 缩放
			{
				setScale(any_cast<Vector3>(value));
				return true;
			}
			break;
		case ID_Visible:		// 可见
			{
				setVisible(any_cast<bool>(value));
				return true;
			}
			break;
		case ID_Parent:		// 父节点
			{
				String parentName(any_cast<String>(value));
				if(parentName.empty())
					removeFromParentObject();
				else
					setParent(m_scene->getSceneObject(parentName));
				return true;
			}
			break;
		case ID_CanCollide:		// 碰撞计算
			{
				m_canCollide = any_cast<bool>(value);
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// 获取属性值
	Any NullObject::getProperty(uint id)const
	{
		switch(id)
		{
		case ID_General:		// 一般设置
			{
				return Any();
			}
			break;
		case ID_Name:		// 名称
			{
				return Any(getName());
			}
			break;
		case ID_Type:		// 位置
			{
				return Any(getType());
			}
			break;
		case ID_Position:	// 位置
			{
				return Any(getPosition());
			}
			break;
		case ID_Rotate:		// 旋转
			{
				Vector3 rotate(MathEx::QuaternionToEular(getOrientation()));
				rotate.x = Math::RadiansToDegrees(rotate.x);
				rotate.y = Math::RadiansToDegrees(rotate.y);
				rotate.z = Math::RadiansToDegrees(rotate.z);
				return Any(rotate);
			}
			break;
		case ID_Scale:		// 缩放
			{
				return Any(getScale());
			}
			break;
		case ID_Visible:		// 可见
			{
				return Any(isVisible());
			}
			break;
		case ID_Parent:		// 父节点
			{
				return Any(m_parent ? m_parent->getName() : StringUtil::BLANK);
			}
			break;
		case ID_CanCollide:		// 碰撞计算
			{
				return Any(m_canCollide);
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// 获得场景对象的可见对象，用来在编辑器里选择
	ISceneObjectVisible *NullObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new NullObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

};