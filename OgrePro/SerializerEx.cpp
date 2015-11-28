#include "StdAfx.h"
#include "SerializerEx.h"

namespace Ogre
{
	/// stream overhead = ID + size
	const size_t STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);

	SerializerEx::SerializerEx(void)
	{
	}

	SerializerEx::~SerializerEx(void)
	{
	}

	// 开始一个Chunk
	void SerializerEx::beginWriteChunk(uint16 chunkStartID , uint16 chunkEndID)
	{
		// 压入盏顶
		m_chunks.push_back(Chunk(chunkStartID , chunkEndID , ftell(mpfFile)));
		// 开始包
		writeChunkHeader(chunkStartID , STREAM_OVERHEAD_SIZE);
	}

	// 结束一个Chunk
	void SerializerEx::endWriteChunk(uint16 chunkStartID , uint16 chunkEndID)
	{
		if(m_chunks.empty()
			|| m_chunks.back().startID != chunkStartID
			|| m_chunks.back().endID != chunkEndID
			)
		{
			LogManager::getSingleton().logMessage("beginWriteChunk / endWriteChunk 不匹配！正在结束Chunk "
				+ StringConverter::toString(chunkStartID));
			return;
		}
		Chunk &chunk = m_chunks.back();
		// 保存当前位置
		size_t pos = ftell(mpfFile);
		// 移动到Chunk大小的位置
		fseek(mpfFile , (long)(chunk.pos + sizeof(uint16)) , SEEK_SET);
		// 写入大小
		uint32 size = (uint32)(pos - chunk.pos + STREAM_OVERHEAD_SIZE);
		fwrite((void* const)&size, sizeof(size), 1, mpfFile);
		// 移动回来
		fseek(mpfFile , (long)pos , SEEK_SET);
		// 写入end的ChunkID
		writeChunkHeader(chunkEndID , STREAM_OVERHEAD_SIZE);
		// 弹出盏
		m_chunks.pop_back();
	}

	// 跳过当前流
	void SerializerEx::skipCurrentChunk(DataStreamPtr &stream)
	{
		if (!stream->eof())
		{
			// 一并跳过开始和结束Chunk
			if(mCurrentstreamLen)
			{
				stream->skip(mCurrentstreamLen);
			}
		}
	}

	void SerializerEx::writeNameValuePairList(const NameValuePairList& params)
	{
		uint32 size = (uint32)params.size();
		// 写入数量
		writeInts(&size , 1);
		// 所有名称和值
		for(NameValuePairList::const_iterator iter = params.begin() ; iter != params.end() ; iter ++)
		{
			writeString(iter->first);
			writeString(iter->second);
		}
	}

	void SerializerEx::readNameValuePairList(DataStreamPtr& stream , NameValuePairList &params)
	{
		uint32 size;
		readInts(stream , &size , 1);
		String name , value;
		for(uint32 i = 0 ; i < size ; i ++)
		{
			name = readString(stream);
			value = readString(stream);
			params[name] = value;
		}
	}

	void SerializerEx::writeStringVector(const StringVector& params)
	{
		uint32 size = (uint32)params.size();
		// 写入数量
		writeInts(&size , 1);
		// 所有名称和值
		for(StringVector::const_iterator iter = params.begin() ; iter != params.end() ; iter ++)
		{
			writeString(*iter);
		}

	}
	void SerializerEx::readStringVector(DataStreamPtr& stream , StringVector &params)
	{
		uint32 size;
		readInts(stream , &size , 1);
		String name , value;
		for(uint32 i = 0 ; i < size ; i ++)
		{
			name = readString(stream);
			params.push_back(value);
		}

	}


	// 写入Any值
	void SerializerEx::writeAny(const Any &any , PropertyContainer::PropertyType type)
	{
		PropertyContainer::writeAny(mpfFile , any , type);
	}
	// 读取Any值
	Any SerializerEx::readAny(DataStreamPtr& stream , PropertyContainer::PropertyType type)
	{
		return PropertyContainer::readAny(stream , type);
	}
};