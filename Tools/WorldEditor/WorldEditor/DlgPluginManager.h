#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgPluginManager dialog

class CDlgPluginManager : public CDialog
{
	DECLARE_DYNAMIC(CDlgPluginManager)

public:
	CDlgPluginManager(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPluginManager();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLUGIN_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstPlugins;
	CButton m_chkAutoLoad;
	CButton m_chkLoad;
	afx_msg void OnHdnItemchangedListPlugins(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
