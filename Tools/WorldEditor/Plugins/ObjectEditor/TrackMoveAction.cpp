#include "StdAfx.h"
#include "TrackMoveAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackMoveAction::TrackMoveAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackMoveAction::~TrackMoveAction(void)
	{
	}
	// 访问这个对象，当返回false时不再遍历后面的对象
	bool TrackMoveAction::visit(ISceneObject *obj)
	{
		// 只有保存着动画指针的才可以选择
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			obj->translate(m_translate);
		}
		return true;
	}

	// 获得所有选中物体的平均中心点
	bool TrackMoveAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center , m_object);
	}
};