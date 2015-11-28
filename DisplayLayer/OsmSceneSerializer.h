#pragma once
#include "TinyXMLInclude.h"
#include "ISceneSerializer.h"

namespace Ogre
{
	class NullObject;
	// XML格式的场景文件串行化器，兼容oFusion的OSM格式
	class OsmSceneSerializer : public ISceneSerializer
	{
	public:
		OsmSceneSerializer(void);
		virtual ~OsmSceneSerializer(void);

		// 导入场景
		virtual bool importScene(DataStreamPtr& stream, IScene* scene);
		// 导出场景
		virtual bool exportScene(IScene* scene, const String& filename);

	protected:
		// XML文档
		TiXmlDocumentPtr mXMLDoc;
		// 当前操作的场景
		IScene *m_scene;
		// Ogre场景管理器
		SceneManager *m_sceneMgr;
		// 读取场景设置
		void readScene(TiXmlElement *parentElement);
		// 读取NullObject
		void readNullObject(NullObject *obj , TiXmlElement *parentElement);
		// 读取Entities
		void readEntities(TiXmlElement *parentElement);
		// 读取Entity
		void readEntity(TiXmlElement *parentElement);
		// 读取Particles
		void readParticles(TiXmlElement *parentElement);
		// 读取Particle
		void readParticle(TiXmlElement *parentElement);
		// 读取RibbonTrails
		void readRibbonTrails(TiXmlElement *parentElement);
		// 读取RibbonTrail
		void readRibbonTrail(TiXmlElement *parentElement);
		// 读取动画集
		void readAnimations(NullObject *entity , TiXmlElement *parentElement);
		// 读取动画
		void readAnimation(NullObject *entity , TiXmlElement *parentElement);
		// 读取动画帧
		void readKeyframe(NodeAnimationTrack *track , TiXmlElement *parentElement);

		// 获得颜色属性，对应xml节点里的r、g、b属性，0~1表示最暗和最亮
		ColourValue getColorAttribute(TiXmlElement *element);
		// 获得Vector3，对应xml节点的x/y/z属性
		Vector3 getVector3Attribute(TiXmlElement *element);
		// 获得Quatation，对应xml节点的x/y/z/w属性
		Quaternion getQuaternionAttribute(TiXmlElement *element);
		// 获得字符串属性
		String getAttribute(TiXmlElement *element , const char *name);
	};
};