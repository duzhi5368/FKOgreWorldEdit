#include "StdAfx.h"
#include "SneSceneSerializer.h"
#include "IScene.h"
namespace Ogre
{
	SneSceneSerializer::SneSceneSerializer(void)
		: m_scene(0)
	{
	}

	SneSceneSerializer::~SneSceneSerializer(void)
	{
	}


	// 导入场景
	bool SneSceneSerializer::importScene(DataStreamPtr& stream, IScene* scene)
	{
		m_scene = scene;
		// Determine endianness (must be the first thing we do!)
		determineEndianness(stream);
		// Check header
		readFileHeader(stream);

		SneSceneChunkID streamID;
		while(!stream->eof())
		{
			streamID = (SneSceneChunkID)readChunk(stream);
			switch (streamID)
			{
			case S_SCENE:
				readScene(stream);
				break;
			default:
				// 不认识这个流，就跳过他
				skipCurrentChunk(stream);
				break;
			}
		}
		return true;
	}

	// 读取场景
	void SneSceneSerializer::readScene(DataStreamPtr& stream)
	{
		ISceneObject *obj = m_scene->getRootObject();
		SneSceneChunkID streamID;
		while(!stream->eof())
		{
			streamID = (SneSceneChunkID)readChunk(stream);
			if(streamID == S_SCENE_OBJECT)
			{
				readSceneObject(stream, obj);
			}
			else if(streamID == S_SCENE_END)
			{
				return;
			}
			else
			{
				// 不认识这个流，就跳过他
				skipCurrentChunk(stream);
			}
		}
	}
	// 读取场景对象
	void SneSceneSerializer::readSceneObject(DataStreamPtr& stream , ISceneObject *parent)
	{
		String name = readString(stream);
		String type = readString(stream);
		NameValuePairList params;
		readNameValuePairList(stream , params);
		// 创建场景对象
		ISceneObject *obj = parent->createChildObject(name , type , &params);
		uint32 size;
		readInts(stream , &size , 1);
		for(uint32 i = 0 ; i < size ; i ++)
		{
			uint32 id , type;
			readInts(stream , &id , 1);
			readInts(stream , &type , 1);
			// 读取值
			Any value = readAny(stream , (PropertyContainer::PropertyType)type);
			if(!value.isEmpty())
			{
				obj->setProperty(id , value);
			}
		}
		// 读出结束
		uint16 streamID = readChunk(stream);
		if(streamID != S_SCENE_OBJECT_END)
		{
			// 不认识这个流，就跳过他
			skipCurrentChunk(stream);
			LogManager::getSingleton().logMessage("不匹配的SceneObject结束符号 Begin "
				+ StringConverter::toString(S_SCENE_OBJECT) + " , End " + StringConverter::toString(streamID));
		}
	}

	// 导出场景
	bool SneSceneSerializer::exportScene(IScene* scene, const String& filename)
	{
		m_scene = scene;

		LogManager::getSingleton().logMessage("SneSceneSerializer writing SneScene data to " + filename + "...");

		// Decide on endian mode
		determineEndianness(ENDIAN_NATIVE);
		mpfFile = fopen(filename.c_str(), "wb");
		if (!mpfFile)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Unable to open file " + filename + " for writing",
				"SneSceneSerializer::exportScene");
			return false;
		}

		writeFileHeader();
		LogManager::getSingleton().logMessage("File header written.");

		LogManager::getSingleton().logMessage("Writing SneScene data...");
		writeScene();
		LogManager::getSingleton().logMessage("SneScene data exported.");

		fclose(mpfFile);
		LogManager::getSingleton().logMessage("SneSceneSerializer export successful.");


		return true;
	}
	// 写入场景
	void SneSceneSerializer::writeScene()
	{
		// 开始包
		beginWriteChunk(S_SCENE , S_SCENE_END);

		ISceneObject *obj = m_scene->getRootObject();
		writeSceneObjectChilds(obj);
		// 结束包
		endWriteChunk(S_SCENE , S_SCENE_END);
	}
	// 写入场景对象
	void SneSceneSerializer::writeSceneObject(ISceneObject *obj)
	{
		const PropertyContainer::MapProperty *properties;
		properties = obj->getPropertiesList();
		// 开始包
		beginWriteChunk(S_SCENE_OBJECT , S_SCENE_OBJECT_END);
		// 写入名称
		writeString(obj->getName());
		// 写入类型
		writeString(obj->getType());
		// 写入创建参数
		writeNameValuePairList(obj->getCreationParams());
	
		// 写入属性数量
		uint32 size = 0;
		// 计算需要写入的属性数量
		for(PropertyContainer::MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.saveable)
			{
				size++;
			}
		}
		// 写入数量	
		writeInts(&size , 1);
		// 写入所有属性
		for(PropertyContainer::MapProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			if(iter->second.saveable)
			{
				writeInts(&iter->second.id , 1);
				writeInts(&(uint32&)iter->second.type , 1);
				writeAny(obj->getProperty(iter->second.id) , iter->second.type);
			}
		}
		// 结束包
		endWriteChunk(S_SCENE_OBJECT , S_SCENE_OBJECT_END);
		// 写入所有子对象
		writeSceneObjectChilds(obj);
	}

	// 写入子场景对象
	void SneSceneSerializer::writeSceneObjectChilds(ISceneObject *obj)
	{
		ISceneObject::ChildObjectIterator iter = obj->getChildObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *objChild = iter.getNext();
			writeSceneObject(objChild);
		}
	}
};