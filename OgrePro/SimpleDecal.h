/******************************************************************
** 文件名:	DecalSystem.h
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:30
** 描  述:
******************************************************************/
#pragma once
#include "IDecalSystem.h"
namespace Ogre
{
// 贴花队列，在地形之后，物件之前
#define RENDER_QUEUE_DECAL (RENDER_QUEUE_3)

	class SimpleDecal : public IDecal
	{
	public:
		SimpleDecal(SceneManager *mgr , const String &name);
		virtual ~SimpleDecal();

		// 获得这个贴花的名字
		virtual const String &getName()const;

		// 获得场景管理器
		virtual SceneManager* getSceneManager()const
		{
			return m_sceneMgr;
		}

		// 设置贴花材质名字
		virtual void setMaterialName(const String &materialName);
	
		// 获得贴花材质名字
		virtual const String &getMaterialName()const;

		// 设置贴花所在位置
		virtual void setPosition(const Vector3 &pos);
		// 获取贴花所在位置
		virtual const Vector3 &getPosition()const;

		// 设置贴花大小
		virtual void setSize(const Vector2 &size);
		// 获取贴花大小
		virtual const Vector2 &getSize()const;

		// 获取贴花的包围盒（本地坐标）
		virtual const AxisAlignedBox &getBoundingBox()const;

		// 获取贴花的世界包围盒（本地坐标）
		virtual const AxisAlignedBox &getWorldBoundingBox()const;

		// 设置贴花的深度，指最大拉伸的高度
		virtual void setDepth(float depth);
		// 获取贴花深度
		virtual float getDepth()const;

		// 设置贴花沿Y轴的旋转量
		virtual void setRotate(const Radian &angle);
		// 获取贴花沿Y轴的旋转量
		virtual const Radian &getRotate()const;

		// 设置贴花可视属性
		virtual void setVisible(bool visible);
		// 获得贴花是否显示
		virtual bool isVisible()const;


		// 设置挂接物体相对偏移
		virtual void setAttachObjectsOffset(const Vector3 &offsetPosition = Vector3::ZERO
			, const Quaternion &offsetOrientation = Quaternion::IDENTITY
			, const Vector3 &offsetScale = Vector3(1.0f , 1.0f , 1.0f)
			);
		// 获得挂接物体相对旋转
		virtual const Quaternion &getOffsetOrientation()const;
		// 获得挂接物体相对位置
		virtual const Vector3 &getOffsetPosition()const;
		// 获得挂接物体相对缩放
		virtual const Vector3 &getOffsetScale()const;

		// 挂接一个物件到贴花中心点上
		virtual void attachObject(MovableObject *obj);
		// 取消挂接物件
		virtual void dettachObject(MovableObject *obj);
		// 根据索引号取消挂接物件
		virtual void dettachObject(ushort index);
		// 取消挂接所有物件
		virtual void detachAllObjects();

		// 根据索引号获得挂接了的物件
		virtual MovableObject* getAttachObject(ushort index)const;
		// 获得已挂接的物件枚举
		virtual SceneNode::ObjectIterator getAttachedObjectIterator(void);

		// 每帧更新贴花
		virtual void update(float timeSinceLastFrame);

		// 获得贴花节点
		virtual SceneNode* getSceneNode()
		{
			return m_node;
		}
		// 获得用来附加物件的节点
		virtual SceneNode* getAttachObjectSceneNode()
		{
			return m_nodeAttachObject;
		}
	protected:
		// 当属性改变了，需要重构贴花的时候
		virtual void updateDecal() = 0;

		// 贴花名称
		String m_name;

		// 贴花的渲染节点
		SceneNode *m_node;
		// 用来挂接物件的节点
		SceneNode *m_nodeAttachObject;
		// 使用的场景管理器
		SceneManager *m_sceneMgr;

		// 贴花材质
		String m_materialName;
		// 贴花所在位置
		Vector3 m_position;
		// 贴花大小
		Vector2 m_size;
		// 贴花的深度，指最大拉伸的高度
		float m_depth;
		// 贴花沿Y轴的旋转量
		Radian m_rotate;
		// 贴花可视属性
		bool m_visible;

		// 挂接物体相对旋转
		Quaternion m_offsetOrientation;
		// 挂接物体相对位置
		Vector3 m_offsetPosition;
		// 挂接物体相对缩放
		Vector3 m_offsetScale;

		// 涉及到贴花的属性被改变了
		enum DIRTY_FLAGS
		{
			DIRTY_MATERIAL_NAME = 1 << 0,
			DIRTY_POSITION		= 1 << 1,
			DIRTY_SIZE			= 1 << 2,
			DIRTY_DEPTH			= 1 << 3,
			DIRTY_ROTATE		= 1 << 4,
			DIRTY_VISIBLE		= 1 << 5,
			DIRTY_OFFSET		= 1 << 6,
			DIRTY_MAX_COUNT		= 7,
		};
		// 贴花属性改变，DIRTY_FLAGS标记集
		ulong m_dirty;

		// 本地包围盒
		mutable AxisAlignedBox m_aabb;
		mutable bool m_aabbNeedUpdate;
		// 世界包围盒
		mutable AxisAlignedBox m_worldAABB;
		mutable bool m_worldAABBNeedUpdate;
	};

};