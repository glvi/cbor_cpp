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
#include <compare>
#include <concepts>
#include <cstdint>

/**
   Unsigned 64-bit quantity for use with CBOR data types.

   To avoid confusion, especially with the CBOR NINT type for negative
   integers, this API does not allow direct construction of CBOR data
   types from C++ integral types.
 */
class CBOR_U64 {
public:
  using value_type = std::uint64_t;

private:
  value_type value = 0;

public:
  /**
     Constructs a CBOR U64 from an unsigned integer.

     @tparam N some unsigned integer type
     @param n some integer in the interval `[0ull,0xffffffffffffffffull]`.
   */
  template <std::unsigned_integral N>
    requires requires { sizeof(N) <= sizeof(value_type); }
  constexpr explicit CBOR_U64(N const n) noexcept : value{n} {}

  /**
     Constructs a CBOR U64 initialised to 0.
   */
  explicit CBOR_U64() = default;

  /**
     Compares two CBOR U64 values.
   */
  friend std::strong_ordering operator<=>(CBOR_U64 const &,
                                          CBOR_U64 const &) = default;

  /**
     Converts a CBOR U64 to a value_type
   */
  constexpr explicit operator value_type() const noexcept { return value; }
};

/**
   CBOR U64 literal
 */
constexpr auto operator""_cbor(unsigned long long n) noexcept -> CBOR_U64 {
  return CBOR_U64(n);
}
