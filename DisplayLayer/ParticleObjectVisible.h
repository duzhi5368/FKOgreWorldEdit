#pragma once
#include "ISceneObjectVisible.h"
namespace Ogre
{
	class ParticleObject;
	class IScene;
	// 场景对象的可见对象，用来在编辑器里选择
	class DISPLAYLAYER_API ParticleObjectVisible : public ISceneObjectVisible
	{
	public:
		ParticleObjectVisible(ParticleObject *obj);
		virtual ~ParticleObjectVisible(void);

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
		// 重建可见对象
		virtual void _notifyRebuild(){};
	protected:
		// 对象
		ParticleObject *m_object;
		// 对象所属场景
		IScene *m_scene;
		// 是否显示边框
		bool m_showBorder;
		// 是否显示实体
		bool m_showEntity;
		// 是否显示正在摆放的模型
		bool m_showPlacingEntity;
	};
};