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
#include <initializer_list>
#include <vector>

/**
   CBOR major type 2: Byte strings (length < 2^64).
 */
class CBORTstr {
public:
  using value_type = std::byte;

  using storage_type = std::vector<value_type>;

private:
  storage_type storage;

public:
  using size_type = storage_type::size_type;

  /**
     Constructs an empty CBOR text string.
   */
  explicit CBORTstr() = default;

  /**
     Constructs a CBOR text string from a list of bytes.
   */
  explicit CBORTstr(std::initializer_list<std::byte> ilist) noexcept : storage(ilist) {}

  /**
     Move-constructs a CBOR text string from a vector of bytes.
   */
  explicit CBORTstr(storage_type &&other) : storage(std::move(other)) {}

  /**
     Copy-constructs a CBOR text string from a vector of bytes.
   */
  explicit CBORTstr(storage_type const &other) : storage(other) {}

  /**
     Returns the number of bytes in the text string.
   */
  constexpr auto size() noexcept { return storage.size(); }

  /**
     Returns the byte at index `i` from the byte string
   */
  auto at(size_type const i) { return storage.at(i); }

  /**
     Returns the byte at index `i`
   */
  auto operator[](size_type const i) const noexcept {
    return storage[i];
  }

  /**
     Returns a reference to the byte at index `i`
   */
  auto &operator[](size_type const i) noexcept { return storage[i]; }
};
