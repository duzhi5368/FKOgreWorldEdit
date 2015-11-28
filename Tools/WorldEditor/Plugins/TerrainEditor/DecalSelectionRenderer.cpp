#include "StdAfx.h"
#include "DecalSelectionRenderer.h"
namespace Ogre
{

	DecalSelectionRenderer::DecalSelectionRenderer(Selection *pSelection , TerrainBrush *brush)
		: SelectionRenderer(pSelection , brush)
		, m_decal(0)
	{
		m_decal = IDecalSystem::getSingleton().createDecal(DT_MESH , "DecalSelectionRenderer");
		m_decal->setVisible(false);

		m_texture = TextureManager::getSingleton().create("DecalSelectionRenderer_texture" , "General");
		m_texture->setTreatLuminanceAsAlpha(true);
		updateBrush();

		m_material = MaterialManager::getSingleton().create("DecalSelectionRenderer_material" , "General");
		m_material->getTechnique(0)->getPass(0)->setSceneBlending(SBT_ADD);
		m_material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		//m_material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(true);
		TextureUnitState *state = m_material->getTechnique(0)->getPass(0)->createTextureUnitState();
		state->setTextureName(m_texture->getName());
		m_material->load();
		m_decal->setMaterialName(m_material->getName());
	}

	DecalSelectionRenderer::~DecalSelectionRenderer(void)
	{
		IDecalSystem::getSingleton().destroyDecal(m_decal);
		MaterialManager::getSingleton().unload(m_material->getName());
		MaterialManager::getSingleton().remove(m_material->getName());
		TextureManager::getSingleton().unload(m_texture->getName());
		TextureManager::getSingleton().remove(m_texture->getName());
	}

	void DecalSelectionRenderer::setPositionPost(const GridVector2 &pos)
	{
		m_decal->setVisible(true);
		ETTerrain *terrain = m_selection->getTerrain();
		GridVector2 size = m_selection->getMax() - m_selection->getMin();
		m_decal->setSize(Vector2(terrain->getTerrainInfo()->getScaling().x * size.x , terrain->getTerrainInfo()->getScaling().z * size.y));

		m_decal->setPosition(terrain->getTerrainPos(pos.getX() , pos.getY()));
	};

	void DecalSelectionRenderer::updateBrush()
	{
		m_texture->unload();
		m_texture->loadImage(*m_brush->getImage());
	}
};
