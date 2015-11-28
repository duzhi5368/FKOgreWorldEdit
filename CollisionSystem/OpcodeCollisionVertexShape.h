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
	class OpcodeCollisionVertexShape : public OpcodeCollisionShape
	{
	public:

		/** 
		*/
		OpcodeCollisionVertexShape();

		/** 
		*/
		virtual ~OpcodeCollisionVertexShape();

		/** Initialize this shape using the mesh and scale provided
		*/
		bool init(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const ushort *indexBuffer , size_t indexCount , bool autoDeleteIndex);

		// 返回碰撞模型
		virtual Opcode::Model* getCollisionModel(void) const { return mCollisionModel; }

		/** Return index array
		*/
		ushort* getIndexList() const { return mIndexList; }

		/** Return vertex array (in opcode Point format)
		*/
		IceMaths::Point* getVertexList() const { return mVertexList; }

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
		Opcode::Model* mCollisionModel; // The collision model
		Opcode::MeshInterface* mMeshInterface;	// 
		ushort* mIndexList; // Index list for the indices
		IceMaths::Point* mVertexList; // Vertex list for the faces

		bool mAutoDeleteIndexList;
		bool mAutoDeleteVertexList;
	};
}