#pragma once
#include "ObjectDeleteAction.h"

namespace Ogre
{
	// 轨迹删除操作
	class TrackDeleteAction : public ObjectDeleteAction
	{
	public:
		TrackDeleteAction(AnimationTrackObject *obj);
		virtual ~TrackDeleteAction(void);

		// 访问这个对象，当返回false时不再遍历后面的对象
		virtual bool visit(ISceneObject *obj);

		// 删除物件
		virtual void deleteObject();

	protected:
		// 动画轨迹对象
		AnimationTrackObject *m_object;
	};

};