#pragma once
#include "PropertyContainer.h"
namespace Ogre
{

	// 串行化器的加强版，增加了一些非常有用的函数
	class OGREPRO_API SerializerEx : public Serializer
	{
	public:
		SerializerEx(void);
		virtual ~SerializerEx(void);
	protected:
		struct Chunk
		{
			Chunk(uint16 chunkStartID , uint16 chunkEndID , size_t posStart)
				: startID(chunkStartID)
				, endID(chunkEndID)
				, pos(posStart)
			{

			}
			// 包开始ID
			uint16 startID;
			// 包结束ID
			uint16 endID;
			// 包开始位置
			size_t pos;
		};
		typedef std::list<Chunk> ListChunk;
		ListChunk m_chunks;

		// 开始一个Chunk
		void beginWriteChunk(uint16 chunkStartID , uint16 chunkEndID);
		// 结束一个Chunk
		void endWriteChunk(uint16 chunkStartID , uint16 chunkEndID);
		// 跳过当前流
		void skipCurrentChunk(DataStreamPtr &stream);

		// 写入NameValuePairList数组
		void writeNameValuePairList(const NameValuePairList& params);
		// 读取NameValuePairList数组
		void readNameValuePairList(DataStreamPtr& stream , NameValuePairList &params);

		// 写入StringVector数组
		void writeStringVector(const StringVector& params);
		// 读取StringVector数组
		void readStringVector(DataStreamPtr& stream , StringVector &params);


		// 写入Any值
		void writeAny(const Any &any , PropertyContainer::PropertyType type);
		// 读取Any值
		Any readAny(DataStreamPtr& stream , PropertyContainer::PropertyType type);
	};

};