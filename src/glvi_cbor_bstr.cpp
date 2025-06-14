//  -*- mode: c++; coding: utf-8-unix; -*-
//  cbor: Utilities for decoding Concise Binary Object Notation
//  Copyright (C) 2025 GLVI Gesellschaft für Luftverkehrsinformatik mbH.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or (at
//  your option) any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <https://www.gnu.org/licenses/>.
#include "glvi_cbor_bstr.h"

using Self = CBORBstr;

static_assert(std::is_nothrow_default_constructible_v<Self>);
static_assert(std::is_nothrow_move_constructible_v<Self>);
static_assert(std::is_copy_constructible_v<Self>);
static_assert(std::is_nothrow_move_assignable_v<Self>);
static_assert(std::is_copy_assignable_v<Self>);

static_assert(std::is_constructible_v<Self, Self::storage_type&&>);
static_assert(std::is_constructible_v<Self, Self::storage_type const&>);

static_assert(Self().size() == 0);

[[maybe_unused]]
char const *_glvi_cbor_bstr() {
  return "GLVI CBOR BSTR";
}
