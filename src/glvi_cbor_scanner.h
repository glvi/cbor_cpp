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
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <iterator>
#include <variant>
#include <vector>

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
///
/// Syntax tokens and associated types:
///
///     UINT	=> std::uint64_t
///     NINT	=> std::uint64_t
///     BSTRX   => void
///     BSTR	=> std::vector<std::byte>
///     TSTRX   => void
///     TSTR	=> std::vector<std::byte>
///     ARRAYX  => void
///     ARRAY	=> std::uint64_t
///     MAPX    => void
///     MAP	=> std::uint64_t
///     TAG	=> std::uint64_t
///     SIMPLE	=> std::uint8_t
///     FLOAT	=> std::uint64_t
///     BREAK   => void
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
  struct BstrX {};
  /// Token for a byte string
  struct Bstr {
    std::vector<std::byte> value;
  };
  /// Token for a text string of indefinite length
  struct TstrX {};
  /// Token for a text string
  struct Tstr {
    std::vector<std::byte> value;
  };
  /// Token for an array of indefinite length
  struct ArrayX {};
  /// Token for an array
  struct Array {
    std::uint64_t value;
  };
  /// Token for a map of indefinite length
  struct MapX {};
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
  struct Break {};
  /// Syntax token, a.k.a. terminal symbol
  using Token = std::variant<Uint, Nint, BstrX, Bstr, TstrX, Tstr, ArrayX,
                             Array, MapX, Map, Tag, Simple, Float, Break>;

  inline namespace error {
    struct NotUint {};
    struct NotNint {};
    struct NotBstrX {};
    struct NotBstr {};
    struct NotTstrX {};
    struct NotTstr {};
    struct NotArrayX {};
    struct NotArray {};
    struct NotMapX {};
    struct NotMap {};
    struct NotTag {};
    struct NotSimple {};
    struct NotFloat {};
    struct NotBreak {};
  } // namespace error
} // namespace token

template <typename T>
concept token_type = std::constructible_from<token::Token, T>;

/// Syntax token, a.k.a. terminal symbol
class Token {
  token::Token token;

  template <typename T> constexpr auto holds() noexcept {
    return std::holds_alternative<T>(token);
  }

  template <typename T> constexpr auto& get() {
    return std::get<T>(token);
  }

public:
  template <token_type Token> Token(Token token) : token{std::move(token)} {
  }

  Token() = delete;

  constexpr auto is_uint() noexcept -> bool {
    return holds<token::Uint>();
  }

  constexpr auto is_nint() noexcept -> bool {
    return holds<token::Nint>();
  }

  constexpr auto is_bstrx() noexcept -> bool {
    return holds<token::BstrX>();
  }

  constexpr auto is_bstr() noexcept -> bool {
    return holds<token::Bstr>();
  }

  constexpr auto is_tstrx() noexcept -> bool {
    return holds<token::TstrX>();
  }

  constexpr auto is_tstr() noexcept -> bool {
    return holds<token::Tstr>();
  }

  constexpr auto is_arrayx() noexcept -> bool {
    return holds<token::ArrayX>();
  }

  constexpr auto is_array() noexcept -> bool {
    return holds<token::Array>();
  }

  constexpr auto is_mapx() noexcept -> bool {
    return holds<token::MapX>();
  }

  constexpr auto is_map() noexcept -> bool {
    return holds<token::Map>();
  }

  constexpr auto is_tag() noexcept -> bool {
    return holds<token::Tag>();
  }

  constexpr auto is_simple() noexcept -> bool {
    return holds<token::Simple>();
  }

  constexpr auto is_float() noexcept -> bool {
    return holds<token::Float>();
  }

  constexpr auto is_break() noexcept -> bool {
    return holds<token::Break>();
  }

  constexpr auto as_uint()
      -> std::expected<std::uint64_t, token::error::NotUint> {
    if (not is_uint())
      return std::unexpected(token::error::NotUint{});
    return get<token::Uint>().value;
  }

  constexpr auto as_nint()
      -> std::expected<std::uint64_t, token::error::NotNint> {
    if (not is_nint())
      return std::unexpected(token::error::NotNint{});
    return get<token::Nint>().value;
  }

