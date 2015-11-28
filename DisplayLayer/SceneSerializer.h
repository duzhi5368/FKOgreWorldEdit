#pragma once
#include "SneSceneSerializer.h"
#include "OsmSceneSerializer.h"
namespace Ogre
{
	class IScene;
	class ISceneSerializer;
	// 场景序列化实现
	class SceneSerializer
	{
	public:
		SceneSerializer(void);
		virtual ~SceneSerializer(void);

		// 导入场景，会根据文件扩展名自动分辨这个场景的格式，究竟是Xml格式还是sne格式
		bool importScene(const String& filename, IScene* scene);
		// 导出场景，会根据文件扩展名自动分辨这个场景的格式，究竟是Xml格式还是sne格式
		bool exportScene(IScene* scene, const String& filename);

		// 根据文件扩展名自动分辨这个场景的格式，获得串行化器
		ISceneSerializer *getSerializer(const String& filename);
	protected:
		ISceneSerializer *m_serializer;
	};

};