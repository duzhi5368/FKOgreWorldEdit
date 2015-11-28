/******************************************************************
** 文件名:	DecalSystem.cpp
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:29
** 描  述:
******************************************************************/
#include "stdafx.h"
#include "MeshDecalClipper.h"
namespace Ogre
{
	MeshDecalClipper::MeshDecalClipper(const Vector3& center, const Vector3& normal, const Vector3& tangent, float width, float height, float depth)
	{
		m_width = width;
		m_height = height;
		m_depth = depth;

		m_decalCenter = center;
		m_decalNormal = normal;
		m_decalTangent = tangent;
		// 计算垂直于切线的副法线
		m_decalBinormal = normal.crossProduct(m_decalTangent);

		// 设置本地包围盒
		m_aabb.setExtents(-width / 2.0f , -depth , -height / 2.0f , width / 2.0f , depth , height / 2.0f);
		// 转换到世界包围盒
		Matrix4 mat;
		mat.makeTransform(center , Vector3(1,1,1) , Quaternion(m_decalTangent , m_decalNormal , m_decalBinormal));
		// 仿射变换到世界坐标
		m_aabb.transformAffine(mat);

		// 计算盒子的边界平面
		float d = center.dotProduct(m_decalTangent);
		m_leftPlane = Vector4(m_decalTangent.x, m_decalTangent.y, m_decalTangent.z, width * 0.5F - d);
		m_rightPlane = Vector4(-m_decalTangent.x, -m_decalTangent.y, -m_decalTangent.z, width * 0.5F + d);

		d = center.dotProduct(m_decalBinormal);
		m_bottomPlane = Vector4(m_decalBinormal.x, m_decalBinormal.y, m_decalBinormal.z, height * 0.5F - d);
		m_topPlane = Vector4(-m_decalBinormal.x, -m_decalBinormal.y, -m_decalBinormal.z, height * 0.5F + d);

		d = center.dotProduct(normal);
		m_frontPlane = Vector4(-normal.x, -normal.y, -normal.z, depth + d);
		m_backPlane = Vector4(normal.x, normal.y, normal.z, depth - d);

	}

	bool MeshDecalClipper::addPolygon(long vertexCount, const Vector3 *vertex, const Vector3 *normal)
	{
		long count = m_decalVertexCount;
		if (count + vertexCount >= MaxDecalVertices) return (false);

		// Add polygon as a triangle fan
		Triangle *triangle = m_triangleArray + m_decalTriangleCount;
		m_decalTriangleCount += vertexCount - 2;
		for (long a = 2; a < vertexCount; a++)
		{
			triangle->index[0] = (unsigned short) count;
			triangle->index[1] = (unsigned short) (count + a - 1);
			triangle->index[2] = (unsigned short) (count + a);
			triangle++;
		}

		// Assign vertex colors
		float f = 1.0F / (1.0F - DecalEpsilon);
		RenderSystem* rs = Root::getSingleton().getRenderSystem();

		for (long b = 0; b < vertexCount; b++)
		{
			m_vertexArray[count].pos = vertex[b];
			const Vector3& n = normal[b];
			float alpha = (m_decalNormal.dotProduct(n) / n.length() - DecalEpsilon) * f;
			rs->convertColourValue(ColourValue(1.0F, 1.0F, 1.0F, (alpha > 0.0F) ? alpha : 0.0F)
				, &m_vertexArray[count].color);
			count++;
		}

		m_decalVertexCount = count;
		return (true);
	}

	void MeshDecalClipper::clipMesh(size_t indexCount, const ushort *indices, const Vector3 *vertex, const Vector3 *normal)
	{
		Vector3		newVertex[9];
		Vector3		newNormal[9];
		size_t triangleCount = indexCount / 3;
		const Triangle *triangle = (const Triangle *)indices;
		// Clip one triangle at a time
		for (size_t a = 0; a < triangleCount; a++)
		{
			long i1 = triangle[a].index[0];
			long i2 = triangle[a].index[1];
			long i3 = triangle[a].index[2];

			const Vector3& v1 = vertex[i1];
			const Vector3& v2 = vertex[i2];
			const Vector3& v3 = vertex[i3];

			Vector3 cross = (v2 - v1).crossProduct(v3 - v1);
			if (m_decalNormal.dotProduct(cross) > DecalEpsilon * cross.length())
			{
				newVertex[0] = v1;
				newVertex[1] = v2;
				newVertex[2] = v3;

				newNormal[0] = normal[i1];
				newNormal[1] = normal[i2];
				newNormal[2] = normal[i3];

				long count = clipPolygon(3, newVertex, newNormal, newVertex, newNormal);
				if ((count != 0) && (!addPolygon(count, newVertex, newNormal))) break;
			}
		}
	}

	long MeshDecalClipper::clipPolygon(long vertexCount, const Vector3 *vertex, const Vector3 *normal, Vector3 *newVertex, Vector3 *newNormal) const
	{
		Vector3		tempVertex[9];
		Vector3		tempNormal[9];

		// Clip against all six planes
		long count = clipPolygonAgainstPlane(m_leftPlane, vertexCount, vertex, normal, tempVertex, tempNormal);
		if (count != 0)
		{
			count = clipPolygonAgainstPlane(m_rightPlane, count, tempVertex, tempNormal, newVertex, newNormal);
			if (count != 0)
			{
				count = clipPolygonAgainstPlane(m_bottomPlane, count, newVertex, newNormal, tempVertex, tempNormal);
				if (count != 0)
				{
					count = clipPolygonAgainstPlane(m_topPlane, count, tempVertex, tempNormal, newVertex, newNormal);
					if (count != 0)
					{
						count = clipPolygonAgainstPlane(m_backPlane, count, newVertex, newNormal, tempVertex, tempNormal);
						if (count != 0)
						{
							count = clipPolygonAgainstPlane(m_frontPlane, count, tempVertex, tempNormal, newVertex, newNormal);
						}
					}
				}
			}
		}

		return (count);
	}