  constexpr auto as_bstr()
      -> std::expected<std::vector<std::byte>, token::error::NotBstr> {
    if (not is_bstr())
      return std::unexpected(token::error::NotBstr{});
    return get<token::Bstr>().value;
  }

  constexpr auto as_tstr()
      -> std::expected<std::vector<std::byte>, token::error::NotTstr> {
    if (not is_tstr())
      return std::unexpected(token::error::NotTstr{});
    return get<token::Tstr>().value;
  }

  constexpr auto as_array()
      -> std::expected<std::uint64_t, token::error::NotArray> {
    if (not is_array())
      return std::unexpected(token::error::NotArray{});
    return get<token::Array>().value;
  }

  constexpr auto as_map()
      -> std::expected<std::uint64_t, token::error::NotMap> {
    if (not is_map())
      return std::unexpected(token::error::NotMap{});
    return get<token::Map>().value;
  }

  constexpr auto as_tag()
      -> std::expected<std::uint64_t, token::error::NotTag> {
    if (not is_tag())
      return std::unexpected(token::error::NotTag{});
    return get<token::Tag>().value;
  }

  constexpr auto as_simple()
      -> std::expected<std::uint8_t, token::error::NotSimple> {
    if (not is_simple())
      return std::unexpected(token::error::NotSimple{});
    return get<token::Simple>().value;
  }

  constexpr auto as_float()
      -> std::expected<std::uint64_t, token::error::NotFloat> {
    if (not is_float())
      return std::unexpected(token::error::NotFloat{});
    return get<token::Float>().value;
  }
};

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

  inline namespace error {
    struct NotIncomplete {};
    struct NotComplete {};
    struct NotError {};
  } // namespace error

} // namespace scan_result

template <typename T>
concept scan_result_type = std::constructible_from<scan_result::ScanResult, T>;

class ScanResult {
  scan_result::ScanResult result;

  template <typename T> constexpr auto holds() noexcept {
    return std::holds_alternative<T>(result);
  }

  template <typename T> constexpr auto& get() {
    return std::get<T>(result);
  }

public:
  ScanResult() = delete;

  template <scan_result_type Result>
  ScanResult(Result result) : result{std::move(result)} {
  }

  ScanResult(ScanError error) : result{std::move(error)} {
  }

  constexpr auto is_incomplete() noexcept -> bool {
    return holds<scan_result::Incomplete>();
  }

  constexpr auto is_complete() noexcept -> bool {
    return holds<scan_result::Complete>();
  }

  constexpr auto is_error() noexcept -> bool {
    return holds<ScanError>();
  }

  constexpr auto as_incomplete()
      -> std::expected<ScanState, scan_result::error::NotIncomplete> {
    if (not is_incomplete())
      return std::unexpected(scan_result::error::NotIncomplete{});
    auto [state] = get<scan_result::Incomplete>();
    return state;
  }

  constexpr auto as_complete()
      -> std::expected<std::tuple<ScanState, Token>,
                       scan_result::error::NotComplete> {
    if (not is_complete())
      return std::unexpected(scan_result::error::NotComplete{});
    auto [state, token] = get<scan_result::Complete>();
    return std::make_tuple(state, token);
  }

  constexpr auto as_error()
      -> std::expected<ScanError, scan_result::error::NotError> {
    if (not is_error())
      return std::unexpected(scan_result::error::NotError{});
    return get<ScanError>();
  }

  template <typename Visitor>
  friend constexpr auto visit(Visitor&& v, ScanResult&& r) {
    return std::visit(std::forward<Visitor>(v), std::move(r.result));
  }
};

auto consume(ScanState&& state, std::uint8_t byte) -> ScanResult;

template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
  requires std::same_as<std::uint8_t, typename Iterator::value_type>
auto consume(ScanState&& state, Iterator first, Sentinel last) -> ScanResult {
  if (first == last)
    return scan_error::UnexpectedEof{};
  for (;;) {
    auto result = ::consume(std::move(state), *first++);
    if (result.is_error() or result.is_complete())
      return result;
    state = std::move(result.as_incomplete().value());
  }
}

template <std::ranges::input_range Range>
  requires std::same_as<std::uint8_t, typename Range::value_type>
auto consume(ScanState&& state, Range range) -> ScanResult {
  return consume(std::move(state), range.begin(), range.end());
}
