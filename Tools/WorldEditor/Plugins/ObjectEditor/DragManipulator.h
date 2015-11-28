#ifndef __DragManipulator_H
#define __DragManipulator_H

#include "AxisManipulator.h"

namespace Ogre 
{

	//forward declaration
	class DragContainer;

	/** This class handles dragging objects by casting a ray into the scene 
	*/
	class DragManipulator : public AxisManipulator
	{
	public:

		/** 
		*/
		DragManipulator();

		/** 
		*/
		virtual ~DragManipulator();

		/** 
		*/
		virtual bool init(IScene* scene , const String &name);

		//  Û±Í“∆∂Ø
		virtual bool onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

		/** called each frame start while there's something being manipulated by the manipulator
		deltaTime - time since last frame
		position - the position of the object being manipulated
		*/
		virtual void update(IChildView *view, const Vector3& position, const Quaternion& rotate);

	protected:

		Entity* mEnt;
		ICollisionEntity *mCol;
		AxisManipulatorHandle* mHandle;

	};

}
#endif