#include "StdAfx.h"
#include "TrackPlaceFloorAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackPlaceFloorAction::TrackPlaceFloorAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackPlaceFloorAction::~TrackPlaceFloorAction(void)
	{
	}

	// 访问这个对象，当返回false时不再遍历后面的对象
	bool TrackPlaceFloorAction::visit(ISceneObject *obj)
	{
		// 只有保存着动画指针的才可以选择
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			Vector3 pos = obj->getWorldPosition();
			pos.y = 9999;
			Vector3 intersection;
			Ray ray(pos , Vector3::NEGATIVE_UNIT_Y);
			if(obj->getScene()->collide(ray , true , false , intersection))
			{
				obj->setWorldPosition(intersection);
			}
		}
		return true;
	}


};