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
# GLVI_CXX_LIB_EXPECTED
# --------------------------------------
# Check whether C++ library feature '__cpp_lib_expected' (C++23) is available
AC_DEFUN([GLVI_CXX_LIB_EXPECTED],
[AC_CACHE_CHECK([for std::expected], [glvi_cv_cxx_lib_expected],
[glvi_cv_cxx_lib_expected=no
AC_LANG([C++])
AC_LANG_CONFTEST([AC_LANG_SOURCE([
#include <expected>
auto test_std_expected() -> std::expected<float,int> {
  return std::unexpected(0);
}])])
AC_COMPILE_IFELSE(, [glvi_cv_cxx_lib_expected=yes])])])
dnl (end of file)
