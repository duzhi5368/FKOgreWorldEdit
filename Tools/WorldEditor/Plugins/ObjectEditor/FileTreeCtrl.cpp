// FileTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FileTreeCtrl.h"


// CFileTreeCtrl

IMPLEMENT_DYNAMIC(CFileTreeCtrl, CTreeCtrl)

CFileTreeCtrl::CFileTreeCtrl()
{

}

CFileTreeCtrl::~CFileTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CFileTreeCtrl, CTreeCtrl)
END_MESSAGE_MAP()



// CFileTreeCtrl message handlers


// ³õÊ¼»¯Ä¿Â¼Ê÷
size_t CFileTreeCtrl::loadTree(HTREEITEM hParent , const CString &strRootPath , const CString &strFileFilter , bool deleteNullDir , LoadTreeCallback *callback)
{
	CFileFind finder;
	size_t count = 0;
	BOOL bWorking = finder.FindFile(strRootPath + "\\*.*");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;
		HTREEITEM hItem;
		if(finder.IsDirectory())
		{
			hItem = InsertItem(finder.GetFileName() , hParent);
			count += loadTree(hItem , finder.GetFilePath() , strFileFilter , deleteNullDir , callback);
		}
		else
		{
			if(finder.GetFileName().Right(strFileFilter.GetLength()) != strFileFilter)
				continue;
			hItem = InsertItem(finder.GetFileName() , hParent);
			count++;
		}
		if(callback)
		{
			callback->loadFile(hItem , finder);
		}
	}
	if(count == 0 && hParent != TVI_ROOT && deleteNullDir)
	{
		DeleteItem(hParent);
	}
	return count;
}
