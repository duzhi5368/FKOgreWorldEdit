#include "ETPrerequisites.h"
#include "ETIndexHandler.h"

#include <OgreHardwareBufferManager.h>



namespace Ogre
{
	IndexHandler::IndexHandler(ushort tileSize)
		: m_tileSize(tileSize)
		, m_lodCount(2)
		, m_indicesData(0)
		, m_indicesBuffer(0)
		, m_lodDistance(0)
	{
		m_indicesData = new IndexData*[m_lodCount];
		memset(m_indicesData , 0 , m_lodCount * sizeof(IndexData*));
		m_indicesBuffer = new ushort*[m_lodCount];
		memset(m_indicesBuffer , 0 , m_lodCount * sizeof(ushort*));
		m_lodDistance = new float[m_lodCount];
		memset(m_lodDistance , 0 , m_lodCount * sizeof(float));
		for(ushort i = 0 ; i < m_lodCount ; i ++)
		{
			m_lodDistance[i] = ((i + 1) * 33) * ((i + 1) * 33);
		}
		createIndices();
	}

	IndexHandler::~IndexHandler()
	{
		destroyIndices();
		OGRE_DELETE_ARRAY(m_lodDistance);
	}

	// 请求索引数据
	IndexData* IndexHandler::getIndexData(ushort lod)
	{
		assert(lod < m_lodCount && "Requested unexpected LOD level");
		return m_indicesData[lod];
	}

	// 请求索引的内存数据
	ushort* IndexHandler::getIndexBuffer(ushort lod)
	{
		assert(lod < m_lodCount && "Requested unexpected LOD level");
		return m_indicesBuffer[lod];
	}
	// 获得缝合处的索引数据
	IndexData* IndexHandler::getSutureIndexData(ushort lod , Edge edge)
	{
		assert(lod < m_lodCount && "Requested unexpected LOD level");
		return m_mapSutureIndexData[_toSutureKey(lod , edge)];
	}


	// 销毁索引数据
	void IndexHandler::destroyIndices()
	{
		for (ushort i = 0; i < m_lodCount; ++i)
		{
			OGRE_DELETE(m_indicesData[i]);
			OGRE_DELETE_ARRAY(m_indicesBuffer[i]);
		}
		OGRE_DELETE_ARRAY(m_indicesData);
		OGRE_DELETE_ARRAY(m_indicesBuffer);
		for(MapSutureIndexData::iterator iter = m_mapSutureIndexData.begin() ; iter != m_mapSutureIndexData.end() ; iter ++)
		{
			delete iter->second;
		}
		m_mapSutureIndexData.clear();

	}

	// 创建索引数据
	void IndexHandler::createIndices()
	{
		for(ushort i = 0 ; i < m_lodCount ; i ++)
		{
			// 生成索引数据
			createIndices(i);
			// 生成缝合数据
			for(int j = 0 ; j < Edge_Max ; j ++)
			{
				createSutureIndices(i , (Edge)j);
			}
		}
	}
	// 创建缝合处的索引数据
	void IndexHandler::createSutureIndices(ushort lod , Edge edge)
	{
		IndexData *&data = m_mapSutureIndexData[_toSutureKey(lod , edge)];
		// 如果已经创建了，就不创建了
		if(data)
			return;

		// 创建顶点缓冲区
		data = new IndexData;
		if(lod == 0)
		{
			data->indexCount = (m_tileSize - 1) * 2 * 3;
		}
		else
		{
			data->indexCount = (m_tileSize - 1) / (1 << lod) * 4 * 3;
		}

		data->indexBuffer = 
			HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT
			, data->indexCount
			, HardwareBuffer::HBU_STATIC_WRITE_ONLY
			);

