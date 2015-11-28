#include "StdAfx.h"
#include "OpcodeCollisionSubMeshShape.h"
#include <opcode.h>

using namespace Opcode;
using namespace IceMaths;

namespace Ogre
{

	//-----------------------------------------------------------------------
	static void MeshCallback(udword triIndex, VertexPointers& triangle, void* userData)
	{
		OpcodeCollisionSubMeshShape* obj = (OpcodeCollisionSubMeshShape*)userData;

		udword* indexList = obj->getIndexList();
		Point* vertexList = obj->getVertexList();


		unsigned int index = triIndex * 3;

		triangle.Vertex[0] = &vertexList[indexList[index]];
		triangle.Vertex[1] = &vertexList[indexList[index + 1]];
		triangle.Vertex[2] = &vertexList[indexList[index + 2]];
	}

	String OpcodeCollisionSubMeshShape::SHAPE_TYPE = "OpcodeCollisionSubMeshShape";
	//-----------------------------------------------------------------------
	OpcodeCollisionSubMeshShape::OpcodeCollisionSubMeshShape()
	{
		mMeshInterface = 0;
		mCollisionModel = 0;
		mIndexList = 0;
		mVertexList = 0;
	}

	//-----------------------------------------------------------------------
	OpcodeCollisionSubMeshShape::~OpcodeCollisionSubMeshShape()
	{
		// add by 王宏张 2007-7-27
		// 解决内存泄漏
		OGRE_DELETE(mMeshInterface);
		OGRE_DELETE(mCollisionModel);
		OGRE_DELETE_ARRAY(mIndexList);
		OGRE_DELETE_ARRAY(mVertexList);
	}

	//-----------------------------------------------------------------------
	bool OpcodeCollisionSubMeshShape::init(const String &meshName , const String &subMeshName, const Vector3& position, const Quaternion& orient, const Vector3& scale)
	{
		OPCODECREATE OPCC;

		size_t vertexCount;
		size_t indexCount;
		//Vector3* vertices;
		unsigned long* indices;

		MeshPtr mesh;
		SubMesh *subMesh = 0;
		try
		{
			mesh = MeshManager::getSingleton().load(meshName, "General");
			subMesh = mesh->getSubMesh(subMeshName);
		}
		catch (Exception&)
		{
			// 如果只有自己这个引用，就卸载之
			if(!mesh.isNull() && mesh.useCount() == ResourceGroupManager::RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS + 1)
			{
				mesh->unload();
			}
			return false;
		}

		if(!initFromSubMesh(subMesh, vertexCount, mVertexList, indexCount, indices , position , orient , scale))
			return false;

		// 如果只有自己这个引用，就卸载之
		if(mesh.useCount() == ResourceGroupManager::RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS + 1)
		{
			mesh->unload();
		}

		mIndexList = (unsigned int*)indices;

		mCollisionModel = new Model;

		if(!mCollisionModel)
			return false;

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

		m_name = meshName + "_" + subMeshName;

		return mCollisionModel->Build(OPCC);
	}

	//-----------------------------------------------------------------------
	void OpcodeCollisionSubMeshShape::getTriCoords(int triIndex, const Matrix4& world, Vector3& v0, Vector3& v1, Vector3& v2) const
	{
		udword* indexList = getIndexList();
		Point* vertexList = getVertexList();

		unsigned int index = triIndex * 3;

		Point* pvert1 = &vertexList[indexList[index]];
		Point* pvert2 = &vertexList[indexList[index + 1]];
		Point* pvert3 = &vertexList[indexList[index + 2]];
		v0 = Vector3(pvert1->x, pvert1->y, pvert1->z);
		v1 = Vector3(pvert2->x, pvert2->y, pvert2->z);
		v2 = Vector3(pvert3->x, pvert3->y, pvert3->z);
		v0 = world * v0;
		v1 = world * v1;
		v2 = world * v2;
	}

	//-----------------------------------------------------------------------
	bool OpcodeCollisionSubMeshShape::initFromSubMesh( const SubMesh *subMesh, size_t& vertexCount, IceMaths::Point*& vertices, size_t& indexCount, unsigned long*& indices, const Vector3& position, const Quaternion& orient, const Vector3& scale)
	{
		bool addedShared = false;
		size_t currentOffset = 0;
		size_t sharedOffset = 0;
		size_t nextOffset = 0;
		size_t indexOffset = 0;

		vertexCount = indexCount = 0;

		// Calculate how many vertices and indices we're going to need

		// We only need to add the shared vertices once
		if(subMesh->useSharedVertices)
		{
			if( !addedShared )
			{
				vertexCount += subMesh->parent->sharedVertexData->vertexCount;
				addedShared = true;
			}
		}
		else
		{
			vertexCount += subMesh->vertexData->vertexCount;
		}

		// Add the indices
		indexCount += subMesh->indexData->indexCount;


		// Allocate space for the vertices and indices
		vertices = new IceMaths::Point[vertexCount];
		indices = new unsigned long[indexCount];

		addedShared = false;

		// Run through the sub meshes again, adding the data into the arrays

		VertexData* vertexData = subMesh->useSharedVertices ? subMesh->parent->sharedVertexData : subMesh->vertexData;

		if((!subMesh->useSharedVertices)||(subMesh->useSharedVertices && !addedShared))
		{
			if(subMesh->useSharedVertices)
			{
				addedShared = true;
				sharedOffset = currentOffset;
			}

			const VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);

			HardwareVertexBufferSharedPtr vbuf = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Real or a double
			//  as second argument. So make it float, to avoid trouble when Real will
			//  be comiled/typedefed as double:
			//      Real* pReal;
			float* pReal;

			for( size_t j = 0; j < vertexData->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Vector3 pt(pReal[0], pReal[1], pReal[2]);

				// add by 王宏张 2007-7-27
				// 转换赋值
				Vector3 ptOut((orient * (pt * scale)) + position);
				vertices[currentOffset + j].x = ptOut.x;
				vertices[currentOffset + j].y = ptOut.y;
				vertices[currentOffset + j].z = ptOut.z;
			}

			vbuf->unlock();
			nextOffset += vertexData->vertexCount;
		}

		IndexData* indexData = subMesh->indexData;
		size_t numTris = indexData->indexCount / 3;
		HardwareIndexBufferSharedPtr ibuf = indexData->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);

		unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


		size_t offset = (subMesh->useSharedVertices)? sharedOffset : currentOffset;

		if ( use32bitindexes )
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				indices[indexOffset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				indices[indexOffset++] = static_cast<unsigned long>(pShort[k]) +
					static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		currentOffset = nextOffset;

		return true;
	}


}
