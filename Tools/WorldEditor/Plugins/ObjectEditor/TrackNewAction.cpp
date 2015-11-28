#include "StdAfx.h"
#include "TrackNewAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{
	TrackNewAction::TrackNewAction(DlgTrackTools *tools)
		: m_dlgTrackTools(tools)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this , false);
	}
	TrackNewAction::~TrackNewAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// 子视口消息
	bool TrackNewAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYUP:
			{
				if(wParam == VK_DELETE)
				{
					m_dlgTrackTools->popBackKeyFrame();
					return false;
				} 
			}
			break;
		case WM_LBUTTONUP:
			{
				short xPos = GET_X_LPARAM(lParam); 
				short yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				Vector3 pos;
				Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
					, yPos / (float)rect.Height());

				Vector3 posIntersect;
				// 计算地面的坐标
				if(ObjectEditorPlugin::getSingleton().collide(ray , true , false , posIntersect))
				{
					AnimationTrackObject::KeyFrame keyFrame;
					keyFrame.m_translate = posIntersect;
					keyFrame.m_length = 0.0f;
					// 如果有关键帧，就计算新的方向和时间
					AnimationTrackObject *aniObject = m_dlgTrackTools->getAnimationTrackObject();
					size_t count = aniObject->getKeyFrameCount();
					if(count > 0)
					{
						size_t lastIndex = count - 1;
						AnimationTrackObject::KeyFrame lastKeyFrame = aniObject->getKeyFrame(lastIndex);
						Vector3 dir = keyFrame.m_translate - lastKeyFrame.m_translate;
						// 计算距离
						float distance = dir.normalise();
						// 计算转向的方向
						lastKeyFrame.m_rotate = MathEx::DirectionToQuaternion(dir);
						// 刚建立的动画速度初始为10米/秒
						keyFrame.m_length = distance / 10.0f;
						// 修改最后一个节点的方向
						aniObject->modifyKeyFrame(lastIndex , lastKeyFrame.m_length , lastKeyFrame.m_translate , lastKeyFrame.m_scale , lastKeyFrame.m_rotate);
					}
					// 在尾部增加一个关键帧
					KeyObjectData newData = m_dlgTrackTools->pushBackKeyFrame(keyFrame);
					deselectAllObject();
					ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(newData.m_keyObject);

				}
			}
			break;
		}
		return true;
	}


	// 在选择之前先反选所有物件
	void TrackNewAction::deselectAllObject()
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

};