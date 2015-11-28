#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
m4_define([pkg_name], [CEImagesetEditor])
m4_define([pkg_vers_major], [0])
m4_define([pkg_vers_minor], [1])
m4_define([pkg_vers_micro], [0])
m4_define([pkg_version], [pkg_vers_major.pkg_vers_minor.pkg_vers_micro])
m4_define([pkg_maintener], [CEGUI Team <team@cegui.org.uk>])

AC_PREREQ(2.59)
AC_INIT([pkg_name], [pkg_version], [pkg_maintener], [pkg_full_name])
AM_INIT_AUTOMAKE([pkg_name], [pkg_version])
AC_CONFIG_SRCDIR([src/ElasticBox.cpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES(CEGUI, CEGUI-OPENGL >= 0.5.0)

# wxWidgets testing
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG([2.6.0], [wxWin=1], [wxWin=0], [gl,core,adv])
if test x$wxWin != x1; then
  AC_MSG_ERROR([
    wxWidgets must be installed on your system
    but wx-config script couldn't be found.

    Please check that wx-config is in path, the directory
    where wxWidgets libraries are installed (returned by
    wx-config - -libs' command) is in LD_LIBRARY_PATH or
    equivalent variable and wxWidgets version is 2.6.0 or above.
  ])
fi

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.


# substitutions
AC_SUBST(WX_CPPFLAGS)
AC_SUBST(WX_LIBS)
AC_SUBST([pkgdatadir], ["\${datadir}/$PACKAGE"])

# Output makefiles
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
