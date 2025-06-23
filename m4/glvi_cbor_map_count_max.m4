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
# GLVI_CBOR_ARG_ENABLE_MAP_COUNT_MAX
# --------------------------------------
# Protection feature for CBOR maps
AC_DEFUN([GLVI_CBOR_ARG_ENABLE_MAP_COUNT_MAX],
[AC_ARG_ENABLE([cbor-map-count-max],
              [AS_HELP_STRING([--enable-cbor-map-count-max@<:@=yes/no/number/unit@:>@],
	                      [limit the number of elements in a CBOR map
			      (default=512).
			      Append 'k', 'M', 'G', 'T', or 'P' to the number to specify
			      the maximum count in units of
			      (1,024),
			      (1,048,576),
			      (1,073,741,824),
			      (1,099,511,627,776), or
			      (1,125,899,906,842,624) respectively.
			      Set to '0' or 'no', or use '--disable-map-count-max',
			      to disable map protection])],
	      [AS_CASE([$enableval],
                       [yes],[enable_cbor_map_count_max=512],
	               [no],[enable_cbor_map_count_max=0])],
	      [enable_cbor_map_count_max=512])
# strip trailing stuff
[tmp=$(expr $enable_cbor_map_count_max : '^\([0-9]*[kKmMgGtTpP]\).*')]
AS_IF([test "x$tmp" != "x"],
      [enable_cbor_map_count_max="$tmp"])
AS_CASE([$enable_cbor_map_count_max],
        [*k|*K], [dnl
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max : "^\([0-9]*\)")]
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max '*' 1024)]],
        [*m|*M], [dnl
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max : "^\([0-9]*\)")]
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max '*' 1024 '*' 1024)]],
        [*g|*G], [dnl
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max : "^\([0-9]*\)")]
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max '*' 1024 '*' 1024 '*' 1024)]],
        [*t|*T], [dnl
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max : "^\([0-9]*\)")]
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max '*' 1024 '*' 1024 '*' 1024 '*' 1024)]],
        [*p|*P], [dnl
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max : "^\([0-9]*\)")]
	  [enable_cbor_map_count_max=$(expr $enable_cbor_map_count_max '*' 1024 '*' 1024 '*' 1024 '*' 1024 '*' 1024)]],
	  )
AC_DEFINE_UNQUOTED([GLVI_CBOR_MAP_COUNT_MAX],
                   [$enable_cbor_map_count_max],
                   [Maximum number of elements in CBOR maps])
])
