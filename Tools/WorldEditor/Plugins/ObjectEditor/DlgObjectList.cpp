// DlgObjectList.cpp : implementation file
//

#include "stdafx.h"
#include "DlgObjectList.h"
#include "ObjectEditorPlugin.h"

using namespace Ogre;


// DlgObjectList dialog

IMPLEMENT_DYNAMIC(DlgObjectList, CDialog)

DlgObjectList::DlgObjectList(CWnd* pParent /*=NULL*/)
	: CDialog(DlgObjectList::IDD, pParent)
	, m_listState(LS_Free)
{

}

DlgObjectList::~DlgObjectList()
{
}

void DlgObjectList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECT_LIST, m_list);
}


BEGIN_MESSAGE_MAP(DlgObjectList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &DlgObjectList::OnBnClickedButtonRefresh)
//	ON_NOTIFY(HDN_ITEMCHANGED, 0, &DlgObjectList::OnHdnItemchangedObjectList)
//	ON_NOTIFY(HDN_ITEMCHANGING, 0, &DlgObjectList::OnHdnItemchangingObjectList)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_OBJECT_LIST, &DlgObjectList::OnLvnItemchangingObjectList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJECT_LIST, &DlgObjectList::OnLvnItemchangedObjectList)
END_MESSAGE_MAP()


// DlgObjectList message handlers
// 重新载入物件列表
void DlgObjectList::reloadList()
{ 
	if(m_listState != LS_Free)
		return;
	m_listState = LS_Processing;
	m_list.DeleteAllItems();
	IMapManager * mapManager = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager();
	IScene::SceneObjectIterator iter = mapManager->getScene()->getSceneObjectIterator();
	while(iter.hasMoreElements())
	{
		ISceneObject *obj = iter.getNext();
		if(ObjectEditorPlugin::getSingleton().isEditable(obj))
		{
			int item = m_list.InsertItem(m_list.GetItemCount() , obj->getName().c_str());
			m_list.SetItemText(item , 1 , obj->getType().c_str());
			if(mapManager->isObjectSelected(obj))
			{
				m_list.SetItemState(item , LVIS_SELECTED , LVIS_SELECTED);
			}
			m_list.SetItemData(item , (DWORD)obj);
		}
	}
	m_listState = LS_Free;
}


// 选中一个场景对象
void DlgObjectList::onSelectObject(Ogre::ISceneObject *obj)
{
	if(m_listState != LS_Free || m_list.GetSafeHwnd() == 0)
		return;
	m_listState = LS_Processing;
	int count = m_list.GetItemCount();
	for(int i = 0 ; i < count ; i ++)
	{
		if(m_list.GetItemData(i) == (DWORD)obj)
		{
			m_list.SetItemState(i , LVIS_SELECTED , LVIS_SELECTED);
		}
	}
	m_listState = LS_Free;
}
// 选中一批场景对象
void DlgObjectList::onSelectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject)
{
	if(m_listState != LS_Free || m_list.GetSafeHwnd() == 0)
		return;
	m_listState = LS_Processing;
	int count = m_list.GetItemCount();

	for(int i = 0 ; i < count ; i ++)
	{
		DWORD data = m_list.GetItemData(i);
		if(std::find(listSceneObject.begin() , listSceneObject.end() , (ISceneObject*)data) != listSceneObject.end())
		{
			m_list.SetItemState(i , LVIS_SELECTED , LVIS_SELECTED);
		}
	}
	m_listState = LS_Free;
}
// 反选一个场景对象
void DlgObjectList::onDeselectObject(Ogre::ISceneObject *obj)
{
	if(m_listState != LS_Free || m_list.GetSafeHwnd() == 0)
		return;
	m_listState = LS_Processing;
	int count = m_list.GetItemCount();
	for(int i = 0 ; i < count ; i ++)
	{
		if(m_list.GetItemData(i) == (DWORD)obj)
		{
			m_list.SetItemState(i , 0 , LVIS_SELECTED);
		}
	}
	m_listState = LS_Free;
}
// 反选一批场景对象
void DlgObjectList::onDeselectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject)
{
	if(m_listState != LS_Free || m_list.GetSafeHwnd() == 0)
		return;
	m_listState = LS_Processing;
	int count = m_list.GetItemCount();

	for(int i = 0 ; i < count ; i ++)
	{
		DWORD data = m_list.GetItemData(i);
		if(std::find(listSceneObject.begin() , listSceneObject.end() , (ISceneObject*)data) != listSceneObject.end())
		{
			m_list.SetItemState(i , 0 , LVIS_SELECTED);
		}
	}
	m_listState = LS_Free;
}

void DlgObjectList::OnBnClickedButtonRefresh()
{
	reloadList();
}

BOOL DlgObjectList::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  Add extra initialization here
	m_list.InsertColumn(0 , "物件名称"  , LVCFMT_LEFT , 100);
	m_list.InsertColumn(1 , "物件类型"  , LVCFMT_LEFT , 100);
	reloadList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DlgObjectList::OnLvnItemchangingObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void DlgObjectList::OnLvnItemchangedObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(m_listState == LS_Free)
	{
		m_listState = LS_Selecting;
		ISceneObject *obj = (ISceneObject*)m_list.GetItemData(pNMLV->iItem);
		if(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->isSceneObjectValid(obj))
		{
			if(m_list.GetItemState(pNMLV->iItem , LVIS_SELECTED))
			{
				ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(obj);
			}
			else
			{
				ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
			}
		}
		m_listState = LS_Free;
	}
	*pResult = 0;
}
