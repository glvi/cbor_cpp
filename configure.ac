dnl -*- mode: autoconf; coding: utf-8-unix; -*-
dnl
dnl cbor: Utilities for decoding Concise Binary Object Representation
dnl Copyright (C) 2025 GLVI Gesellschaft für Luftverkehrsinformatik mbH.
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or (at
dnl your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program. If not, see <https://www.gnu.org/licenses/>.

dnl ********************************************************************
dnl Autoconf requirements
AC_CONFIG_MACRO_DIRS([m4])
dnl ********************************************************************

dnl ********************************************************************
dnl information on the package
AC_INIT([glvi_cbor],
        [0.1],
	[info@glvi.de],
	[glvi-cbor],
	[https://github.com/glvi/cbor_cpp])
AC_ARG_VAR(AUTOGEN, [Automated program generator])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/glvi_cbor.cpp])
dnl ********************************************************************

dnl ********************************************************************
dnl checks for programs
AM_INIT_AUTOMAKE([-Wall -Werror dejagnu foreign])
AC_PROG_CXX
AM_PROG_AR
LT_INIT
AC_PATH_PROG(AUTOGEN, autogen)
dnl ********************************************************************

dnl ********************************************************************
dnl
dnl checks for libraries
dnl (empty)
dnl ********************************************************************

dnl ********************************************************************
dnl checks for header files
dnl
AC_CHECK_HEADER_STDBOOL
dnl ********************************************************************

dnl ********************************************************************
dnl Check for DejaGNU
dnl
AC_LANG([C++])
AC_CHECK_HEADERS(
  [dejagnu.h],,
  [AC_MSG_WARN([DejaGNU required for `make check'])])
dnl ********************************************************************

dnl ********************************************************************
dnl
dnl checks for types
dnl
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
dnl ********************************************************************

dnl ********************************************************************
dnl
dnl checks for structures
dnl
GLVI_CXX_LIB_RANGES
GLVI_CXX_LIB_EXPECTED
dnl ********************************************************************

dnl ********************************************************************
dnl checks for compiler characteristics
AC_C_INLINE
dnl ********************************************************************

dnl ********************************************************************
dnl checks for library functions
dnl (empty)
dnl ********************************************************************

dnl ********************************************************************
dnl checks for system services
dnl (empty)
dnl ********************************************************************

dnl ********************************************************************
dnl checks for other stuff
GLVI_CBOR_ARG_ENABLE_BSTR_COUNT_MAX
GLVI_CBOR_ARG_ENABLE_TSTR_COUNT_MAX
GLVI_CBOR_ARG_ENABLE_ARRAY_COUNT_MAX
GLVI_CBOR_ARG_ENABLE_MAP_COUNT_MAX
dnl ********************************************************************

dnl ********************************************************************
dnl Emit configuration
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
dnl ********************************************************************

dnl Final notes
test "x$glvi_cv_cxx_lib_ranges" = "xyes" || AC_MSG_WARN([Required C++ library feature '__cpp_lib_ranges' (C++20) missing.])
test "x$glvi_cv_cxx_lib_expected" = "xyes" || AC_MSG_WARN([Required C++ library feature '__cpp_lib_expected' (C++23) missing.])
dnl ********************************************************************
dnl (end of file)