		// 索引缓冲区
		ushort* buffer = static_cast<ushort*>(
			data->indexBuffer->lock(0, 
			data->indexBuffer->getSizeInBytes(), 
			HardwareBuffer::HBL_DISCARD));
		ushort tileIndexSplit = (1 << lod);
		// 填充缓冲区
		switch(edge)
		{
		case Edge_Top:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(0 , 0) , GridVector2(m_tileSize - 1 , 1));
			break;
		case Edge_Left:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(0 , 0) , GridVector2(1 , m_tileSize - 1));
			break;
		case Edge_Bottom:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(0 , m_tileSize - 1 - tileIndexSplit) , GridVector2(m_tileSize - 1 , m_tileSize - 1));
			break;
		case Edge_Right:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(m_tileSize - 1 - tileIndexSplit , 0) , GridVector2(m_tileSize - 1 , m_tileSize - 1));
			break;
		}
		// 解锁
		data->indexBuffer->unlock();
	}
	// 根据lod索引计算索引数
	ushort IndexHandler::getIndexCount(ushort lod)
	{
		if(lod == 0)
		{
			return (m_tileSize - 1) * (m_tileSize - 1) * 2 * 3;
		}
		else
		{
			return (m_tileSize - 1) / (1 << lod) * (m_tileSize - 1) / (1 << lod) * 4 * 3;
		}
	}
	// 根据lod索引计算索引缓冲区大小，字节为单位
	size_t IndexHandler::getIndexBufferSize(ushort lod)
	{
		return getIndexCount(lod) * sizeof(ushort);
	}
	// 计算并填充索引数据到buffer
	size_t IndexHandler::fillBuffer(ushort *dstBuffer , size_t dstBufferSize , ushort lod , const GridVector2 &from , const GridVector2 &to)
	{
		ushort *buffer = dstBuffer;
		// 计算索引缓冲区
		if(lod == 0)
		{
			ushort tileIndexSplit = (1 << lod);
			ushort tileIndexSplitPower = (1 << (lod + 1));
			for(ushort y = from.y ; y < to.y ; y += tileIndexSplit)
			{
				for(ushort x = from.x ; x < to.x ; x += tileIndexSplit)
				{
					if((y % tileIndexSplitPower == 0 && x % tileIndexSplitPower == 0)
						|| (y % tileIndexSplitPower > 0 && x % tileIndexSplitPower > 0))
					{
						*buffer++ = y * m_tileSize + x;
						*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + x;

						*buffer++ = y * m_tileSize + x;
						*buffer++ = y * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
					}
					else
					{
						*buffer++ = y * m_tileSize + x;
						*buffer++ = y * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + x;

						*buffer++ = y * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + x;
					}
				}
			}
		}
		else
		{
			ushort tileIndexSplit = (1 << lod);
			ushort tileCenter = tileIndexSplit / 2;
			for(ushort y = from.y ; y < to.y ; y += tileIndexSplit)
			{
				for(ushort x = from.x ; x < to.x ; x += tileIndexSplit)
				{
					// 上三角形
					*buffer++ = y * m_tileSize + x;
					*buffer++ = y * m_tileSize + (x + tileIndexSplit);
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);

					// 左三角形
					*buffer++ = y * m_tileSize + x;
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);
					*buffer++ = (y + tileIndexSplit) * m_tileSize + x;

					// 右三角形
					*buffer++ = y * m_tileSize + (x + tileIndexSplit);
					*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);

					// 下三角形
					*buffer++ = (y + tileIndexSplit) * m_tileSize + x;
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);
					*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
				}
			}
		}
		assert((size_t)(buffer - dstBuffer) <= (size_t)dstBufferSize);

		return buffer - dstBuffer;
	}
	// 创建一个Lod的索引数据
	void IndexHandler::createIndices(ushort lod)
	{
		// 如果已经创建了，就不创建了
		if(m_indicesBuffer[lod])
			return;

		// 创建缓冲区
		m_indicesBuffer[lod] = new ushort[getIndexCount(lod)];
		ushort *buffer = m_indicesBuffer[lod];
		// 填充缓冲区
		fillBuffer(buffer , getIndexCount(lod) , lod , GridVector2(0 , 0) , GridVector2(m_tileSize - 1 , m_tileSize - 1));
		// 把索引数据从内存拷贝到显存
		fillBufferData(lod);
	}
	// 把索引数据从内存拷贝到显存
	void IndexHandler::fillBufferData(ushort lod)
	{
		if(m_indicesData[lod])
			return;
		// 创建顶点缓冲区
		m_indicesData[lod] = new IndexData;
		m_indicesData[lod]->indexBuffer = 
			HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT,
			getIndexCount(lod), HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// 索引缓冲区
		ushort* indexBuffer = static_cast<ushort*>(
			m_indicesData[lod]->indexBuffer->lock(0, 
			m_indicesData[lod]->indexBuffer->getSizeInBytes(), 
			HardwareBuffer::HBL_DISCARD));
		// 写入数据
		memcpy(indexBuffer , m_indicesBuffer[lod] , getIndexBufferSize(lod));
		// 解锁
		m_indicesData[lod]->indexBuffer->unlock();
		// 写入索引数量
		m_indicesData[lod]->indexCount = getIndexCount(lod);
	}
}
