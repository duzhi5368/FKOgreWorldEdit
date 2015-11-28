#include "StdAfx.h"
#include "CameraObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "CameraObject.h"

namespace Ogre
{

	CameraObjectVisible::CameraObjectVisible(CameraObject *obj)
		: NullObjectVisible(obj)
	{
		destroyVisibleSphere();
		m_sphereColor = ColourValue::Red;
		createVisibleSphere();
	}

	CameraObjectVisible::~CameraObjectVisible(void)
	{
	}

	// ÏÔÊ¾±ß¿ò
	void CameraObjectVisible::showBorder(bool show)
	{
		m_showBorder = show;
		m_object->getOgreMovableObject()->setVisible(show);
	}

};