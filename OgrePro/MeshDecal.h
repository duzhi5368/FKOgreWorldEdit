/******************************************************************
** 文件名:	DecalSystem.h
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:30
** 描  述:
******************************************************************/
#pragma once
#include "SimpleDecal.h"
namespace Ogre
{
	class MeshDecalClipper;

	class DecalObject : public SimpleRenderable
	{
	public:
		DecalObject();
		virtual ~DecalObject();

		Real getSquaredViewDepth(const Camera* cam) const;
		Real getBoundingRadius(void) const { return m_radius; }

		// 填充Decal数据
		void fillData(MeshDecalClipper *clipper);

	protected:
		/** Override this method to prevent parent transforms (rotation,translation,scale)
		*/
		void getWorldTransforms( Matrix4* xform ) const;
		/** @copydoc Renderable::getWorldOrientation */
		const Quaternion& getWorldOrientation(void) const;
		/** @copydoc Renderable::getWorldPosition */
		const Vector3& getWorldPosition(void) const;
		Real m_radius;
	};

	// Mesh贴花实现
	class MeshDecal : public SimpleDecal
	{
	public:
		MeshDecal(SceneManager *mgr , const String &name);
		virtual ~MeshDecal();

		// 获得贴花的类型
		virtual const DecalType getType()const
		{
			return DT_MESH;
		}


	protected:
		// 当属性改变了，需要重构贴花的时候
		virtual void updateDecal();

		// 填充Decal数据
		void fillData(MeshDecalClipper *clipper);
		// 贴花平面
		Plane m_decalPlane;
		// 贴花对象
		DecalObject* m_decal;
	};

};