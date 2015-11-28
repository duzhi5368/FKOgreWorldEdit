#include "StdAfx.h"
#include "SceneSerializer.h"
#include "IScene.h"
#include "OsmSceneSerializer.h"
#include "SneSceneSerializer.h"
#include "XMLSceneSerializer.h"
namespace Ogre
{

	SceneSerializer::SceneSerializer(void)
		: m_serializer(0)
	{
	}

	SceneSerializer::~SceneSerializer(void)
	{
		OGRE_DELETE(m_serializer);
	}

	// 导入场景
	bool SceneSerializer::importScene(const String& filename, IScene* scene)
	{
		// 打开文件
		DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(filename);
		if(stream.isNull())
			return false;
		// 获得串行化器
		ISceneSerializer *serializer = getSerializer(filename);
		if(serializer == 0)
			return false;
		scene->_notifyOrigin(filename);
		// 导入场景
		return serializer->importScene(stream , scene);
	}
	// 导出场景
	bool SceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		// 获得串行化器
		ISceneSerializer *serializer = getSerializer(filename);
		if(serializer == 0)
			return false;
		// 导出场景
		return serializer->exportScene(scene , filename);
	}

	// 根据文件扩展名自动分辨这个场景的格式，获得串行化器
	ISceneSerializer *SceneSerializer::getSerializer(const String& filename)
	{
		// 分析文件名
		String outFilePath , outFileName , outBaseName , outExtention;
		StringUtil::splitFilename(filename , outFileName , outFilePath);
		StringUtil::splitBaseFilename(outFileName , outBaseName , outExtention);
		StringUtil::toLowerCase(outExtention);
		OGRE_DELETE(m_serializer);
		// 使用Osm串行化器来导入
		if(outExtention == "osm")
		{
			m_serializer = new OsmSceneSerializer;
			return m_serializer;
		}
		// 使用SNE串行化器来导入
		if(outExtention == "sne")
		{
			m_serializer = new SneSceneSerializer;
			return m_serializer;
		}
		// 使用xml串行化器来导入
		if(outExtention == "xml")
		{
			m_serializer = new XMLSceneSerializer;
			return m_serializer;
		}
		return 0;
	}

};