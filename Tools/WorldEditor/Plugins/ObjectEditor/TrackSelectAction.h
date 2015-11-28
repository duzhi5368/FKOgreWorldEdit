#pragma once

#include "ObjectSelectAction.h"
namespace Ogre
{

	// 轨迹选择操作
	class TrackSelectAction : public ObjectSelectAction
	{
	public:
		TrackSelectAction(AnimationTrackObject *obj);
		virtual ~TrackSelectAction(void);
	protected:
		// 选择一个MO
		virtual bool selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject);
		// 在选择之前先反选所有物件
		virtual void deselectAllObject();
		// 没有选择
		virtual void onNoSelect();

		// 动画轨迹对象
		AnimationTrackObject *m_object;

	};

};