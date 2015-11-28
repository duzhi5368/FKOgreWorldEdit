#pragma once
#include "ObjectMoveAction.h"

namespace Ogre
{
	// 轨迹移动操作
	class TrackMoveAction : public ObjectMoveAction
	{
	public:
		TrackMoveAction(AnimationTrackObject *obj);
		virtual ~TrackMoveAction(void);

		// 访问这个对象，当返回false时不再遍历后面的对象
		virtual bool visit(ISceneObject *obj);
	protected:
		// 获得所有选中物体的平均中心点
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

		// 动画轨迹对象
		AnimationTrackObject *m_object;
	};

};