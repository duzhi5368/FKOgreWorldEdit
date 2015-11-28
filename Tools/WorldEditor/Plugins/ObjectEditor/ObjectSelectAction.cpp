#include "StdAfx.h"
#include "ObjectSelectAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectSelectAction::ObjectSelectAction()

	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);

		

		mSceneMgr = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->getOgreSceneManager();

		// 创建渲染矩形
		mRect = new SelectionRectangle("Selection SelectionRectangle");
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mRect);
		mRect->setVisible(false);

		// 创建体积查询
		mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(PlaneBoundedVolumeList());
		// 创建射线查询
		mRayQuery = mSceneMgr->createRayQuery(Ray());


	}
	ObjectSelectAction::~ObjectSelectAction(void)
	{
		mSceneMgr->destroyQuery(mRayQuery);
		mSceneMgr->destroyQuery(mVolQuery);
		OGRE_DELETE(mRect);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// 子视口消息
	bool ObjectSelectAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
			{
				SHORT xPos = GET_X_LPARAM(lParam); 
				SHORT yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				view->getWnd()->GetClientRect(&rect);
				mStart.x = (float)xPos / (float)rect.Width();
				mStart.y = (float)yPos / (float)rect.Height();
				mStop = mStart;
				mRect->setCorners(mStart , mStop);
				mRect->setVisible(true);
			}
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MOUSEMOVE:
			{
				SHORT xPos = GET_X_LPARAM(lParam); 
				SHORT yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				view->getWnd()->GetClientRect(&rect);
				mStop.x = (float)xPos / (float)rect.Width();
				mStop.y = (float)yPos / (float)rect.Height();
				mRect->setCorners(mStart , mStop);
				// 单击就选择物体
				if(message == WM_LBUTTONUP)
				{
					mRect->setVisible(false);
					
					Vector2 moved(mStop - mStart);
					if(std::min(abs(moved.x) , abs(moved.y)) < 0.01f)
					{
						raySelection(view->getCamera() , mStart , mStop , 0);
					}
					else
					{
						volSelection(view->getCamera() , mStart , mStop);
					}
				}
				// 双击就测试是否是轨迹物体，如果是轨迹物体，就激活轨迹编辑器，否则就直接选择物体
				else if(message == WM_LBUTTONDBLCLK)
				{
					mRect->setVisible(false);
					Vector2 moved(mStop - mStart);
					if(std::min(abs(moved.x) , abs(moved.y)) < 0.01f)
					{
						ISceneObject *obj = 0;
						raySelection(view->getCamera() , mStart , mStop , &obj);
						if(obj)
						{
							// 如果单独寻中了一个动画轨迹对象，则激活编辑这个轨迹
							if(obj->getType() == AnimationTrackObjectFactory::FACTORY_TYPE)
							{
								ObjectEditorPlugin::getSingleton().getDlgTrackTools()->activate(static_cast<AnimationTrackObject*>(obj));
							}
							else
							{
								ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(obj);
							}
						}
					}
				}
			}
			break;
		}
		return true;
	}

	// 执行射线查询
	void ObjectSelectAction::raySelection(Camera *camera , const Vector2 &first, const Vector2 &second , ISceneObject **selectObject)
	{
		if(selectObject)
		{
			*selectObject = 0;
		}
		// 先反选全部
		deselectAllObject();
		Ray ray = camera->getCameraToViewportRay(first.x, first.y);
		ListRayCheckInfo listInfo;
		// 把所有跟这个射线相交的对象放到listInfo中
		if(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->collide(ray , false , true , false , listInfo))
		{
			// 对所有对象执行精确查询
			for(ListRayCheckInfo::iterator iter = listInfo.begin() ; iter != listInfo.end() ; iter ++)
			{
				if(iter->obj->getSceneObjectVisible()->getCollisionEntity())
				{
					RayCollisionInfo objInfo;
					// 精确查询，如果发生了碰撞，就保存新的碰撞信息，否则就删除之
					if(iter->obj->getSceneObjectVisible()->getCollisionEntity()->collide(ray , objInfo))
					{
						iter->info = objInfo;
					}
					else
					{
						iter->info.distance = FLT_MAX;
					}
				}
			}
			// 重新按照新的位置排序
			std::sort(listInfo.begin() , listInfo.end());

			// 选择之
			if(!listInfo.empty())
			{
				if(listInfo.front().info.distance != FLT_MAX)
				{
					if(selectObject)
					{
						*selectObject = listInfo.front().obj;
					}
					else
					{
						ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(listInfo.front().obj);
					}
				}
			}
		}
		else
		{
			onNoSelect();
		}
	}
	// 执行体积查询
	void ObjectSelectAction::volSelection(Camera *camera , const Vector2 &first, const Vector2 &second)
	{
		float left = first.x, right = second.x,
			top = first.y, bottom = second.y;

		if (left > right)
			std::swap(left, right);

		if (top > bottom)
			std::swap(top, bottom);

		if ((right - left) * (bottom - top) < 0.0001)
		{
			onNoSelect();
			return;
		}

		// Convert screen positions into rays
		Ray topLeft = camera->getCameraToViewportRay(left, top);
		Ray topRight = camera->getCameraToViewportRay(right, top);
		Ray bottomLeft = camera->getCameraToViewportRay(left, bottom);
		Ray bottomRight = camera->getCameraToViewportRay(right, bottom);

		// The plane faces the counter clockwise position.
		PlaneBoundedVolume vol;
		vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         // front plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(100), topRight.getPoint(100)));         // top plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(100), topLeft.getPoint(100)));       // left plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), bottomRight.getPoint(100), bottomLeft.getPoint(100)));   // bottom plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), topRight.getPoint(100), bottomRight.getPoint(100)));     // right plane

		PlaneBoundedVolumeList volList;
		volList.push_back(vol);

		mVolQuery->setVolumes(volList);
		SceneQueryResult result = mVolQuery->execute();
		SceneQueryResultMovableList::iterator itr;
		// 先反选全部
		deselectAllObject();

		IMapManager::ListSceneObject listSceneObject;
		for (itr = result.movables.begin(); itr != result.movables.end(); ++itr)
		{
			selectObject(*itr , &listSceneObject);
		}
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(listSceneObject);
		if(listSceneObject.empty())
		{
			onNoSelect();
		}
	}

	// 选择一个MO
	bool ObjectSelectAction::selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject)
	{
		ISceneObject *sceneObject = ObjectEditorPlugin::getSingleton().getEditableSceneObject(obj);
		if(sceneObject)
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
		return false;
	}
	// 在选择之前先反选所有物件
	void ObjectSelectAction::deselectAllObject()
	{
		// 先反选全部
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectAllObject();
	}

};