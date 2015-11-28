#                                              -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
m4_define([pkg_name], [CELayoutEditor])
m4_define([pkg_vers_major], [0])
m4_define([pkg_vers_minor], [6])
m4_define([pkg_vers_micro], [0])
m4_define([pkg_version], [pkg_vers_major.pkg_vers_minor.pkg_vers_micro])
m4_define([pkg_maintener], [Scriptkid <Scripkid mail>])
m4_define([pkg_full_name], [pkg_name-pkg_version])


AC_PREREQ(2.59)
AC_INIT([pkg_name], [pkg_version], [pkg_maintener], [pkg_full_name])
AM_INIT_AUTOMAKE([pkg_name], [pkg_version])
AC_CONFIG_SRCDIR([inc/CELayoutEditor.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_CPP

# Checks for libraries.
PKG_CHECK_MODULES(CEGUI, CEGUI-OPENGL >= 0.3.0)

# wxWidgets testing
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG([2.8.0], [wxWin=1], [wxWin=0], [gl,core,adv])
if test x$wxWin != x1; then
  AC_MSG_ERROR([
    wxWidgets must be installed on your system
    but wx-config script couldn't be found.
    
    Please check that wx-config is in path, the directory
    where wxWidgets libraries are installed (returned by
    wx-config - -libs' command) is in LD_LIBRARY_PATH or
    equivalent variable and wxWidgets version is 2.8.0 or above.
  ])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([select])

# Define default location to look for datafiles
CLEDITOR_CFLAGS='-DCLEDITOR_DATAPATH="\"$(datadir)/$(PACKAGE)/datafiles\""'

# substitutions
AC_SUBST(WX_CPPFLAGS)
AC_SUBST(WX_LIBS)
AC_SUBST(CLEDITOR_CFLAGS)

# Output makefiles
AC_CONFIG_FILES([Makefile
                 bin/Makefile
                 src/Makefile])
AC_OUTPUT
