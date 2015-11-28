// DlgTrackTools.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTrackTools.h"
#include "ObjectEditorPlugin.h"

#include "TrackSelectAction.h"
#include "TrackDragAction.h"
#include "TrackCloneAction.h"
#include "TrackMoveAction.h"
#include "TrackRotateAction.h"
#include "TrackScaleAction.h"
#include "TrackDeleteAction.h"
#include "TrackPlaceFloorAction.h"
#include "TrackNewAction.h"

#include "TrackKeyFrameObject.h"
#include "AnimationTrackObjectVisible.h"

// DlgTrackTools dialog

IMPLEMENT_DYNAMIC(DlgTrackTools, CDialog)

DlgTrackTools::DlgTrackTools(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTrackTools::IDD, pParent)
	, m_object(0)
	, m_simulateObject(0)
	, m_keyObjectRoot(0)
{

}

DlgTrackTools::~DlgTrackTools()
{
}

void DlgTrackTools::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgTrackTools, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_NEW, &DlgTrackTools::OnBnClickedButtonNew)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &DlgTrackTools::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &DlgTrackTools::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_DELETE3, &DlgTrackTools::OnBnClickedButtonDelete3)
	ON_BN_CLICKED(IDC_BUTTON_CLONE2, &DlgTrackTools::OnBnClickedButtonClone2)
	ON_BN_CLICKED(IDC_BUTTON_PLACEFLOOR2, &DlgTrackTools::OnBnClickedButtonPlacefloor2)
	ON_BN_CLICKED(IDC_BUTTON_DRAG, &DlgTrackTools::OnBnClickedButtonDrag)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_X, &DlgTrackTools::OnBnClickedButtonMoveX)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_X2, &DlgTrackTools::OnBnClickedButtonRotateX2)
	ON_BN_CLICKED(IDC_BUTTON_SCALE_X2, &DlgTrackTools::OnBnClickedButtonScaleX2)
	ON_BN_CLICKED(IDC_BUTTON_SIMULATE, &DlgTrackTools::OnBnClickedButtonSimulate)
END_MESSAGE_MAP()


// DlgTrackTools message handlers

void DlgTrackTools::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(!bShow)
	{
		deactivate();
	}
}


// 激活编辑一个动画轨迹对象
void DlgTrackTools::activate(AnimationTrackObject *obj)
{
	if(isActivate())
		return;
	deactivate();
	m_object = obj;

	// 创建关键点的根
	m_keyObjectRoot = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("NullObject");
	size_t index = 0;
	AnimationTrackObject::ConstKeyFrameIterator iter = m_object->getConstKeyFrameIterator();
	// 创建所有关键点
	while(iter.hasMoreElements())
	{
		const AnimationTrackObject::KeyFrame &keyFrame = iter.getNext();
		KeyObjectData data = createKeyObject(index++);
		m_listKeyObject.push_back(data.m_keyObject);
	}
	// 重新生成所有关键帧索引
	rebuildKeyFrameIndex();

	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();

	// 清除Action
	plugin.setObjectAction(0);

	// 如果没有关键帧，就切换到新建关键帧序列模式
	if(m_object->getKeyFrameCount() == 0)
	{
		plugin.setObjectAction(0);
		plugin.setObjectAction(new TrackNewAction(this));
	}

	// 开始侦听窗口消息
	plugin.getWorldEditor()->getMainFrame()->addChildViewListener(this , false);

	// 显示边框
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showBorder(true);
	// 隐藏关键帧节点
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showKeyNode(false);

	// 关闭物件编辑器
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTrackTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgEditTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgPlaceSceneObject::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgObjectList::IDD);

}

