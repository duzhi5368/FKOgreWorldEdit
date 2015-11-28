#pragma once
#include "NullObjectVisible.h"
namespace Ogre
{
	class CameraObject;
	class IScene;
	// 场景对象的可见对象，用来在编辑器里选择
	class DISPLAYLAYER_API CameraObjectVisible : public NullObjectVisible
	{
	public:
		CameraObjectVisible(CameraObject *obj);
		virtual ~CameraObjectVisible(void);

		// 显示边框
		virtual void showBorder(bool show);

	};
};