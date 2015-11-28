#pragma once
#include "ObjectAction.h"
#include "RotationManipulator.h"
namespace Ogre
{
	// 物件移动操作
	class ObjectRotateAction : public ObjectAction
		, public RotationManipulator
		, public IMapManager::ObjectVisitor
		, public ManipulatorObserver
	{
	public:
		ObjectRotateAction();
		virtual ~ObjectRotateAction(void);
		// 访问这个对象，当返回false时不再遍历后面的对象
		virtual bool visit(ISceneObject *obj);

		virtual void handleManipulatorRotate(const Quaternion& rotation);

		/** This is called when the manipulator begins manipulating an object
		*/
		virtual void handleManipulatorStartManipulating();

		/** 
		*/
		virtual void handleManipulatorStopManipulating();

		virtual bool manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection);

		// 准备更新窗口
		virtual bool preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt);

	protected:
		Quaternion m_rotate;

		// 获得所有选中物体的平均中心点
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

	};
};