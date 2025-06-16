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
#include <cstddef>
#include <utility>
#include <vector>

struct CBORValue;

/**
   CBOR major type 5: Map of pairs of CBOR values.
 */
class CBORMap {
  using Self = CBORMap;

public:
  using storage_type = std::vector<CBORValue>;
  using value_type = storage_type::value_type;
  using size_type = storage_type::size_type;

  CBORMap() = default;
  CBORMap(CBORMap&&) = default;
  CBORMap(CBORMap const&) = default;
  CBORMap& operator=(CBORMap&&) = default;
  CBORMap& operator=(CBORMap const&) = default;

private:
  storage_type entries;
};
