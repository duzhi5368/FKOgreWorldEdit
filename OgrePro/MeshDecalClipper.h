/******************************************************************
** 文件名:	DecalSystem.h
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:30
** 描  述:
******************************************************************/
#pragma once
namespace Ogre
{
	// 最大贴花顶点数
	static const long MaxDecalVertices = 256;
	// 贴花斜率
	static const float DecalEpsilon = 0.25f;
	// 贴花剪裁器
	class MeshDecalClipper
	{
	public:

		struct Vertex
		{
			Vector3 pos;
			Vector2 uv;
			uint32 color;
		};

		MeshDecalClipper(const Vector3& center, const Vector3& normal, const Vector3& tangent, float width, float height, float depth);
		// 开始更新贴花，更新过程重要调用clipMesh
		void begin();
		// 剪裁网格到贴花
		void clipMesh(size_t indexCount, const ushort *indices, const Vector3 *vertex, const Vector3 *normal);
		// 剪裁网格到贴花
		void clipMesh(Renderable *renderable);
		// 检查包围盒是否与Decal相交
		bool intersects(const AxisAlignedBox &box);
		// 检查点是否与Decal相交
		bool intersects(const Vector3 &pt);
		// 结束更新贴花，剪裁完毕后要结束
		void end();

		// 填充贴花到ManualObject
		void fill(ManualObject *obj , const String &matName);

		// 获得贴花的包围盒
		const AxisAlignedBox &getWorldBoundingBox()const
		{
			return m_aabb;
		}

		// 获得贴花顶点数
		long getDecalVertexCount()const
		{
			return m_decalVertexCount;
		}

		// 获得贴花索引数
		long getDecalIndexCount()const
		{
			return m_decalTriangleCount * 3;
		}

		// 获得顶点缓冲区
		const Vertex* getVertexArray()const
		{
			return m_vertexArray;
		}

		// 获得索引缓冲区
		const ushort* getIndexArray()const
		{
			return (ushort*)m_triangleArray;
		}

	private:
		struct Triangle
		{
			unsigned short	index[3];
		};
		Vector3		m_decalCenter;
		Vector3		m_decalTangent;
		Vector3		m_decalBinormal;
		Vector3		m_decalNormal;

		Vector4		m_leftPlane;
		Vector4		m_rightPlane;
		Vector4		m_bottomPlane;
		Vector4		m_topPlane;
		Vector4		m_frontPlane;
		Vector4		m_backPlane;

		float		m_width;
		float		m_height;
		float		m_depth;

		long		m_decalVertexCount;
		long		m_decalTriangleCount;

		Vertex		m_vertexArray[MaxDecalVertices];
		Triangle	m_triangleArray[MaxDecalVertices];

		// 贴花的aabb
		AxisAlignedBox m_aabb;

		bool addPolygon(long vertexCount, const Vector3 *vertex, const Vector3 *normal);
		long clipPolygon(long vertexCount, const Vector3 *vertex, const Vector3 *normal, Vector3 *newVertex, Vector3 *newNormal) const;
		static long clipPolygonAgainstPlane(const Vector4& plane, long vertexCount, const Vector3 *vertex, const Vector3 *normal, Vector3 *newVertex, Vector3 *newNormal);
	};
};