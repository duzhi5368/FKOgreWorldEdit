#include "stdafx.h"
#include "DragManipulator.h"
namespace Ogre
{
	//-----------------------------------------------------------------------
	DragManipulator::DragManipulator()
	{
		mEnt = NULL;
		mCol = NULL;
		mHandle = NULL;
	}

	//-----------------------------------------------------------------------
	DragManipulator::~DragManipulator()
	{
		if(mEnt)
			mScene->getOgreSceneManager()->destroyEntity(mEnt);

		if(mCol)
			mCollisionWorld->destroyCollisionEntity(mCol);
		if(mHandle)
			delete mHandle;

	}

	//-----------------------------------------------------------------------
	bool DragManipulator::init(IScene* scene , const String &name)
	{
		bool success = AxisManipulator::init(scene, name);

		if(success)
		{
			//create and set up the nodes, meshes, collision shapes and handles
			mEnt = mScene->getOgreSceneManager()->createEntity(name, "DragVisible.mesh");
			mEnt->setMaterialName("ManipulatorHandlesBlueMat");
			mEnt->setRenderQueueGroup(RENDER_QUEUE_9);
			mEnt->setCastShadows(false);
			mSceneNode->attachObject(mEnt);
			mCol = mCollisionWorld->createEntityCollisionEntity(mEnt , CT_OTHER_MESH , "DragCollision.mesh");
			mHandle = new AxisManipulatorHandle();
			mHandle->mManipulator = this;
			mHandle->mMode = AM_UNKNOWN;
			mHandle->mCol = mCol;
			mEnt->setUserAny(Any(mHandle));
		}


		return success;
	}
	// Êó±êÒÆ¶¯
	bool DragManipulator::onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		bool success = AxisManipulator::onMouseMove(view, moveDistance , message , wParam , lParam , pResult);

		if(mActive && mHasUpdated && mActiveHandle && mManipulating)
		{
			//this is called when we're dragging something - our drag code casts a ray into the scene
			//and finds intersection point
			//drag the selected object(s) to the position found

			CPoint point(GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam));
			float screenX = (float)point.x / (float)view->getWndRect().Width();
			float screenY = (float)point.y / (float)view->getWndRect().Height();

			Ray mouseRay = view->getCamera()->getCameraToViewportRay(screenX, screenY);

			Vector3 intersection;
			if(mObserver->manipulatorRayCollide(mouseRay, intersection))
			{
				mObserver->handleManipulatorMove(intersection);
			}
		}

		return success;
	}


	//-----------------------------------------------------------------------
	void DragManipulator::update(IChildView *view, const Vector3& position, const Quaternion& rotate)
	{
		AxisManipulator::update(view, position, rotate);

		//reset all materials
		if(mActiveHandle == mHandle)
			mEnt->setMaterialName("ManipulatorHandlesYellowMat");
		else
			mEnt->setMaterialName("ManipulatorHandlesBlueMat");
	}
}
