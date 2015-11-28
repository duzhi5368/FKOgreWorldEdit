#include "stdafx.h"
#include "EntityMaterialInstance.h"
namespace Ogre
{

EntityMaterialInstance::EntityMaterialInstance (Entity *e) 
{
	if(e)
	{
		setEntity(e);
	}
}

EntityMaterialInstance::~EntityMaterialInstance () 
{
	clear();
}

void EntityMaterialInstance::clear () 
{
	std::vector<SubEntityMaterialInstance *>::iterator it, iend;
	iend = mSEMIs.end ();
	for (it = mSEMIs.begin (); it != iend; ++it) 
	{
		delete *it;
	}
	// add by FreeKnight 2007-8-24
	// 最后清空数组
	mSEMIs.clear();
}
// add by FreeKnight 2007-8-24
// 设置模型实体
void EntityMaterialInstance::setEntity(Entity *e)
{
	clear();
	if(e)
	{
		for (unsigned int i = 0; i < e->getNumSubEntities (); i++) 
		{
			mSEMIs.push_back (new SubEntityMaterialInstance (e->getSubEntity (i)));
		}
	}
}

void EntityMaterialInstance::setMaterialName (String name) 
{
	std::vector<SubEntityMaterialInstance *>::iterator it, iend;
	iend = mSEMIs.end ();
	for (it = mSEMIs.begin (); it != iend; ++it) 
	{
		(*it)->setMaterialName (name);
	}
}

void EntityMaterialInstance::setSceneBlending (SceneBlendType sbt) 
{
	std::vector<SubEntityMaterialInstance *>::iterator it, iend;
	iend = mSEMIs.end ();
	for (it = mSEMIs.begin (); it != iend; ++it) 
	{
		(*it)->setSceneBlending (sbt);
	}
}

void EntityMaterialInstance::setTransparency (Real transparency) 
{
	mCurrentTransparency = transparency;
	if (mCurrentTransparency > 1.0f)
		mCurrentTransparency = 1.0f;
	if (mCurrentTransparency < 0.0f)
		mCurrentTransparency = 0.0f;

	std::vector<SubEntityMaterialInstance *>::iterator it, iend;
	iend = mSEMIs.end ();
	for (it = mSEMIs.begin (); it != iend; ++it) 
	{
		(*it)->setTransparency (mCurrentTransparency);
	}
}

SubEntityMaterialInstancesIterator EntityMaterialInstance::getSubEntityMaterialInstancesIterator () 
{
	return SubEntityMaterialInstancesIterator (mSEMIs.begin (), mSEMIs.end ());
}
};