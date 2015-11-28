//////////////////////////////////////////////////////////////////////////////
//  For project details and authors, refer to "CELayoutEditor.cpp".
//
//  This file is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This file is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//  To view the licence online, go to: http://www.gnu.org/copyleft/gpl.html
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "CELayoutEditor.h"
#include "EditorCanvas.h"
#include "SelectionMover.h"
#include "DialogMain.h"
#include "DialogGrid.h"
#include "CEGUIDefaultResourceProvider.h"
#include "Config.h"

#include <wx/dir.h>
#include <wx/filename.h>

// Toolbar icons
#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/delete.xpm"
#include "bitmaps/help.xpm"
#include "bitmaps/unlock.xpm"
#include "bitmaps/lock.xpm"

//////////////////////////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////////////////////////

#define ASSERT_DOCUMENT_MSG wxT("The m_document member is NULL!")

//////////////////////////////////////////////////////////////////////////
// EDITORFRAME IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_CLASS(EditorFrame, wxDocParentFrame)

//////////////////////////////////////////////////////////////////////////
// EDITORFRAME EVENT TABLE
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
BEGIN_EVENT_TABLE(EditorFrame, wxDocParentFrame)

EVT_SIZE(EditorFrame::OnResize)

EVT_UPDATE_UI(ID_VIEW_640x480, EditorFrame::OnUpdate640x480)
EVT_MENU(ID_VIEW_640x480, EditorFrame::On640x480)
//
EVT_UPDATE_UI(ID_VIEW_800x600, EditorFrame::OnUpdate800x600)
EVT_MENU(ID_VIEW_800x600, EditorFrame::On800x600)
//
EVT_UPDATE_UI(ID_VIEW_1024x786, EditorFrame::OnUpdate1024x768)
EVT_MENU(ID_VIEW_1024x786, EditorFrame::On1024x768)
EVT_MENU(ID_VIEW_FULLSCREEN, EditorFrame::OnFullScreen)
//
EVT_UPDATE_UI(ID_EDIT_CLEAR, EditorFrame::OnUpdateEditClear)
EVT_MENU(ID_EDIT_CLEAR, EditorFrame::OnEditClear)
//
EVT_UPDATE_UI(ID_EDIT_COPY, EditorFrame::OnUpdateEditCopy)
EVT_MENU(ID_EDIT_COPY, EditorFrame::OnEditCopy)
//
EVT_UPDATE_UI(ID_EDIT_CUT, EditorFrame::OnUpdateEditCut)
EVT_MENU(ID_EDIT_CUT, EditorFrame::OnEditCut)
//
EVT_UPDATE_UI(ID_EDIT_PASTE, EditorFrame::OnUpdateEditPaste)
EVT_MENU(ID_EDIT_PASTE, EditorFrame::OnEditPaste)
EVT_MENU(ID_EDIT_SELECT_ALL, EditorFrame::OnSelectAll) 
//
EVT_UPDATE_UI(ID_VIEW_SET_BACKGROUND, EditorFrame::OnUpdateSetBackground)
EVT_MENU(ID_VIEW_SET_BACKGROUND, EditorFrame::OnSetBackground)
//
EVT_UPDATE_UI(ID_VIEW_SHOW_BACKGROUND, EditorFrame::OnUpdateShowBackground)
EVT_MENU(ID_VIEW_SHOW_BACKGROUND, EditorFrame::OnShowBackground)
EVT_MENU(ID_VIEW_SET_GRID, EditorFrame::OnSetGrid)
EVT_MENU(ID_VIEW_SET_FONT, EditorFrame::OnSetFont)
//
EVT_UPDATE_UI(ID_ALIGN, EditorFrame::OnUpdateAlign)
EVT_MENU(ID_ALIGN_LEFT, EditorFrame::OnAlignLeft)
EVT_MENU(ID_ALIGN_RIGHT, EditorFrame::OnAlignRight)
EVT_MENU(ID_ALIGN_TOP, EditorFrame::OnAlignTop)
EVT_MENU(ID_ALIGN_BOTTOM, EditorFrame::OnAlignBottom)
EVT_MENU(ID_ALIGN_WIDTH, EditorFrame::OnAlignWidth)
EVT_MENU(ID_ALIGN_HEIGHT, EditorFrame::OnAlignHeight)

