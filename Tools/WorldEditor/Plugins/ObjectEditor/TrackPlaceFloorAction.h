#pragma once
#include "ObjectPlaceFloorAction.h"

namespace Ogre
{
	// 轨迹贴地操作
	class TrackPlaceFloorAction : public ObjectPlaceFloorAction
	{
	public:
		TrackPlaceFloorAction(AnimationTrackObject *obj);
		virtual ~TrackPlaceFloorAction(void);

		// 访问这个对象，当返回false时不再遍历后面的对象
		virtual bool visit(ISceneObject *obj);

	protected:
		// 动画轨迹对象
		AnimationTrackObject *m_object;
	};

};