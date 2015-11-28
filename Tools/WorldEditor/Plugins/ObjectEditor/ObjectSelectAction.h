#pragma once
#include "ObjectAction.h"
#include "SelectionRectangle.h"
namespace Ogre
{
	// 选择物件操作
	class ObjectSelectAction : public ObjectAction
		, public IChildView::Listener
	{
	public:
		ObjectSelectAction();
		virtual ~ObjectSelectAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	protected:
		// 执行体积查询
		virtual void volSelection(Camera *camera , const Vector2 &first, const Vector2 &second);
		// 执行射线查询
		virtual void raySelection(Camera *camera , const Vector2 &first, const Vector2 &second , ISceneObject **selectObject);
		// 选择一个MO
		virtual bool selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject);

		// 在选择之前先反选所有物件
		virtual void deselectAllObject();

		// 没有选择
		virtual void onNoSelect(){};

		Vector2 mStart;
		Vector2 mStop;
		SceneManager *mSceneMgr;
		PlaneBoundedVolumeListSceneQuery *mVolQuery;
		RaySceneQuery *mRayQuery;
		SelectionRectangle *mRect;
	};
};