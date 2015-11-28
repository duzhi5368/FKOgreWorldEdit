#include "ETPrerequisites.h"
#include "ETShowBrush.h"

namespace ET
{
	ShowBrush::ShowBrush()
	{
		//mIndexData = new Ogre::IndexData;
		//mIndexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
		//	Ogre::HardwareIndexBuffer::IT_16BIT, 600, Ogre::HardwareBuffer::HBU_DYNAMIC);

		this->setMaterial("BaseWhiteNoLighting");
		
	}

	ShowBrush::~ShowBrush(void)
	{
	}

	Ogre::Real ShowBrush::getSquaredViewDepth(const Ogre::Camera *cam) const
	{
		const Ogre::Vector3 vMin = mBox.getMinimum();
		const Ogre::Vector3 vMax = mBox.getMaximum();
		const Ogre::Vector3 vMid = ((vMin - vMax) * 0.5) + vMin;
		const Ogre::Vector3 vDist = cam->getDerivedPosition() - vMid;

		return vDist.squaredLength();
	}

	Ogre::Real ShowBrush::getBoundingRadius(void) const
	{
		return Ogre::Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}

	ShowCircleBrush::ShowCircleBrush()
	{	
	}

	ShowCircleBrush::~ShowCircleBrush(void)
	{
	}

	void ShowCircleBrush::draw(Ogre::RenderOperation *op, size_t startx, size_t startz, size_t endx, size_t endz)
	{
		//unsigned int numIndexes = 0;
		//unsigned int step = 1 << 5;

		//unsigned short *pIdx = static_cast<unsigned short*>(mIndexData->indexBuffer->lock(
		//	0, mIndexData->indexBuffer->getSizeInBytes(), Ogre::HardwareBuffer::HBL_DISCARD));

		//for(unsigned int j = startx; j < endx; j += step)
		//{
		//	for (unsigned int i = startz; i < endz; i += step)
		//	{
		//		*pIdx++ = mIndexHandler->index(i, j);
		//		*pIdx++ = mIndexHandler->index(i, j+step);
		//		*pIdx++ = mIndexHandler->index(i+step, j);

		//		*pIdx++ = mIndexHandler->index(i, j+step);
		//		*pIdx++ = mIndexHandler->index(i+step, j+step);
		//		*pIdx++ = mIndexHandler->index(i+step, j);

		//		numIndexes += 6;
		//	}
		//}

		//mIndexData->indexBuffer->unlock();
		//mIndexData->indexCount = numIndexes;
		//mIndexData->indexStart = 0;
		//
		//op->indexData = mIndexData;
		this->setRenderOperation(*op);
	}

}
