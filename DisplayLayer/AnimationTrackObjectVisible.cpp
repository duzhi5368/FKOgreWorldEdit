#include "StdAfx.h"
#include "AnimationTrackObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "AnimationTrackObject.h"

namespace Ogre
{

	AnimationTrackObjectVisible::AnimationTrackObjectVisible(AnimationTrackObject *obj)
		: NullObjectVisible(obj)
		, m_trackLines(0)
		, m_controller(0)
		, m_trackNode(0)
		, m_needRebuild(true)
		, m_showKeyNode(true)
	{
		destroyVisibleSphere();
		m_sphereColor = ColourValue(0 , 1 , 1);
		createVisibleSphere();

		// 创建用来计算轨迹线的控制器
		m_controller = static_cast<AnimationTrackController*>(static_cast<AnimationTrackObject*>(m_object)->createTrack(m_object));

		// 一开始就重建
		rebuild();

		Root::getSingleton().addFrameListener(this);
	}

	AnimationTrackObjectVisible::~AnimationTrackObjectVisible(void)
	{
		Root::getSingleton().removeFrameListener(this);
		// 销毁轨迹线
		destroyTrackLines();
		// 销毁控制器
		if(m_controller)
		{
			static_cast<AnimationTrackObject*>(m_object)->destroyTrack(m_controller);
			m_controller = 0;
		}
	}
	// 显示关键点
	void AnimationTrackObjectVisible::showKeyNode(bool show)
	{
		m_showKeyNode = show;
		if(m_trackKeyNode)
		{
			m_trackKeyNode->setVisible(m_showKeyNode);
		}
	}

	// 显示边框
	void AnimationTrackObjectVisible::showBorder(bool show)
	{
		NullObjectVisible::showBorder(show);
		if(m_trackNode)
		{
			m_trackNode->setVisible(show);
		}
		if(m_trackKeyNode)
		{
			m_trackKeyNode->setVisible(m_showKeyNode && show);
		}
	}
	// 重建可见对象
	void AnimationTrackObjectVisible::_notifyRebuild()
	{
		m_needRebuild = true;
	}
	// 销毁轨迹线
	void AnimationTrackObjectVisible::destroyTrackLines()
	{
		// 销毁轨迹线
		if(m_trackLines)
		{
			m_scene->getOgreSceneManager()->destroyManualObject(m_trackLines);
			m_trackLines = 0;
		}
		// 销毁关键点
		for(ListKeyNode::iterator iter = m_listKeyNode.begin() ; iter != m_listKeyNode.end() ; iter ++)
		{
			m_scene->getOgreSceneManager()->destroyEntity(*iter);
		}
		m_listKeyNode.clear();
		// 销毁所有节点
		if(m_trackNode)
		{
			m_trackNode->getParentSceneNode()->removeAndDestroyChild(m_trackNode->getName());
			m_trackNode = 0;
			m_trackKeyNode = 0;
		}
	}

	// 重建可见对象
	void AnimationTrackObjectVisible::rebuild()
	{
		if(!m_needRebuild)
			return;
		// 先销毁轨迹线
		destroyTrackLines();
		// 先重建动画控制器
		m_controller->_rebuild();
		// 创建轨迹线
		m_trackLines = m_scene->getOgreSceneManager()->createManualObject(m_object->getFullName() + "_TrackLines");

		// 创建轨迹节点
		m_trackNode = m_scene->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
		// 把轨迹线挂接到节点上去
		m_trackNode->attachObject(m_trackLines);

		float timePos = 0.0f;
		NodeAnimationTrack *track = m_controller->getNodeAnimationTrack();
		Animation *animation = m_controller->getAnimation();
		// 创建一个节点用来接收动画信息
		SceneNode *node = m_scene->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
		m_trackLines->begin(DisplaySystem::getSingleton().getColorMaterial(ColourValue::Red) , RenderOperation::OT_LINE_STRIP);
		// 一直循环到最后
		while(timePos < m_controller->getLength())
		{
			node->resetToInitialState();
			// 把动画信息应用到node中
			track->applyToNode(node , animation->_getTimeIndex(timePos));
			// 方向改变或者到最后一个节点，就写入
			m_trackLines->position(node->getPosition());
			// 每0.1秒计算一次
			timePos += 0.1f;
		}
		m_trackLines->end();

		// 画原始的轨迹
		m_trackLines->begin(DisplaySystem::getSingleton().getColorMaterial(ColourValue::Blue) , RenderOperation::OT_LINE_STRIP);
		{
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_controller->getAnimationTrackObject()->getConstKeyFrameIterator();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame &keyFrame = iterator.getNext();
				m_trackLines->position(keyFrame.m_translate);
			}
		}
		m_trackLines->end();

		// 画关键点
		{
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_controller->getAnimationTrackObject()->getConstKeyFrameIterator();
			size_t index = 0;
			m_trackKeyNode = m_trackNode->createChildSceneNode();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame &keyFrame = iterator.getNext();
				SceneNode *child = m_trackKeyNode->createChildSceneNode(keyFrame.m_translate , keyFrame.m_rotate);
				// 创建一个关键点
				Entity *entity = m_scene->getOgreSceneManager()->createEntity(m_object->getFullName() + "_KeyFrame_" + StringConverter::toString(index++) , "axes.mesh");
				child->setScale(keyFrame.m_scale * (1.0f / entity->getBoundingRadius()));
				m_listKeyNode.push_back(entity);
				child->attachObject(entity);
			}
		}


		// 销毁这个节点
		m_scene->getOgreSceneManager()->destroySceneNode(node->getName());

		showBorder(isShowBorder());
		m_needRebuild = false;
	}

};
