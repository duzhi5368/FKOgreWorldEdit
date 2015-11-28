#include "stdafx.h"
#include "AxisManipulator.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	const float AxisManipulator::ROTATION_SPEED = 4.0f; //a multiplier for rotation

	//-----------------------------------------------------------------------
	AxisManipulator::AxisManipulator()
	{
		mCollisionWorld = 0;
		mCameraDistance = 0.0f;
		mActiveHandle = NULL;
		mSceneNode = NULL;
		mHasUpdated = false;
	}

	//-----------------------------------------------------------------------
	AxisManipulator::~AxisManipulator()
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
		mScene->getOgreSceneManager()->destroySceneNode(mSceneNode->getName());
	}

	//-----------------------------------------------------------------------
	bool AxisManipulator::init(IScene *scene , const String &name)
	{
		bool success = Manipulator::init(scene , name);

		if(success)
		{
			mSceneNode = mScene->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode(name);
			mCollisionWorld = DisplaySystem::getSingleton().getCollisionWorld();

		}
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
		return success;
	}

	//-----------------------------------------------------------------------
	void AxisManipulator::hide()
	{
		bool hidden = mHidden;

		Manipulator::hide();

		if(hidden)
			return;

		mScene->getOgreSceneManager()->getRootSceneNode()->removeChild(mSceneNode);
	}

	//-----------------------------------------------------------------------
	void AxisManipulator::show()
	{
		bool hidden = mHidden;

		Manipulator::show();

		if(!hidden)
			return;

		mScene->getOgreSceneManager()->getRootSceneNode()->addChild(mSceneNode);
	}
	// 鼠标移动
	bool AxisManipulator::onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{

		//do a ray cast and see if we can find handles
		float screenX = (float)moveDistance.x / (float)view->getWndRect().Width();
		float screenY = (float)moveDistance.y / (float)view->getWndRect().Height();
		if(mActive && mHasUpdated && mActiveHandle && mManipulating)
			performManipulation(view , -screenX , -screenY , mActiveHandle->mMode);

		//only check if we've moved on or off a handle if we're not manipulating
		if(!mManipulating)
		{
			CPoint point(GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam));
			float screenX = (float)point.x / (float)view->getWndRect().Width();
			float screenY = (float)point.y / (float)view->getWndRect().Height();

			Ray mouseRay = view->getCamera()->getCameraToViewportRay(screenX, screenY);
			RaySceneQuery *query = mScene->getOgreSceneManager()->createRayQuery(mouseRay);
			RaySceneQueryResult& queryResult = query->execute();

			AxisManipulatorHandle* handle = NULL;
			bool foundHandle = false;

			// Get result and check for AxisManipulatorHandle
			for(RaySceneQueryResult::iterator itr = queryResult.begin(); itr != queryResult.end(); ++itr)
			{
				if(itr->movable)
				{
					MovableObject *movable = itr->movable;
					AxisManipulatorHandle **pHandle = (AxisManipulatorHandle **)any_cast<AxisManipulatorHandle*>(&movable->getUserAny());
					if(pHandle)
					{
						handle = *pHandle;
						RayCollisionInfo colInfo;
						//perform a more precise collision test here
						if((*pHandle)->mCol->collide(mouseRay, colInfo))
						{
							foundHandle = true;
							break;
						}
					}
					else
					{
						handle = 0;
					}
				} // if
			} //for

			mScene->getOgreSceneManager()->destroyQuery(query);

			if(foundHandle)
				mActiveHandle = handle;
			else
				mActiveHandle = NULL;

		}

		return true;
	}
	// 子视口消息
	bool AxisManipulator::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		//check if mouse pressed and over a handle
		switch(message)
		{
		case WM_LBUTTONDOWN:
			if(mActiveHandle && !mManipulating)
			{
				//begin manipulating
				if(mObserver)
					mObserver->handleManipulatorStartManipulating();
				mManipulating = true;
			}
			break;
		case WM_LBUTTONUP:
			//check if mouse pressed and over a handle
			if(mManipulating)
			{
				//begin manipulating
				mManipulating = false;
				if(mObserver)
					mObserver->handleManipulatorStopManipulating();
			}
			break;
		}
		return true;
	}

	//-----------------------------------------------------------------------
	void AxisManipulator::activate()
	{
		Manipulator::activate();
		mHasUpdated = false;
	}

	//-----------------------------------------------------------------------
	void AxisManipulator::deactivate()
	{
		Manipulator::deactivate();
		mHasUpdated = false;
	}

	//-----------------------------------------------------------------------
	void AxisManipulator::update(IChildView *view, const Vector3& position, const Quaternion& rotate)
	{
		//rescale ourselves relative to the distance to the camera
		mCameraDistance = Vector3(view->getCamera()->getDerivedPosition() - position).length();
		float scaleFactor = mCameraDistance / 8.0f;
		mSceneNode->setScale(Vector3(scaleFactor, scaleFactor, scaleFactor));
		mSceneNode->setPosition(position);
		mSceneNode->setOrientation(rotate);
		view->getCamera()->getDerivedOrientation().ToAxes(mCamXAxis, mCamYAxis, mCamZAxis);
		mHasUpdated = true;
	}

	//-----------------------------------------------------------------------
	void AxisManipulator::performManipulation(IChildView *view , float relativeX, float relativeY, AxisMode mode)
	{
		if(!mObserver)
			return;

		Vector3 trans = Vector3::ZERO;
		Quaternion rotate = Quaternion::IDENTITY;
		Vector3 scale = Vector3::ZERO;

		bool translated = false;
		bool rotated = false;
		bool scaled = false;

		if(mode == AM_TRANSLATE_X || mode == AM_TRANSLATE_XY || mode == AM_TRANSLATE_XZ)
		{
			float xyPlane = (mCamZAxis.y < 0 ? -1 : 1);
			float yAxis = (mCamXAxis.z < 0 ? 1 : -1);
			trans.x += relativeX * mCamXAxis.x;
			trans.x += relativeY * (1 - mCamXAxis.x) * xyPlane * yAxis;
			translated = true;
		}

		if(mode == AM_TRANSLATE_Y || mode == AM_TRANSLATE_XY || mode == AM_TRANSLATE_YZ)
		{
			trans.y += -relativeY * (mCamYAxis.y > 0 ? 1 : -1);
			translated = true;
		}

		if(mode == AM_TRANSLATE_Z || mode == AM_TRANSLATE_XZ || mode == AM_TRANSLATE_YZ)
		{
			float yAxis = (mCamZAxis.z < 0 ? -1 : 1);
			trans.z += relativeX * mCamXAxis.z;
			trans.z += relativeY * (1.0f - mCamXAxis.z) * yAxis;
			translated = true;
		}

		if(translated)
		{
			trans = trans * (mCameraDistance);
			mObserver->handleManipulatorTranslate(trans);
			return;
		}

		if(mode == AM_ROTATE_X) 
		{
			rotate.FromAngleAxis(Radian(relativeY * ROTATION_SPEED), Vector3::UNIT_X);
			rotated = true;
		} 
		else if(mode == AM_ROTATE_Y) 
		{
			rotate.FromAngleAxis(Radian(relativeX * ROTATION_SPEED), Vector3::UNIT_Y);
			rotated = true;
		} 
		else if(mode == AM_ROTATE_Z) 
		{
			rotate.FromAngleAxis(Radian(relativeY * ROTATION_SPEED), Vector3::UNIT_Z);
			rotated = true;
		}

		if(rotated)
		{
			mObserver->handleManipulatorRotate(rotate);
			return;
		}

		float div = mCameraDistance / 4.0f;
		float x = relativeX / div;
		float y = relativeY / div;

		if(mode == AM_SCALE_X) 
		{
			scale.x += x;
			scaled = true;
		} 
		else if(mode == AM_SCALE_Y) 
		{
			scale.y += -y;
			scaled = true;
		} 
		else if(mode == AM_SCALE_Z) 
		{
			scale.z += -x;
			scaled = true;
		} 
		else if(mode == AM_SCALE_ALL) 
		{
			scale.x += x;
			scale.y += x;
			scale.z += x;
		}

		if(scaled)
		{
			mObserver->handleManipulatorScale(scale);
			return;
		}
	}

}
