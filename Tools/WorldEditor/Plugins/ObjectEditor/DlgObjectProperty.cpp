// DlgObjectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "DlgObjectProperty.h"
#include "ObjectEditorPlugin.h"

using namespace Ogre;

// DlgObjectProperty dialog

IMPLEMENT_DYNAMIC(DlgObjectProperty, CDialog)

DlgObjectProperty::DlgObjectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(DlgObjectProperty::IDD, pParent)
{
}

DlgObjectProperty::~DlgObjectProperty()
{
}

void DlgObjectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROPERTY, m_wndProperty);
}


BEGIN_MESSAGE_MAP(DlgObjectProperty, CDialog)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// DlgObjectProperty message handlers


// 刷新属性
void DlgObjectProperty::refreshProperty(PropertyContainer *obj)
{
	clearProperty();
	m_listObject.clear();
	m_listObject.push_back(obj);
	refreshProperty();
}
// 刷新属性
void DlgObjectProperty::refreshProperty(const ListPropertyContainer &obj)
{
	clearProperty();
	m_listObject = obj;
	refreshProperty();
}

// 清除属性
void DlgObjectProperty::clearProperty()
{
	m_wndProperty.ResetContent();
	m_listObject.clear();
	m_mapGridItem.clear();
}

// 刷新属性
void DlgObjectProperty::refreshProperty()
{
	if(m_wndProperty.GetSafeHwnd() == 0)
		return;
	m_wndProperty.ResetContent();
	m_mapGridItem.clear();
	// 遍历所有对象的所有属性
	for(ListPropertyContainer::const_iterator iter = m_listObject.begin() ; iter != m_listObject.end() ; iter ++)
	{
		PropertyContainer *obj = *iter;
		const PropertyContainer::VTProperty *properties = obj->getPropertiesTree();
		for(PropertyContainer::VTProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
		{
			CXTPPropertyGridItem* item = 0;
			if(m_mapGridItem.find(iter->id) != m_mapGridItem.end())
			{
				item = m_mapGridItem[iter->id];
			}
			else
			{
				item = m_wndProperty.AddCategory(iter->name.c_str());
				fillProperty(obj , item , *iter);
			}
			addProperty(obj , item , &iter->childs);
			item->Expand();
		}
	}
}
// 填充属性
void DlgObjectProperty::fillProperty(PropertyContainer *obj , CXTPPropertyGridItem *item , const PropertyContainer::Property &property)
{
	item->SetReadOnly(!property.editable);
	item->SetValue(obj->getPropertyByID(property.id).c_str());
	item->SetDescription(property.desc.c_str());
	item->SetID(property.id);
	m_mapGridItem[property.id] = item;
}

// 添加属性
void DlgObjectProperty::addProperty(PropertyContainer *obj , CXTPPropertyGridItem *parent , const Ogre::PropertyContainer::VTProperty *properties)
{
	for(PropertyContainer::VTProperty::const_iterator iter = properties->begin() ; iter != properties->end() ; iter ++)
	{
		if(!iter->showable)
			continue;
		CXTPPropertyGridItem* item = 0;
		if(m_mapGridItem.find(iter->id) == m_mapGridItem.end())
		{
			switch(iter->type)
			{
			case PropertyContainer::PT_GROUP:
				{
					item = parent->AddChildItem(new CXTPPropertyGridItemCategory(iter->name.c_str()));
					item->Expand();
					addProperty(obj , item , &iter->childs);
				}
				break;
			case PropertyContainer::PT_BOOL:
				{
					item = parent->AddChildItem(new CXTPPropertyGridItemBool(iter->name.c_str()));
					static_cast<CXTPPropertyGridItemBool*>(item)->SetCheckBoxStyle(TRUE);
				}
				break;
			case PropertyContainer::PT_REAL:
				{
					item = parent->AddChildItem(new CXTPPropertyGridItemDouble(iter->name.c_str()));
				}
				break;
			case PropertyContainer::PT_INT:
			case PropertyContainer::PT_UINT:
			case PropertyContainer::PT_SHORT:
			case PropertyContainer::PT_USHORT:
			case PropertyContainer::PT_LONG:
			case PropertyContainer::PT_ULONG:
				{
					item = parent->AddChildItem(new CXTPPropertyGridItemNumber(iter->name.c_str()));
				}
				break;
			case PropertyContainer::PT_STRING:
			case PropertyContainer::PT_VECTOR2:
			case PropertyContainer::PT_VECTOR3:
			case PropertyContainer::PT_MATRIX3:
			case PropertyContainer::PT_MATRIX4:
			case PropertyContainer::PT_QUATERNION:
			case PropertyContainer::PT_COLOURVALUE:
			case PropertyContainer::PT_AXISALIGNEDBOX:
				{
					item = parent->AddChildItem(new CXTPPropertyGridItem(iter->name.c_str()));
				}
				break;
			default:
				continue;
				break;
			}
			fillProperty(obj , item , *iter);
		}
	}
}



LRESULT DlgObjectProperty::OnGridNotify(WPARAM wParam, LPARAM lParam)
{

	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"), pItem->GetCaption(), pItem->GetID(), pItem->GetValue());
		if(!m_listObject.empty())
		{
			// 遍历所有对象的所有属性
			for(ListPropertyContainer::iterator iter = m_listObject.begin() ; iter != m_listObject.end() ; iter ++)
			{
				PropertyContainer *obj = *iter;
				String oldValue = obj->getPropertyByName(String(pItem->GetCaption()));
				try
				{
					obj->setPropertyByName(String(pItem->GetCaption()) , String(pItem->GetValue()));
				}
				catch(Exception &e)
				{
					MessageBox(e.getFullDescription().c_str(), "设置属性时出错", MB_OK | MB_ICONERROR | MB_TASKMODAL);
					pItem->SetValue(oldValue.c_str());
					obj->setPropertyByName(String(pItem->GetCaption()) , oldValue);
				}
			}
		}
	}
	return 0;
}
void DlgObjectProperty::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_wndProperty.GetSafeHwnd())
	{
		m_wndProperty.SetWindowPos(0 , 0 , 0 , cx , cy , 0);
	}
}
