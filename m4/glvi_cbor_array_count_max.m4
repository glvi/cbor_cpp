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
# GLVI_CBOR_ARG_ENABLE_ARRAY_COUNT_MAX
# --------------------------------------
# Protection feature for CBOR arrays
AC_DEFUN([GLVI_CBOR_ARG_ENABLE_ARRAY_COUNT_MAX],
[AC_ARG_ENABLE([cbor-array-count-max],
              [AS_HELP_STRING([--enable-cbor-array-count-max@<:@=yes/no/number/unit@:>@],
	                      [limit the number of elements in a CBOR array
			      (default=1024).
			      Append 'k', 'M', 'G', 'T', or 'P' to the number to specify
			      the maximum count in units of
			      (1,024),
			      (1,048,576),
			      (1,073,741,824),
			      (1,099,511,627,776), or
			      (1,125,899,906,842,624) respectively.
			      Set to '0' or 'no', or use '--disable-array-count-max',
			      to disable array protection])],
	      [AS_CASE([$enableval],
                       [yes],[enable_cbor_array_count_max=1024],
	               [no],[enable_cbor_array_count_max=0])],
	      [enable_cbor_array_count_max=1024])
# strip trailing stuff
[tmp=$(expr $enable_cbor_array_count_max : '^\([0-9]*[kKmMgGtTpP]\).*')]
AS_IF([test "x$tmp" != "x"],
      [enable_cbor_array_count_max="$tmp"])
AS_CASE([$enable_cbor_array_count_max],
        [*k|*K], [dnl
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max : "^\([0-9]*\)")]
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max '*' 1024)]],
        [*m|*M], [dnl
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max : "^\([0-9]*\)")]
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max '*' 1024 '*' 1024)]],
        [*g|*G], [dnl
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max : "^\([0-9]*\)")]
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max '*' 1024 '*' 1024 '*' 1024)]],
        [*t|*T], [dnl
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max : "^\([0-9]*\)")]
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max '*' 1024 '*' 1024 '*' 1024 '*' 1024)]],
        [*p|*P], [dnl
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max : "^\([0-9]*\)")]
	  [enable_cbor_array_count_max=$(expr $enable_cbor_array_count_max '*' 1024 '*' 1024 '*' 1024 '*' 1024 '*' 1024)]],
	  )
AC_DEFINE_UNQUOTED([GLVI_CBOR_ARRAY_COUNT_MAX],
                   [$enable_cbor_array_count_max],
                   [Maximum number of elements in CBOR arrays])
])
