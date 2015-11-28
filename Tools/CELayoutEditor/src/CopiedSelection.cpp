///////////////////////////////////////////////////////////////////////////////
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
#include "CopiedSelection.h"

//////////////////////////////////////////////////////////////////////////
// COPIEDSELECTION CONSTRUCTORS/DESTRUCTORS
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
CopiedSelection::CopiedSelection() :
m_pasteCounter(0),
m_applyPasteOffset(false),
m_deepCopies()
{
    // NOTE: Prefer constructor initialization lists (which are 
    // often more efficient) to assignment inside the 
    // constructor. Members in the initialization list should 
    // appear in the order they are declared in the class, as 
    // this is the order they will be initialized (no matter what).
    // See item 4 in 'Effective C++ 3rd Edition' by Scott Meyers
}

//------------------------------------------------------------------------
CopiedSelection::~CopiedSelection() 
{
    // Swallow any exceptions found to prevent the destructor from emitting them.
    // NOTE: An exception should never leave a destructor as it may yield
    // undefined behavior or even lead to program abnormal termination 
    // (see Item 8 in 'Effective C++ 3rd Edition' by Scott Meyers)
    try
    {
        Reset();
    }
    // Add exception to log
    LOG_EXCEPTION_SAFE("CopiedSelection::~CopiedSelection", "Failed to destroy copied selection", "Failed to destroy copied selection", "Failed to destroy copied selection");
}

//////////////////////////////////////////////////////////////////////////
// COPIEDSELECTION PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
void CopiedSelection::AddRecursiveCopy (const CEGUI::Window* const pWindow)
{
    DeepCopy* const newDeepCopy = new DeepCopy();
    AddRecursiveCopyInt(newDeepCopy, pWindow);

    // copy to its address
    // NOTE: No need to free the custodial pointer newDeepCopy since
    // it will be handled internally by std::vector::push_back()
    m_deepCopies.push_back(newDeepCopy);
}

//------------------------------------------------------------------------
void CopiedSelection::PasteSelection (CEGUI::Window* pTarget)
{
    m_pasteCounter++;   // For the unique names.
    m_applyPasteOffset = true;  // Will be toggled off after the first call.
    for (size_t i = 0; i < m_deepCopies.size(); ++i)
    {
        PasteWindowRecursiveInt( *m_deepCopies[i],pTarget );
    }
}

//------------------------------------------------------------------------
void CopiedSelection::Reset () 
{
    for(unsigned int i = 0; i < m_deepCopies.size(); ++i)
    {
        delete m_deepCopies[i];
    }
    // Clear selection
    m_deepCopies.clear () ;
    // Reset counter
    m_pasteCounter = 0;
}

//------------------------------------------------------------------------
size_t CopiedSelection::Size() const
{
    return m_deepCopies.size() ;
}

//////////////////////////////////////////////////////////////////////////
// COPIEDSELECTION PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
void CopiedSelection::AddCopyInt( DeepCopy* pData, const CEGUI::Window* const pWindow ) const
{
    // Validations
    wxASSERT_MSG(pWindow != NULL, wxT("Supplied window is NULL"));
    wxASSERT_MSG(pData != NULL, wxT("Supplied data is NULL"));

    pData->strName = pWindow->getName();
    pData->strType = pWindow->getType();

    // Write the properties
    // NOTE: getPropertyIterator() makes sure the compiler doesn't confuse EventSet 
    // and PropertySet (it would if we used getIterator()) thus avoiding useless casts
    CEGUI::PropertySet::Iterator propertyIt = pWindow->getPropertyIterator();

    while (!propertyIt.isAtEnd()) 
    {
        (void)pData->mapProperties.insert(std::make_pair (propertyIt.getCurrentKey(), pWindow->getProperty(propertyIt.getCurrentKey())));
        propertyIt++;
    }
}

