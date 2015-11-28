// DlgTerrainHeight.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTerrainHeight.h"
#include "TerrainEditorPlugin.h"
#include "RaiseHeightAction.h"
#include "RaiseUpDownAction.h"
#include "SmoothAction.h"
#include "AvgFlattenAction.h"
#include "SelectTerrainAction.h"
#include "DlgCreateTerrain.h"
#include "BlockerAction.h"


// DlgTerrainHeight dialog

IMPLEMENT_DYNAMIC(DlgTerrainHeight, CDialog)

DlgTerrainHeight::DlgTerrainHeight(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTerrainHeight::IDD, pParent)
{

}

DlgTerrainHeight::~DlgTerrainHeight()
{
}

void DlgTerrainHeight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgTerrainHeight, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RAISE_HEIGHT, &DlgTerrainHeight::OnBnClickedButtonRaiseHeight)
	ON_BN_CLICKED(IDC_BUTTON_NONE, &DlgTerrainHeight::OnBnClickedButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_RAISE_UP, &DlgTerrainHeight::OnBnClickedButtonRaiseUp)
	ON_BN_CLICKED(IDC_BUTTON_RAISE_DOWN, &DlgTerrainHeight::OnBnClickedButtonRaiseDown)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH, &DlgTerrainHeight::OnBnClickedButtonSmooth)
	ON_BN_CLICKED(IDC_BUTTON_AVG, &DlgTerrainHeight::OnBnClickedButtonAvg)
	ON_BN_CLICKED(IDC_BUTTON_SELECTTERRAIN, &DlgTerrainHeight::OnBnClickedButtonSelectterrain)
	ON_BN_CLICKED(IDC_BUTTON_CREATETERRAIN, &DlgTerrainHeight::OnBnClickedButtonCreateterrain)
	ON_BN_CLICKED(IDC_BUTTON_DELETETERRAIN, &DlgTerrainHeight::OnBnClickedButtonDeleteterrain)
	ON_BN_CLICKED(IDC_BUTTON_SETBLOCK_ON, &DlgTerrainHeight::OnBnClickedButtonSetblockOn)
	ON_BN_CLICKED(IDC_BUTTON_SETBLOCK_OFF, &DlgTerrainHeight::OnBnClickedButtonSetblockOff)
	ON_BN_CLICKED(IDC_BUTTON_SHOWBLOCK, &DlgTerrainHeight::OnBnClickedButtonShowblock)
END_MESSAGE_MAP()


// DlgTerrainHeight message handlers

void DlgTerrainHeight::OnBnClickedButtonRaiseHeight()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new RaiseHeightAction(plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonNone()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new SingleSelectionAction(plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonRaiseUp()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new RaiseUpDownAction(true
		, plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonRaiseDown()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new RaiseUpDownAction(false
		, plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonSmooth()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new SmoothAction(plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonAvg()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new AvgFlattenAction(plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonSelectterrain()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	plugin.setTerrainAction(new SelectTerrainAction(plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonCreateterrain()
{
	DlgCreateTerrain dlg;
	dlg.DoModal();
}

void DlgTerrainHeight::OnBnClickedButtonDeleteterrain()
{
	IScene *scene = TerrainEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene();

	// 检查是否已经创建
	if(!scene->hasSceneObject("Terrain_0_0"))
	{
		AfxMessageBox("地形已经不存在，删除失败。");
		return;
	}
	if(AfxMessageBox("是否真的要删除地形？" , MB_YESNOCANCEL) == IDYES)
	{
		TerrainEditorPlugin::getSingleton().deactivateTerrain();
		scene->destroySceneObject("Terrain_0_0");
	}
}

void DlgTerrainHeight::OnBnClickedButtonSetblockOn()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new BlockerAction(true , plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonSetblockOff()
{
	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	// 如果数据未准备好，就失败
	if(!plugin.getActivatedTerrain()
		|| ! plugin.getSelectionRenderer()
		|| !plugin.getSelection() 
		|| !plugin.getTerrainBrush())
	{
		return;
	}
	plugin.setTerrainAction(new BlockerAction(false , plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}

void DlgTerrainHeight::OnBnClickedButtonShowblock()
{
	CWnd *wnd = GetDlgItem(IDC_BUTTON_SHOWBLOCK);
	CString strText;
	wnd->GetWindowText(strText);
	if(strText == "隐藏障碍")
	{
		TerrainEditorPlugin::getSingleton().removeTerrainBlockerPass();
		wnd->SetWindowText("显示障碍");

		GetDlgItem(IDC_BUTTON_SETBLOCK_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SETBLOCK_OFF)->EnableWindow(FALSE);

		TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
		if(dynamic_cast<BlockerAction*>(plugin.getTerrainAction()))
		{
			plugin.setTerrainAction(0);
		}
	}
	else
	{
		TerrainEditorPlugin::getSingleton().addTerrainBlockerPass();
		wnd->SetWindowText("隐藏障碍");
		GetDlgItem(IDC_BUTTON_SETBLOCK_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SETBLOCK_OFF)->EnableWindow(TRUE);
	}
}
