#pragma once
#include "resource.h"


// DlgObjectList dialog

class DlgObjectList : public CDialog
	, public Ogre::IMapManager::Listener
{
	DECLARE_DYNAMIC(DlgObjectList)

public:
	DlgObjectList(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgObjectList();

// Dialog Data
	enum { IDD = IDD_DIALOG_OBJECT_LIST };
	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};

	// 重新载入物件列表
	void reloadList();
	// 选中一个场景对象
	virtual void onSelectObject(Ogre::ISceneObject *obj);
	// 选中一批场景对象
	virtual void onSelectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);
	// 反选一个场景对象
	virtual void onDeselectObject(Ogre::ISceneObject *obj);
	// 反选一批场景对象
	virtual void onDeselectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CListCtrl m_list;

	enum ListState
	{
		LS_Free,		// 空闲状态
		LS_Processing,	// 正在处理状态
		LS_Selecting,	// 选择中状态
	};
	ListState m_listState;
public:
	afx_msg void OnBnClickedButtonRefresh();
	virtual BOOL OnInitDialog();
//	afx_msg void OnHdnItemchangedObjectList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnHdnItemchangingObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangingObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedObjectList(NMHDR *pNMHDR, LRESULT *pResult);
};