EVT_MENU(wxID_SELECTALL, EditorFrame::OnSelectAll)    
EVT_UPDATE_UI(wxID_SELECTALL, EditorFrame::OnUpdateSelectAll) 
EVT_MENU(ID_SKINS, EditorFrame::OnLayoutSkin)
EVT_UPDATE_UI(ID_SKINS, EditorFrame::OnUpdateLayoutSkin)
EVT_UPDATE_UI(ID_LOCK_SELECTION, EditorFrame::OnUpdateLockSelection)
EVT_MENU(ID_LOCK_SELECTION, EditorFrame::OnLockSelection)

END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// EDITORFRAME FUNCTIONS
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
EditorFrame::EditorFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title,
                         const wxPoint& pos, const wxSize& size, const long type):
m_GUISystem(wx_static_cast(CEGUI::System*, NULL)),
m_GUIRenderer(wx_static_cast(CEGUI::OpenGLRenderer*, NULL)),
m_dialogMain(wx_static_cast(DialogMain*, NULL)),
m_renderCanvas(wx_static_cast(EditorCanvas*, NULL)),
m_document(wx_static_cast(EditorDocument*, NULL)),
mEditMenu(wx_static_cast(wxMenu*, NULL)),
wxDocParentFrame(manager, frame, id, title, pos, size, type)
{
    // NOTE: Prefer constructor initialization lists (which are 
    // often more efficient) to assignment inside the 
    // constructor. Members in the initialization list should 
    // appear in the order they are declared in the class, as 
    // this is the order they will be initialized (no matter what).
    // See item 4 in 'Effective C++ 3rd Edition' by Scott Meyers

    // Initialize dialog
    Setup();
}

//------------------------------------------------------------------------
EditorFrame::~EditorFrame()
{
    // Swallow any exceptions found to prevent the destructor from emitting them.
    // NOTE: An exception should never leave a destructor as it may yield
    // undefined behavior or even lead to program abnormal termination 
    // (see Item 8 in 'Effective C++ 3rd Edition' by Scott Meyers)
    try
    {
        // Windows exception handling specific cleanup
        // NOTE: We need to process this here and not in the CELayoutEditor destructor, because
        // by then the Logger singleton no longer exists and it will not be possible to log
        // this info.
        #if defined(WIN32) && defined(_DEBUG) && CHECK_STACK_TRACES
        // Verify if there's any "eaten" (caught but not processed in a try/catch block) 
        // exception to log
        // NOTE: Please note that "eaten" exceptions might only manifest themselves much later
        // than when they first appeared since they are only detected when a second exception
        // is thrown (or at program shutdown - which is the case here).
        #if CHECK_EATEN_EXCEPTIONS
        if(!ExceptionManager::GetInstancePtr()->IsExceptionProcessed())
            // NOTE: Always use LogSilentError here, because we're exiting the application (perhaps even in an unstable state) and there's no need to bother the user
            LogSilentError(wxT("An \"eaten\" exception (caught but not processed/logged) was found.\n\n\"EATEN\" EXCEPTION INFO:%s"), ExceptionManager::GetInstancePtr()->GetExceptionVEHInfo());
        #endif // CHECK_EATEN_EXCEPTIONS

        // Unregister VEH exception handler
        ExceptionManager::GetInstancePtr()->UnRegisterExceptionHandler();
        #endif // defined(WIN32) && defined(_DEBUG) && CHECK_STACK_TRACES

        // Free member pointers
        if(m_dialogMain)
        {
            (void)m_dialogMain->Destroy();
            delete m_dialogMain;
        }
        if(m_GUISystem)
            delete m_GUISystem;
        if(m_GUIRenderer)
            delete m_GUIRenderer;
        if(m_renderCanvas)
            delete m_renderCanvas;

        // Zero out the member pointers
        // NOTE: All of these will be freed internally by wxWidgets
        m_document = wx_static_cast(EditorDocument*, NULL);
        mEditMenu = wx_static_cast(wxMenu*, NULL);
    }
    // Add exception to log
    LOG_EXCEPTION_SAFE("EditorFrame::~EditorFrame", "Failed to destroy editor frame", "Failed to destroy editor frame", "Failed to destroy editor frame");    
}

//------------------------------------------------------------------------
void EditorFrame::Setup()
{
    // Should be enough to fix this bug: http://www.cegui.org.uk/mantis/view.php?id=80
    wxWindowBase::SetSizeHints(320, 200);

    // Add a menu bar, a toolbar and a statusbar
    AttachMenubar();
    AttachToolbar();
    (void)wxFrameBase::CreateStatusBar();

    // Don't do earlier, because we need the toolbar and statusbar in the calculations!
    RestoreIniSize();

    if (m_dialogMain == NULL)
    {
        // Create the main dialog
        // The dialog registers itself to document event so all important
        // changes will automatically be propagated to it.
        m_dialogMain = new DialogMain(this);
    }
}

