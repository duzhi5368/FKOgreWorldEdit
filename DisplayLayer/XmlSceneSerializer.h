#pragma once
#include "XMLSerializer.h"
#include "ISceneSerializer.h"
#include "ISceneObject.h"

namespace Ogre
{

	// 使用XML格式存放的场景串行化
	class XMLSceneSerializer : public ISceneSerializer
		, public XMLSerializer
	{
	public:
		XMLSceneSerializer(void);
		virtual ~XMLSceneSerializer(void);

		// 导入场景
		virtual bool importScene(DataStreamPtr& stream, IScene* scene);
		// 导出场景
		virtual bool exportScene(IScene* scene, const String& filename);

	protected:
		IScene *m_scene;

		// 读取场景
		void readScene(TiXmlElement *parent);
		// 读取场景对象
		void readSceneObject(TiXmlElement *element , ISceneObject *parent);
		// 读取所有子对象
		void readSceneObjectChilds(TiXmlElement *element , ISceneObject *parent);


		// 写入场景
		void writeScene(TiXmlElement *parent);

		// 写入场景对象
		void writeSceneObject(TiXmlElement *parent , ISceneObject *obj);
		// 写入子场景对象
		void writeSceneObjectChilds(TiXmlElement *parent , ISceneObject *obj);

	};

};