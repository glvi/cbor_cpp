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
#include <vector>

struct CBORValue;

/**
   CBOR major type 4: Array of CBOR values.
 */
class CBORArray {
  using Self = CBORArray;

public:
  using storage_type = std::vector<CBORValue>;
  using value_type = storage_type::value_type;
  using size_type = storage_type::size_type;

private:
  storage_type elements;

public:
  explicit CBORArray() noexcept = default;

  CBORArray(CBORArray&&) = default;
  CBORArray(CBORArray const&) = default;
  CBORArray& operator=(CBORArray&&) = default;
  CBORArray& operator=(CBORArray const&) = default;

  size_type size() noexcept;

  constexpr void sassert();
};

constexpr void CBORArray::sassert() {
  static_assert(std::is_nothrow_default_constructible_v<Self>);
  static_assert(std::is_copy_constructible_v<Self>);
  static_assert(std::is_nothrow_move_constructible_v<Self>);
  static_assert(not std::is_constructible_v<Self, unsigned>);
  static_assert(not std::is_constructible_v<Self, signed>);
}
