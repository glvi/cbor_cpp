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
#include "glvi_cbor_tstr.hh"
#include <type_traits>

static_assert(std::is_nothrow_default_constructible_v<CBORTstr>);
static_assert(std::is_nothrow_move_constructible_v<CBORTstr>);
static_assert(std::is_copy_constructible_v<CBORTstr>);
static_assert(std::is_nothrow_move_assignable_v<CBORTstr>);
static_assert(std::is_copy_assignable_v<CBORTstr>);

static_assert(CBORTstr().size() == 0);
static_assert(CBORTstr{}.size() == 0);
