// DlgTerrainPaint.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTerrainPaint.h"
#include "TerrainEditorPlugin.h"
#include "PaintAction.h"

using namespace Ogre;

// DlgTerrainPaint dialog

IMPLEMENT_DYNAMIC(DlgTerrainPaint, CDialog)

DlgTerrainPaint::DlgTerrainPaint(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTerrainPaint::IDD, pParent)
{

}

DlgTerrainPaint::~DlgTerrainPaint()
{
}

void DlgTerrainPaint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	for(int i = 0 ; i < SPLATTING_TEXTURE_NUM ; i ++)
	{
		DDX_Control(pDX, IDC_BUTTON_TEXTURE_0 + i, m_texture[i]);
	}
}


BEGIN_MESSAGE_MAP(DlgTerrainPaint, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_0, &DlgTerrainPaint::OnBnClickedStaticTexture0)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_1, &DlgTerrainPaint::OnBnClickedStaticTexture1)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_2, &DlgTerrainPaint::OnBnClickedStaticTexture2)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_3, &DlgTerrainPaint::OnBnClickedStaticTexture3)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_4, &DlgTerrainPaint::OnBnClickedStaticTexture4)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_5, &DlgTerrainPaint::OnBnClickedStaticTexture5)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_TEXTURE_0, &DlgTerrainPaint::OnBnDblclickStaticTexture0)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_TEXTURE_1, &DlgTerrainPaint::OnBnDblclickStaticTexture1)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_TEXTURE_2, &DlgTerrainPaint::OnBnDblclickStaticTexture2)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_TEXTURE_3, &DlgTerrainPaint::OnBnDblclickStaticTexture3)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_TEXTURE_4, &DlgTerrainPaint::OnBnDblclickStaticTexture4)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_TEXTURE_5, &DlgTerrainPaint::OnBnDblclickStaticTexture5)
END_MESSAGE_MAP()


// DlgTerrainPaint message handlers

void DlgTerrainPaint::OnBnClickedStaticTexture0()
{
	activateTexture(0);
}

void DlgTerrainPaint::OnBnClickedStaticTexture1()
{
	activateTexture(1);
}

void DlgTerrainPaint::OnBnClickedStaticTexture2()
{
	activateTexture(2);
}

void DlgTerrainPaint::OnBnClickedStaticTexture3()
{
	activateTexture(3);
}

void DlgTerrainPaint::OnBnClickedStaticTexture4()
{
	activateTexture(4);
}

void DlgTerrainPaint::OnBnClickedStaticTexture5()
{
	activateTexture(5);
}


void DlgTerrainPaint::OnBnDblclickStaticTexture0()
{
	changeTexture(0);
}

void DlgTerrainPaint::OnBnDblclickStaticTexture1()
{
	changeTexture(1);
}

void DlgTerrainPaint::OnBnDblclickStaticTexture2()
{
	changeTexture(2);
}

void DlgTerrainPaint::OnBnDblclickStaticTexture3()
{
	changeTexture(3);
}

void DlgTerrainPaint::OnBnDblclickStaticTexture4()
{
	changeTexture(4);
}

void DlgTerrainPaint::OnBnDblclickStaticTexture5()
{
	changeTexture(5);
}

// 刷新纹理
void DlgTerrainPaint::refreshTexture(size_t index)
{
	Technique *tech = TerrainEditorPlugin::getSingleton().getActivatedTerrain()->getTerrainImpl()->getMaterial()->getTechnique(0);
	Pass *pass = tech->getPass(index / SPLATTING_TEXTURE_NUM);
	if(pass)
	{
		TextureUnitState* pState = pass->getTextureUnitState((ushort)COVERAGE_TEXTURE_NUM + index);
		const TexturePtr &pTexture = pState->_getTexturePtr();
		HardwarePixelBufferSharedPtr pBuffer = pTexture->getBuffer();
		int iWidth = 62;
		int iHeight = 62;
		// 全部图像数据保存到pVoid中
		uchar *pVoid = new uchar[iWidth * iHeight * 4];
		// add by 王宏张 2007-8-30
		// GDI必须使用PF_R5G6B5才支持16位色深
		PixelBox box(iWidth , iHeight , 1 , PF_X8R8G8B8/*PF_R5G6B5*/ , pVoid);
		pBuffer->blitToMemory(box);

		// add by 王宏张 2007-8-19
		// 创建到ImageList中
		CBitmap bitmap;
		bitmap.CreateBitmap(iWidth , iHeight ,1 , 32 , pVoid);

		m_texture[index].SetIcon(CSize(iWidth , iHeight)
			, CXTPTransparentBitmap(bitmap).ConvertToIcon());
		OGRE_DELETE_ARRAY(pVoid);
	}

}

// 双击一个纹理
void DlgTerrainPaint::changeTexture(size_t index)
{
	if(!TerrainEditorPlugin::getSingleton().getActivatedTerrain())
		return;
	String texFile = TerrainEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->showOpenImageDialog("请选择一个地表贴图");
	if(!texFile.empty())
	{
		String fileName , filePath;
		StringUtil::splitFilename(texFile , fileName , filePath);
		// 替换地形纹理
		if(TerrainEditorPlugin::getSingleton().getActivatedTerrain()->getTerrainImpl()->setPaintTexutreName(index , fileName))
		{
			refreshTexture(index);
		}
		else
		{
			AfxMessageBox(("打开纹理失败" + texFile).c_str());
		}
		activateTexture(index);
	}
}

// 激活一个纹理
void DlgTerrainPaint::activateTexture(size_t index)
{
	if(!TerrainEditorPlugin::getSingleton().getActivatedTerrain())
		return;
	for(int i = 0 ; i < SPLATTING_TEXTURE_NUM ; i ++)
	{
		m_texture[i].SetStateX(FALSE);
	}
	m_texture[index].SetStateX(TRUE);

	TerrainEditorPlugin &plugin = TerrainEditorPlugin::getSingleton();
	plugin.setTerrainAction(new PaintAction(index , plugin.getActivatedTerrain()
		, plugin.getSelectionRenderer()
		, plugin.getSelection() 
		, plugin.getTerrainBrush()
		));
}
// 刷新所有纹理
void DlgTerrainPaint::refreshAllTexture()
{
	if(!TerrainEditorPlugin::getSingleton().getActivatedTerrain())
	{
		return;
	}
	for(int i = 0 ; i < SPLATTING_TEXTURE_NUM ; i ++)
	{
		refreshTexture(i);
	}
}
BOOL DlgTerrainPaint::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	for(int i = 0 ; i < SPLATTING_TEXTURE_NUM ; i ++)
	{
		m_texture[i].ShowWindow(SW_SHOW);
	}

	refreshAllTexture();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
