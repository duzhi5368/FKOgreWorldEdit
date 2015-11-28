#include "StdAfx.h"
#include "ObjectCloneAction.h"
#include "ObjectEditorPlugin.h"
#include "ObjectSelectAction.h"
namespace Ogre
{
	ObjectCloneAction::ObjectCloneAction()
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
	}
	ObjectCloneAction::~ObjectCloneAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// 子视口消息
	bool ObjectCloneAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYUP:
			{
				if(wParam == VK_DELETE)
				{
				}
			}
			break;
		}
		return true;
	}

	// 访问这个对象，当返回false时不再遍历后面的对象
	bool ObjectCloneAction::visit(ISceneObject *obj)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
		ISceneObject *clonedObject = obj->clone();
		clonedObject->translate(Vector3(0.5f , 0 , 0.5f));
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(clonedObject);
		return true;
	}

	// 克隆物件
	void ObjectCloneAction::cloneObject()
	{
		// 遍历所有被选择的对象
		IMapManager::SelectedObjectsIterator iter = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsIterator();
		IMapManager::ListSceneObject listSceneObject;
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			listSceneObject.push_back(obj);
		}
		for(IMapManager::ListSceneObject::iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			visit(*iter);
		}
	}

};