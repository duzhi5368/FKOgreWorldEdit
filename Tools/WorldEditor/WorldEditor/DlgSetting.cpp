// DlgSetting.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "DlgSetting.h"
#include "SettingManager.h"


// CDlgSetting dialog

IMPLEMENT_DYNAMIC(CDlgSetting, CDialog)

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetting::IDD, pParent)
{

}

CDlgSetting::~CDlgSetting()
{
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPERTY_GRID_PLACER, m_stcPropertyGridPlacer);
}


BEGIN_MESSAGE_MAP(CDlgSetting, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSetting message handlers

BOOL CDlgSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CRect rc;
	m_stcPropertyGridPlacer.GetWindowRect( &rc );
	ScreenToClient( &rc );

	// create the property grid.
	if ( m_wndPropertyGrid.Create( rc, this, IDC_PROPERTY_GRID ) )
	{
		m_wndPropertyGrid.SetVariableItemsHeight(TRUE);

		TiXmlDocumentPtr & setting = Ogre::SettingManager::getSingleton().getSetting();
		TiXmlElement* group = 0;
		// 遍历所有分组
		for(group = (TiXmlElement*)setting->RootElement()->FirstChild()
			; group 
			; group = (TiXmlElement*)group->NextSibling()
			)
		{
			CXTPPropertyGridItem* groupSetting = m_wndPropertyGrid.AddCategory(group->Value());
			TiXmlElement* value = 0;
			// 遍历所有值
			for(value = (TiXmlElement*)group->FirstChild()
				; value
				; value = (TiXmlElement*)value->NextSibling()
				)
			{
				Ogre::SettingType st;
				value->Attribute("type" , (int*)&st);
				groupSetting->AddChildItem(new CXTPPropertyGridItem(value->Value() , value->Attribute("value")));
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetting::OnBnClickedOk()
{
	TiXmlDocumentPtr & setting = Ogre::SettingManager::getSingleton().getSetting();
	TiXmlElement* group = 0;
	int groupIndex = 0;
	// 遍历所有分组
	for(group = (TiXmlElement*)setting->RootElement()->FirstChild()
		; group 
		; group = (TiXmlElement*)group->NextSibling()
		)
	{
		CXTPPropertyGridItem* groupSetting = m_wndPropertyGrid.GetCategories()->GetAt(groupIndex++);
		TiXmlElement* value = 0;
		int valueIndex = 0;
		// 遍历所有值
		for(value = (TiXmlElement*)group->FirstChild()
			; value
			; value = (TiXmlElement*)value->NextSibling()
			)
		{
			CXTPPropertyGridItem* item = groupSetting->GetChilds()->GetAt(valueIndex++);
			value->SetAttribute("value" , (LPCSTR)item->GetValue());
		}
	}
	// 马上保存
	Ogre::SettingManager::getSingleton().flush();

	OnOK();
}
