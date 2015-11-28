#ifndef __AxisManipulator_H
#define __AxisManipulator_H

#include "Manipulator.h"
namespace Ogre 
{

	//forward declaration
	class AxisManipulatorHandle;

	/** This manipulator is a base class for derived manipulators that will be working with axis, such as rotating, translating, etc. around an axis
	Provides functionality that is shared amongst those derived manipulators
	*/
	// 轴向操作器
	class AxisManipulator : public Manipulator
	{
	public:

		const static float ROTATION_SPEED;

		enum AxisMode
		{
			AM_TRANSLATE_X,
			AM_TRANSLATE_Y,
			AM_TRANSLATE_Z,
			AM_TRANSLATE_XY,
			AM_TRANSLATE_XZ,
			AM_TRANSLATE_YZ,
			AM_ROTATE_X,
			AM_ROTATE_Y,
			AM_ROTATE_Z,
			AM_SCALE_X,
			AM_SCALE_Y,
			AM_SCALE_Z,
			AM_SCALE_ALL,
			AM_UNKNOWN
		};

		/** 
		*/
		AxisManipulator();

		/** 
		*/
		virtual ~AxisManipulator();

		/** Hide the manipulator
		*/
		virtual void hide();

		/** Show the manipulator
		*/
		virtual void show();

		/** 
		*/
		virtual bool init(IScene* scene , const String &name);

		// 鼠标移动
		virtual bool onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

		/** The caller will call activate when the mouse is over the manipulator (as indicated by mouseMoved)
		and the user pressed the left mouse button
		*/
		virtual void activate();

		/** The caller will call deactivate when the manipulator is active and the user
		releases the mouse button
		*/
		virtual void deactivate();

		/** called each frame start while there's something being manipulated by the manipulator
		deltaTime - time since last frame
		position - the position of the object being manipulated
		*/
		virtual void update(IChildView *view, const Vector3& position, const Quaternion& rotate);


	protected:

		/** 
		*/
		void performManipulation(IChildView *view , float relativeX, float relativeY, AxisMode mode);

		float mCameraDistance;
		AxisManipulatorHandle* mActiveHandle;
		SceneNode* mSceneNode;
		bool mHasUpdated;
		Vector3 mCamXAxis;
		Vector3 mCamYAxis;
		Vector3 mCamZAxis;

		ICollisionWorld *mCollisionWorld;

	};

	/** This little class associates a scene node with a manipulator and collision shape
	*/
	class AxisManipulatorHandle : public UserDefinedObject
	{
	public:
		AxisManipulator* mManipulator;
		AxisManipulator::AxisMode mMode; //what action does this handle perform
		ICollisionEntity *mCol;
	};
}
#endif