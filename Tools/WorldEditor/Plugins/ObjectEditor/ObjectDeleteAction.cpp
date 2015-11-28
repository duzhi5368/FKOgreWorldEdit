#include "StdAfx.h"
#include "ObjectDeleteAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{
	ObjectDeleteAction::ObjectDeleteAction()
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
	}
	ObjectDeleteAction::~ObjectDeleteAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// 子视口消息
	bool ObjectDeleteAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
	bool ObjectDeleteAction::visit(ISceneObject *obj)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->destroySceneObject(obj);
		return true;
	}

	// 删除物件
	void ObjectDeleteAction::deleteObject()
	{
		// 遍历所有被选择的对象
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

};