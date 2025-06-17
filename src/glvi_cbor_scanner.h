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
#include <cstdint>
#include <variant>
#include <vector>

/**
   Numerical representation of kinds of token.
*/
enum class Kind {
  /// Indicates a natural number
  Uint,
  /// Indicates a negative integer
  Nint,
  /// Indicates a sequence of bytes, indefinite length
  BstrX,
  /// Indicates a sequence of bytes, definite length
  Bstr,
  /// Indicates a text string encoded in UTF-8, indefinite length
  TstrX,
  /// Indicates a text string encoded in UTF-8, definite length
  Tstr,
  /// Indicates a sequence of CBOR values, indefinite length
  ArrayX,
  /// Indicates a sequence of CBOR values, definite length
  Array,
  /// Indicates a sequence of pairs of CBOR values, indefinite length
  MapX,
  /// Indicates a sequence of pairs of CBOR values, definite length
  Map,
  /// Indicates a tagged value
  Tag,
  /// Indicates a simple value
  Simple,
  /// Indicates a rational value in binary floating-point representation
  Float,
  /// Indicates the end of an indefinite sequence
  Break,
};

/// Structured representation of syntax tokens, a.k.a. terminal symbols
namespace token {
  /// Token for a natural number
  struct Uint {
    std::uint64_t value;
  };
  /// Token for a negative integer
  struct Nint {
    std::uint64_t value;
  };
  /// Token for a byte string of indefinite length
  struct BstrX {
  };
  /// Token for a byte string
  struct Bstr {
    std::vector<std::byte> value;
  };
  /// Token for a text string of indefinite length
  struct TstrX {
  };
  /// Token for a text string
  struct Tstr {
    std::vector<std::byte> value;
  };
  /// Token for an array of indefinite length
  struct ArrayX {
  };
  /// Token for an array
  struct Array {
    std::uint64_t value;
  };
  /// Token for a map of indefinite length
  struct MapX {
  };
  /// Token for a map
  struct Map {
    std::uint64_t value;
  };
  /// Token for a tagged value
  struct Tag {
    std::uint64_t value;
  };
  /// Token for a simple value
  struct Simple {
    std::uint8_t value;
  };
  /// Token for a floating-point value
  struct Float {
    std::uint64_t value;
  };
  /// Token for the end of a sequence of indefinite length
  struct Break {
  };
  /// Syntax token, a.k.a. terminal symbol
  using Token = std::variant<Uint, Nint, BstrX, Bstr, TstrX, Tstr, ArrayX,
                             Array, MapX, Map, Tag, Simple, Float, Break>;
} // namespace token

/// Syntax token, a.k.a. terminal symbol
using token::Token;

namespace scan_error {

  struct UnexpectedEof {};

  struct UnexpectedHead {
    std::byte head;
  };

  struct Excessive {
    std::uint64_t count;
  };

  using ScanError = std::variant<UnexpectedEof, UnexpectedHead, Excessive>;
} // namespace scan_error

using scan_error::ScanError;

namespace scan_state {

  struct Head {};

  struct Arg {
    Kind kind;
    std::uint64_t arg;
    std::size_t pending;
  };

  struct Pay {
    Kind kind;
    std::vector<std::byte> bytes;
    std::size_t pending;
  };

  using ScanState = std::variant<Head, Arg, Pay>;

} // namespace scan_state

using scan_state::ScanState;

namespace scan_result {

  struct Incomplete {
    ScanState state;
  };

  struct Complete {
    ScanState state;
    Token token;
  };

  using ScanResult = std::variant<Incomplete, Complete, ScanError>;

} // namespace scan_result

using scan_result::ScanResult;

auto consume(ScanState&& state, std::uint8_t byte) -> ScanResult;