// 反激活
void DlgTrackTools::deactivate()
{
	if(m_object == 0)
		return;
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	// 销毁轨迹关键点对象
	for(ListKeyObject::iterator iter = m_listKeyObject.begin() ; iter != m_listKeyObject.end() ; iter ++)
	{
		plugin.getWorldEditor()->getMapManager()->deselectObject(*iter);
		m_keyObjectRoot->destroyChildObject(*iter);
	}
	m_listKeyObject.clear();
	
	// 销毁关键点对象的根节点
	m_keyObjectRoot->getScene()->destroySceneObject(m_keyObjectRoot);
	m_keyObjectRoot = 0;
	// 设置成选择模式
	plugin.setObjectAction(0);
	plugin.setObjectAction(new ObjectSelectAction());
	// 停止侦听窗口消息
	plugin.getWorldEditor()->getMainFrame()->removeChildViewListener(this);

	// 销毁模拟物件
	if(m_simulateObject)
	{
		m_object->destroyTrack(m_controller);
		m_controller = 0;
		m_simulateObject->getScene()->destroySceneObject(m_simulateObject);
		m_simulateObject = 0;
	}
	// 重置轨迹动画对象的位置为关键帧的开始帧
	if(m_object->getKeyFrameCount() > 0)
	{
		const AnimationTrackObject::KeyFrame &keyFrame = m_object->getKeyFrame(0);
		m_object->setWorldPosition(keyFrame.m_translate);
		m_object->setWorldScale(keyFrame.m_scale);
		m_object->setWorldOrientation(keyFrame.m_rotate);
	}
	// 显示关键帧节点
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showKeyNode(true);
	// 隐藏边框
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showBorder(false);
	// 反选轨迹动画对象
	plugin.getWorldEditor()->getMapManager()->deselectObject(m_object);

	GetDlgItem(IDC_BUTTON_SIMULATE)->SetWindowText("模拟(&I)");

	m_object = 0;
	// 打开物件编辑器
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTrackTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgEditTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgPlaceSceneObject::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgObjectList::IDD);

}

// 删除一个关键帧
void DlgTrackTools::deleteKeyFrame(KeyObjectData &data)
{
	// 反选对象
	ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(data.m_keyObject);
	// 销毁对象
	m_keyObjectRoot->destroyChildObject(data.m_keyObject);
	// 从列表中删除
	m_listKeyObject.erase(std::find(m_listKeyObject.begin() , m_listKeyObject.end() , data.m_keyObject));
	// 删除关键帧
	m_object->removeKeyFrame(data.m_index);	
	// 重新生成所有关键帧索引
	rebuildKeyFrameIndex();
}

// 创建一个关键点对象
KeyObjectData DlgTrackTools::createKeyObject(size_t index)
{
	const AnimationTrackObject::KeyFrame &keyFrame = m_object->getKeyFrame(index);
	NameValuePairList params;
	params["mesh"] = "axes.mesh";
	KeyObjectData newData;
	TrackKeyFrameObject * keyObject = static_cast<TrackKeyFrameObject*>(m_keyObjectRoot->createChildObject("TrackKeyFrameObject" , &params));
	newData.m_object = m_object;
	newData.m_index = index;
	newData.m_dlgTrackTools = this;
	newData.m_keyObject = keyObject;
	keyObject->setUserAny(Any(newData));
	keyObject->updateFromKeyFrame();
	return newData;
}

// 克隆一个关键帧
KeyObjectData DlgTrackTools::cloneKeyFrame(KeyObjectData &data)
{
	AnimationTrackObject::KeyFrame newKeyFrame = m_object->getKeyFrame(data.m_index);
	newKeyFrame.m_translate += Vector3(0.5f , 0 , 0.5f);
	size_t index = m_object->insertKeyFrame(data.m_index , newKeyFrame.m_length , newKeyFrame.m_translate , newKeyFrame.m_scale , newKeyFrame.m_rotate);
	KeyObjectData newData = createKeyObject(index);
	// 添加到列表中
	m_listKeyObject.insert(std::find(m_listKeyObject.begin() , m_listKeyObject.end() , data.m_keyObject) + 1 , newData.m_keyObject);

	// 重新生成所有关键帧索引
	rebuildKeyFrameIndex();

	return safe_cast<KeyObjectData>(newData.m_keyObject->getUserAny());
}


// 重新生成所有关键帧索引
void DlgTrackTools::rebuildKeyFrameIndex()
{
	size_t index = 0;
	// 重新生成轨迹关键点对象的索引
	for(ListKeyObject::iterator iter = m_listKeyObject.begin() ; iter != m_listKeyObject.end() ; iter ++)
	{
		KeyObjectData data = safe_cast<KeyObjectData>((*iter)->getUserAny());
		data.m_index = index++;
		(*iter)->setUserAny(Any(data));
		(*iter)->updateFromKeyFrame();
	}
}

