#pragma once
#include "ObjectAction.h"
class DlgTrackTools;
namespace Ogre
{
	// 新建轨迹序列操作
	class TrackNewAction : public ObjectAction
		, public IChildView::Listener
	{
	public:
		TrackNewAction(DlgTrackTools *tools);
		virtual ~TrackNewAction(void);
		// 子视口消息
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	protected:
		// 动画轨迹编辑器
		DlgTrackTools *m_dlgTrackTools;
		// 在选择之前先反选所有物件
		virtual void deselectAllObject();

	};
};