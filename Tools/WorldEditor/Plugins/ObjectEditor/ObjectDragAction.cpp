#include "StdAfx.h"
#include "ObjectDragAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectDragAction::ObjectDragAction()

	{
		// 设置轴向操作观察者为自己
		setManipulatorObserver(this);

		DragManipulator::init(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene() , "ObjectDragAction");
		activate();


		
	}
	ObjectDragAction::~ObjectDragAction(void)
	{
	}

	// 访问这个对象，当返回false时不再遍历后面的对象
	bool ObjectDragAction::visit(ISceneObject *obj)
	{
		obj->translate(m_translate);
		return true;
	}


	void ObjectDragAction::handleManipulatorMove(const Vector3& worldPosition)
	{
		Vector3 center;
		if(!getSelectedObjectsAvgCenter(center))
			return;
		m_translate = worldPosition - center;
		// 遍历所有被选择的对象
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

	/** This is called when the manipulator begins manipulating an object
	*/
	void ObjectDragAction::handleManipulatorStartManipulating()
	{
	}

	/** 
	*/
	void ObjectDragAction::handleManipulatorStopManipulating() 
	{
	}

	bool ObjectDragAction::manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection)
	{
		return ObjectEditorPlugin::getSingleton().collide(ray , true , false , intersection);
	};


	// 准备更新窗口
	bool ObjectDragAction::preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt)
	{
		IMapManager *map = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager();
		IMapManager::SelectedObjectsIterator iter = map->getSelectedObjectsIterator();
		if(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			Vector3 center;
			if(!getSelectedObjectsAvgCenter(center))
				return true;
			update(view, center, Quaternion::IDENTITY);
		}
		return true;
	}


	// 获得所有选中物体的平均中心点
	bool ObjectDragAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center);
	}
};