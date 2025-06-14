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
#include "glvi_cbor_array.h"
#include "glvi_cbor_bstr.h"
#include "glvi_cbor_float.h"
#include "glvi_cbor_map.h"
#include "glvi_cbor_nint.h"
#include "glvi_cbor_simple.h"
#include "glvi_cbor_tag.h"
#include "glvi_cbor_tstr.h"
#include "glvi_cbor_uint.h"
#include <functional>
#include <optional>
#include <variant>

/**
   CBOR value
 */
class CBORValue {
public:
  using storage_type =
      std::variant<CBORUint, CBORNint, CBORBstr, CBORTstr, CBORArray, CBORMap,
                   CBORTag, CBORSimple, CBORFloat>;

private:
  storage_type storage;

public:
  /**
     Default-constructs a CBOR value.

     A default-constructed CBOR value is `CBOR_undefined`, see `CBORSimple`.
   */
  constexpr explicit CBORValue() noexcept : storage(CBOR_Undefined) {}

  /**
     Move-constructs a CBOR value from a value of one of the CBOR major types.
     @tparam Type CBOR major type
     @param value of `Type`
   */
  template <typename Type>
    requires std::constructible_from<storage_type, Type &&>
  constexpr CBORValue(Type &&value) noexcept(
      noexcept(storage_type(std::move(value))))
      : storage(std::move(value)) {}

  /**
     Copy-constructs a CBOR value from a value of one of the CBOR major types.
     @tparam Type CBOR major type
     @param value of `Type`
   */
  template <typename Type>
    requires std::constructible_from<storage_type, Type const &>
  constexpr CBORValue(Type const &value) noexcept(noexcept(storage_type(value)))
      : storage(value) {}

  constexpr auto is_uint() const noexcept {
    return std::holds_alternative<CBORUint>(storage);
  }

  constexpr auto is_nint() const noexcept {
    return std::holds_alternative<CBORNint>(storage);
  }

  constexpr auto is_bstr() const noexcept {
    return std::holds_alternative<CBORBstr>(storage);
  }

  constexpr auto is_tstr() const noexcept {
    return std::holds_alternative<CBORTstr>(storage);
  }

  constexpr auto is_array() const noexcept {
    return std::holds_alternative<CBORArray>(storage);
  }

  constexpr auto is_map() const noexcept {
    return std::holds_alternative<CBORMap>(storage);
  }

  constexpr auto is_tag() const noexcept {
    return std::holds_alternative<CBORTag>(storage);
  }

  constexpr auto is_simple() const noexcept {
    return std::holds_alternative<CBORSimple>(storage);
  }

  constexpr auto is_float() const noexcept {
    return std::holds_alternative<CBORFloat>(storage);
  }

  auto as_uint() const noexcept -> std::optional<CBOR_U64> {
    using type = CBORUint;
    if (std::holds_alternative<type>(storage)) {
      return static_cast<CBOR_U64>(std::get<type>(storage));
    } else {
      return std::nullopt;
    }
  }

  auto as_nint() const noexcept -> std::optional<CBOR_U64> {
    using type = CBORNint;
    if (std::holds_alternative<type>(storage)) {
      return static_cast<CBOR_U64>(std::get<type>(storage));
    } else {
      return std::nullopt;
    }
  }

  auto as_bstr() const noexcept -> std::optional<CBORBstr> {
    using type = CBORBstr;
    if (std::holds_alternative<type>(storage)) {
      return std::get<type>(storage);
    } else {
      return std::nullopt;
    }
  }

  auto as_bstr_ref() noexcept -> std::optional<std::reference_wrapper<CBORBstr>> {
    using type = CBORBstr;
    if (std::holds_alternative<type>(storage)) {
      return std::ref(std::get<type>(storage));
    } else {
      return std::nullopt;
    }
  }

  auto as_bstr_cref() const noexcept -> std::optional<std::reference_wrapper<CBORBstr const>> {
    using type = CBORBstr;
    if (std::holds_alternative<type>(storage)) {
      return std::cref(std::get<type>(storage));
    } else {
      return std::nullopt;
    }
  }

  auto move_bstr(CBORBstr& target) {
    using type = CBORBstr;
    if (std::holds_alternative<type>(storage)) {
      std::exchange(target, std::get<type>(storage));
      *this = CBOR_Undefined;
    }
  }
};
