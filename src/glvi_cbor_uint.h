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
#pragma once
#include "glvi_cbor_u64.h"

/**
   CBOR major type 0: Unsigned integers in the range 0 .. (2^64-1).
 */
class CBORUint {
  CBOR_U64 value;

public:
  /**
     Constructs a CBOR unsigned integer initialised to 0.
   */
  explicit CBORUint() = default;

  /**
     Constructs a CBOR unsigned integer initialised to the specified
     value `v`.

     @param v unsigned integer
   */
  constexpr explicit CBORUint(CBOR_U64 &&v) : value{std::move(v)} {}

  /**
     Constructs a CBOR unsigned integer initialised to the specified
     value `v`.

     @param v unsigned integer
   */
  constexpr explicit CBORUint(CBOR_U64 const &v) : value{v} {}

  /**
     Extract the associated CBOR U64 value.
   */
  constexpr explicit operator CBOR_U64() const noexcept { return value; }

  /**
     Compares two CBOR unsigned integers.
   */
  friend constexpr std::strong_ordering operator<=>(CBORUint const &,
						    CBORUint const &) = default;
};
