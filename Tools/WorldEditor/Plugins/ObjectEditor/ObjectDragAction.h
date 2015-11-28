#pragma once
#include "ObjectAction.h"
#include "DragManipulator.h"
namespace Ogre
{
	// 物件延地面拖曳操作
	class ObjectDragAction : public ObjectAction
		, public DragManipulator
		, public IMapManager::ObjectVisitor
		, public ManipulatorObserver
	{
	public:
		ObjectDragAction();
		virtual ~ObjectDragAction(void);
		// 访问这个对象，当返回false时不再遍历后面的对象
		virtual bool visit(ISceneObject *obj);


		virtual void handleManipulatorMove(const Vector3& worldPosition);

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

		// 获得所有选中物体的平均中心点
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

		// 需要移动的距离
		Vector3 m_translate;
	};
};