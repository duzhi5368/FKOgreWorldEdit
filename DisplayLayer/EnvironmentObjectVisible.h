#pragma once
#include "NullObjectVisible.h"
namespace Ogre
{
	class EnvironmentObject;
	class IScene;
	// 场景对象的可见对象，用来在编辑器里选择
	class DISPLAYLAYER_API EnvironmentObjectVisible : public NullObjectVisible
	{
	public:
		EnvironmentObjectVisible(EnvironmentObject *obj);
		virtual ~EnvironmentObjectVisible(void);


	};
};