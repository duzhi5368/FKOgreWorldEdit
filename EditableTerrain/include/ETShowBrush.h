/*
Create by 邓迎雪 2007-07-30
画地形选区
*/
#ifndef __ETSHOWBRUSH_H__
#define __ETSHOWBRUSH_H__

//#include "Impl/ETIndexHandler.h"

namespace ET
{
	class _ETManagerExport ShowBrush : public Ogre::SimpleRenderable
	{
	public:
		ShowBrush();
		virtual ~ShowBrush(void);
		
		virtual void draw(Ogre::RenderOperation *op, size_t startx, size_t startz, size_t endx, size_t endz) = 0;

		// 基类纯虚
		Ogre::Real getSquaredViewDepth(const Ogre::Camera *cam) const;
		Ogre::Real getBoundingRadius() const;

	protected:
		Ogre::IndexData *mIndexData;
		//ET::Impl::IndexHandler *mIndexHandler;
	};
	
	class _ETManagerExport ShowCircleBrush : public ShowBrush
	{
	public:
		ShowCircleBrush();
		virtual ~ShowCircleBrush(void);
		
		virtual void draw(Ogre::RenderOperation *op, size_t startx, size_t startz, size_t endx, size_t endz);
	};

}
#endif