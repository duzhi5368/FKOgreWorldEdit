#include "StdAfx.h"
#include "OsmSceneSerializer.h"
#include "IScene.h"
#include "EntityObject.h"
#include "ParticleObject.h"
#include "RibbonTrailObject.h"
#include "IAnimationStateController.h"
namespace Ogre
{
	OsmSceneSerializer::OsmSceneSerializer(void)
		: m_scene(0)
	{
	}

	OsmSceneSerializer::~OsmSceneSerializer(void)
	{
	}

	// 导入场景
	bool OsmSceneSerializer::importScene(DataStreamPtr& stream, IScene* scene)
	{
		mXMLDoc.bind(new TiXmlDocument());

		m_scene = scene;
		m_sceneMgr = m_scene->getOgreSceneManager();

		// 分析XML文件
		size_t iSize = stream->size();
		char *pBuf = new char[iSize+1];
		memset(pBuf, 0, iSize+1);
		stream->read(pBuf, iSize);
		stream.setNull();
		mXMLDoc->Parse(pBuf);
		OGRE_DELETE_ARRAY(pBuf);

		// 检查XML文件是否读取成功
		if(mXMLDoc->Error())
		{
			mXMLDoc.setNull();

			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"导入场景失败 : " + scene->getOrigin(),
				"OsmSceneSerializer::importScene");
			return false;
		}
		// 读取整个场景
		readScene(mXMLDoc->RootElement());
		return true;
	}
	// 导出场景
	bool OsmSceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		m_scene = scene;
		m_sceneMgr = m_scene->getOgreSceneManager();
		return true;
	}
	// 获得颜色属性，对应xml节点里的r、g、b属性，0~1表示最暗和最亮
	ColourValue OsmSceneSerializer::getColorAttribute(TiXmlElement *element)
	{
		return ColourValue(StringConverter::parseReal(getAttribute(element , "r"))
			, StringConverter::parseReal(getAttribute(element , "g"))
			, StringConverter::parseReal(getAttribute(element , "b"))
			, element->Attribute("a") ? StringConverter::parseReal(getAttribute(element , "a")) : 1.0f
			);
	}
	// 获得Vector3，对应xml节点的x/y/z属性
	Vector3 OsmSceneSerializer::getVector3Attribute(TiXmlElement *element)
	{
		return Vector3(StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// 获得Quatation，对应xml节点的x/y/z/w属性
	Quaternion OsmSceneSerializer::getQuaternionAttribute(TiXmlElement *element)
	{
		return Quaternion(StringConverter::parseReal(getAttribute(element , "w"))
			, StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// 获得字符串属性
	String OsmSceneSerializer::getAttribute(TiXmlElement *element , const char *name)
	{
		if(!element)
			return StringUtil::BLANK;
		const char *value = element->Attribute(name);
		if(!value)
			return StringUtil::BLANK;
		else
			return value;
	}
	// 读取场景设置
	void OsmSceneSerializer::readScene(TiXmlElement *parentElement)
	{
		TiXmlElement *element;
		// 环境光
		element = parentElement->FirstChildElement("lightColor");
		m_sceneMgr->setAmbientLight(getColorAttribute(element));
		// 所有Entity实体
		element = parentElement->FirstChildElement("entities");
		if(element)
		{
			readEntities(element);
		}
		// 所有Particle实体
		element = parentElement->FirstChildElement("particles");
		if(element)
		{
			readParticles(element);
		}
		// 所有RibbonTrail实体
		element = parentElement->FirstChildElement("ribbonTrails");
		if(element)
		{
			readRibbonTrails(element);
		}
		// 自动播放动画
		if(m_scene->hasAnimationStateController("autoplay"))
		{
			IAnimationStateController* ani = m_scene->getAnimationStateController("autoplay");
			if(ani)
			{
				ani->play();
			}
		}
	}
	// 读取NullObject
	void OsmSceneSerializer::readNullObject(NullObject *obj , TiXmlElement *parentElement)
	{
		// 设置是否可见
		obj->setVisible(!StringConverter::parseBool(getAttribute(parentElement , "hidden")));
		// 位置
		obj->setPosition(getVector3Attribute(parentElement->FirstChildElement("position")));
		// 旋转
		obj->setOrientation(getQuaternionAttribute(parentElement->FirstChildElement("rotation")));
		// 缩放
		obj->setScale(getVector3Attribute(parentElement->FirstChildElement("scale")));
		// 帧动画
		TiXmlElement *element = parentElement->FirstChildElement("animations");
		if(element)
		{
			readAnimations(obj , element);
		}

	}

	// 读取Entities
	void OsmSceneSerializer::readEntities(TiXmlElement *parentElement)
	{
		// 遍历所有Entity
		for (TiXmlElement* element = parentElement->FirstChildElement("entity");
			element != 0; element = element->NextSiblingElement("entity"))
		{
			readEntity(element);
		}
	}
	// 读取Entity
	void OsmSceneSerializer::readEntity(TiXmlElement *parentElement)
	{
		NameValuePairList params;
		params["mesh"] = getAttribute(parentElement , "filename");
		// 创建实体
		EntityObject *entity = static_cast<EntityObject*>(m_scene->getRootObject()->createChildObject(getAttribute(parentElement , "name")
			, "EntityObject"
			, &params
			));
		// 是否计算阴影
		entity->getOgreEntity()->setCastShadows(StringConverter::parseBool(getAttribute(parentElement , "CastShadows")));
		// 读取节点属性
		readNullObject(entity , parentElement);
	}


	// 读取Particles
	void OsmSceneSerializer::readParticles(TiXmlElement *parentElement)
	{
		// 遍历所有Entity
		for (TiXmlElement* element = parentElement->FirstChildElement("particle");
			element != 0; element = element->NextSiblingElement("particle"))
		{
			readParticle(element);
		}
	}

	// 读取Particle
	void OsmSceneSerializer::readParticle(TiXmlElement *parentElement)
	{
		NameValuePairList params;
		params["templateName"] = getAttribute(parentElement , "templatename");
		// 创建实体
		ParticleObject *particle = static_cast<ParticleObject*>(m_scene->getRootObject()->createChildObject(getAttribute(parentElement , "name")
			, "ParticleObject"
			, &params
			));
		// 读取节点属性
		readNullObject(particle , parentElement);
	}

	// 读取RibbonTrails
	void OsmSceneSerializer::readRibbonTrails(TiXmlElement *parentElement)
	{
		// 遍历所有Entity
		for (TiXmlElement* element = parentElement->FirstChildElement("particle");
			element != 0; element = element->NextSiblingElement("particle"))
		{
			readRibbonTrail(element);
		}
	}

	// 读取RibbonTrail
	void OsmSceneSerializer::readRibbonTrail(TiXmlElement *parentElement)
	{
		NameValuePairList params;
		// 创建实体
		RibbonTrailObject *ribbonTrail = static_cast<RibbonTrailObject*>(m_scene->getRootObject()->createChildObject(getAttribute(parentElement , "name")
			, "RibbonTrailObject"
			, &params
			));
		// 读取节点属性
		readNullObject(ribbonTrail , parentElement);
	}


	// 读取动画集
	void OsmSceneSerializer::readAnimations(NullObject *entity , TiXmlElement *parentElement)
	{
		// 遍历所有animation
		for (TiXmlElement* element = parentElement->FirstChildElement("animation");
			element != 0; element = element->NextSiblingElement("animation"))
		{
			readAnimation(entity , element);
		}
	}
	// 读取动画
	void OsmSceneSerializer::readAnimation(NullObject *entity , TiXmlElement *parentElement)
	{
		Animation *animation;
		String name(getAttribute(parentElement , "name"));
		if(!m_sceneMgr->hasAnimation(name))
		{
			// 创建动画
			animation = m_sceneMgr->createAnimation(name
				, StringConverter::parseReal(getAttribute(parentElement , "length"))
				);
			// 创建动画控制器
			m_scene->createAnimationStateController(name , name);
		}
		else
		{
			// 获得之前的动画
			animation = m_sceneMgr->getAnimation(name);
		}
		// 创建节点序列动画
		NodeAnimationTrack *track = animation->createNodeTrack(animation->getNumNodeTracks() + 1
			, entity->getOgreSceneNode()
			);
		// 遍历所有animation
		for (TiXmlElement* element = parentElement->FirstChildElement("keyframe");
			element != 0; element = element->NextSiblingElement("keyframe"))
		{
			readKeyframe(track , element);
		}
	}
	// 读取动画帧
	void OsmSceneSerializer::readKeyframe(NodeAnimationTrack *track , TiXmlElement *parentElement)
	{
		// 创建关键帧
		TransformKeyFrame* keyframe = track->createNodeKeyFrame(StringConverter::parseReal(getAttribute(parentElement , "time")));
		// 关键帧位移
		keyframe->setTranslate(getVector3Attribute(parentElement->FirstChildElement("position")));
		// 关键帧旋转
		keyframe->setRotation(getQuaternionAttribute(parentElement->FirstChildElement("rotation")));
		// 关键帧缩放
		keyframe->setScale(getVector3Attribute(parentElement->FirstChildElement("scale")));
	}
};