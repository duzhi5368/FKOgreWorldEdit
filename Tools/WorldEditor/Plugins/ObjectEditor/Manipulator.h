#ifndef __Manipulator_H
#define __Manipulator_H


namespace Ogre 
{

	//forward declaration
	class ManipulatorObserver;

	// ²Ù×÷Æ÷
	class Manipulator : public IChildView::Listener
	{
	public:

		/** 
		*/
		Manipulator()
		{
			mActive = false;
			mObserver = NULL;
			mHidden = false;
			mScene = NULL;
			mManipulating = false;
		}

		/** 
		*/
		virtual ~Manipulator() {}

		/** 
		*/
		virtual bool init(IScene* scene , const String &name)
		{
			mScene = scene;
			return true;
		}

		/** Hide the manipulator
		*/
		virtual void hide() { mHidden = true; }

		/** Show the manipulator
		*/
		virtual void show() { mHidden = false; }

		/** Activate is called when the manipulator is chosen by the user to be their active
		manipulator.
		*/
		virtual void activate() { mActive = true; }

		/** Deactivate is called when this manipulator is no longer to be the active manipulator
		*/
		virtual void deactivate() { mActive = false; }

		/** Called each frame start while there's something being manipulated by the manipulator
		position - the position of the object being manipulated
		xAxis, yAxis, zAxis - these are the axis that we're using - (the gizmo should be oriented to this axis)
		deltaTime - time since last frame
		*/
		virtual void update(IChildView *view, const Vector3& position, const Quaternion& rotate) {}

		/** 
		*/
		virtual void setManipulatorObserver(ManipulatorObserver* observer) { mObserver = observer; }

		/** 
		*/
		bool getIsActive() const { return mActive; }

		/** 
		*/
		bool getIsManipulating() const { return mManipulating; }

	protected:
		ManipulatorObserver* mObserver;
		bool mActive;
		bool mHidden;
		bool mManipulating;
		IScene *mScene;
	};

	/** This class is used to receive a callback when a manipulator has translated
	*/
	class ManipulatorObserver
	{
	public:

		/** 
		*/
		virtual void handleManipulatorTranslate(const Vector3& translation) {}

		/** 
		*/
		virtual void handleManipulatorRotate(const Quaternion& rotation) {}

		/** 
		*/
		virtual void handleManipulatorScale(const Vector3& scale) {}

		/** Unlike translate, move is not a translate, it's a world position to set our position to
		*/
		virtual void handleManipulatorMove(const Vector3& worldPosition) {}

		/** This should be implemented by the observer in order to perform ray collision detection if needed
		by the manipulator
		*/
		virtual bool manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection) { return false; }

		/** This is called when the manipulator begins manipulating an object
		*/
		virtual void handleManipulatorStartManipulating() {}

		/** 
		*/
		virtual void handleManipulatorStopManipulating() {}

	};
}
#endif