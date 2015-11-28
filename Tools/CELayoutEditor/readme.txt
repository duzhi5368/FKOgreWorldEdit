CELayoutEditor 0.6.0
http://www.cegui.org.uk

Copyright 2004 - 2007 CELayoutEditor Team & Contributing Authors

Documentation:
----------------------

See also the following files:
AUTHORS.txt   - Contains a list of authors and contributors.
releasenotes.txt - Provides a low-level view (SVN update reports) of the relevant changes made to the project.
Changelog.txt - Contains a high-level list of changes made between releases.
ProjectRules.txt - Provides guidelines for developers.

For the very latest information and help, visit the project home page at http://www.cegui.org.uk.

Requirements:
---------------------

CELayoutEditor requires the following third party external libraries:

- Latest CEGUI SVN trunk (the editor doesn't compile with the last stable CEGUI release 0.5.0c as it requires more recent elements) --> http://www.cegui.org.uk.
- wxWidgets 2.8.* or higher (tested with 2.8.5, 2.8.6 and 2.8.7) --> www.wxwidgets.org.

Supported Systems:
------------------------------

CELayoutEditor is currently known to compile and run on the following systems:

Various flavors of GNU/Linux using gcc.
Microsoft Windows running VC++7.1 or VC++8

The project includes solutions for both VS2003 and VS2005. Check below
for detailed instructions on how to build them.

It is very possible that the system will compile and run on other systems and
compilers, though the above systems have been tested.

Visual Studio Build Instructions:
----------------------------------------------

Check 'CELayoutEditor Build Guide' (forum entry http://www.cegui.org.uk/phpBB2/viewtopic.php?t=2760) for detailed build instructions using VS2003 (VC++7.1) 
and VS2005 (VC++8). The guide is regularly updated to reflect all the latest changes made to the editor.

Visual Studio 6.0 and 2002 (VC++7.0) are no longer officially supported.

Please note that the solutions use the following environment variables:

$(CEGUI) - Path of the CEGUI directory (e.g. D:\Dev\CEGUI-0.5.0).
$(WXWIDGETS) - Path of the WxWidgets directory (e.g. D:\Dev\wxWidgets-2.8.7).
$(CE_LAYOUT_EDITOR) - Path of the CELayoutEditor (e.g. D:\Dev\CELayoutEditor).

NOTE: To create the above variables, follow these steps:

1) Go to My Computer\Properties\Advanced\Environment Variables. 

2) Press new in the user variables area.

3) Add the above variables.

Linux/Mac Build Instructions:
------------------------------------------

If you are running Linux or Max, check http://www.cegui.org.uk/wiki/index.php/Getting_Started.

NOTE: Some of the content on that Wiki page is now obsolete (due to recent changes to the editor) and you should check the visual Studio instructions page (http://www.cegui.org.uk/phpBB2/viewtopic.php?t=2760) 
and try to adapt it to your own needs. 

Usage:
----------

Check the 'CELayoutEditor Manual' wiki page (http://www.cegui.org.uk/wiki/index.php/CELayoutEditor_Manual) for usage instructions.

NOTE: The CELayoutEditor manual needs to be updated to reflect the latest changes. 

Frederico Jerónimo (fjeronimo)
Added : 07/12/07
Updated : 18/12/07