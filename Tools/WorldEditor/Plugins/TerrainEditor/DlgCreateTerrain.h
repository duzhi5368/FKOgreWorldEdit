#pragma once
#include "resource.h"


// DlgCreateTerrain dialog

class DlgCreateTerrain : public CDialog
{
	DECLARE_DYNAMIC(DlgCreateTerrain)

public:
	DlgCreateTerrain(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgCreateTerrain();

// Dialog Data
	enum { IDD = IDD_DIALOG_CREATE_TERRAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	CComboBox m_cmbTileX;
	CComboBox m_cmbTileY;
	CEdit m_edtSize;
	afx_msg void OnCbnSelchangeComboTileX();
	afx_msg void OnCbnSelchangeComboTileY();
};
