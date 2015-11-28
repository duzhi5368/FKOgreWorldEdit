#pragma once
namespace Ogre
{
	class ISceneObject;
	// 场景对象的可见对象，用来在编辑器里选择
	class ISceneObjectVisible
	{
	public:

		ISceneObjectVisible(void)
		{
		}

		virtual ~ISceneObjectVisible(void)
		{
		}

		// 获得场景对象
		virtual ISceneObject *getSceneObject()const = 0;

		// 显示边框
		virtual void showBorder(bool show) = 0;
		// 是否显示边框
		virtual bool isShowBorder() const = 0;

		// 显示实体
		virtual void showEntity(bool show) = 0;
		// 是否显示实体
		virtual bool isShowEntity() const = 0;

		// 显示正在摆放的模型
		virtual void showPlacingEntity(bool show) = 0;
		// 是否显示正在摆放的模型
		virtual bool isShowPlacingEntity() const = 0;

		// 重建可见对象
		virtual void _notifyRebuild() = 0;

		// 获得可见对象的碰撞实体
		virtual ICollisionEntity *getCollisionEntity()const = 0;
	};
};