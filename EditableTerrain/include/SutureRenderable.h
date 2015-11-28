#pragma once

namespace Ogre
{

	class Tile;
	// 地形LOD边缘缝合处的渲染器
	class SutureRenderable : public Renderable
	{
	public:
		SutureRenderable();
		virtual ~SutureRenderable(void);

		virtual const MaterialPtr& getMaterial(void) const;
		virtual void getRenderOperation(RenderOperation& op);
		virtual void getWorldTransforms(Matrix4* xform) const;
		virtual const Quaternion& getWorldOrientation(void) const;
		virtual const Vector3& getWorldPosition(void) const;
		virtual Real getSquaredViewDepth(const Camera* cam) const;
		virtual const LightList& getLights(void) const;

		void _updateCustomGpuParameter(const GpuProgramParameters::AutoConstantEntry& constEntry,
			GpuProgramParameters* params) const;


		// 设置缝合索引数据
		void setSutureData(Tile *tile , IndexData *sutureData)
		{
			m_tile = tile;
			m_sutureData = sutureData;
		}
	protected:
		// 所属的地形Tile
		Tile *m_tile;

		// 缝合数据
		IndexData *m_sutureData;
	};

};