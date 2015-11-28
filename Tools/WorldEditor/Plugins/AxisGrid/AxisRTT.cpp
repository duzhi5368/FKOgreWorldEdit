#include "StdAfx.h"
#include "AxisRTT.h"
namespace Ogre
{
	AxisRTT::AxisRTT()
		: EntityRTT("AxisRTT")
		, m_axisObject(0)
		, m_axisText(0)
		, m_worldEditor(0)
	{
	}

	AxisRTT::~AxisRTT(void)
	{
		destroy();
	}

	void AxisRTT::destroy()
	{
		if(m_worldEditor)
		{
			m_worldEditor->getDisplaySystem()->getMainSceneMgr()->destroyManualObject(m_axisObject);
			m_worldEditor->getDisplaySystem()->getMainSceneMgr()->destroyBillboardSet(m_axisText);

			m_axisObject = 0;
			m_axisText = 0;

			MaterialManager::getSingleton().remove(m_axisMat->getName());
			m_axisMat.setNull();
		}
		EntityRTT::destroy();
	}



	// 创建坐标轴标识
	ManualObject* AxisRTT::createAxis()
	{
		ManualObject* axisObject = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->createManualObject("AxisObject");
		// 红色x轴
		axisObject->begin(m_worldEditor->getDisplaySystem()->getColorMaterial(ColourValue::Red) , RenderOperation::OT_LINE_LIST);
		axisObject->position(0,0,0);
		axisObject->position(1,0,0);
		axisObject->end();
		// 绿色y轴
		axisObject->begin(m_worldEditor->getDisplaySystem()->getColorMaterial(ColourValue::Green) , RenderOperation::OT_LINE_LIST);
		axisObject->position(0,0,0);
		axisObject->position(0,1,0);
		axisObject->end();
		// 蓝色z轴
		axisObject->begin(m_worldEditor->getDisplaySystem()->getColorMaterial(ColourValue::Blue) , RenderOperation::OT_LINE_LIST);
		axisObject->position(0,0,0);
		axisObject->position(0,0,1);
		axisObject->end();

		return axisObject;
	}

	// 创建坐标轴界面
	void AxisRTT::createAxisText()
	{
		// 创建坐标轴
		m_axisObject = createAxis();
		m_nodeEntity->attachObject(m_axisObject);

		const float textSize = 0.38f;
		m_axisText = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->createBillboardSet("AxisText" , 3);
		m_axisText->setMaterialName("AxisText");
		m_axisText->setDefaultDimensions(0,0);
		Billboard *boardX = m_axisText->createBillboard(1 , 0 , 0);
		boardX->setDimensions(textSize,textSize);
		boardX->setTexcoordRect(0,0,0.25,1);

		Billboard *boardY = m_axisText->createBillboard(0 , 1 , 0);
		boardY->setDimensions(textSize,textSize);
		boardY->setTexcoordRect(0.25,0,0.5,1);
 
		Billboard *boardZ = m_axisText->createBillboard(0 , 0 , 1);
		boardZ->setDimensions(textSize,textSize);
		boardZ->setTexcoordRect(0.5,0,0.75,1);

		m_axisText->_updateBounds();

		m_nodeEntity->attachObject(m_axisText);
	}

	// 视口更新
	void AxisRTT::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		m_nodeEntity->setOrientation(evt.source->getCamera()->getRealOrientation().Inverse());
		m_target->update();
	}

	void AxisRTT::init(IWorldEditor *worldEditor)
	{
		destroy();
		EntityRTT::init(worldEditor->getDisplaySystem()->getMainSceneMgr() , 64 , 64);
		m_worldEditor = worldEditor;
		// 延迟创建RTT纹理
		delayInit();
		createAxisText();
		// 调整摄像机到实体面前
		adjustCamera();
		m_viewport->setOverlaysEnabled(false);
		m_viewport->setSkiesEnabled(false);
		m_viewport->setShadowsEnabled(false);
		m_viewport->setVisibilityMask(0xFF);
		m_axisMat = MaterialManager::getSingleton().create("AxisRTT" , ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Pass *pass = m_axisMat->getTechnique(0)->getPass(0);
		pass->setDepthCheckEnabled(false);
		pass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
		TextureUnitState* state = pass->createTextureUnitState(m_rtt->getName());
		state->setTextureFiltering(TFO_NONE);
		m_setedEntity = true;
	}

	// 调整摄像机到实体面前
	void AxisRTT::adjustCamera()
	{
		m_camera->setPosition(m_node->getPosition() + Vector3(0,0,2.5));

		m_camera->lookAt(m_node->getPosition());

		// 使用正交投影
		m_camera->setProjectionType(PT_ORTHOGRAPHIC);
		m_camera->setOrthoWindow(2.5 , 2.5);
	}
};