//------------------------------------------------------------------------
void CopiedSelection::AddRecursiveCopyInt( DeepCopy* pData, const CEGUI::Window* const pWindow )
{
    // Validations
    wxASSERT_MSG(pWindow != NULL, wxT("Supplied window is NULL"));
    wxASSERT_MSG(pData != NULL, wxT("Supplied data is NULL"));

    AddCopyInt( pData, pWindow );
    pData->children.reserve(pWindow->getChildCount());
    for( unsigned int i = 0; i < pWindow->getChildCount(); ++i )
    {
        pData->children.push_back(new DeepCopy());
        AddRecursiveCopyInt( pData->children[i], pWindow->getChildAtIdx(i) );
    }
}

//------------------------------------------------------------------------
CEGUI::Window* CopiedSelection::PasteWindowInt( const DeepCopy& data, CEGUI::Window* pTarget )
{
    // Validations
    wxASSERT_MSG(pTarget != NULL, wxT("Supplied target is NULL"));

    std::map<CEGUI::String, CEGUI::String>::const_iterator it;
    CEGUI::Window *pNew = 0; 
    try
    {
        // Combine original name and counter into a unique name
        const CEGUI::String  strUniqueName = CreateUniqueName( data.strName );

        pNew = CEGUI::WindowManager::getSingleton().createWindow (data.strType, strUniqueName);
        pTarget->addChildWindow (pNew);
        for (it = data.mapProperties.begin();it != data.mapProperties.end();++it)
        {
            try
            {
                //LogDebug(wxT("pasteSelection - set '%s' to '%s'"), StringHelper::ToWXString(it->first), StringHelper::ToWXString(it->second));
                if (it->second != "")
                {   // This needs a fix because of this possible exception:
                    // CCopiedSelection::pasteSelection - Window::setLookNFeel - There is already a look'n'feel assigned to the window 'XXX'
                    pNew->setProperty (it->first, it->second);
                }
            }
            catch (const CEGUI::AlreadyExistsException& guiException) 
            {
                // No show-stopper, but do log it - log basic info (description, location, message), line and 
                // file info (CEGUI exception specific) and VEH info if under Windows (stack trace, exception code & flags)
                LogDebug(ExceptionManager::GetInstancePtr()->GetExceptionInfo(wxT("CopiedSelection::PasteWindowInt"), wxT("Unexpected Error - Property already exists."), StringHelper::ToWXString(guiException.getMessage()), StringHelper::ToWXString(guiException.getFileName()), guiException.getLine()));
            }
        }
        // As promised...
        if (m_applyPasteOffset)
        {   // Offset depends on paste counter. To get a cascading effect.
            CEGUI::UVector2 position = pNew->getPosition();  // Get rid of the const
            position += CEGUI::UVector2(CEGUI::UDim(0.05f * m_pasteCounter, 0.0f), CEGUI::UDim(0.05f * m_pasteCounter, 0.0f));
            pNew->setPosition(position);
            m_applyPasteOffset = false;
        }
    }
    // If something goes wrong, add to log
    LOG_EXCEPTION("CopiedSelection::PasteWindowInt", "Error", "Unexpected standard exception while pasting window", "Unexpected non-standard exception while pasting window");
    return pNew;
}

//------------------------------------------------------------------------
void CopiedSelection::PasteWindowRecursiveInt( const DeepCopy& data, CEGUI::Window* pTarget )
{
    CEGUI::Window* const pNew = PasteWindowInt( data, pTarget );

    for( unsigned int i = 0; i < data.children.size(); ++i )
    {
        PasteWindowRecursiveInt( *data.children[i], pNew );
    }
}

//------------------------------------------------------------------------
CEGUI::String CopiedSelection::CreateUniqueName( const CEGUI::String& oldName ) const
{
    int nCounter = m_pasteCounter;
    CEGUI::String res = oldName;

    while (CEGUI::WindowManager::getSingleton().isWindowPresent(res))
    {
        wxChar p_chNamePostfix[100];
        (void)wxSnprintf (p_chNamePostfix, 100, wxT("%d"), nCounter++);
        res = oldName + StringHelper::ToCEGUIString(p_chNamePostfix); 
    }
    return res;
}
