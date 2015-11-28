#include "StdAfx.h"
#include "ObjectScaleAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectScaleAction::ObjectScaleAction()

	{
		// 设置轴向操作观察者为自己
		setManipulatorObserver(this);

		TranslationManipulator::init(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene() , "ObjectScaleAction");
		activate();

		
	}
	ObjectScaleAction::~ObjectScaleAction(void)
	{
	}

	// 访问这个对象，当返回false时不再遍历后面的对象
	bool ObjectScaleAction::visit(ISceneObject *obj)
	{
		obj->scale(m_translate);
		return true;
	}


	void ObjectScaleAction::handleManipulatorTranslate(const Vector3& translation)
	{
		IMapManager *map = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager();
		m_translate = Vector3::UNIT_SCALE + translation;

		// 遍历所有被选择的对象
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

	/** This is called when the manipulator begins manipulating an object
	*/
	void ObjectScaleAction::handleManipulatorStartManipulating()
	{

	}

	/** 
	*/
	void ObjectScaleAction::handleManipulatorStopManipulating() 
	{

	}


	bool ObjectScaleAction::manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection)
	{
		return mScene->collide(ray , true , false , intersection);
	};


	// 准备更新窗口
	bool ObjectScaleAction::preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt)
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
	bool ObjectScaleAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center);
	}
};