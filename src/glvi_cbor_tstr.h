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
#include <string>
#include <utility>

/**
   CBOR major type 2: Byte strings (length < 2^64).
 */
class CBORTstr {
public:
  using storage_type = std::u8string;

private:
  storage_type storage;

public:
  using size_type = storage_type::size_type;

  /**
     Constructs an empty CBOR text string.
   */
  explicit CBORTstr() = default;

  /**
     Move-constructs a CBOR text string from a C++ UTF-8 string.
   */
  explicit CBORTstr(storage_type&& other) : storage(std::move(other)) {}

  /**
     Copy-constructs a CBOR text string from a C++ UTF-8 string.
   */
  explicit CBORTstr(storage_type const& other) : storage(other) {}

  /**
     Move-assigns a CBOR text string from a C++ UTF-8 string.
  */
  CBORTstr& operator=(storage_type&& other) {
    storage = std::move(other);
    return *this;
  }

  /**
     Copy-assigns a CBOR text string from a C++ UTF-8 string.
   */
  CBORTstr& operator=(storage_type const& other) {
    storage = other;
    return *this;
  }

  /**
     Returns the number of bytes in the text string.
   */
  constexpr auto size() const noexcept { return storage.size(); }

  /**
     Returns the byte at index `i` from the byte string
   */
  auto at(size_type const i) { return storage.at(i); }

  /**
     Returns the byte at index `i`
   */
  auto operator[](size_type const i) const noexcept { return storage[i]; }

  /**
     Returns a reference to the byte at index `i`
   */
  auto& operator[](size_type const i) noexcept { return storage[i]; }

  /**
     Compares two CBOR text strings
   */
  friend constexpr auto operator<=>(CBORTstr const& a,
                                    CBORTstr const& b) noexcept = default;

  /**
     Compares a CBOR text string with a C++ UTF-8 string
   */
  friend constexpr auto operator<=>(CBORTstr const& a,
                                    storage_type const& b) noexcept {
    return a.storage <=> b;
  }

  /**
     Compares a C++ UTF-8 string with a CBOR text string
   */
  friend constexpr auto operator<=>(storage_type const& a,
                                    CBORTstr const& b) noexcept {
    return a <=> b.storage;
  }

  friend constexpr bool operator==(CBORTstr const& a,
                                   std::u8string const& b) noexcept {
    return a <=> b == 0;
  }

  friend constexpr bool operator==(std::u8string const& a,
                                   CBORTstr const& b) noexcept {
    return a <=> b == 0;
  }
};

constexpr CBORTstr operator""_cbor_tstr(char8_t const *const s,
                                        unsigned long const n) {
  return CBORTstr(std::u8string(s, n));
}
