#pragma once

#include "Resource.h"
#include "ETPrerequisites.h"
// DlgTerrainPaint dialog

class DlgTerrainPaint : public CDialog
{
	DECLARE_DYNAMIC(DlgTerrainPaint)

public:
	DlgTerrainPaint(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgTerrainPaint();

// Dialog Data
	enum { IDD = IDD_DIALOG_TERRAIN_PAINT };

protected:
	CXTButton m_texture[SPLATTING_TEXTURE_NUM];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStaticTexture0();
	afx_msg void OnBnClickedStaticTexture1();
	afx_msg void OnBnClickedStaticTexture2();
	afx_msg void OnBnClickedStaticTexture3();
	afx_msg void OnBnClickedStaticTexture4();
	afx_msg void OnBnClickedStaticTexture5();
	afx_msg void OnBnDblclickStaticTexture0();
	afx_msg void OnBnDblclickStaticTexture1();
	afx_msg void OnBnDblclickStaticTexture2();
	afx_msg void OnBnDblclickStaticTexture3();
	afx_msg void OnBnDblclickStaticTexture4();
	afx_msg void OnBnDblclickStaticTexture5();

	// 刷新所有纹理
	void refreshAllTexture();
	// 刷新纹理
	void refreshTexture(size_t index);
	// 修改一个纹理
	void changeTexture(size_t index);
	// 激活一个纹理
	void activateTexture(size_t index);
	virtual BOOL OnInitDialog();

	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};
};