// 在尾部增加一个关键帧
KeyObjectData DlgTrackTools::pushBackKeyFrame(const AnimationTrackObject::KeyFrame &keyFrame)
{
	// 添加到关键帧
	size_t index = m_object->addKeyFrameToBack(keyFrame.m_length , keyFrame.m_translate , keyFrame.m_scale , keyFrame.m_rotate);
	KeyObjectData data = createKeyObject(index);
	// 添加到列表中
	m_listKeyObject.push_back(data.m_keyObject);
	// 重新生成所有关键帧索引
	rebuildKeyFrameIndex();
	return safe_cast<KeyObjectData>(data.m_keyObject->getUserAny());
}

// 把尾部的一个关键帧删除
void DlgTrackTools::popBackKeyFrame()
{
	if(m_object->getKeyFrameCount() > 0)
	{
		deleteKeyFrame(safe_cast<KeyObjectData>(m_listKeyObject.back()->getUserAny()));
	}
}

// 子视口消息，返回false则不执行后面的侦听器
bool DlgTrackTools::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();

	if(message == WM_KEYUP)
	{
		switch(wParam)
		{
		case 'S':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackSelectAction(m_object));

				return false;
			}
			break;
		case 'D':
		case VK_DELETE:
			{
				TrackDeleteAction action(m_object);
				action.execute();
				return false;
			}
			break;
		case 'C':
			{
				TrackCloneAction action(m_object);
				action.execute();
				return false;
			}
			break;
		case 'T':
			{
				TrackPlaceFloorAction action(m_object);
				action.execute();
				return false;
			}
			break;
		case 'A':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackDragAction(m_object));
				return false;
			}
			break;
		case 'M':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackMoveAction(m_object));

				return false;
			}
			break;
		case 'R':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackRotateAction(m_object));
				return false;
			}
			break;
		case 'L':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackScaleAction(m_object));
				return false;
			}
			break;
		case 'N':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackNewAction(this));
				return false;
			}
			break;
		case 'I':
			{
				OnBnClickedButtonSimulate();
				return false;
			}
			break;
		}
	}
	return true;
}

void DlgTrackTools::OnBnClickedButtonClose()
{
	ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTrackTools::IDD);
}

void DlgTrackTools::OnBnClickedButtonNew()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackNewAction(this));
}

void DlgTrackTools::OnBnClickedButtonSelect()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackSelectAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonDelete3()
{
	TrackDeleteAction action(m_object);
	action.execute();
}

void DlgTrackTools::OnBnClickedButtonClone2()
{
	TrackCloneAction action(m_object);
	action.execute();
}

void DlgTrackTools::OnBnClickedButtonPlacefloor2()
{
	TrackPlaceFloorAction action(m_object);
	action.execute();
}

void DlgTrackTools::OnBnClickedButtonDrag()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackDragAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonMoveX()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackMoveAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonRotateX2()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackRotateAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonScaleX2()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackScaleAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonSimulate()
{
	if(m_simulateObject == 0)
	{
		NameValuePairList params;
		params["mesh"] = "R_F_Z_Head_01.mesh";
		m_simulateObject = static_cast<CharacterObject*>(DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("CharacterObject" , &params));
		m_simulateObject->getAnimationController().play("run");
		m_keyObjectRoot->setVisible(false);
		m_controller = m_object->createTrack(m_simulateObject);
		m_controller->playByIndex();

		GetDlgItem(IDC_BUTTON_SIMULATE)->SetWindowText("停止(&I)");

		Root::getSingleton().addFrameListener(this);
	}
	else
	{
		m_object->destroyTrack(m_controller);
		m_controller = 0;
		m_simulateObject->getScene()->destroySceneObject(m_simulateObject);
		m_simulateObject = 0;

		m_keyObjectRoot->setVisible(true);
		// 销毁轨迹关键点对象
		for(ListKeyObject::iterator iter = m_listKeyObject.begin() ; iter != m_listKeyObject.end() ; iter ++)
		{
			(*iter)->getSceneObjectVisible()->showBorder((*iter)->getSceneObjectVisible()->isShowBorder());
		}

		GetDlgItem(IDC_BUTTON_SIMULATE)->SetWindowText("模拟(&I)");

		Root::getSingleton().removeFrameListener(this);
	}
}
// 每帧侦听是否模拟结束
bool DlgTrackTools::frameStarted(const FrameEvent& evt)
{
	if(m_simulateObject && m_controller)
	{
		if(m_controller->hasEnded())
		{
			if(m_simulateObject->getAnimationController().getPlayingAnimation(m_simulateObject->getAnimationInfo()["idle"]).isNull())
			{
				m_simulateObject->getAnimationController().play("idle");
			}
		}
	}
	return true;
}