	long MeshDecalClipper::clipPolygonAgainstPlane(const Vector4& plane, long vertexCount, const Vector3 *vertex, const Vector3 *normal, Vector3 *newVertex, Vector3 *newNormal)
	{
		bool	negative[10];

		// Classify vertices
		long negativeCount = 0;
		for (long a = 0; a < vertexCount; a++)
		{
			bool neg = (plane.dotProduct(Vector4(vertex[a])) < 0.0F);
			negative[a] = neg;
			negativeCount += neg;
		}

		// Discard this polygon if it's completely culled
		if (negativeCount == vertexCount) return (0);

		long count = 0;
		for (long b = 0; b < vertexCount; b++)
		{
			// c is the index of the previous vertex
			long c = (b != 0) ? b - 1 : vertexCount - 1;

			if (negative[b])
			{
				if (!negative[c])
				{
					// Current vertex is on negative side of plane,
					// but previous vertex is on positive side.
					const Vector3& v1 = vertex[c];
					const Vector3& v2 = vertex[b];
					float t = plane.dotProduct(Vector4(v1)) / (plane.x * (v1.x - v2.x) + plane.y * (v1.y - v2.y) + plane.z * (v1.z - v2.z));
					newVertex[count] = v1 * (1.0F - t) + v2 * t;

					const Vector3& n1 = normal[c];
					const Vector3& n2 = normal[b];
					newNormal[count] = n1 * (1.0F - t) + n2 * t;
					count++;
				}
			}
			else
			{
				if (negative[c])
				{
					// Current vertex is on positive side of plane,
					// but previous vertex is on negative side.
					const Vector3& v1 = vertex[b];
					const Vector3& v2 = vertex[c];
					float t = plane.dotProduct(Vector4(v1)) / (plane.x * (v1.x - v2.x) + plane.y * (v1.y - v2.y) + plane.z * (v1.z - v2.z));
					newVertex[count] = v1 * (1.0F - t) + v2 * t;

					const Vector3& n1 = normal[b];
					const Vector3& n2 = normal[c];
					newNormal[count] = n1 * (1.0F - t) + n2 * t;
					count++;
				}

				// Include current vertex
				newVertex[count] = vertex[b];
				newNormal[count] = normal[b];
				count++;
			}
		}

		// Return number of vertices in clipped polygon
		return (count);
	}

	void MeshDecalClipper::begin()
	{
		// 清空贴花缓冲
		m_decalVertexCount = 0;
		m_decalTriangleCount = 0;
	}

	void MeshDecalClipper::end()
	{ 
		// 绑定纹理坐标
		float one_over_w = 1.0F / m_width;
		float one_over_h = 1.0F / m_height;
		for (long a = 0; a < m_decalVertexCount; a++)
		{
			Vector3 v = m_vertexArray[a].pos - m_decalCenter;
			float s = v.dotProduct(m_decalTangent) * one_over_w + 0.5F;
			float t = v.dotProduct(m_decalBinormal) * one_over_h + 0.5F;
			m_vertexArray[a].uv = Vector2(s, t);
		}
	}
	// 填充贴花到ManualObject
	void MeshDecalClipper::fill(ManualObject *obj , const String &matName)
	{
		// 如果没有数据，就清空
		if(m_decalVertexCount == 0)
		{
			obj->clear();
			return;
		}
		// 动态建立缓冲区
		if(obj->getNumSections() == 0)
		{
			obj->begin(matName , RenderOperation::OT_TRIANGLE_LIST);
		}
		else
		{
			// 重新设置材质名
			obj->setMaterialName(0 , matName);
			obj->beginUpdate(0);
		}

		// 写入顶点数据
		for(int i = 0 ; i < m_decalVertexCount ; i ++)
		{
			obj->position(m_vertexArray[i].pos);
			obj->textureCoord(m_vertexArray[i].uv);
			ColourValue color;
			color.setAsRGBA(m_vertexArray[i].color);
			obj->colour(color);
		}

		// 写入索引数据
		for(int i = 0 ; i < m_decalTriangleCount ; i ++)
		{
			obj->index(m_triangleArray[i].index[0]);
			obj->index(m_triangleArray[i].index[1]);
			obj->index(m_triangleArray[i].index[2]);
		}
		obj->end();
	}

	// 检查包围盒是否与Decal相交
	bool MeshDecalClipper::intersects(const AxisAlignedBox &box)
	{
		return m_aabb.intersects(box);
	}
	// 检查点是否与Decal相交
	bool MeshDecalClipper::intersects(const Vector3 &pt)
	{
		return m_aabb.intersects(pt);
	}

	// 剪裁网格到贴花
	void MeshDecalClipper::clipMesh(Renderable *renderable)
	{
		RenderOperation op;
		renderable->getRenderOperation(op);
		if(op.useIndexes)
		{
			uchar *index = (uchar*)op.indexData->indexBuffer->lock(op.indexData->indexStart , op.indexData->indexCount , HardwareBuffer::HBL_DISCARD);
		}
	}
};