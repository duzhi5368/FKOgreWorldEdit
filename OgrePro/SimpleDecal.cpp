/******************************************************************
** 文件名:	DecalSystem.cpp
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:29
** 描  述:
******************************************************************/
#include "stdafx.h"
#include "SimpleDecal.h"

 
namespace Ogre
{
	SimpleDecal::SimpleDecal(SceneManager *mgr , const String &name)
		: m_name(name)
		, m_sceneMgr(mgr)
		, m_node(0)
		, m_nodeAttachObject(0)
		, m_position(Vector3::ZERO)
		, m_size(Vector2::ZERO)
		, m_depth(1)
		, m_rotate(0)
		, m_offsetOrientation(Quaternion::IDENTITY)
		, m_offsetPosition(Vector3::ZERO)
		, m_offsetScale(1.0f , 1.0f , 1.0f)
		, m_visible(false)
		, m_dirty(0xFFFFFFFF)
		, m_aabbNeedUpdate(true)
		, m_worldAABBNeedUpdate(true)
	{
		m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode(name);
	}

	SimpleDecal::~SimpleDecal()
	{
		if(m_nodeAttachObject)
		{
			m_sceneMgr->destroySceneNode(m_nodeAttachObject->getName());
		}
		if(m_node)
		{
			m_sceneMgr->destroySceneNode(m_node->getName());
		}
	}

	// 获得这个贴花的名字
	const String &SimpleDecal::getName()const
	{
		return m_name;
	}

	// 设置贴花材质名字
	void SimpleDecal::setMaterialName(const String &materialName)
	{
		if(m_materialName != materialName)
		{
			m_dirty |= DIRTY_MATERIAL_NAME;
		}
		m_materialName = materialName;
	}

	// 获得贴花材质名字
	const String &SimpleDecal::getMaterialName()const
	{
		return m_materialName;
	}

	// 设置贴花所在位置
	void SimpleDecal::setPosition(const Vector3 &pos)
	{
		if(m_position != pos)
		{
			m_dirty |= DIRTY_POSITION;
			m_aabbNeedUpdate = true;
		}
		m_position = pos;
	}

	// 获取贴花所在位置
	const Vector3 &SimpleDecal::getPosition()const
	{
		return m_position;
	}


	// 设置贴花大小
	void SimpleDecal::setSize(const Vector2 &size)
	{
		if(m_size != size)
		{
			m_dirty |= DIRTY_SIZE;
			m_aabbNeedUpdate = true;
			m_worldAABBNeedUpdate = true;
		}
		m_size = size;
	}

	// 获取贴花大小
	const Vector2 &SimpleDecal::getSize()const
	{
		return m_size;
	}


	// 设置贴花的深度，指最大拉伸的高度
	void SimpleDecal::setDepth(float depth)
	{
		if(m_depth != depth)
		{
			m_dirty |= DIRTY_DEPTH;
			m_aabbNeedUpdate = true;
			m_worldAABBNeedUpdate = true;
		}
		m_depth = depth;
	}

	// 获取贴花深度
	float SimpleDecal::getDepth()const
	{
		return m_depth;
	}


	// 设置贴花沿Y轴的旋转量
	void SimpleDecal::setRotate(const Radian &angle)
	{
		if(m_rotate != angle)
		{
			m_dirty |= DIRTY_ROTATE;
			m_aabbNeedUpdate = true;
			m_worldAABBNeedUpdate = true;
		}
		m_rotate = angle;
	}

	// 获取贴花沿Y轴的旋转量
	const Radian &SimpleDecal::getRotate()const
	{
		return m_rotate;
	}


	// 设置贴花可视属性
	void SimpleDecal::setVisible(bool visible)
	{
		if(m_visible != visible)
		{
			m_dirty |= DIRTY_VISIBLE;
		}
		m_visible = visible;
	}
 
	// 获得贴花是否显示
	bool SimpleDecal::isVisible()const
	{
		return m_visible;
	}


	// 挂接一个物件到贴花中心点上
	void SimpleDecal::attachObject(MovableObject *obj)
	{
		if(m_nodeAttachObject == 0)
		{
			m_nodeAttachObject = m_node->createChildSceneNode(m_name + "_Attach");
		}
		m_nodeAttachObject->attachObject(obj);
	}

	// 取消挂接物件
	void SimpleDecal::dettachObject(MovableObject *obj)
	{
		if(m_nodeAttachObject)
		{
			m_nodeAttachObject->detachObject(obj);
		}
	}

	// 根据索引号取消挂接物件
	void SimpleDecal::dettachObject(ushort index)
	{
		if(m_nodeAttachObject)
		{
			m_nodeAttachObject->detachObject(index);
		}
	}
	// 取消挂接所有物件
	void SimpleDecal::detachAllObjects()
	{
		if(m_nodeAttachObject)
		{
			m_nodeAttachObject->detachAllObjects();
		}
	}

	// 根据索引号获得挂接了的物件
	MovableObject* SimpleDecal::getAttachObject(ushort index)const
	{
		return m_nodeAttachObject->getAttachedObject(index);
	}

	// 获得已挂接的物件枚举
	SceneNode::ObjectIterator SimpleDecal::getAttachedObjectIterator(void)
	{
		return m_nodeAttachObject->getAttachedObjectIterator();
	}

	// 每帧更新贴花
	void SimpleDecal::update(float timeSinceLastFrame)
	{
		if(m_dirty)
		{
			updateDecal();
			m_dirty = false;
		}
	}


	// 设置挂接物体相对偏移
	void SimpleDecal::setAttachObjectsOffset(const Vector3 &offsetPosition
		, const Quaternion &offsetOrientation
		, const Vector3 &offsetScale
		)
	{	
		if(m_offsetOrientation != offsetOrientation
			|| m_offsetPosition != offsetPosition
			|| m_offsetScale != offsetScale
			)
		{
			m_dirty |= DIRTY_OFFSET;
		}
		m_offsetOrientation = offsetOrientation;
		m_offsetPosition = offsetPosition;
		m_offsetScale = offsetScale;
	}
	// 获得挂接物体相对旋转
	const Quaternion &SimpleDecal::getOffsetOrientation()const
	{
		return m_offsetOrientation;
	}
	// 获得挂接物体相对位置
	const Vector3 &SimpleDecal::getOffsetPosition()const
	{
		return m_offsetPosition;
	}
	// 获得挂接物体相对缩放
	const Vector3 &SimpleDecal::getOffsetScale()const
	{
		return m_offsetScale;
	}

	// 获取贴花的包围盒（本地坐标）
	const AxisAlignedBox &SimpleDecal::getBoundingBox()const
	{
		if(m_aabbNeedUpdate)
		{
			Vector3 halfSize(m_size.x / 2.0f , m_depth , m_size.y / 2.0f);
			m_aabb.setExtents(-halfSize , +halfSize);
			m_aabbNeedUpdate = false;
		}
		return m_aabb;
	}

	// 获取贴花的世界包围盒（本地坐标）
	const AxisAlignedBox &SimpleDecal::getWorldBoundingBox()const
	{
		if(m_worldAABBNeedUpdate)
		{
			m_worldAABB = getBoundingBox();
			Matrix4 mat;
			mat.makeTrans(m_position);
			m_worldAABB.transformAffine(mat);
			m_worldAABBNeedUpdate = false;
		}
		return m_worldAABB;
	}

};