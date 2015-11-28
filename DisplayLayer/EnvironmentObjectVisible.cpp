#include "StdAfx.h"
#include "EnvironmentObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "EnvironmentObject.h"

namespace Ogre
{

	EnvironmentObjectVisible::EnvironmentObjectVisible(EnvironmentObject *obj)
		: NullObjectVisible(obj)
	{
		destroyVisibleSphere();
		m_sphereColor = ColourValue::Blue;
		createVisibleSphere();
	}

	EnvironmentObjectVisible::~EnvironmentObjectVisible(void)
	{
	}


};