#pragma once
#include "NullObject.h"
#include "EntityAnimationController.h"
namespace Ogre
{
	class EntityMaterialInstance;
	// 场景物件的对象工厂
	class DISPLAYLAYER_API EntityObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// 获得该工厂的类型名称
		virtual const String &getType() const;
		// 工厂对象的创建方法
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// 工厂对象的销毁方法
		virtual void destroy(ISceneObject *obj);
	};

	// 模型实体对象
	class DISPLAYLAYER_API EntityObject : public NullObject
		, public FrameListener
	{
	public:
		EntityObject(const String &name);
		virtual ~EntityObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return EntityObjectFactory::FACTORY_TYPE;
		}
		Entity *getOgreEntity()const
		{
			return m_entity;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();

		// 获得动画控制器
		IEntityAnimationController &getAnimationController()
		{
			return m_animationController;
		}
		// 帧侦听器的每帧执行
		virtual bool frameStarted( const FrameEvent& evt );

		// 设置透明度，0代表全透明，1代表不透明
		void setAlpha(float alpha);
		// 获得透明度
		float getAlpha()const;

		// 设置是否显示
		virtual void setVisible(bool visible);

		// 把一个场景物件挂接到骨头上
		virtual void attachObjectToBone(const String &boneName,
			ISceneObject *obj,
			const Quaternion &offsetOrientation = Quaternion::IDENTITY,
			const Vector3 &offsetPosition = Vector3::ZERO);
		// 把一个场景物件从骨头取消挂接
		virtual void detachObjectFromBone(ISceneObject *obj);
		virtual void detachObjectFromBone(const String &objName);
		// 把所有场景物件从骨头取消挂接
		virtual void detachAllObjectsFromBone();

		typedef std::map<String, ISceneObject*> ChildObjectList;
		typedef MapIterator<ChildObjectList> ChildObjectListIterator;
		// 获得已挂接到骨头的场景物件
		ChildObjectListIterator getAttachedObjectIterator(void);

		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_entity;
		}

		// 使用这个Mesh来重构Entity
		bool setMeshFile(const String &meshFileName);
		// 获得Mesh文件名
		const String &getMeshFile()const;


		// 设置属性
		virtual bool setProperty(uint id , const Any &value);
		// 获取属性值
		virtual Any getProperty(uint id)const;
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const;

		// 设置碰撞模型类型
		void setCollisionShapeType(CollisionShapeType type);
		// 获得碰撞模型类型
		CollisionShapeType getCollisionShapeType()const
		{
			return m_collisionEntity ? m_collisionEntity->getCollisionShapeType() : CT_NONE;
		}
		// 设置碰撞模型
		void setCollisionShapeName(const String &name);
		// 获得碰撞模型名称
		const String &getCollisionShapeName()const
		{
			return m_collisionEntity ? m_collisionEntity->getCollisionShapeName() : StringUtil::BLANK;
		}

		// 获得碰撞实体
		ICollisionEntity *getCollisionEntity()const
		{
			return m_collisionEntity;
		}

		// 获得场景对象的可见对象，用来在编辑器里选择
		virtual ISceneObjectVisible *getSceneObjectVisible();
	protected:
		// 载入碰撞模型
		void loadCollisionEntity();
		// 卸载碰撞模型
		void unloadCollisionEntity();
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// 销毁mesh
		void destroyMesh();
		// Mesh文件名
		String m_meshFileName;
		// Ogre实体
		Entity *m_entity;
		// 实体动画控制器
		EntityAnimationController m_animationController;
		// 透明度
		float m_alpha;
		// 透明控制器
		EntityMaterialInstance *m_alphaController;
		// 已挂接到骨头的场景物件列表
		ChildObjectList m_childObjectList;

		// 碰撞体类型
		CollisionShapeType m_collisionType;
		// 碰撞模型名称
		String m_collisionName;

		// 碰撞实体
		ICollisionEntity *m_collisionEntity;
	};


};