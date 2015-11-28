#pragma once
#include "ObjectAction.h"
namespace Ogre
{
	// 插入模型实体
	class PlaceSceneObjectAction : public ObjectAction
		, public IChildView::Listener
	{
	public:
		PlaceSceneObjectAction(ISceneObject *obj);
		virtual ~PlaceSceneObjectAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	protected:
		// 被操作的对象
		ISceneObject *m_object;
	};
};