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
#include <cstdint>

/**
   CBOR major type 7: Simple CBOR values.

   Note: CBOR simple and CBOR float share the same major type.
 */
struct CBORSimple {
  constexpr CBORSimple(std::uint8_t n) : number{n} {}

private:
  std::uint8_t number;
};

constexpr CBORSimple const CBOR_False{0xf4};
constexpr CBORSimple const CBOR_True{0xf5};
constexpr CBORSimple const CBOR_Null{0xf6};
constexpr CBORSimple const CBOR_Undefined{0xf7};
