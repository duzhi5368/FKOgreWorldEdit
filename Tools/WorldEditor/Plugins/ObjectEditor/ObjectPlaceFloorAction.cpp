#include "StdAfx.h"
#include "ObjectPlaceFloorAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectPlaceFloorAction::ObjectPlaceFloorAction()

	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);

	}
	ObjectPlaceFloorAction::~ObjectPlaceFloorAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// 子视口消息
	bool ObjectPlaceFloorAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
	bool ObjectPlaceFloorAction::visit(ISceneObject *obj)
	{
		Vector3 pos = obj->getWorldPosition();
		pos.y = 9999;
		Vector3 intersection;
		Ray ray(pos , Vector3::NEGATIVE_UNIT_Y);
		if(obj->getScene()->collide(ray , true , false , intersection))
		{
			obj->setWorldPosition(intersection);
		}
		return true;
	}

	// 物件贴地
	void ObjectPlaceFloorAction::placeObjectToFloor()
	{
		// 遍历所有被选择的对象
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

};