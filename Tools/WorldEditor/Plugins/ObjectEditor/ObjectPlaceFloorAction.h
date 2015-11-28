#pragma once
#include "ObjectAction.h"
namespace Ogre
{
	// 物件贴地操作
	class ObjectPlaceFloorAction : public ObjectAction
		, public IChildView::Listener
		, public IMapManager::ObjectVisitor
	{
	public:
		ObjectPlaceFloorAction();
		virtual ~ObjectPlaceFloorAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// 访问这个对象，当返回false时不再遍历后面的对象
		virtual bool visit(ISceneObject *obj);

		// 执行这个操作
		virtual void execute(){placeObjectToFloor();};
		// 物件贴地
		void placeObjectToFloor();
	};
};