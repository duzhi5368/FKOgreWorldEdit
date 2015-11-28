#pragma once
#include "ISceneObjectVisible.h"
namespace Ogre
{
	class SphereTriggerObject;
	class IScene;
	// 场景对象的可见对象，用来在编辑器里选择
	class DISPLAYLAYER_API SphereTriggerObjectVisible : public ISceneObjectVisible
	{
	public:
		SphereTriggerObjectVisible(SphereTriggerObject *obj);
		virtual ~SphereTriggerObjectVisible(void);

		// 显示边框
		virtual void showBorder(bool show);
		// 是否显示边框
		virtual bool isShowBorder() const;

		// 显示实体
		virtual void showEntity(bool show);
		// 是否显示实体
		virtual bool isShowEntity() const;

		// 获得场景对象
		virtual ISceneObject *getSceneObject()const;


		// 显示正在摆放的模型
		virtual void showPlacingEntity(bool show);
		// 是否显示正在摆放的模型
		virtual bool isShowPlacingEntity() const;

		// 获得可见对象的碰撞实体
		virtual ICollisionEntity *getCollisionEntity()const;

		// 创建模型实体
		void createEntity();

		// 销毁模型实体
		void destroyEntity();

		// 重建可见对象
		virtual void _notifyRebuild()
		{
			createEntity();
		};
	protected:
		// 对象
		SphereTriggerObject *m_object;
		// 对象所属场景
		IScene *m_scene;
		// 是否显示边框
		bool m_showBorder;
		// 是否显示实体
		bool m_showEntity;
		// 是否显示正在摆放的模型
		bool m_showPlacingEntity;
		// 显示的模型
		Entity *m_entity;
		// 模型的节点
		SceneNode *m_entityNode;
	};
};