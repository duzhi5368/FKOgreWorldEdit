#include "stdafx.h"
#include "TranslationManipulator.h"
namespace Ogre
{
	//-----------------------------------------------------------------------
	TranslationManipulator::TranslationManipulator()
	{
		for(uint i = 0; i < AT_LAST; ++i)
		{
			mNode[i] = NULL;
			mEnt[i] = NULL;
			mConeEnt[i] = NULL;
			mCol[i] = NULL;
			mHandle[i] = NULL;
		}
	}

	//-----------------------------------------------------------------------
	TranslationManipulator::~TranslationManipulator()
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

		if(mConeEnt[AT_X])
			mScene->getOgreSceneManager()->destroyEntity(mConeEnt[AT_X]);
		if(mConeEnt[AT_Y])
			mScene->getOgreSceneManager()->destroyEntity(mConeEnt[AT_Y]);
		if(mConeEnt[AT_Z])
			mScene->getOgreSceneManager()->destroyEntity(mConeEnt[AT_Z]);
	}

	//-----------------------------------------------------------------------
	bool TranslationManipulator::init(IScene* scene , const String &name)
	{
		bool success = AxisManipulator::init(scene, name);

		if(success)
		{

			String temp = name + "_Y";

			//create and set up the nodes, meshes, collision shapes and handles
			mNode[AT_Y] = mSceneNode->createChildSceneNode(temp);
			mEnt[AT_Y] = mScene->getOgreSceneManager()->createEntity(temp + "_Line", "MoveArrowLineVisible.mesh");
			mEnt[AT_Y]->setMaterialName("ManipulatorHandlesRedMat");
			mConeEnt[AT_Y] = mScene->getOgreSceneManager()->createEntity(temp + "_Cone", "MoveArrowConeVisible.mesh");
			mConeEnt[AT_Y]->setMaterialName("ManipulatorHandlesRedMat");
			mEnt[AT_Y]->setRenderQueueGroup(RENDER_QUEUE_9);
			mConeEnt[AT_Y]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_Y]->setCastShadows(false);
			mConeEnt[AT_Y]->setCastShadows(false);
			mNode[AT_Y]->attachObject(mEnt[AT_Y]);
			mNode[AT_Y]->attachObject(mConeEnt[AT_Y]);
			mCol[AT_Y] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_Y] , CT_OTHER_MESH , "MoveArrowCollision.mesh");
			mHandle[AT_Y] = new AxisManipulatorHandle();
			mHandle[AT_Y]->mManipulator = this;
			mHandle[AT_Y]->mMode = AM_TRANSLATE_Y;
			mHandle[AT_Y]->mCol = mCol[AT_Y];
			mConeEnt[AT_Y]->setUserAny(Any(mHandle[AT_Y]));
			mEnt[AT_Y]->setUserAny(Any(mHandle[AT_Y]));

			temp = name + "_X";

			mNode[AT_X] = mSceneNode->createChildSceneNode(temp);
			Quaternion orient;
			orient.FromAngleAxis(Radian(Degree(270)), Vector3::UNIT_Z);
			mNode[AT_X]->setOrientation(mNode[AT_X]->getOrientation() * orient);
			mEnt[AT_X] = mScene->getOgreSceneManager()->createEntity(temp + "_Line", "MoveArrowLineVisible.mesh");
			mEnt[AT_X]->setMaterialName("ManipulatorHandlesGreenMat");
			mConeEnt[AT_X] = mScene->getOgreSceneManager()->createEntity(temp + "_Cone", "MoveArrowConeVisible.mesh");
			mConeEnt[AT_X]->setMaterialName("ManipulatorHandlesGreenMat");
			mEnt[AT_X]->setRenderQueueGroup(RENDER_QUEUE_9);
			mConeEnt[AT_X]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_X]->setCastShadows(false);
			mConeEnt[AT_Y]->setCastShadows(false);
			mNode[AT_X]->attachObject(mEnt[AT_X]);
			mNode[AT_X]->attachObject(mConeEnt[AT_X]);
			mCol[AT_X] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_X] , CT_OTHER_MESH , "MoveArrowCollision.mesh");
			mHandle[AT_X] = new AxisManipulatorHandle();
			mHandle[AT_X]->mManipulator = this;
			mHandle[AT_X]->mMode = AM_TRANSLATE_X;
			mHandle[AT_X]->mCol = mCol[AT_X];
			mConeEnt[AT_X]->setUserAny(Any(mHandle[AT_X]));
			mEnt[AT_X]->setUserAny(Any(mHandle[AT_X]));

			temp = name + "_Z";

			mNode[AT_Z] = mSceneNode->createChildSceneNode(temp);
			mNode[AT_Z]->rotate(Vector3::UNIT_X, Degree(90));
			mEnt[AT_Z] = mScene->getOgreSceneManager()->createEntity(temp + "_Line", "MoveArrowLineVisible.mesh");
			mEnt[AT_Z]->setMaterialName("ManipulatorHandlesBlueMat");
			mConeEnt[AT_Z] = mScene->getOgreSceneManager()->createEntity(temp + "_Cone", "MoveArrowConeVisible.mesh");
			mConeEnt[AT_Z]->setMaterialName("ManipulatorHandlesBlueMat");
			mEnt[AT_Z]->setRenderQueueGroup(RENDER_QUEUE_9);
			mConeEnt[AT_Z]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_Z]->setCastShadows(false);
			mConeEnt[AT_Z]->setCastShadows(false);
			mNode[AT_Z]->attachObject(mEnt[AT_Z]);
			mNode[AT_Z]->attachObject(mConeEnt[AT_Z]);
			mCol[AT_Z] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_Z] , CT_OTHER_MESH , "MoveArrowCollision.mesh");
			mHandle[AT_Z] = new AxisManipulatorHandle();
			mHandle[AT_Z]->mManipulator = this;
			mHandle[AT_Z]->mMode = AM_TRANSLATE_Z;
			mHandle[AT_Z]->mCol = mCol[AT_Z];
			mConeEnt[AT_Z]->setUserAny(Any(mHandle[AT_Z]));
			mEnt[AT_Z]->setUserAny(Any(mHandle[AT_Z]));

			temp = name + "_XY";

			mNode[AT_XY] = mSceneNode->createChildSceneNode(temp);
			mEnt[AT_XY] = mScene->getOgreSceneManager()->createEntity(temp, "MoveTwoAxisVisible.mesh");
			mEnt[AT_XY]->setMaterialName("ManipulatorHandlesRedMat");
			mEnt[AT_XY]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_XY]->setCastShadows(false);
			mNode[AT_XY]->attachObject(mEnt[AT_XY]);
			mCol[AT_XY] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_XY] , CT_OTHER_MESH , "MoveTwoAxisCollision.mesh");
			mHandle[AT_XY] = new AxisManipulatorHandle();
			mHandle[AT_XY]->mManipulator = this;
			mHandle[AT_XY]->mMode = AM_TRANSLATE_XY;
			mHandle[AT_XY]->mCol = mCol[AT_XY];
			mEnt[AT_XY]->setUserAny(Any(mHandle[AT_XY]));

			temp = name + "_XZ";

			mNode[AT_XZ] = mSceneNode->createChildSceneNode(temp);
			orient.FromAngleAxis(Radian(Degree(90)), Vector3::UNIT_X);
			mNode[AT_XZ]->setOrientation(mNode[AT_XZ]->getOrientation() * orient);
			mEnt[AT_XZ] = mScene->getOgreSceneManager()->createEntity(temp, "MoveTwoAxisVisible.mesh");
			mEnt[AT_XZ]->setMaterialName("ManipulatorHandlesGreenMat");
			mEnt[AT_XZ]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_XZ]->setCastShadows(false);
			mNode[AT_XZ]->attachObject(mEnt[AT_XZ]);
			mCol[AT_XZ] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_XZ] , CT_OTHER_MESH , "MoveTwoAxisCollision.mesh");
			mHandle[AT_XZ] = new AxisManipulatorHandle();
			mHandle[AT_XZ]->mManipulator = this;
			mHandle[AT_XZ]->mMode = AM_TRANSLATE_XZ;
			mHandle[AT_XZ]->mCol = mCol[AT_XZ];
			mEnt[AT_XZ]->setUserAny(Any(mHandle[AT_XZ]));

			temp = name + "_YZ";

			mNode[AT_YZ] = mSceneNode->createChildSceneNode(temp);
			orient.FromAngleAxis(Radian(Degree(270)), Vector3::UNIT_Y);
			mNode[AT_YZ]->setOrientation(mNode[AT_YZ]->getOrientation() * orient);
			mEnt[AT_YZ] = mScene->getOgreSceneManager()->createEntity(temp, "MoveTwoAxisVisible.mesh");
			mEnt[AT_YZ]->setMaterialName("ManipulatorHandlesBlueMat");
			mEnt[AT_YZ]->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt[AT_YZ]->setCastShadows(false);
			mNode[AT_YZ]->attachObject(mEnt[AT_YZ]);
			mCol[AT_YZ] = mCollisionWorld->createEntityCollisionEntity(mEnt[AT_YZ] , CT_OTHER_MESH , "MoveTwoAxisCollision.mesh");
			mHandle[AT_YZ] = new AxisManipulatorHandle();
			mHandle[AT_YZ]->mManipulator = this;
			mHandle[AT_YZ]->mMode = AM_TRANSLATE_YZ;
			mHandle[AT_YZ]->mCol = mCol[AT_YZ];
			mEnt[AT_YZ]->setUserAny(Any(mHandle[AT_YZ]));
		}

		return success;
	}

	//-----------------------------------------------------------------------
	void TranslationManipulator::update(IChildView *view, const Vector3& position, const Quaternion& rotate)
	{
		AxisManipulator::update(view, position, rotate);

		//reset all materials
		if(mActiveHandle == mHandle[AT_X] || mActiveHandle == mHandle[AT_XY] || mActiveHandle == mHandle[AT_XZ])
			mEnt[AT_X]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_X]->setMaterialName("ManipulatorHandlesGreenMat");

		if(mActiveHandle == mHandle[AT_XY])
			mEnt[AT_XY]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_XY]->setMaterialName("ManipulatorHandlesRedMat");

		if(mActiveHandle == mHandle[AT_Y] || mActiveHandle == mHandle[AT_XY] || mActiveHandle == mHandle[AT_YZ])
			mEnt[AT_Y]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_Y]->setMaterialName("ManipulatorHandlesRedMat");

		if(mActiveHandle == mHandle[AT_XZ])
			mEnt[AT_XZ]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_XZ]->setMaterialName("ManipulatorHandlesGreenMat");

		if(mActiveHandle == mHandle[AT_Z] || mActiveHandle == mHandle[AT_XZ] || mActiveHandle == mHandle[AT_YZ])
			mEnt[AT_Z]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_Z]->setMaterialName("ManipulatorHandlesBlueMat");

		if(mActiveHandle == mHandle[AT_YZ])
			mEnt[AT_YZ]->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt[AT_YZ]->setMaterialName("ManipulatorHandlesBlueMat");

	}
}
