#pragma once
#include "FileTreeCtrl.h"
#include "resource.h"

// DlgPlaceSceneObject dialog

class DlgPlaceSceneObject : public CDialog , public CFileTreeCtrl::LoadTreeCallback
{
	DECLARE_DYNAMIC(DlgPlaceSceneObject)

public:
	DlgPlaceSceneObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgPlaceSceneObject();

// Dialog Data
	enum { IDD = IDD_DIALOG_TREE };
	// 载入回调
	virtual void loadFile(HTREEITEM hItem , CFileFind &file);

	// 重新载入树
	void reloadTree();
protected:

	enum LoadingType
	{
		LT_NONE,		// 没有正在载入
		LT_MODEL,		// 正在载入模型
		LT_PARTICLE,	// 正在载入粒子效果
		LT_SOUND,		// 正在载入声音效果
		LT_SPECIAL,		// 特殊物件
	};

	LoadingType m_loadingType;

	void skipToNextCloseBrace(Ogre::DataStreamPtr& stream);
	void skipToNextOpenBrace(Ogre::DataStreamPtr& stream);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CFileTreeCtrl m_fileTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedFiletree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickFiletree(NMHDR *pNMHDR, LRESULT *pResult);

	// 选中一个模型
	void selectItem(HTREEITEM hItem);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};

};
