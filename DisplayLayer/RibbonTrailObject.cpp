#include "StdAfx.h"
#include "RibbonTrailObject.h"
#include "IScene.h"
#include "EntityObject.h"
#include "OgreTagPoint.h"
namespace Ogre
{

	const String RibbonTrailObjectFactory::FACTORY_TYPE = "RibbonTrailObject";

	// 获得该工厂的类型名称
	const String &RibbonTrailObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// 工厂对象的创建方法
	ISceneObject *RibbonTrailObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		RibbonTrailObject *obj = new RibbonTrailObject(name);
		obj->create(scene , params);
		return obj;
	}

	// 工厂对象的销毁方法
	void RibbonTrailObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	RibbonTrailObject::RibbonTrailObject(const String &name)
		: NullObject(name)
		, m_ribbonTrail(0)
	{
	}

	RibbonTrailObject::~RibbonTrailObject(void)
	{
		destroy();
	}

	// 创建对象实体
	void RibbonTrailObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// 创建实体
		m_ribbonTrail = static_cast<RibbonTrail*>(m_scene->getOgreSceneManager()->
			createMovableObject(getFullName(), RibbonTrailFactory::FACTORY_TYPE_NAME, params));
		m_ribbonTrail->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// 挂接上去
		m_node->attachObject(m_ribbonTrail);
	}

	// 销毁对象实体
	void RibbonTrailObject::destroy()
	{
		if(m_ribbonTrail)
		{
			// 销毁模型实体
			m_scene->getOgreSceneManager()->destroyMovableObject(m_ribbonTrail);
			m_ribbonTrail = 0;
		}
		NullObject::destroy();
	}

	// 用条带跟踪这个场景对象，返回chainIndex
	size_t RibbonTrailObject::trail(ISceneObject *obj)
	{
		ListTrailObject::iterator iter = std::find(m_listTrailObject.begin() , m_listTrailObject.end() , obj);
		if(iter != m_listTrailObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"名称为 '" + obj->getName() + "的场景对象已经被跟踪", 
				"RibbonTrailObject::trail");
			return -1;
		}

		// 添加到条带链
		m_ribbonTrail->addNode(static_cast<NullObject*>(obj)->getOgreSceneNode());
		// 添加到被跟踪列表
		m_listTrailObject.push_back(TrailObject(obj));
		return m_listTrailObject.size() - 1;
	}

	size_t RibbonTrailObject::trail(const String &objName)
	{
		return trail(m_scene->getSceneObject(objName));
	}

	// 停止跟踪
	void RibbonTrailObject::stopTrail(ISceneObject *obj)
	{
		ListTrailObject::iterator iter = std::find(m_listTrailObject.begin() , m_listTrailObject.end() , obj);
		if(iter == m_listTrailObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"找不到名称为 '" + obj->getName() + "的场景对象", 
				"RibbonTrailObject::stopTrail");
			return;
		}
		m_ribbonTrail->removeNode(static_cast<NullObject*>(obj)->getOgreSceneNode());
		m_listTrailObject.erase(iter);
	}

	void RibbonTrailObject::stopTrail(const String &objName)
	{
		stopTrail(m_scene->getSceneObject(objName));
	}

	void RibbonTrailObject::stopTrail(size_t chainIndex)
	{
		if(chainIndex >= m_listTrailObject.size())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"索引越界，索引值为 '" + StringConverter::toString(chainIndex) + "，而数组大小为" + StringConverter::toString(m_listTrailObject.size()), 
				"RibbonTrailObject::stopTrail");
			return;
		}
		const TrailObject &trail = m_listTrailObject[chainIndex];
		if(trail.boneName.empty())
		{
			stopTrail(trail.object);
		}
		else
		{
			stopTrail(trail.entity , trail.boneName);
		}
	}


	// 用条带跟踪这个骨骼，返回chainIndex
	size_t RibbonTrailObject::trail(EntityObject *obj , const String &boneName)
	{
		SkeletonInstance* skeleton = obj->getOgreEntity()->getSkeleton();
		if(skeleton == 0)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景对象 '" + obj->getName() + "'没有骨骼",
				"RibbonTrailObject::trail");
			return -1;
		}

		// 检查重复跟踪
		for(ListTrailObject::iterator iter = m_listTrailObject.begin() ; iter != m_listTrailObject.end() ; iter ++)
		{
			if(iter->entity == obj
				&& iter->boneName == boneName
				&& iter->boneTag != 0
				)
			{
				OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
					"名称为 '" + obj->getName() + "'的场景对象的骨骼'" + boneName + "'已经被跟踪", 
					"RibbonTrailObject::trail");
				return -1;
			}
		}

		TagPoint* tag = skeleton->createTagPointOnBone(skeleton->getBone(boneName));
		tag->setParentEntity(obj->getOgreEntity());
		tag->setChildObject(m_ribbonTrail);
		m_ribbonTrail->addNode(tag);

		// 添加到被跟踪列表
		m_listTrailObject.push_back(TrailObject(obj , boneName , tag));
		return m_listTrailObject.size() - 1;
	}

	size_t RibbonTrailObject::trail(const String &entityObjName , const String &boneName)
	{
		return trail(static_cast<EntityObject*>(m_scene->getSceneObject(entityObjName)) , boneName);
	}

	// 停止跟踪
	void RibbonTrailObject::stopTrail(EntityObject *obj , const String &boneName)
	{
		// 找到被跟踪的骨头
		ListTrailObject::iterator iterFound;
		for(ListTrailObject::iterator iter = m_listTrailObject.begin() ; iter != m_listTrailObject.end() ; iter ++)
		{
			if(iter->entity == obj
				&& iter->boneName == boneName
				&& iter->boneTag != 0
				)
			{
				iterFound = iter;
				break;
			}
		}
		if(iterFound == m_listTrailObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"名称为 '" + obj->getName() + "'的场景对象的骨骼'" + boneName + "'没有被跟踪", 
				"RibbonTrailObject::stopTrail");
			return;
		}
		m_ribbonTrail->removeNode(iterFound->boneTag);
		obj->getOgreEntity()->getSkeleton()->freeTagPoint(iterFound->boneTag);
		m_listTrailObject.erase(iterFound);
	}

	void RibbonTrailObject::stopTrail(const String &entityObjName , const String &boneName)
	{
		return stopTrail(static_cast<EntityObject*>(m_scene->getSceneObject(entityObjName)) , boneName);
	}

	// 设置条带初始颜色
	void RibbonTrailObject::setInitialColour(size_t chainIndex, const ColourValue& col)
	{
		m_ribbonTrail->setInitialColour(chainIndex , col);
	}

	const ColourValue& RibbonTrailObject::getInitialColour(size_t chainIndex) const
	{
		return m_ribbonTrail->getInitialColour(chainIndex);
	}


	// 设置条带颜色每秒减去的颜色
	void RibbonTrailObject::setColourChange(size_t chainIndex, const ColourValue& valuePerSecond)
	{
		m_ribbonTrail->setColourChange(chainIndex , valuePerSecond);
	}

	const ColourValue& RibbonTrailObject::getColourChange(size_t chainIndex) const
	{
		return m_ribbonTrail->getColourChange(chainIndex);
	}


	// 设置条带默认宽度
	void RibbonTrailObject::setInitialWidth(size_t chainIndex, float width)
	{
		m_ribbonTrail->setInitialWidth(chainIndex , width);
	}

	float RibbonTrailObject::getInitialWidth(size_t chainIndex) const
	{
		return m_ribbonTrail->getInitialWidth(chainIndex);
	}

	// 设置条带每秒减去的宽度
	void RibbonTrailObject::setWidthChange(size_t chainIndex, float widthDeltaPerSecond)
	{
		m_ribbonTrail->setInitialWidth(chainIndex , widthDeltaPerSecond);
	}

	float RibbonTrailObject::getWidthChange(size_t chainIndex) const
	{
		return m_ribbonTrail->getWidthChange(chainIndex);
	}

	// 设置条带的长度
	void RibbonTrailObject::setTrailLength(float len)
	{
		m_ribbonTrail->setTrailLength(len);
	}

	float RibbonTrailObject::getTrailLength(void) const
	{
		return m_ribbonTrail->getTrailLength();
	}

	// 设置条带的材质
	void RibbonTrailObject::setMaterialName(const String &name)
	{
		m_ribbonTrail->setMaterialName(name);
	}

	const String &RibbonTrailObject::getMaterialName(void) const
	{
		return m_ribbonTrail->getMaterialName();
	}

};