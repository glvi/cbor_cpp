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
#include "glvi_cbor_u64.hh"
#include <memory>
#include <type_traits>

struct CBORValue;

/**
   CBOR major type 6: Tagged CBOR values.
 */
class CBORTag {
  using Self = CBORTag;

public:
  using value_type = CBORValue;

private:
  using storage_type = std::shared_ptr<value_type>;

public:
  explicit CBORTag(CBOR_U64 n, value_type&& v)
    : number(n)
    , storage(std::make_shared<value_type>(std::move(v)))
  {}

  explicit CBORTag(CBOR_U64 n, value_type const& v)
    : number(n)
    , storage(std::make_shared<value_type>(v))
  {}

  template<typename CBORMajorType>
  explicit CBORTag(CBOR_U64 n, CBORMajorType&& v)
    : number(n)
    , storage(std::make_shared<value_type>(std::move(v)))
  {}

  CBOR_U64 tag() noexcept { return number; }

  auto& value() noexcept { return *storage; }

  auto const& value() const noexcept { return *storage; }

  constexpr void sassert();

private:
  CBOR_U64 number;
  storage_type storage;
};

constexpr void CBORTag::sassert() {
  static_assert(not std::is_default_constructible_v<Self>);
  static_assert(std::is_nothrow_copy_constructible_v<Self>);
  static_assert(std::is_nothrow_move_constructible_v<Self>);
  static_assert(std::is_nothrow_copy_assignable_v<Self>);
  static_assert(std::is_nothrow_move_assignable_v<Self>);
}
