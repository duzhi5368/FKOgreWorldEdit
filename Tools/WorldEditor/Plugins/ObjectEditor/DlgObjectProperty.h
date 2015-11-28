#pragma once
#include "resource.h"

using namespace Ogre;
typedef std::list<PropertyContainer*> ListPropertyContainer;

// 属性编辑器
class DlgObjectProperty : public CDialog
{
	DECLARE_DYNAMIC(DlgObjectProperty)

public:
	DlgObjectProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgObjectProperty();

// Dialog Data
	enum { IDD = IDD_DIALOG_OBJECT_PROPERTY };


	// 刷新属性
	void refreshProperty(PropertyContainer *obj);
	// 刷新属性
	void refreshProperty(const ListPropertyContainer &obj);
	// 刷新属性
	void refreshProperty();
	// 清除属性
	void clearProperty();
protected:
	// 添加属性
	void addProperty(PropertyContainer *obj , CXTPPropertyGridItem *parent , const PropertyContainer::VTProperty *properties);
	// 填充属性
	void fillProperty(PropertyContainer *obj , CXTPPropertyGridItem *item , const PropertyContainer::Property &property);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CXTPPropertyGrid m_wndProperty;
	typedef std::map<uint , CXTPPropertyGridItem *> MapGridItem;
	// 用来存储属性ID到CXTPPropertyGridItem的关联
	MapGridItem m_mapGridItem;
	// 属性对象
	ListPropertyContainer m_listObject;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 防止按Enter和Esc把窗口关闭了
	virtual void OnOK(){};
	virtual void OnCancel(){};

};
