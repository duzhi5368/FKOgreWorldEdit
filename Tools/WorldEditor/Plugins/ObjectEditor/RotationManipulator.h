#ifndef __RotationManipulator_H
#define __RotationManipulator_H
#include "AxisManipulator.h"
namespace Ogre 
{

	/** This manipulator is used to translate an object along a particular axis. It loads visible representation
	of the axis, as well as a collision shape so users can click and drag on an axis mesh.
	*/
	class RotationManipulator : public AxisManipulator
	{
	public:

		enum AxisType
		{
			AT_X = 0,
			AT_Y = 1,
			AT_Z = 2,
			AT_LAST = 3
		};

		/** 
		*/
		RotationManipulator();

		/** 
		*/
		virtual ~RotationManipulator();


		/** 
		*/
		virtual bool init(IScene* scene , const String &name);

		/** called each frame start while there's something being manipulated by the manipulator
		deltaTime - time since last frame
		position - the position of the object being manipulated
		*/
		virtual void update(IChildView *view, const Vector3& position, const Quaternion& rotate);

	protected:

		SceneNode* mNode[3];
		Entity* mEnt[3];
		ICollisionEntity* mCol[3];
		AxisManipulatorHandle* mHandle[3];

	};
}
#endif