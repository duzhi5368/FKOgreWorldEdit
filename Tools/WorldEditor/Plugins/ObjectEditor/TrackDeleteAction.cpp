#include "StdAfx.h"
#include "TrackDeleteAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackDeleteAction::TrackDeleteAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackDeleteAction::~TrackDeleteAction(void)
	{
	}
	// 访问这个对象，当返回false时不再遍历后面的对象
	bool TrackDeleteAction::visit(ISceneObject *obj)
	{
		// 只有保存着动画指针的才可以选择
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
			data.m_dlgTrackTools->deleteKeyFrame(data);
		}
		return true;
	}


	// 删除物件
	void TrackDeleteAction::deleteObject()
	{
		ObjectDeleteAction::deleteObject();
		// 选择轨迹动画对象
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(m_object);
	}

};