//------------------------------------------------------------------------
void EditorFrame::AttachMenubar()
{
    // Make a menubar (note that the Edit menu is a member!)
    wxMenu* const fileMenu = new wxMenu;
    mEditMenu = new wxMenu;
    wxMenu* const alignSubmenu = new wxMenu;
    wxMenu* const viewMenu = new wxMenu;
    wxMenu* const helpMenu = new wxMenu;
    wxMenuBar* const menubar = new wxMenuBar;

    // File items

    // Many of these ID's (the ones which start with "wxID") are already mapped
    // internally, so you won't see them in the EVENT_TABLE.
    (void)fileMenu->Append(wxID_NEW, wxT("新建...\tCtrl+N"), wxT("Create a new document."));
    (void)fileMenu->Append(wxID_OPEN, wxT("打开...\tCtrl+O"), wxT("Open an existing document."));
    (void)fileMenu->Append(wxID_CLOSE, wxT("关闭"), wxT("Close the active document."));
    (void)fileMenu->Append(wxID_SAVE, wxT("保存\tCtrl+S"), wxT("Save the active document."));
    (void)fileMenu->Append(wxID_SAVEAS, wxT("另存为..."), wxT("Save the active document with a new name."));
    (void)fileMenu->AppendSeparator();
    (void)fileMenu->Append(wxID_EXIT, wxT("退出\tCtrl+Q"), wxT("Quit the application; prompts to save document."));

    // Edit items
    (void)mEditMenu->Append(wxID_UNDO, wxT("撤销\tCtrl+Z"), wxT("Undo the last action."));
    (void)mEditMenu->Append(wxID_REDO, wxT("重做\tCtrl+Y"), wxT("Redo the previously undone action."));
    (void)mEditMenu->AppendSeparator();
    (void)mEditMenu->Append(ID_EDIT_CUT, wxT("剪切\tCtrl+X"), wxT("Cut the current selection."));
    (void)mEditMenu->Append(ID_EDIT_COPY, wxT("复制\tCtrl+C"), wxT("Copy the current selection."));
    (void)mEditMenu->Append(ID_EDIT_PASTE, wxT("粘贴\tCtrl+V"), wxT("Paste the latest cut or copied selection."));
    (void)mEditMenu->Append(ID_EDIT_CLEAR, wxT("删除\tDel"), wxT("Delete the current selection."));
    (void)mEditMenu->AppendSeparator();
    (void)mEditMenu->Append(wxID_SELECTALL, wxT("全选\tCtrl+A"), wxT("Select all windows in the layout."));

    (void)mEditMenu->AppendSeparator();

    // Align items
    // NOTE: No need to free the custodial pointer alignSubmenu since
    // it will be handled internally by wxWidgets wxMenuBase::Append()
    (void)alignSubmenu->Append(ID_ALIGN_LEFT, wxT("左"), wxT("Give entire selection the same X1 value."));
    (void)alignSubmenu->Append(ID_ALIGN_RIGHT, wxT("右"), wxT("Give entire selection the same X2 value."));
    (void)alignSubmenu->Append(ID_ALIGN_TOP, wxT("上"), wxT("Give entire selection the same Y1 value."));
    (void)alignSubmenu->Append(ID_ALIGN_BOTTOM, wxT("下"), wxT("Give entire selection the same Y2 value."));
    (void)alignSubmenu->AppendSeparator();
    (void)alignSubmenu->Append(ID_ALIGN_WIDTH, wxT("宽"), wxT("Give entire selection the same width."));
    (void)alignSubmenu->Append(ID_ALIGN_HEIGHT, wxT("高"), wxT("Give entire selection the same height."));
    (void)mEditMenu->Append(ID_ALIGN, wxT("对齐"), alignSubmenu);

    (void)mEditMenu->Append(ID_SKINS, wxT("应用皮肤..."), wxT("Applies a skin to the entire layout."));

    // View items
    (void)viewMenu->Append(ID_VIEW_640x480, wxT("匹配到 640 x 480"), wxT("Convenience resizer for 640 x 480."));
    (void)viewMenu->Append(ID_VIEW_800x600, wxT("匹配到 800 x 600"), wxT("Convenience resizer for 800 x 600."));
    (void)viewMenu->Append(ID_VIEW_1024x786, wxT("匹配到 1024 x 768"), wxT("Convenience resizer for 1024 x 768."));
    (void)viewMenu->Append(ID_VIEW_FULLSCREEN, wxT("全屏幕"), wxT("Convenience resizer to fullscreen."));
    (void)viewMenu->AppendSeparator();
    (void)viewMenu->Append(ID_VIEW_SET_BACKGROUND, wxT("设置背景参考图片..."));
    (void)viewMenu->AppendCheckItem(ID_VIEW_SHOW_BACKGROUND, wxT("显示背景参考图片..."));
    (void)viewMenu->AppendSeparator();
    (void)viewMenu->Append(ID_VIEW_SET_GRID, wxT("网格设置...\tCtrl+G"), wxT("Set grid size and visibility."));
    (void)viewMenu->Append(ID_VIEW_SET_FONT, wxT("设置默认字体...\tCtrl+F"), wxT("Set default font from now on."));

    // Help items
    (void)helpMenu->Append(wxID_ABOUT, wxT("关于"), wxT("Display program information, version number and copyright."));

    // Fill the menubar
    // NOTE: No need to free the custodial pointers fileMenu, viewMenu 
    // and helpMenu since they will be handled internally by wxWidgets wxMenuBar::Append()
    (void)menubar->Append(fileMenu, wxT("文件"));
    (void)menubar->Append(mEditMenu, wxT("编辑"));
    (void)menubar->Append(viewMenu, wxT("查看"));
    (void)menubar->Append(helpMenu, wxT("帮助"));

    // Associate the menu bar with the frame
    // NOTE: No need to free the custodial pointer menubar since
    // it will be handled internally by wxWidgets wxFrameBase::SetMenuBar()
    wxFrameBase::SetMenuBar(menubar);
}

