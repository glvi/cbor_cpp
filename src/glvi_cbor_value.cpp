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
#include "glvi_cbor_value.hh"

static_assert(std::is_nothrow_default_constructible_v<CBORValue>);
static_assert(std::is_nothrow_move_constructible_v<CBORValue>);
static_assert(std::is_copy_constructible_v<CBORValue>);
static_assert(std::is_nothrow_move_assignable_v<CBORValue>);
static_assert(std::is_copy_assignable_v<CBORValue>);

static_assert(std::is_nothrow_constructible_v<CBORValue, CBORUint &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORNint &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORBstr &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORTstr &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORArray &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORMap &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORTag &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORSimple &&>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORFloat &&>);

static_assert(std::is_nothrow_constructible_v<CBORValue, CBORUint const &>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORNint const &>);
static_assert(std::is_constructible_v<CBORValue, CBORBstr const &>);
static_assert(std::is_constructible_v<CBORValue, CBORTstr const &>);
static_assert(std::is_constructible_v<CBORValue, CBORArray const &>);
static_assert(std::is_constructible_v<CBORValue, CBORMap const &>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORTag const &>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORSimple const &>);
static_assert(std::is_nothrow_constructible_v<CBORValue, CBORFloat const &>);

static_assert(CBORValue().is_simple());
static_assert(CBORValue(CBORUint()).is_uint());
static_assert(CBORValue(CBORNint()).is_nint());
static_assert(CBORValue(CBORBstr()).is_bstr());
static_assert(CBORValue(CBORTstr()).is_tstr());
static_assert(CBORValue(CBORArray()).is_array());
static_assert(CBORValue(CBORMap()).is_map());
//static_assert(CBORValue(CBORTag(0_cbor, CBORUint())).is_tag());
static_assert(CBORValue(CBORSimple(0)).is_simple());
static_assert(CBORValue(CBORFloat()).is_float());

[[maybe_unused]]
static void dummy() {
  CBORValue x = CBORUint(0_cbor);
  CBORValue y = CBORBstr { std::byte {0x01}, std::byte {0x02} };
  if (auto opt = y.as_bstr()) {
    (void) *opt;
  }
}
