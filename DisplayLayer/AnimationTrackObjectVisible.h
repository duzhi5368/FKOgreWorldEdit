#pragma once
#include "NullObjectVisible.h"
#include "AnimationTrackController.h"
namespace Ogre
{
	class AnimationTrackObject;
	class IScene;
	// 场景对象的可见对象，用来在编辑器里选择
	class DISPLAYLAYER_API AnimationTrackObjectVisible : public NullObjectVisible
		, public FrameListener
	{
	public:
		AnimationTrackObjectVisible(AnimationTrackObject *obj);
		virtual ~AnimationTrackObjectVisible(void);

		// 显示边框
		virtual void showBorder(bool show);

		// 显示关键点
		void showKeyNode(bool show);

		// 是否显示关键点
		bool isShowKeyNode()const
		{
			return m_showKeyNode;
		}
		// 重建可见对象
		virtual void _notifyRebuild();

		// 每帧检查是否需要重建可见对象
		virtual bool frameStarted(const FrameEvent& evt)
		{
			rebuild();
			return true;
		}
	protected:
		// 销毁轨迹线
		void destroyTrackLines();
		// 重建可见对象
		void rebuild();
		// 轨迹线
		ManualObject *m_trackLines;
		// 轨迹节点
		SceneNode *m_trackNode;
		typedef std::list<Entity*> ListKeyNode;
		// 轨迹关键点列表
		ListKeyNode m_listKeyNode;
		// 轨迹关键点的节点
		SceneNode *m_trackKeyNode;
		// 是否显示关键点
		bool m_showKeyNode;
		// 需要重建
		bool m_needRebuild;
		// 用来计算轨迹线的控制器
		AnimationTrackController* m_controller;
	};
};