//------------------------------------------------------------------------
void EditorFrame::AttachToolbar()
{
    // Create toolbar with icons-only
    wxToolBar* const toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER);
    //
    (void)toolBar->AddTool(wxID_NEW, wxBitmap (new_xpm), wxT("新建")); 
    (void)toolBar->AddTool(wxID_OPEN, wxBitmap (open_xpm), wxT("打开"));
    (void)toolBar->AddTool(wxID_SAVE, wxBitmap (save_xpm), wxT("保存"));
    (void)toolBar->AddSeparator();
    (void)toolBar->AddTool(ID_EDIT_CUT, wxBitmap (cut_xpm), wxT("剪切"));
    (void)toolBar->AddTool(ID_EDIT_COPY, wxBitmap (copy_xpm), wxT("复制"));
    (void)toolBar->AddTool(ID_EDIT_PASTE, wxBitmap (paste_xpm), wxT("粘贴"));
    (void)toolBar->AddTool(ID_EDIT_CLEAR, wxBitmap (delete_xpm), wxT("删除"));
    (void)toolBar->AddSeparator();

    (void)toolBar->AddTool(ID_LOCK_SELECTION, wxBitmap (lock_xpm), wxT("锁定选择"));
    (void)toolBar->AddSeparator();

    (void)toolBar->AddTool(wxID_ABOUT, wxBitmap (help_xpm), wxT("关于"));
    // Don't forget this one!
    (void)toolBar->Realize();

    // Assign toolbar
    // NOTE: No need to free the custodial pointer toolbar since
    // it will be handled internally by wxWidgets wxFrameBase::SetToolBar()
    wxFrameBase::SetToolBar(toolBar);    
}

//------------------------------------------------------------------------
void EditorFrame::SetDocument(EditorDocument* document)
{
    m_document = document;

    // Always set the (new) document
    if (m_dialogMain)
    {
        m_dialogMain->SetDocument (document) ;
    }

    // Tell the document's command processor where it can find the Undo and Redo items
    if (m_document)
    {
        m_document->GetCommandProcessor()->SetEditMenu(mEditMenu);
    }
}

//------------------------------------------------------------------------
EditorCanvas *EditorFrame::AttachCanvas(wxView *view)
{
    int width, height;
    GetClientSize(&width, &height);

    // Create the GLCanvas. This approach provides MDI support maybe in the future
    //EditorCanvas* const canvas = new EditorCanvas(view, this, GetClientAreaOrigin(), wxDefaultSize);
    EditorCanvas* const canvas = new EditorCanvas(view, this, wxPoint(0,0), wxDefaultSize);

    // Tell it about the initial size
    //canvas->changeSize(width + BORDER_SIZE, height + BORDER_SIZE);
    canvas->changeSize(width, height);
    canvas->ClearBackground();

    // Create the CEGUI renderable
    InitializeCEGUI();

    // Copy into member
    m_renderCanvas = canvas;

    return canvas;
}

