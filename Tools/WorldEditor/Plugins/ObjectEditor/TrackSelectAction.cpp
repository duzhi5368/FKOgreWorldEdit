#include "StdAfx.h"
#include "TrackSelectAction.h"
#include "ObjectEditorPlugin.h"
#include "DlgTrackTools.h"

namespace Ogre
{
	TrackSelectAction::TrackSelectAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackSelectAction::~TrackSelectAction(void)
	{
	}
	// 选择一个MO
	bool TrackSelectAction::selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject)
	{
		ISceneObject *sceneObject = obj_cast(obj->getUserAny());
		if(sceneObject)
		{
			// 只有保存着动画指针的才可以选择
			KeyObjectData data = safe_cast<KeyObjectData>(sceneObject->getUserAny());
			if(!data.isNull())
			{
				if(!listSceneObject)
				{
					ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(sceneObject);
				}
				else
				{
					listSceneObject->push_back(sceneObject);
				}
				return true;
			}
		}
		return false;
	}

	// 在选择之前先反选所有物件
	void TrackSelectAction::deselectAllObject()
	{
		// 遍历所有被选择的对象
		IMapManager::SelectedObjectsIterator iter = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsIterator();
		IMapManager::ListSceneObject listSceneObject;
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
			if(!data.isNull())
			{
				listSceneObject.push_back(obj);
			}
		}
		// 反选全部
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(listSceneObject);
	}

	// 没有选择
	void TrackSelectAction::onNoSelect()
	{
		// 反选
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(m_object);
		// 选择
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(m_object);

	}

};