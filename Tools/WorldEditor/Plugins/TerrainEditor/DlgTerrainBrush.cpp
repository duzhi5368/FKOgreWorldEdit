// DlgTerrainBrush.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTerrainBrush.h"
#include "TinyXMLInclude.h"
#include "TerrainEditorPlugin.h"
#include "TerrainBrush.h"

// DlgTerrainBrush dialog

IMPLEMENT_DYNAMIC(DlgTerrainBrush, CDialog)

DlgTerrainBrush::DlgTerrainBrush(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTerrainBrush::IDD, pParent)
{

}

DlgTerrainBrush::~DlgTerrainBrush()
{
}

void DlgTerrainBrush::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cmbStyle);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_stcSize);
	DDX_Control(pDX, IDC_SLIDER_SIZE, m_sliSize);
	DDX_Control(pDX, IDC_STATIC_STRENGTH, m_stcStrength);
	DDX_Control(pDX, IDC_SLIDER_STRENTH, m_sliStrength);
}


BEGIN_MESSAGE_MAP(DlgTerrainBrush, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &DlgTerrainBrush::OnCbnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &DlgTerrainBrush::OnCbnSelchangeComboStyle)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SIZE, &DlgTerrainBrush::OnNMReleasedcaptureSliderSize)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_STRENTH, &DlgTerrainBrush::OnNMReleasedcaptureSliderStrenth)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SIZE, &DlgTerrainBrush::OnNMCustomdrawSliderSize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_STRENTH, &DlgTerrainBrush::OnNMCustomdrawSliderStrenth)
END_MESSAGE_MAP()


// DlgTerrainBrush message handlers

void DlgTerrainBrush::OnCbnSelchangeComboType()
{
	m_cmbStyle.ResetContent();
	switch(m_cmbType.GetCurSel())
	{
	case 0:
	case 1:
		m_cmbStyle.AddString("圆形");
		m_cmbStyle.AddString("方形");
		m_cmbStyle.AddString("X轴条形");
		m_cmbStyle.AddString("Y轴条形");
		break;
	case 2:
		StringVector names = TerrainEditorPlugin::getSingleton().getWorldEditor()->getSettingManager()->enumSettingName("自定义笔刷");
		for(size_t i = 0 ; i < names.size() ; i ++)
		{
			m_cmbStyle.AddString(names[i].c_str());
		}
		m_cmbStyle.AddString("自定义...");
		break;
	}
	m_cmbStyle.SetCurSel(0);
	OnCbnSelchangeComboStyle();
}

void DlgTerrainBrush::OnCbnSelchangeComboStyle()
{
	CString strText;
	m_cmbStyle.GetLBText(m_cmbStyle.GetCurSel() , strText);
	switch(m_cmbType.GetCurSel())
	{
	case 0:
		loadBrushFromFile((LPCSTR)(strText + "_软.tga"));
		break;
	case 1:
		loadBrushFromFile((LPCSTR)(strText + "_硬.tga"));
		break;
	case 2:
		if(strText == "自定义...")
		{
			// 打开一个新文件
			String fileName = TerrainEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->showOpenImageDialog("请选择笔刷图片");
			if(!fileName.empty())
			{
				String outBaseName , outPath;
				StringUtil::splitFilename(fileName , outBaseName , outPath);
				// 载入笔刷
				if(loadBrushFromFile(outBaseName))
				{
					TerrainEditorPlugin::getSingleton().getWorldEditor()->getSettingManager()->setString(outBaseName , outPath , "自定义笔刷");
					TerrainEditorPlugin::getSingleton().getWorldEditor()->getSettingManager()->flush();
					OnCbnSelchangeComboType();
					m_cmbStyle.SelectString(0 , outBaseName.c_str());
				}
			}
		}
		else
		{
			loadBrushFromFile((LPCSTR)strText);
		}		
		break;
	}

}

BOOL DlgTerrainBrush::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_cmbType.AddString("内置软笔刷");
	m_cmbType.AddString("内置硬笔刷");
	m_cmbType.AddString("自定义");
	m_cmbType.SetCurSel(0);
	OnCbnSelchangeComboType();

	m_sliSize.SetRange(1 , 64);
	m_sliSize.SetLineSize(1);
	m_sliSize.SetPos(10);

	m_sliStrength.SetRange(1 , 200);
	m_sliStrength.SetLineSize(1);
	m_sliStrength.SetPos(100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DlgTerrainBrush::OnNMReleasedcaptureSliderSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void DlgTerrainBrush::OnNMReleasedcaptureSliderStrenth(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void DlgTerrainBrush::OnNMCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CString strText;
	strText.Format("笔刷大小：%d" , m_sliSize.GetPos());	
	m_stcSize.SetWindowText(strText);
	TerrainEditorPlugin::getSingleton().getTerrainBrush()->setSize(m_sliSize.GetPos());
	*pResult = 0;
}

void DlgTerrainBrush::OnNMCustomdrawSliderStrenth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CString strText;
	float strength = m_sliStrength.GetPos() / ((float)m_sliStrength.GetRangeMax() / 2.0f);
	strText.Format("笔刷力度：%.2f" , strength);	
	m_stcStrength.SetWindowText(strText);
	TerrainEditorPlugin::getSingleton().getTerrainBrush()->setStrength(strength);
	*pResult = 0;
}

// 从文件载入笔刷
bool DlgTerrainBrush::loadBrushFromFile(const String &fileName)
{
	return TerrainEditorPlugin::getSingleton().getTerrainBrush()->loadBrushFromFile(fileName);
}
