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
#pragma once
#include "glvi_cbor_u64.h"
#include <utility>

/**
   CBOR major type 1: Negative integers in the range -2^64 .. -1.
 */
class CBORNint {
  CBOR_U64 value;

public:
  /**
     Constructs a CBOR negative integer initialised to 0 which
     represents the value `-1`.
   */
  explicit CBORNint() = default;

  /**
     Constructs a CBOR negative integer initialised to the specified
     value `v` which represents the value `-1-v`.

     @param v unsigned integer
   */
  constexpr explicit CBORNint(CBOR_U64 &&v) : value{std::move(v)} {}

  /**
     Constructs a CBOR negative integer initialised to the specified
     value `v` which represents the value `-1-v`.

     @param v unsigned integer
   */
  constexpr explicit CBORNint(CBOR_U64 const &v) : value{v} {}

  /**
     Extract the associated CBOR U64 value.
   */
  constexpr explicit operator CBOR_U64() const noexcept { return value; }

  /**
     Compares two CBOR negative integers.
   */
  friend constexpr std::strong_ordering operator<=>(CBORNint const &,
						    CBORNint const &) = default;
};
