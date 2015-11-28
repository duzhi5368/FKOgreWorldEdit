#pragma once
#include "ISceneSerializer.h"
#include "SerializerEx.h"
#include "ISceneObject.h"
namespace Ogre
{

#define CHUNK_END 1

	enum SneSceneChunkID
	{
		// 文件头
		S_HEADER = 0x1000,

		// 场景，一个小场景文件可以有多个场景
		S_SCENE = 0x2000,

			// 场景对象
			S_SCENE_OBJECT = 0x3000,

			S_SCENE_OBJECT_END = S_SCENE_OBJECT + CHUNK_END,

		S_SCENE_END = S_SCENE + CHUNK_END,
	};

	// 二进制格式的场景文件串行化器
	class SneSceneSerializer : public ISceneSerializer
		, public SerializerEx
	{
	public:
		SneSceneSerializer(void);
		virtual ~SneSceneSerializer(void);

		// 导入场景
		virtual bool importScene(DataStreamPtr& stream, IScene* scene);
		// 导出场景
		virtual bool exportScene(IScene* scene, const String& filename);

	protected:
		IScene *m_scene;

		// 读取场景
		void readScene(DataStreamPtr& stream);
		// 读取场景对象
		void readSceneObject(DataStreamPtr& stream , ISceneObject *parent);


		// 写入场景
		void writeScene();

		// 写入场景对象
		void writeSceneObject(ISceneObject *obj);
		// 写入子场景对象
		void writeSceneObjectChilds(ISceneObject *obj);

	};
};