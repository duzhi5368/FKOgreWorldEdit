#include "StdAfx.h"
#include "TrackRotateAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackRotateAction::TrackRotateAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackRotateAction::~TrackRotateAction(void)
	{
	}
	// 访问这个对象，当返回false时不再遍历后面的对象
	bool TrackRotateAction::visit(ISceneObject *obj)
	{
		// 只有保存着动画指针的才可以选择
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			obj->rotate(m_rotate);
		}
		return true;
	}

	// 获得所有选中物体的平均中心点
	bool TrackRotateAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center , m_object);
	}
};