//------------------------------------------------------------------------
void EditorFrame::InitializeCEGUI()
{
    // Setup support for TGA,JPG and PNG
    CEGUI::OpenGLRenderer::setDefaultImageCodecName("SILLYImageCodec");
    //CEGUI::OpenGLRenderer::setDefaultImageCodecName("TGAImageCodec");

    m_GUIRenderer = new CEGUI::OpenGLRenderer (0);
    m_GUISystem = new CEGUI::System(m_GUIRenderer);

    //CEGUI::Logger::getSingleton().setLoggingLevel (CEGUI::Informative);

    CEGUI::Font::setDefaultResourceGroup("configs");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");

    CEGUI::DefaultResourceProvider* const rp = 
        wx_static_cast(CEGUI::DefaultResourceProvider*, CEGUI::System::getSingleton().getResourceProvider());
    // Get the configuration object 
    Options* const opt =  Options::GetInstancePtr();

    // Check if the INI file has proper settings
    COMPILE_TODO("When this all works fine, put it in a separate method.")
    if (opt->GetPathToLookNFeels().empty())	// Just pick one folder to check this.
    {
        const wxString defaultPath = wxT("/");
        // Don't allow creating of a new folder.
        wxDirDialog dialog(this, wxT("请选择 \"Datafiles\" 目录."), defaultPath, (wxDEFAULT_DIALOG_STYLE & ~wxDD_NEW_DIR_BUTTON));
        if (dialog.ShowModal() == wxID_OK)
        {
            const wxString path = dialog.GetPath();
            CEGUI::Logger::getSingletonPtr()->logEvent(StringHelper::ToCEGUIString(wxT("Set \"Datafiles\" folder to \"") + path + wxT("\".")));
            opt->SetPathToConfigs(path + wxT("/configs/"));
            opt->SetPathToFonts(path + wxT("/fonts/"));
            opt->SetPathToImagesets(path + wxT("/imagesets/"));
            opt->SetPathToLookNFeels(path + wxT("/looknfeel/"));
            opt->SetPathToScripts(path + wxT("/lua_scripts/"));
            opt->SetPathToSchemes(path + wxT("/schemes/"));
        }
    }		

    // Paths set (also used in case user canceled out the dialog).
    if (opt->GetPathToLookNFeels().empty())	// Just pick one folder to check this.
    {
        LogError (wxT("EditorFrame::InitializeCEGUI - Need to quit: you either canceled the Datafiles browser, or have an incorrect INI file.\nCheck your CELayoutEditor.ini file for the 'XXXPaths=' fields."));
        (void)Close(true);    // No "veto"
    }
    // Calling these methods means that you can load resources without specifying the full path to them. CEGUI
    // will look for the resources in the given paths. Note that we don't use it for layouts, so they
    // can be saved to and loaded from everywhere.
    rp->setResourceGroupDirectory("schemes", wx_static_cast(const char*, wxConvLibc.cWX2MB(opt->GetPathToSchemes())));
    rp->setResourceGroupDirectory("imagesets", wx_static_cast(const char*, wxConvLibc.cWX2MB(opt->GetPathToImagesets())));
    rp->setResourceGroupDirectory("fonts", wx_static_cast(const char*, wxConvLibc.cWX2MB(opt->GetPathToFonts())));
    rp->setResourceGroupDirectory("looknfeel", wx_static_cast(const char*, wxConvLibc.cWX2MB(opt->GetPathToLookNFeels())));
    rp->setResourceGroupDirectory("lua_scripts", wx_static_cast(const char*, wxConvLibc.cWX2MB(opt->GetPathToScripts())));

    // Loads schemes and fonts (no other resources yet).
    LoadData();
}

