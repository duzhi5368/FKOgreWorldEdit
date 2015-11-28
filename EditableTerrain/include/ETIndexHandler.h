#ifndef __ETINDEXHANDLER_H__
#define __ETINDEXHANDLER_H__

#include "ETPrerequisites.h"
#include "GridVector2.h"

namespace Ogre
{

	// 管理地形索引数据
	class IndexHandler
	{
	public:
		IndexHandler(ushort tileSize);
		virtual ~IndexHandler();

		// 请求索引数据
		IndexData* getIndexData(ushort lod);

		enum Edge
		{
			Edge_Top,
			Edge_Left,
			Edge_Right,
			Edge_Bottom,

			Edge_Max
		};
		// 获得缝合处的索引数据
		IndexData* getSutureIndexData(ushort lod , Edge edge);

		// 请求索引的内存数据
		ushort* getIndexBuffer(ushort lod);
		// 根据lod索引计算索引数
		ushort getIndexCount(ushort lod);
		// 根据lod索引计算索引缓冲区大小，字节为单位
		size_t getIndexBufferSize(ushort lod);
		// 获得LOD距离
		float getLodDistance(ushort lod)const
		{
			return m_lodDistance[lod];
		}
		// 获得LOD数量
		ushort getLodCount()const
		{
			return m_lodCount;
		}
	private:
		// 创建索引数据
		void createIndices();
		// 创建索引数据
		void createIndices(ushort lod);
		// 创建缝合处的索引数据
		void createSutureIndices(ushort lod , Edge edge);
		// 转换到关键索引
		ulong _toSutureKey(ushort lod , Edge edge)
		{
			return ((ulong)lod << 16)  + (ulong)edge;
		}
		// 计算并填充索引数据到buffer
		size_t fillBuffer(ushort *dstBuffer , size_t dstBufferSize , ushort lod , const GridVector2 &from , const GridVector2 &to);
		// 销毁索引数据
		void destroyIndices();
		// 把索引数据从内存拷贝到显存
		void fillBufferData(ushort lod);

		typedef std::map<ulong , IndexData*> MapSutureIndexData;
		// 封接处的顶点数据，map中的first的高16位是lod，低16位是edge
		MapSutureIndexData m_mapSutureIndexData;
		// 显卡的索引缓存
		IndexData** m_indicesData;
		// 内存的索引数据
		ushort **m_indicesBuffer;
		// 一个Tile的大小
		ushort m_tileSize;
		// LOD数量
		ushort m_lodCount;
		// LOD距离
		float *m_lodDistance;
	};
}

#endif
