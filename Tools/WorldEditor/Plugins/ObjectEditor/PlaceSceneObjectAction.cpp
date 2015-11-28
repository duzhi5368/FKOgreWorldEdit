#include "StdAfx.h"
#include "PlaceSceneObjectAction.h"
#include "ISceneObjectVisible.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	PlaceSceneObjectAction::PlaceSceneObjectAction(ISceneObject *obj)
		: m_object(obj)
	{
		// 隐藏掉真正的实体，显示摆放时的实体
		m_object->getSceneObjectVisible()->showEntity(false);
		m_object->getSceneObjectVisible()->showPlacingEntity(true);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
	}
	PlaceSceneObjectAction::~PlaceSceneObjectAction(void)
	{
		m_object->getScene()->destroySceneObject(m_object);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// 子视口消息
	bool PlaceSceneObjectAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
			{
				short xPos = GET_X_LPARAM(lParam); 
				short yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				Vector3 pos;
				Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
					, yPos / (float)rect.Height());

				Vector3 posIntersect;
				if(ObjectEditorPlugin::getSingleton().collide(ray , true , false , posIntersect))
				{
					m_object->setWorldPosition(posIntersect);
					// 左键放下模型
					if(message == WM_LBUTTONDOWN)
					{
						// 显示回真正的实体
						m_object->getSceneObjectVisible()->showPlacingEntity(false);
						m_object->getSceneObjectVisible()->showEntity(true);
						// 克隆之
						ISceneObject *obj = m_object->clone(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene());
						// 克隆后的对象显示真正的实体
						obj->getSceneObjectVisible()->showPlacingEntity(false);
						obj->getSceneObjectVisible()->showEntity(true);
						//// 隐藏掉真正的实体，显示摆放时的实体
						m_object->getSceneObjectVisible()->showEntity(false);
						m_object->getSceneObjectVisible()->showPlacingEntity(true);
						ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectAllObject();
						ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(obj);
						// 如果是一个轨迹动画，就立刻进入编辑模式
						if(obj->getType() == AnimationTrackObjectFactory::FACTORY_TYPE)
						{
							ObjectEditorPlugin::getSingleton().getDlgTrackTools()->activate(static_cast<AnimationTrackObject*>(obj));
						}
					}
				}
			}
			break;
		}
		return true;
	}
}; 
