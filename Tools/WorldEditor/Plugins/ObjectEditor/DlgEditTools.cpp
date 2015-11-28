// DlgEditTools.cpp : implementation file
//

#include "stdafx.h"
#include "DlgEditTools.h"
#include "ObjectEditorPlugin.h"

#include "ObjectSelectAction.h"
#include "ObjectDragAction.h"
#include "ObjectMoveAction.h"
#include "ObjectRotateAction.h"
#include "ObjectScaleAction.h"
#include "ObjectDeleteAction.h"
#include "ObjectCloneAction.h"
#include "ObjectPlaceFloorAction.h"

// DlgEditTools dialog
using namespace Ogre;
IMPLEMENT_DYNAMIC(DlgEditTools, CDialog)

DlgEditTools::DlgEditTools(CWnd* pParent /*=NULL*/)
	: CDialog(DlgEditTools::IDD, pParent)
{

}

DlgEditTools::~DlgEditTools()
{
}

void DlgEditTools::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgEditTools, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &DlgEditTools::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_DRAG, &DlgEditTools::OnBnClickedButtonDrag)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_X, &DlgEditTools::OnBnClickedButtonMoveX)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_X, &DlgEditTools::OnBnClickedButtonRotateX)
	ON_BN_CLICKED(IDC_BUTTON_SCALE_X, &DlgEditTools::OnBnClickedButtonScaleX)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &DlgEditTools::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, &DlgEditTools::OnBnClickedButtonClone)
	ON_BN_CLICKED(IDC_BUTTON_PLACEFLOOR, &DlgEditTools::OnBnClickedButtonPlacefloor)
END_MESSAGE_MAP()


// DlgEditTools message handlers

void DlgEditTools::OnBnClickedButtonSelect()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new ObjectSelectAction());
}

void DlgEditTools::OnBnClickedButtonDrag()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new ObjectDragAction());
}

void DlgEditTools::OnBnClickedButtonMoveX()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new ObjectMoveAction());
}

void DlgEditTools::OnBnClickedButtonRotateX()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new ObjectRotateAction());
}

void DlgEditTools::OnBnClickedButtonScaleX()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new ObjectScaleAction());
}

void DlgEditTools::OnBnClickedButtonDelete()
{
	ObjectDeleteAction action;
	action.execute();
}
void DlgEditTools::OnBnClickedButtonClone()
{
	ObjectCloneAction action;
	action.execute();
}

void DlgEditTools::OnBnClickedButtonPlacefloor()
{
	ObjectPlaceFloorAction action;
	action.execute();
}
