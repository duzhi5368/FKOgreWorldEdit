#include "stdafx.h"
#include "RotationManipulator.h"
namespace Ogre
{
	//-----------------------------------------------------------------------
	RotationManipulator::RotationManipulator()
	{
		for(uint i = 0; i < AT_LAST; ++i)
		{
			mNode[i] = NULL;
			mEnt[i] = NULL;
			mCol[i] = NULL;
			mHandle[i] = NULL;
		}
	}

	//-----------------------------------------------------------------------
	RotationManipulator::~RotationManipulator()
	{
		for(uint i = 0; i < AT_LAST; ++i)
		{
			if(mNode[i])
				mScene->getOgreSceneManager()->destroySceneNode(mNode[i]->getName());

			if(mEnt[i])
				mScene->getOgreSceneManager()->destroyEntity(mEnt[i]);

			if(mHandle[i])
				delete mHandle[i];
		}
	}

	//-----------------------------------------------------------------------
	bool RotationManipulator::init(IScene* scene , const String &name)
	{
		bool success = AxisManipulator::init(scene, name);

		if(success)
		{

			String temp = name + "_Y";

			//create and set up the nodes, meshes, collision shapes and handles
			mNode[AT_Y] = mSceneNode->createChildSceneNode(temp);
			mEnt[AT_Y] = mScene->getOgreSceneManager()->createEntity(temp, "RotationRingVisible.mesh");
			mEnt[AT_Y]->setMaterialName("ManipulatorHandlesRedMat");
			mEnt[AT_Y]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_Y]->setCastShadows(false);
			mNode[AT_Y]->attachObject(mEnt[AT_Y]);
			mCol[AT_Y] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_Y] , CT_OTHER_MESH , "RotationRingCollision.mesh");
			mHandle[AT_Y] = new AxisManipulatorHandle();
			mHandle[AT_Y]->mManipulator = this;
			mHandle[AT_Y]->mMode = AM_ROTATE_Y;
			mHandle[AT_Y]->mCol = mCol[AT_Y];
			mEnt[AT_Y]->setUserAny(Any(mHandle[AT_Y]));

			temp = name + "_X";

			//create and set up the nodes, meshes, collision shapes and handles
			mNode[AT_X] = mSceneNode->createChildSceneNode(temp);
			Quaternion orient;
			orient.FromAngleAxis(Radian(Degree(90.0f)), Vector3::UNIT_Z);
			mNode[AT_X]->setOrientation(mNode[AT_X]->getOrientation() * orient);
			mEnt[AT_X] = mScene->getOgreSceneManager()->createEntity(temp, "RotationRingVisible.mesh");
			mEnt[AT_X]->setMaterialName("ManipulatorHandlesGreenMat");
			mEnt[AT_X]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_X]->setCastShadows(false);
			mNode[AT_X]->attachObject(mEnt[AT_X]);
			mCol[AT_X] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_X] , CT_OTHER_MESH , "RotationRingCollision.mesh");
			mHandle[AT_X] = new AxisManipulatorHandle();
			mHandle[AT_X]->mManipulator = this;
			mHandle[AT_X]->mMode = AM_ROTATE_X;
			mHandle[AT_X]->mCol = mCol[AT_X];
			mEnt[AT_X]->setUserAny(Any(mHandle[AT_X]));

			temp = name + "_Z";

			//create and set up the nodes, meshes, collision shapes and handles
			mNode[AT_Z] = mSceneNode->createChildSceneNode(temp);
			orient.FromAngleAxis(Radian(Degree(90.0f)), Vector3::UNIT_X);
			mNode[AT_Z]->setOrientation(mNode[AT_Z]->getOrientation() * orient);
			mEnt[AT_Z] = mScene->getOgreSceneManager()->createEntity(temp, "RotationRingVisible.mesh");
			mEnt[AT_Z]->setMaterialName("ManipulatorHandlesGreenMat");
			mEnt[AT_Z]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_Z]->setCastShadows(false);
			mNode[AT_Z]->attachObject(mEnt[AT_Z]);
			mCol[AT_Z] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_Z] , CT_OTHER_MESH , "RotationRingCollision.mesh");
			mHandle[AT_Z] = new AxisManipulatorHandle();
			mHandle[AT_Z]->mManipulator = this;
			mHandle[AT_Z]->mMode = AM_ROTATE_Z;
			mHandle[AT_Z]->mCol = mCol[AT_Z];
			mEnt[AT_Z]->setUserAny(Any(mHandle[AT_Z]));
		}

		return success;
	}

	//-----------------------------------------------------------------------
	void RotationManipulator::update(IChildView *view, const Vector3& position, const Quaternion& rotate)
	{
		AxisManipulator::update(view, position, rotate);

		//reset all materials
		if(mActiveHandle == mHandle[AT_X])
			mEnt[AT_X]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_X]->setMaterialName("ManipulatorHandlesGreenMat");

		if(mActiveHandle == mHandle[AT_Y])
			mEnt[AT_Y]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_Y]->setMaterialName("ManipulatorHandlesRedMat");

		if(mActiveHandle == mHandle[AT_Z])
			mEnt[AT_Z]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_Z]->setMaterialName("ManipulatorHandlesBlueMat");
	}
}
