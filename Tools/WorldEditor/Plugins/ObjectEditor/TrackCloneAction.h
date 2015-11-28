#pragma once
#include "ObjectCloneAction.h"

namespace Ogre
{
	// 轨迹克隆操作
	class TrackCloneAction : public ObjectCloneAction
	{
	public:
		TrackCloneAction(AnimationTrackObject *obj);
		virtual ~TrackCloneAction(void);

		// 访问这个对象，当返回false时不再遍历后面的对象
		virtual bool visit(ISceneObject *obj);

	protected:
		// 动画轨迹对象
		AnimationTrackObject *m_object;
	};

};