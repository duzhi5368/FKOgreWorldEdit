#pragma once
#include "Resource.h"

// DlgTerrainBrush dialog
using namespace Ogre;
class DlgTerrainBrush : public CDialog
{
	DECLARE_DYNAMIC(DlgTerrainBrush)

public:
	DlgTerrainBrush(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgTerrainBrush();

// Dialog Data
	enum { IDD = IDD_DIALOG_TERRAIN_BRUSH };
	CSliderCtrl m_sliSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CComboBox m_cmbType;
	CComboBox m_cmbStyle;
	CStatic m_stcSize;
	CStatic m_stcStrength;
	CSliderCtrl m_sliStrength;

	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnCbnSelchangeComboStyle();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMReleasedcaptureSliderSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderStrenth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderStrenth(NMHDR *pNMHDR, LRESULT *pResult);
public:
	// 从文件载入笔刷
	bool loadBrushFromFile(const String &fileName);
	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};

};
