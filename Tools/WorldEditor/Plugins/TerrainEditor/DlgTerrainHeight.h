#pragma once

#include "Resource.h"
// DlgTerrainHeight dialog

class DlgTerrainHeight : public CDialog
{
	DECLARE_DYNAMIC(DlgTerrainHeight)

public:
	DlgTerrainHeight(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgTerrainHeight();

// Dialog Data
	enum { IDD = IDD_DIALOG_TERRAIN_HEIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRaiseHeight();
	afx_msg void OnBnClickedButtonNone();
	afx_msg void OnBnClickedButtonRaiseUp();
	afx_msg void OnBnClickedButtonRaiseDown();
	afx_msg void OnBnClickedButtonSmooth();
	afx_msg void OnBnClickedButtonAvg();
	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};

	afx_msg void OnBnClickedButtonSelectterrain();
	afx_msg void OnBnClickedButtonCreateterrain();
	afx_msg void OnBnClickedButtonDeleteterrain();
	afx_msg void OnBnClickedButtonSetblockOn();
	afx_msg void OnBnClickedButtonSetblockOff();
	afx_msg void OnBnClickedButtonShowblock();
};
