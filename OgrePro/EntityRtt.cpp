/******************************************************************
** 文件名:	EntityRtt.cpp
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-13 15:17:27
** 描  述:
******************************************************************/

#include "stdafx.h"
#include "EntityRtt.h"
namespace Ogre
{

	void EntityRTTListener::preRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		m_rtt->m_node->setVisible(true);
	}

	void EntityRTTListener::postRenderTargetUpdate(const RenderTargetEvent& evt)
	{
		m_rtt->m_node->setVisible(false);
	}

	//构造和析构
	EntityRTT::EntityRTT(const String &name)
		: m_name(name)
		, m_inited(false)
		, m_createdTexture(false)
		, m_setedEntity(false)
		, m_enabled(false)
		, m_sceneMgr(0)
		, m_textureWidth(0)
		, m_textureHeight(0)
		, m_textureFormat(PF_UNKNOWN) 
		, m_bgColor(ColourValue::ZERO)
		, m_nearclipDist(0)
		, m_farclipDist(0)
		, m_fovy(0)
		, m_ratio(0)
		, m_entity(0)
		, m_cloneEntity(0)
		, m_viewport(0)
		, m_camera(0)
		, m_target(0)
		, m_node(0)
		, m_nodeEntity(0)
		, m_listener(0)
		, m_animationController(0)
	{
		// 默认的listener
		m_listener = new EntityRTTListener(this);
		m_animationController = new EntityAnimationController;
	}

	EntityRTT::~EntityRTT()
	{
		destroy();
		OGRE_DELETE(m_animationController);
	}

	// 初始化RTT
	void EntityRTT::init(SceneManager *sceneMgr
		, ushort textureWidth
		, ushort textureHeight
		, PixelFormat textureFormat
		, const ColourValue& bgColor
		, float nearclipDist
		, float farclipDist
		, const Radian &fovy
		, float ratio)
	{
		if(m_inited && m_createdTexture)
			return;
		m_sceneMgr = sceneMgr;
		m_textureWidth = textureWidth;
		m_textureHeight = textureHeight;
		m_textureFormat = textureFormat;
		m_nearclipDist = nearclipDist;
		m_farclipDist = farclipDist;
		m_fovy = fovy;
		m_ratio = ratio;
		m_inited = true;
	}
	// 创建RTT纹理和视口
	void EntityRTT::createViewport()
	{
		if(m_viewport)
			return;
		// 创建纹理
		m_rtt = TextureManager::getSingleton().createManual(m_name + "_RTT", 
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 
			m_textureWidth, m_textureHeight, 0, m_textureFormat, TU_RENDERTARGET );
		m_target = m_rtt->getBuffer()->getRenderTarget();
		// 默认不RTT
		m_target->setActive(false);
		m_enabled = false;
		// 侦听器
		if(m_listener)
		{
			m_target->addListener(m_listener);
		}
		// 创建摄像机
		m_camera = m_sceneMgr->createCamera(m_name + "_camera");
		m_camera->setFOVy(m_fovy);
		m_camera->setNearClipDistance(m_nearclipDist);
		m_camera->setFarClipDistance(m_farclipDist);
		m_camera->setAspectRatio(m_ratio);

		// 创建视口
		m_viewport = m_target->addViewport(m_camera);
		m_viewport->setOverlaysEnabled(false);
		m_viewport->setBackgroundColour(m_bgColor);
		m_viewport->setClearEveryFrame(true);
	}

	// 延迟初始化
	void EntityRTT::delayInit()
	{
		if(m_createdTexture || !m_inited)
			return;
		// 创建RTT纹理和视口
		createViewport();
		// 创建节点
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(m_name);
		m_nodeEntity = m_node->createChildSceneNode(m_name + "_Entity");

		// RTT将渲染在这里
		m_node->setPosition(0 , -10000 , 0);

		m_createdTexture = true;
	}

	// 销毁RTT
	void EntityRTT::destroy()
	{
		if(!m_createdTexture)
			return;
		destroyEntity();
		destroyViewport();
		OGRE_DELETE(m_listener);
		m_sceneMgr->destroySceneNode(m_nodeEntity->getName());
		m_sceneMgr->destroySceneNode(m_node->getName());
		m_inited = false;
		m_createdTexture = false;
	}

	// 销毁视口
	void EntityRTT::destroyViewport()
	{
		// 销毁Viewport
		if(m_viewport)
		{
			m_target->setActive(false);
			m_target->removeAllListeners();
			m_target->removeAllViewports();
			m_sceneMgr->destroyCamera(m_camera);
			if(!m_rtt.isNull())
			{
				//TextureManager::getSingleton().unload(m_rtt->getName());
				//TextureManager::getSingleton().remove(m_rtt->getName());
				m_rtt.setNull();
			}
			m_target = 0;
			m_viewport = 0;
			m_camera = 0;
		}
	}

	// 获得一个不重复的实体名字
	String EntityRTT::getUniqueEntityName(const String &preName)
	{
		int index = 0;
		String name;
		do
		{
			name = preName + "_" + StringConverter::toString(index++);
		}while(m_sceneMgr->hasEntity(name));
		return name;
	}

	// 设置要渲染的实体
	void EntityRTT::setEntity(Entity *entity , const String &defaultAnimationName)
	{
		destroyEntity();
		// 延迟创建RTT纹理
		delayInit();
		m_entity = entity;
		// 克隆一个出来
		m_cloneEntity = m_entity->clone(getUniqueEntityName(entity->getName() + "_Clone"));

		// 附加到节点
		m_nodeEntity->attachObject(m_cloneEntity);
		// 设置转向
		m_nodeEntity->setOrientation(m_rotate);
		// 创建默认动画
		static_cast<EntityAnimationController*>(m_animationController)->create(m_cloneEntity);
		if(!defaultAnimationName.empty())
		{
			m_animationController->play(defaultAnimationName);
		}
		// 调整摄像机到实体面前
		adjustCamera();

		Root::getSingleton().addFrameListener(this);

		m_setedEntity = true;
	}

	bool EntityRTT::frameStarted(const FrameEvent& evt)
	{
		if(m_animationController)
		{
			m_animationController->update(evt.timeSinceLastFrame);
		}
		return true;
	}

	// 调整摄像机到实体面前
	void EntityRTT::adjustCamera()
	{
		m_nodeEntity->_updateBounds();
		AxisAlignedBox box = m_nodeEntity->_getWorldAABB();

		Vector3 halfSize(box.getHalfSize());
		Vector3 offset(box.getCenter());
		float radius = std::max(halfSize.y , halfSize.z);
		// 摄像机放在人物面前，根据摄像机fovy和人物包围盒的高度来计算
		offset.x += radius / tan(m_camera->getFOVy().valueRadians() * 0.5f) + halfSize.x;
		m_camera->setPosition(offset);

		m_camera->lookAt(box.getCenter());
	}

	// 销毁实体
	void EntityRTT::destroyEntity()
	{
		if(!m_setedEntity)
			return;
		static_cast<EntityAnimationController*>(m_animationController)->destroy();
		if(m_entity)
		{
			m_entity = 0;
		}
		if(m_cloneEntity)
		{
			m_sceneMgr->destroyEntity(m_cloneEntity);
			m_cloneEntity = 0;
		}
		Root::getSingleton().removeFrameListener(this);
		m_setedEntity = false;

	}

	// 获得RTT纹理
	TexturePtr &EntityRTT::getRTT()
	{
		return m_rtt;
	}

	// 开启RTT渲染
	void EntityRTT::setEnabled(bool enable)
	{
		m_enabled = enable;
		if(m_target)
		{
			m_target->setActive(m_enabled);
		}
	}

	// 是否已经开启RTT渲染
	bool EntityRTT::isEnabled()const
	{
		return m_enabled;
	}
	// 设置旋转量
	void EntityRTT::setEntityRotate(const Quaternion &rotate)
	{
		m_rotate = rotate;
		if(m_nodeEntity)
		{
			m_nodeEntity->setOrientation(m_rotate);
		}
	}
	// 增量旋转
	void EntityRTT::rotateEntity(const Quaternion &rotate)
	{
		m_rotate = m_rotate * rotate;
		if(m_nodeEntity)
		{
			m_nodeEntity->setOrientation(m_rotate);
		}
	}

	// 获得旋转量
	const Quaternion &EntityRTT::getEntityRotate()const
	{
		return m_rotate;
	}

	// 设置RTT纹理渲染比例
	void EntityRTT::setRatio(float ratio)
	{
		m_ratio = ratio;
		if(m_camera)
		{
			m_camera->setAspectRatio(m_ratio);
		}
	}
	// 设置窗口大小，会根据这个大小自动设置纹理大小和摄像机比率
	void EntityRTT::setWindowSize(ushort width , ushort height)
	{
		// 设置比率
		setRatio((float)width / (float)height);
		// 设置纹理大小
		setTextureSize(width , height);
	}

	// 设置RTT纹理大小
	void EntityRTT::setTextureSize(ushort width , ushort height)
	{
		// 计算最近2次幂
		ushort widthPower = 1;
		while(width > pow(2.0f , widthPower))
		{
			widthPower ++;
		};
		ushort realWidth = pow(2.0f , widthPower);

		// 计算最近2次幂
		ushort heightPower = 1;
		while(height > pow(2.0f , heightPower))
		{
			heightPower ++;
		};
		ushort realHeight = pow(2.0f , heightPower);

		if(m_textureWidth == realWidth && m_textureHeight == realHeight)
			return;
		m_textureWidth = realWidth;
		m_textureHeight = realHeight;
		if(m_viewport)
		{
			bool enabled = isEnabled();
			// 重新创建RTT纹理和视口
			destroyViewport();
			createViewport();
			setEnabled(enabled);
		}
	}
};