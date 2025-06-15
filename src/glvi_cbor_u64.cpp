//  -*- mode: c++; coding: utf-8-unix; -*-
//  cbor: Utilities for decoding Concise Binary Object Representation
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
#include "glvi_cbor_u64.h"

static_assert(sizeof(CBOR_U64) == 8);

static_assert(std::is_nothrow_default_constructible_v<CBOR_U64>);
static_assert(std::is_nothrow_move_constructible_v<CBOR_U64>);
static_assert(std::is_nothrow_copy_constructible_v<CBOR_U64>);
static_assert(std::is_nothrow_move_assignable_v<CBOR_U64>);
static_assert(std::is_nothrow_copy_assignable_v<CBOR_U64>);

static_assert(not std::constructible_from<CBOR_U64, signed char>);
static_assert(not std::constructible_from<CBOR_U64, signed short>);
static_assert(not std::constructible_from<CBOR_U64, signed int>);
static_assert(not std::constructible_from<CBOR_U64, signed long>);
static_assert(not std::constructible_from<CBOR_U64, signed long long>);

static_assert(static_cast<std::uint64_t>(CBOR_U64()) == 0);
static_assert(static_cast<std::uint64_t>(CBOR_U64(0u)) == 0);
static_assert(static_cast<std::uint64_t>(CBOR_U64(1u)) == 1);

static_assert(static_cast<std::uint64_t>(0_cbor) == 0);
static_assert(static_cast<std::uint64_t>(1_cbor) == 1);

static_assert(CBOR_U64() == 0_cbor);
static_assert(CBOR_U64(0u) == 0_cbor);
static_assert(CBOR_U64(1u) == 1_cbor);

static_assert(0_cbor <= 1_cbor);
static_assert(0_cbor < 1_cbor);
static_assert(1_cbor > 0_cbor);
static_assert(1_cbor >= 0_cbor);

[[maybe_unused]]
char const *_glvi_cbor_u64() {
  return "GLVI CBOR U64";
}
