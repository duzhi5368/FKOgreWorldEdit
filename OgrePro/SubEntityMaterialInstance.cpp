#include "stdafx.h"
#include "SubEntityMaterialInstance.h"
namespace Ogre
{

	SubEntityMaterialInstance::SubEntityMaterialInstance (SubEntity *se) : 
MaterialInstance (),
mSubEntity (se)
{      
	initOriginalMaterial ();      
}

SubEntityMaterialInstance::~SubEntityMaterialInstance () 
{
	// Reset to the original material
	if(!mOriginalMat.isNull())
	{
		mSubEntity->setMaterialName (mOriginalMat->getName ());
	}
}

void SubEntityMaterialInstance::setMaterialName (String name) 
{
	clearCopyMaterial ();

	mSubEntity->setMaterialName (name);

	initOriginalMaterial ();

	setTransparency (mCurrentTransparency);
}

void SubEntityMaterialInstance::setTransparency (Real transparency) 
{
	MaterialInstance::setTransparency (transparency);

	if (mCurrentTransparency < 1.0f) 
	{
		if(mCopyMat.isNull())
			return;
		mSubEntity->setMaterialName (mCopyMat->getName ());
	}
	else 
	{
		if(mOriginalMat.isNull())
			return;
		mSubEntity->setMaterialName (mOriginalMat->getName ());
	}
}

void SubEntityMaterialInstance::initOriginalMaterial () 
{
	mOriginalMat = MaterialManager::getSingleton ().getByName (mSubEntity->getMaterialName ());
}
};