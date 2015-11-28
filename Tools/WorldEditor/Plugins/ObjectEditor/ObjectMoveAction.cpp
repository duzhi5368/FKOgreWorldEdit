#include "StdAfx.h"
#include "ObjectMoveAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectMoveAction::ObjectMoveAction()

	{
		// 设置轴向操作观察者为自己
		setManipulatorObserver(this);

		TranslationManipulator::init(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene() , "ObjectMoveAction");
		activate();

		
	}
	ObjectMoveAction::~ObjectMoveAction(void)
	{
	}

	// 访问这个对象，当返回false时不再遍历后面的对象
	bool ObjectMoveAction::visit(ISceneObject *obj)
	{
		obj->translate(m_translate);
		return true;
	}


	void ObjectMoveAction::handleManipulatorTranslate(const Vector3& translation)
	{
		IMapManager *map = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager();
		m_translate = translation;

		// 遍历所有被选择的对象
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

	/** This is called when the manipulator begins manipulating an object
	*/
	void ObjectMoveAction::handleManipulatorStartManipulating()
	{

	}

	/** 
	*/
	void ObjectMoveAction::handleManipulatorStopManipulating() 
	{

	}


	bool ObjectMoveAction::manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection)
	{
		return mScene->collide(ray , true , false , intersection);
	};


	// 准备更新窗口
	bool ObjectMoveAction::preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt)
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
	bool ObjectMoveAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center);
	}
};