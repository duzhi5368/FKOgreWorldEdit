#pragma once

#include "Resource.h"
#include "DlgPlaceSceneObject.h"
// DlgEditTools dialog

class DlgEditTools : public CDialog
{
	DECLARE_DYNAMIC(DlgEditTools)

public:
	DlgEditTools(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgEditTools();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonDrag();
	afx_msg void OnBnClickedButtonMoveX();
	afx_msg void OnBnClickedButtonRotateX();
	afx_msg void OnBnClickedButtonScaleX();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClone();
	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};

	afx_msg void OnBnClickedButtonPlacefloor();
};
