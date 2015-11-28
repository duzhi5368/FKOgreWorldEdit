#include "StdAfx.h"
#include "OpcodeCollisionBoundingBoxShape.h"
#include <opcode.h>

using namespace Opcode;
using namespace IceMaths;

namespace Ogre
{

	//-----------------------------------------------------------------------
	static void MeshCallback(udword triIndex, VertexPointers& triangle, void* userData)
	{
		OpcodeCollisionBoundingBoxShape* obj = (OpcodeCollisionBoundingBoxShape*)userData;

		udword* indexList = obj->getIndexList();
		Point* vertexList = obj->getVertexList();


		unsigned int index = triIndex * 3;

		triangle.Vertex[0] = &vertexList[indexList[index]];
		triangle.Vertex[1] = &vertexList[indexList[index + 1]];
		triangle.Vertex[2] = &vertexList[indexList[index + 2]];
	}

	String OpcodeCollisionBoundingBoxShape::SHAPE_TYPE = "OpcodeCollisionBoundingBoxShape";
	//-----------------------------------------------------------------------
	OpcodeCollisionBoundingBoxShape::OpcodeCollisionBoundingBoxShape()
		: mAutoDeleteIndexList(false)
		, mAutoDeleteVertexList(false)
	{
		mMeshInterface = 0;
		mCollisionModel = NULL;
		mIndexList = NULL;
		mVertexList = NULL;
	}

	//-----------------------------------------------------------------------
	OpcodeCollisionBoundingBoxShape::~OpcodeCollisionBoundingBoxShape()
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

	bool OpcodeCollisionBoundingBoxShape::init(const String &name , const AxisAlignedBox &box)
	{
		OPCODECREATE OPCC;
		mIndexList = (unsigned int*)indexBuffer;
		mVertexList = (IceMaths::Point*)vertexBuffer;
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
