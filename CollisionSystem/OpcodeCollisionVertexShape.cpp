#include "StdAfx.h"
#include "OpcodeCollisionVertexShape.h"
#include <opcode.h>

using namespace Opcode;
using namespace IceMaths;

namespace Ogre
{

	//-----------------------------------------------------------------------
	static void MeshCallback(udword triIndex, VertexPointers& triangle, void* userData)
	{
		OpcodeCollisionVertexShape* obj = (OpcodeCollisionVertexShape*)userData;

		ushort* indexList = obj->getIndexList();
		Point* vertexList = obj->getVertexList();


		unsigned int index = triIndex * 3;

		triangle.Vertex[0] = &vertexList[indexList[index]];
		triangle.Vertex[1] = &vertexList[indexList[index + 1]];
		triangle.Vertex[2] = &vertexList[indexList[index + 2]];
	}

	String OpcodeCollisionVertexShape::SHAPE_TYPE = "OpcodeCollisionVertexShape";
	//-----------------------------------------------------------------------
	OpcodeCollisionVertexShape::OpcodeCollisionVertexShape()
		: mAutoDeleteIndexList(false)
		, mAutoDeleteVertexList(false)
	{
		mMeshInterface = 0;
		mCollisionModel = NULL;
		mIndexList = NULL;
		mVertexList = NULL;
	}

	//-----------------------------------------------------------------------
	OpcodeCollisionVertexShape::~OpcodeCollisionVertexShape()
	{
		// ½â¾öÄÚ´æÐ¹Â©
		OGRE_DELETE(mMeshInterface);
		OGRE_DELETE(mCollisionModel);
		if(mAutoDeleteIndexList)
		{
			OGRE_DELETE_ARRAY(mIndexList);
		}
		if(mAutoDeleteVertexList)
		{
			OGRE_DELETE_ARRAY(mVertexList);
		}
	}
	bool OpcodeCollisionVertexShape::init(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const ushort *indexBuffer , size_t indexCount , bool autoDeleteIndex)
	{
		OPCODECREATE OPCC;
		mIndexList = (ushort*)indexBuffer;
		mVertexList = (IceMaths::Point*)vertexBuffer;
		mAutoDeleteVertexList = autoDeleteVertex;
		mAutoDeleteIndexList = autoDeleteIndex;
		mCollisionModel = new Model;
		mMeshInterface = new Opcode::MeshInterface();
		mMeshInterface->SetNbTriangles((udword)indexCount / 3);
		mMeshInterface->SetNbVertices((udword)vertexCount);

		mMeshInterface->SetCallback(MeshCallback, this);

		Opcode::BuildSettings settings;
		settings.mRules = Opcode::SPLIT_BEST_AXIS;

		OPCC.mIMesh = mMeshInterface;
		OPCC.mSettings = settings;
		OPCC.mNoLeaf = true;
		OPCC.mQuantized = false;
		OPCC.mKeepOriginal = false;
		OPCC.mCanRemap = false;

		m_name = name;

		return mCollisionModel->Build(OPCC);
	}
}
