/******************************************************************
** 文件名:	DecalSystem.h
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:30
** 描  述:	法线贴花的实现，根据所在位置的法线自动调整贴花朝向
******************************************************************/
#pragma once
#include "SimpleDecal.h"
namespace Ogre
{
	class NormalDecal : public SimpleDecal
	{
	public:
		NormalDecal(SceneManager *mgr , const String &name);
		virtual ~NormalDecal();

		// 获得贴花的类型
		virtual const DecalType getType()const
		{
			return DT_NORMAL;
		}

	protected:
		// 当属性改变了，需要重构贴花的时候
		virtual void updateDecal();
		// 贴花对象
		ManualObject *m_decal;
		// 贴花平面
		Plane m_decalPlane;
	};

};