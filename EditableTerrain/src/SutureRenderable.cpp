#include "ETPrerequisites.h"
#include "SutureRenderable.h"
#include "ETTile.h"


namespace Ogre
{

	SutureRenderable::SutureRenderable()
		: m_tile(0)
		, m_sutureData(0)
	{
	}

	SutureRenderable::~SutureRenderable(void)
	{
	}

	const MaterialPtr& SutureRenderable::getMaterial(void) const
	{
		return m_tile->getMaterial();
	}
	void SutureRenderable::getRenderOperation(RenderOperation& op)
	{
		op.useIndexes = true;
		op.operationType = RenderOperation::OT_TRIANGLE_LIST;
		op.vertexData = m_tile->_getVertexData();
		op.indexData = m_sutureData;
	}
	void SutureRenderable::getWorldTransforms(Matrix4* xform) const
	{
		m_tile->getWorldTransforms(xform);
	}
	const Quaternion& SutureRenderable::getWorldOrientation(void) const
	{
		return m_tile->getWorldOrientation();
	}
	const Vector3& SutureRenderable::getWorldPosition(void) const
	{
		return m_tile->getWorldPosition();
	}
	Real SutureRenderable::getSquaredViewDepth(const Camera* cam) const
	{
		return m_tile->getSquaredViewDepth(cam);
	}
	const LightList& SutureRenderable::getLights(void) const
	{
		return m_tile->getLights();
	}
	void SutureRenderable::_updateCustomGpuParameter(const GpuProgramParameters::AutoConstantEntry& constEntry,
		GpuProgramParameters* params) const
	{
		m_tile->_updateCustomGpuParameter(constEntry , params);
	}
};