//------------------------------------------------------------------------
void EditorFrame::LoadData() const
{
    wxArrayString	filenames;
    size_t			index;
    Options* const opt =  Options::GetInstancePtr();

    // Find all ".font" files.
    (void)wxDir::GetAllFiles(opt->GetPathToFonts(), &filenames, wxT("*.font"), wxDIR_FILES);
    if (filenames.GetCount() == 0)
    {
        CEGUI::Logger::getSingletonPtr()->logEvent("Warning: no fonts were found. Loading of layouts will most likely fail now.");
    }
    for (index=0; index<filenames.GetCount(); index++)
    {
        try
        {
            // Dalfy: i added the resource folder to avoid defaulting.
            (void)CEGUI::FontManager::getSingleton().createFont(StringHelper::ToCEGUIString(wxFileName(filenames[index]).GetFullName()));
        }
        // If something goes wrong, show user
        SHOW_EXCEPTION("EditorFrame::LoadData", "Error", "Unexpected standard exception while loading data", "Unexpected non-standard exception while loading data");
    }

    // Find all ".scheme" files.
    filenames.Clear();
    (void)wxDir::GetAllFiles(opt->GetPathToSchemes(), &filenames, wxT("*.scheme"), wxDIR_FILES);
    if (filenames.GetCount() == 0)
    {
        CEGUI::Logger::getSingletonPtr()->logEvent("Warning: no schemes were found. Loading of layouts will most likely fail now.");
    }
    for (index=0; index<filenames.GetCount(); index++)
    {
        try
        {
            // Dalfy: i added the resource folder to avoid defaulting.
            (void)CEGUI::SchemeManager::getSingleton().loadScheme (StringHelper::ToCEGUIString(wxFileName(filenames[index]).GetFullName()), "schemes");
        }
        catch (const CEGUI::AlreadyExistsException& guiException)
        {
            // This exception always seems to occur at the moment (for every datafile structure processing), 
            // so we're explicitly not showing it to the user (as it gets very annoying). However, we still want
            // to log the exception info composed of log basic info (description, location, message), line and 
            // file info (CEGUI exception specific) and VEH info if under Windows (stack trace, exception code & flags)
            LogDebug(ExceptionManager::GetInstancePtr()->GetExceptionInfo(wxT("EditorFrame::LoadData"), wxT("Unexpected Error"), StringHelper::ToWXString(guiException.getMessage()), StringHelper::ToWXString(guiException.getFileName()), guiException.getLine()));
        }
        // If something goes wrong, show user
        SHOW_EXCEPTION("EditorFrame::LoadData", "Error", "Unexpected standard exception while loading data", "Unexpected non-standard exception while loading data");
    }
}

//------------------------------------------------------------------------
void EditorFrame::OnResize(wxSizeEvent& WXUNUSED(event)) 
{
    // We need the client size
    int clientWidth, clientHeight;
    GetClientSize(&clientWidth, &clientHeight);

    if (m_document)
    {   // Recalculate the resize boxes
        Selection* const selection = m_document->GetSelection();
        for (Selection::Boxes::iterator boxIt = selection->GetBoxes().begin(); boxIt != selection->GetBoxes().end(); ++boxIt)
        {        
            boxIt->Reset();
        }
    }

    if (m_renderCanvas)
    {
        // Apply settings to our GLCanvase
        m_renderCanvas->changeSize(clientWidth, clientHeight);
    }

    if (m_GUIRenderer)
    {
        // Apply to renderer and store in INI
        m_GUIRenderer->setDisplaySize (CEGUI::Size(wx_static_cast(float, clientWidth) , wx_static_cast(float, clientHeight)));
        Options::GetInstancePtr()->SetCurrentResolution(clientWidth, clientHeight);
    }
}

//------------------------------------------------------------------------
void EditorFrame::RestoreIniSize() 
{
    // Load the previously stored resolution
    int lastWidth;
    int lastHeight;
    Options::GetInstancePtr()->GetCurrentResolution(&lastWidth, &lastHeight);

    // The OnResize handler will apply the setting to the renderer and INI file
    SetClientSize(lastWidth, lastHeight);
}

