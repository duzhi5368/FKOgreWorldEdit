#include "StdAfx.h"
#include "TrackCloneAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackCloneAction::TrackCloneAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackCloneAction::~TrackCloneAction(void)
	{
	}
	// 访问这个对象，当返回false时不再遍历后面的对象
	bool TrackCloneAction::visit(ISceneObject *obj)
	{
		// 只有保存着动画指针的才可以选择
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
			KeyObjectData newData = data.m_dlgTrackTools->cloneKeyFrame(data);
			ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(newData.m_keyObject);
		}
		return true;
	}
};