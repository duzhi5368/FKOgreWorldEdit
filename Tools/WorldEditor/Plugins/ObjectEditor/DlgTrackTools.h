#pragma once
#include "Resource.h"
#include "TrackKeyFrameObject.h"

// DlgTrackTools dialog
using namespace Ogre;

class DlgTrackTools;

// 关键帧对象存储的数据
class KeyObjectData
{
public:
	KeyObjectData()
		: m_index(-1)
		, m_object(0)
		, m_dlgTrackTools(0)
		, m_keyObject(0)
	{

	}
	// 是否为空
	bool isNull()const
	{
		return m_index == -1 || m_object == 0 || m_dlgTrackTools == 0 || m_keyObject == 0;
	}

	// 串行化操作
	inline friend std::ostream& operator <<
	( std::ostream& o, const KeyObjectData& v )
	{
		o << "KeyObjectData(" << v.m_index << ", " << (v.m_object ? v.m_object->getName() : StringUtil::BLANK) << ")";
		return o;
	}
	// 关键帧索引
	size_t m_index;
	// 动画轨迹对象
	AnimationTrackObject *m_object;
	// 动画关键帧显示对象
	TrackKeyFrameObject *m_keyObject;
	// 轨迹编辑器
	DlgTrackTools *m_dlgTrackTools;
};

// 轨迹编辑器
class DlgTrackTools : public CDialog
	, public IChildView::Listener
	, public FrameListener
{
	DECLARE_DYNAMIC(DlgTrackTools)

public:
	DlgTrackTools(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgTrackTools();

// Dialog Data
	enum { IDD = IDD_DIALOG_TRACK_TOOLS };

	// 激活编辑一个动画轨迹对象
	void activate(AnimationTrackObject *obj);
	// 反激活
	void deactivate();

	// 是否已经激活
	bool isActivate()const
	{
		return m_object != 0;
	}
	// 子视口消息，返回false则不执行后面的侦听器
	virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// 删除一个关键帧
	void deleteKeyFrame(KeyObjectData &data);
	// 克隆一个关键帧
	KeyObjectData cloneKeyFrame(KeyObjectData &data);


	// 在尾部增加一个关键帧
	KeyObjectData pushBackKeyFrame(const AnimationTrackObject::KeyFrame &keyFrame);
	// 把尾部的一个关键帧删除
	void popBackKeyFrame();

	// 重新生成所有关键帧索引
	void rebuildKeyFrameIndex();

	// 获得动画轨迹对象
	AnimationTrackObject *getAnimationTrackObject()const
	{
		return m_object;
	}
	// 每帧侦听是否模拟结束
	virtual bool frameStarted(const FrameEvent& evt);
	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	// 创建一个关键点对象
	KeyObjectData createKeyObject(size_t index);
	// 动画轨迹对象
	AnimationTrackObject *m_object;

	typedef std::deque<TrackKeyFrameObject*> ListKeyObject;
	// 所有关键点
	ListKeyObject m_listKeyObject;

	// 关键点的根
	ISceneObject *m_keyObjectRoot;

	// 模拟物件
	CharacterObject *m_simulateObject;
	// 模拟控制器
	AnimationTrackObject::IAnimationTrackController *m_controller;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonDelete3();
	afx_msg void OnBnClickedButtonClone2();
	afx_msg void OnBnClickedButtonPlacefloor2();
	afx_msg void OnBnClickedButtonDrag();
	afx_msg void OnBnClickedButtonMoveX();
	afx_msg void OnBnClickedButtonRotateX2();
	afx_msg void OnBnClickedButtonScaleX2();
	afx_msg void OnBnClickedButtonSimulate();
};