//------------------------------------------------------------------------
void EditorFrame::Refresh(bool eraseBackground, const wxRect* rect)
{
    if (m_document)
    {   // Recalculate the resize boxes
        Selection* const selection = m_document->GetSelection();
        for (Selection::Boxes::iterator boxIt = selection->GetBoxes().begin(); boxIt != selection->GetBoxes().end(); ++boxIt)
        {        
            boxIt->Reset();
        }
    }
    if (m_renderCanvas)
    {
        m_renderCanvas->Refresh(eraseBackground, rect);
        m_renderCanvas->Update();
    }
    // Call superclass
    wxFrame::Refresh(eraseBackground, rect);
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdate640x480(wxUpdateUIEvent& event)
{
    event.Enable(true);
}

//------------------------------------------------------------------------
void EditorFrame::On640x480(wxCommandEvent& WXUNUSED(event)) 
{
    // The OnResize handler will apply the setting to the renderer and INI file
    SetClientSize(640, 480);
    EditorFrame::Refresh();
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdate800x600(wxUpdateUIEvent& event) 
{
    event.Enable(true);
}

//------------------------------------------------------------------------
void EditorFrame::On800x600(wxCommandEvent& WXUNUSED(event)) 
{
    // The OnResize handler will apply the setting to the renderer and INI file
    SetClientSize(800, 600);
    EditorFrame::Refresh();
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdate1024x768(wxUpdateUIEvent& event) 
{
    event.Enable(true);
}

//------------------------------------------------------------------------
void EditorFrame::On1024x768(wxCommandEvent& WXUNUSED(event))
{
    // The OnResize handler will apply the setting to the renderer and INI file
    SetClientSize(1024, 786);
    EditorFrame::Refresh();
}

//------------------------------------------------------------------------
void EditorFrame::OnFullScreen(wxCommandEvent& WXUNUSED(event))
{
    (void)wxFrame::ShowFullScreen(true);
    EditorFrame::Refresh();
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateEditClear(wxUpdateUIEvent& event) 
{	
    event.Enable (m_document && m_document->GetSelection()->Size() > 0);
}

//------------------------------------------------------------------------
void EditorFrame::OnEditClear(wxCommandEvent& WXUNUSED(event)) 
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    m_document->DeleteSelection() ;
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateEditCopy(wxUpdateUIEvent& event) 
{
    event.Enable (m_document && m_document->GetSelection()->Size() > 0);
}

//------------------------------------------------------------------------
void EditorFrame::OnEditCopy(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    m_document->CopySelection();
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateEditCut(wxUpdateUIEvent& event) 
{
    event.Enable (m_document && m_document->GetSelection()->Size() > 0);
}

//------------------------------------------------------------------------
void EditorFrame::OnEditCut(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    m_document->CutSelection();
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateEditPaste(wxUpdateUIEvent& event) 
{
    event.Enable (m_document && m_document->GetCopiedSelection()->Size() > 0);
}

//------------------------------------------------------------------------
void EditorFrame::OnEditPaste(wxCommandEvent& WXUNUSED(event)) 
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    m_document->PasteCopiedSelection();
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateSetBackground(wxUpdateUIEvent& event) 
{
    // Must have a document
    event.Enable (m_document != NULL && m_document->IsBackgroundAvailable()) ;
}

//------------------------------------------------------------------------
void EditorFrame::OnSetBackground(wxCommandEvent& WXUNUSED(event)) 
{
    // Validations
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);

    // Restore directory before returning otherwise all file-access
    // will happen in this folder, including saving of layouts and such.
    const wxString currentDirectory = wxGetCwd();
    // Make sure to only include extensions, which are supported by "CEGUI"!
    wxFileDialog dialog(this, wxT("Open a background image"), currentDirectory, wxT(""), wxT("Layout files (*.tga;*.jpg;*.png)|*.tga;*.jpg;*.png"), wxOPEN);
    if (dialog.ShowModal() == wxID_OK)
    {
        m_document->SetBackgroundImage(dialog.GetPath());
    }
    (void)wxSetWorkingDirectory(currentDirectory);
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateShowBackground(wxUpdateUIEvent& event) 
{
    // You don't need to have an open document in order to show or hide backgrounds
    //event.Enable (m_renderCanvas && m_renderCanvas->m_background.isVisible()) ;
    event.Check (m_renderCanvas && Options::GetInstancePtr()->IsBackgroundVisible());
    event.Enable (Options::GetInstancePtr()->GetCurrentBackground() != wxT("")) ;
}

//------------------------------------------------------------------------
void EditorFrame::OnShowBackground(wxCommandEvent& event) 
{
    // Validations
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);

    m_document->SetBackgroundVisibility(event.IsChecked());
}

//------------------------------------------------------------------------
void EditorFrame::OnSetGrid(wxCommandEvent& WXUNUSED(event))
{
    // Prompts the user with the grid dialog
    // We don't need the return value, since the dialog updates the Options itself
    (void)(new DialogGrid (this))->ShowModal();
    EditorFrame::Refresh();
}

//------------------------------------------------------------------------
void EditorFrame::OnSetFont(wxCommandEvent& WXUNUSED(event))
{
    // Prompts the user with the available fonts to choose one from.
    wxArrayString	fonts;	// Duplicate code from DialogMain, needs refactoring(?)

    CEGUI::FontManager::FontIterator fontIt = CEGUI::FontManager::getSingletonPtr()->getIterator();
    while (!fontIt.isAtEnd())
    {
        (void)fonts.Add (StringHelper::ToWXString(fontIt.getCurrentKey()));
        ++fontIt;
    }

    // Found any?
    if (fonts.GetCount() > 0)
    {
        wxSingleChoiceDialog fontChooser (this, wxT("Apply this font to new windows:"), wxT("Set default font"), fonts);
        if (fontChooser.ShowModal() == wxID_OK)
        {
            // Overwrite previous value
            Options::GetInstancePtr()->SetDefaultFont(fontChooser.GetStringSelection());
        }
    }
    else
    {
        LogWarning (wxT("No fonts available to choose from."));
    }
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateAlign(wxUpdateUIEvent& event) 
{
    event.Enable (m_document && m_document->GetSelection()->Size() > 0);
}

//------------------------------------------------------------------------
void EditorFrame::OnAlignLeft(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    SelectionMover(m_document->GetSelection()).AlignSelection(SelectionMover::AlignLeft);
}

//------------------------------------------------------------------------
void EditorFrame::OnAlignRight(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    SelectionMover(m_document->GetSelection()).AlignSelection(SelectionMover::AlignRight);
}

//------------------------------------------------------------------------
void EditorFrame::OnAlignTop(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    SelectionMover(m_document->GetSelection()).AlignSelection(SelectionMover::AlignTop);
}

//------------------------------------------------------------------------
void EditorFrame::OnAlignBottom(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    SelectionMover(m_document->GetSelection()).AlignSelection(SelectionMover::AlignBottom);	
}

//------------------------------------------------------------------------
void EditorFrame::OnAlignWidth(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    SelectionMover(m_document->GetSelection()).AlignSelection(SelectionMover::AlignWidth);
}

//------------------------------------------------------------------------
void EditorFrame::OnAlignHeight(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    SelectionMover(m_document->GetSelection()).AlignSelection(SelectionMover::AlignHeight);
}

//------------------------------------------------------------------------
void EditorFrame::OnSelectAll(wxCommandEvent& WXUNUSED(event))
{
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);
    m_document->SelectAll();

} 

//------------------------------------------------------------------------
void EditorFrame::OnUpdateSelectAll(wxUpdateUIEvent& event) 
{
    COMPILE_FIXME("The EditorDocument::SelectAll() function is faulty and needs to be corrected. For the moment, the 'Select All' option is always disabled.")
    event.Enable(false);
    //event.Enable (m_document != NULL);
}

//------------------------------------------------------------------------
void EditorFrame::OnLayoutSkin(wxCommandEvent& WXUNUSED(event))
{
    // Validations
    wxASSERT_MSG(m_dialogMain != NULL, wxT("No layout is currently loaded!"));

	// Create a dialog that lists the registered Look'N'Feels and that asks the user to choose
    // one of the options. 
    const wxString skin = wxGetSingleChoice(StringHelper::ToWXString("请选择需要应用到整个窗口的皮肤:")
		, StringHelper::ToWXString("皮肤选择")
		, m_dialogMain->GetAvailableLooks());

    // Apply chosen skin to ALL the widgets in the current layout
    if(skin && skin.Length() > 0)
        m_dialogMain->SetWidgetsLook(StringHelper::ToCEGUIString(skin));
} 

//------------------------------------------------------------------------
void EditorFrame::OnUpdateLayoutSkin(wxUpdateUIEvent& event) 
{
    // Only enable the update layout skin when a document is open
    event.Enable (m_document != NULL);
}

//------------------------------------------------------------------------
void EditorFrame::OnUpdateLockSelection(wxUpdateUIEvent& event) 
{
    // Only enable the lock/unlock selection icon when the user has a valid widget selection
    event.Enable (m_document && m_document->GetSelection()->Size() > 0);
}

//------------------------------------------------------------------------
void EditorFrame::OnLockSelection(wxCommandEvent& WXUNUSED(event))
{
    // Validations
    wxASSERT_MSG(m_document != NULL, ASSERT_DOCUMENT_MSG);

    // Change the current lock selection status and update the lock/unlock 
    // toolbar icon to reflect this
    m_document->SwitchLockedSelection();
    UpdateLockedSelectionIcon(m_document);
}

//------------------------------------------------------------------------
void EditorFrame::UpdateLockedSelectionIcon(const EditorDocument* const document) const
{
    // Do we have a valid document to analyze?
    if(!document)
        return;

    // Obtain toolbar
    wxToolBar* const toolBar = this->GetToolBar();

    // Is the current user selection locked?
    if(document->GetLockedSelection())
    {
        // Selection locked, so change to locked icon
        toolBar->SetToolNormalBitmap(ID_LOCK_SELECTION, wxBitmap (lock_xpm));
        toolBar->SetToolShortHelp(ID_LOCK_SELECTION, wxT("Unlock Selection"));
    }
    else
    {
        // Selection unlocked, so change to unlocked icon
        toolBar->SetToolNormalBitmap(ID_LOCK_SELECTION, wxBitmap (unlock_xpm));
        toolBar->SetToolShortHelp(ID_LOCK_SELECTION, wxT("Lock Selection"));
    }
}
