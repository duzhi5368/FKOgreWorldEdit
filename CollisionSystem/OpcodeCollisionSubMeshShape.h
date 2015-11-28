#pragma once
#include "OpcodeCollisionShape.h"

//forward declarations
namespace Opcode
{
	class Model;
	class MeshInterface;
}

namespace IceMaths
{
	class Matrix4x4;
	class Point;
}

namespace Ogre
{
	/** An abstract base class for a collision shape
	*/
	class OpcodeCollisionSubMeshShape : public OpcodeCollisionShape
	{
	public:

		/** 
		*/
		OpcodeCollisionSubMeshShape();

		/** 
		*/
		virtual ~OpcodeCollisionSubMeshShape();

		/** Initialize this shape using the mesh and scale provided
		*/
		bool init(const String &meshName , const String &subMeshName, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);

		// 返回碰撞模型
		virtual Opcode::Model* getCollisionModel(void) const { return mCollisionModel; }

		/** Return index array
		*/
		unsigned int* getIndexList() const { return mIndexList; }

		/** Return vertex array (in opcode Point format)
		*/
		IceMaths::Point* getVertexList() const { return mVertexList; }

		/** Get the three vertices from index, index + 1, index + 2
		*/
		void getTriCoords(int triIndex, const Matrix4& world, Vector3& v0, Vector3& v1, Vector3& v2) const;


		// 获得名称
		virtual const String &getName()const
		{
			return m_name;
		}
		// 获得类型
		virtual const String &getType()const
		{
			return SHAPE_TYPE;
		}

		// 类型
		static String SHAPE_TYPE;


	protected:

		String m_name;
		/** initialize from the mesh provided
		Fills the vertices, vertexCount, indices, and indexCount variables which are passed by ref
		Returns success
		*/
		bool initFromSubMesh( const SubMesh *subMesh, size_t& vertexCount, IceMaths::Point*& vertices, size_t& indexCount, unsigned long*& indices, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);

		Opcode::Model* mCollisionModel; // The collision model
		Opcode::MeshInterface* mMeshInterface;	// 
		unsigned int* mIndexList; // Index list for the indices
		IceMaths::Point* mVertexList; // Vertex list for the faces
	};
}