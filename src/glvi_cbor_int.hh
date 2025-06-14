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
#include "glvi_cbor_uint.hh"
#include "glvi_cbor_nint.hh"
#include <type_traits>
#include <variant>

/**
   Helper class that assists in handling C++ integral types.

   If constructed from a negative integer, internally constructs the
   associated CBORNint. If constructed from a positive integer,
   internally constructs the associated CBORUint.
 */
class CBORInt {
  using value_type = std::variant<CBORUint, CBORNint>;
  value_type value;

  template <std::signed_integral N>
  static constexpr auto make_nint(N const n) noexcept -> CBORNint {
    using unsigned_type = std::make_unsigned_t<N>;
    return CBORNint(CBOR_U64(static_cast<unsigned_type>(-1 - n)));
  }

  template <std::signed_integral N>
  static constexpr auto make_uint(N const n) noexcept -> CBORUint {
    using unsigned_type = std::make_unsigned_t<N>;
    return CBORUint(CBOR_U64(static_cast<unsigned_type>(n)));
  }

public:
  explicit CBORInt() = default;

  template <std::signed_integral N>
    requires requires { sizeof(N) <= sizeof(CBOR_U64::value_type); }
  constexpr explicit CBORInt(N const n) noexcept
      : value{n < 0 ? value_type(make_nint(n)) : value_type(make_uint(n))} {}

  explicit constexpr CBORInt(std::unsigned_integral auto n) noexcept
      : value{CBORUint(n)} {}

  friend constexpr std::strong_ordering operator<=>(CBORInt const &a,
                                                    CBORInt const &b) noexcept {
    if (std::holds_alternative<CBORNint>(a.value)) {   // a < 0
      if (std::holds_alternative<CBORNint>(b.value)) { // b < 0
        return std::get<CBORNint>(a.value) <=> std::get<CBORNint>(b.value);
      } else {
        return std::strong_ordering::less;
      }
    } else {
      if (std::holds_alternative<CBORNint>(b.value)) { // b < 0
        return std::strong_ordering::greater;
      } else {
        return std::get<CBORUint>(a.value) <=> std::get<CBORUint>(b.value);
      }
    }
  }

  friend constexpr auto operator<=>(CBORInt const &a,
                                    CBORUint const &b) noexcept {
    if (std::holds_alternative<CBORUint>(a.value)) {
      return std::get<CBORUint>(a.value) <=> b;
    } else {
      return std::strong_ordering::less;
    }
  }

  friend constexpr auto operator<=>(CBORUint const &a,
                                    CBORInt const &b) noexcept {
    if (std::holds_alternative<CBORUint>(b.value)) {
      return a <=> std::get<CBORUint>(b.value);
    } else {
      return std::strong_ordering::greater;
    }
  }

  friend constexpr bool operator==(CBORInt const &a,
                                   CBORUint const &b) noexcept {
    return a <=> b == 0;
  }

  friend constexpr bool operator==(CBORUint const &a,
                                   CBORInt const &b) noexcept {
    return a <=> b == 0;
  }

  friend constexpr auto operator<=>(CBORInt const &a,
                                    CBORNint const &b) noexcept {
    if (std::holds_alternative<CBORNint>(a.value)) {
      return std::get<CBORNint>(a.value) <=> b;
    } else {
      return std::strong_ordering::greater;
    }
  }

  friend constexpr auto operator<=>(CBORNint const &a,
                                    CBORInt const &b) noexcept {
    if (std::holds_alternative<CBORNint>(b.value)) {
      return a <=> std::get<CBORNint>(b.value);
    } else {
      return std::strong_ordering::less;
    }
  }

  friend constexpr bool operator==(CBORInt const &a,
                                   CBORNint const &b) noexcept {
    return a <=> b == 0;
  }

  friend constexpr bool operator==(CBORNint const &a,
                                   CBORInt const &b) noexcept {
    return a <=> b == 0;
  }

  constexpr void sassert();
};

constexpr void CBORInt::sassert() { static_assert(CBORInt() == CBORUint()); }
