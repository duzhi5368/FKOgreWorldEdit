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
#include "Selection.h"

//////////////////////////////////////////////////////////////////////
// SELECTION PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
size_t Selection::Size() const
{
    return m_boxes.size() ;
}

//------------------------------------------------------------------------
bool Selection::Select (CEGUI::Window* aWindow) 
{
    // Toggle when already selected
    if (IsSelected (aWindow))
    {
        Unselect(aWindow);
    }
    else if (!IsParentOrChild(aWindow))
    {
        // Box the window and add it
        m_boxes.push_back (WindowBox(aWindow));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------
void Selection::Unselect(const CEGUI::Window* const aWindow)
{
    // Find its box and remove it
    for (Boxes::iterator boxIt = m_boxes.begin(); boxIt != m_boxes.end(); ++boxIt)
    {
        if (aWindow == boxIt->GetWindow())
        {
            // Remove selected element
            (void)m_boxes.erase(boxIt) ;
            return;
        }
    }
}

//------------------------------------------------------------------------
bool Selection::IsSelected(const CEGUI::Window* const aWindow) 
{
    // Find its box
    for (Boxes::iterator boxIt = m_boxes.begin(); boxIt != m_boxes.end(); ++boxIt)
    {
        if (aWindow == boxIt->GetWindow())
        {			
            return true;
        }
    }

    // Isn't selected
    return false;
}

//------------------------------------------------------------------------
void Selection::ClearSelection() 
{
    // Remove all boxes
    m_boxes.clear();
}

//------------------------------------------------------------------------
WindowBox* Selection::FindBoxFor(const CEGUI::Window* const aWindow) 
{
    for (Boxes::iterator boxIt = m_boxes.begin(); boxIt != m_boxes.end(); ++boxIt)
    {
        if (boxIt->GetWindow() == aWindow)
        {
            return &*boxIt;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
// SELECTION PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
bool Selection::IsParentOrChild(const CEGUI::Window* const aWindow) 
{
    Boxes::iterator boxIt;

    // Is the given element the parent of one of the currently selected elements?
    for (boxIt = m_boxes.begin(); boxIt != m_boxes.end(); ++boxIt)
    {
        if (boxIt->GetWindow()->getParent() == aWindow)
        {
            // Bummer... cannot select then
            return true;
        }
    }

    // Is the given element the child of one of the currently selected elements? We only have to look one level deep (no recursion required)
    for (boxIt = m_boxes.begin(); boxIt != m_boxes.end(); ++boxIt)
    {
        if (boxIt->GetWindow()->isChild(aWindow))
        {
            // Bummer... cannot select then
            return true;
        }
    }

    return false;
}
