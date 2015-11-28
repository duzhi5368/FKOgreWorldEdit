#ifndef __TranslationManipulator_H
#define __TranslationManipulator_H

#include "AxisManipulator.h"
namespace Ogre 
{

	/** This manipulator is used to translate an object along a particular axis. It loads visible representation
	of the axis, as well as a collision shape so users can click and drag on an axis mesh.
	*/
	class TranslationManipulator : public AxisManipulator
	{
	public:

		enum AxisType
		{
			AT_X = 0,
			AT_Y = 1,
			AT_Z = 2,
			AT_XY = 3,
			AT_XZ = 4,
			AT_YZ = 5,
			AT_LAST = 6
		};

		/** 
		*/
		TranslationManipulator();

		/** 
		*/
		virtual ~TranslationManipulator();

		/** 
		*/
		virtual bool init(IScene* scene , const String &name);

		/** called each frame start while there's something being manipulated by the manipulator
		deltaTime - time since last frame
		position - the position of the object being manipulated
		*/
		virtual void update(IChildView *view, const Vector3& position, const Quaternion& rotate);

	protected:
		// add by 王宏张 2007-7-12
		// 不要用魔术数，搞到缓存溢出
		SceneNode* mNode[AT_LAST];
		Entity* mEnt[AT_LAST];
		Entity* mConeEnt[AT_LAST];
		ICollisionEntity* mCol[AT_LAST];
		AxisManipulatorHandle* mHandle[AT_LAST];

	};
}
#endif