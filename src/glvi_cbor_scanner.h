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
#include "config.h"
#include "glvi_cbor_token.h"

/**
   Errors specific to lexical scanning
 */
namespace scan_error {

  /**
     This error indicates that the scanner encountered an unspecified
     byte in head position.

     An unspecified byte is one that does not contain a valid
     combination of CBOR major type and argument.
   */
  struct UnexpectedHead {
    std::byte head;
  };

  /**
     This error indicates that the scanner encountered an excessive count.

     When one of the definite-length major types -- that is byte
     string, text string, array, or map -- is specified as having a
     count value larger than a hard-coded maximum.

     ```c++
     auto max_count = std::numeric_limits<std::size_t>::max();
     ```
   */
  struct Excessive {
    std::size_t count;
  };

  /**
     Errors that can occur during scanning
   */
  using ScanError = std::variant<UnexpectedHead, Excessive>;

} // namespace scan_error

/// @copydoc scan_error::ScanError
using scan_error::ScanError;

/// Internal states of the lexical scanner
namespace scan_state {

  /**
     Limits the maximum number of bytes in a CBOR byte string.

     Settings this to any number > 0 will cause the scanner to reject
     `BSTR` tokens whose argument exceeds this value.
   */
#if defined(GLVI_CBOR_BSTR_COUNT_MAX) && GLVI_CBOR_BSTR_COUNT_MAX > 0
  static std::size_t bstr_count_max = GLVI_CBOR_BSTR_COUNT_MAX;
#else
  static std::size_t bstr_count_max = std::numeric_limits<std::size_t>::max();
#endif

  /**
     Limits the maximum number of bytes in a CBOR byte string.

     Settings this to any number > 0 will cause the scanner to reject
     `TSTR` tokens whose argument exceeds this value.
   */
#if defined(GLVI_CBOR_TSTR_COUNT_MAX) && GLVI_CBOR_TSTR_COUNT_MAX > 0
  static std::size_t tstr_count_max = GLVI_CBOR_TSTR_COUNT_MAX;
#else
  static std::size_t tstr_count_max = std::numeric_limits<std::size_t>::max();
#endif

  /**
     Limits the maximum number of entries for CBOR arrays.

     Settings this to any number > 0 will cause the scanner to reject
     `Array` tokens whose argument exceeds this value.
   */
#if defined(GLVI_CBOR_ARRAY_COUNT_MAX) && GLVI_CBOR_ARRAY_COUNT_MAX > 0
  static std::size_t array_count_max = GLVI_CBOR_ARRAY_COUNT_MAX;
#else
  static std::size_t array_count_max = std::numeric_limits<std::size_t>::max();
#endif

  /**
     Limits the maximum number of entries for CBOR maps.

     Settings this to any number > 0 will cause the scanner to reject
     `Map` tokens whose argument exceeds this value.
   */
#if defined(GLVI_CBOR_MAP_COUNT_MAX) && GLVI_CBOR_MAP_COUNT_MAX > 0
  static std::size_t map_count_max = GLVI_CBOR_MAP_COUNT_MAX;
#else
  static std::size_t map_count_max = std::numeric_limits<std::size_t>::max();
#endif

  /**
     Expecting the next byte in "head" position
   */
  struct Head {};

  /**
     Expecting the next byte in "argument" position
   */
  struct Arg {
    Kind kind;
    std::uint64_t arg;
    std::size_t pending;
  };

  /**
     Expecting the next byte in "payload" position
   */
  struct Pay {
    Kind kind;
    std::vector<std::byte> bytes;
    std::size_t pending;
  };

  /// State of the lexical scanner
  using ScanState = std::variant<Head, Arg, Pay>;

} // namespace scan_state

/// State of the lexical scanner
using scan_state::ScanState;

/**
   Possible results when scanning input
 */
namespace scan_result {
  /**
     More input is needed
   */
  struct Incomplete {
    ScanState state;
  };

  /**
     We have a token!
   */
  struct Complete {
    ScanState state;
    Token token;
  };

  /// Result of scanning some input
  using ScanResult = std::variant<Incomplete, Complete, ScanError>;

  /// Errors that occur when accessing a scan result.
  inline namespace access_error {
    struct NotIncomplete {};
    struct NotComplete {};
    struct NotError {};
  } // namespace access_error

} // namespace scan_result

/**
   identifies types that can be used to construct a scan result.
 */
template <typename T>
concept scan_result_type = std::constructible_from<scan_result::ScanResult, T>;

/**
   Result of scanning some input
 */
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
  ScanResult(Result&& result) : result{std::forward<Result>(result)} {
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
      -> std::expected<ScanState, scan_result::access_error::NotIncomplete> {
    if (not is_incomplete())
      return std::unexpected(scan_result::access_error::NotIncomplete{});
    auto [state] = get<scan_result::Incomplete>();
    return state;
  }

  constexpr auto as_complete()
      -> std::expected<std::tuple<ScanState, Token>,
                       scan_result::access_error::NotComplete> {
    if (not is_complete())
      return std::unexpected(scan_result::access_error::NotComplete{});
    auto [state, token] = get<scan_result::Complete>();
    return std::make_tuple(state, token);
  }

  constexpr auto as_error()
      -> std::expected<ScanError, scan_result::access_error::NotError> {
    if (not is_error())
      return std::unexpected(scan_result::access_error::NotError{});
    return get<ScanError>();
  }

  template <typename Visitor>
  friend constexpr auto visit(Visitor&& v, ScanResult&& r) {
    return std::visit(std::forward<Visitor>(v), std::move(r.result));
  }
};

/**
   Main scanning operation.

   Given `state`, consumes `byte` and returns one of the following:

   - `scan_result::Incomplete` when more bytes are needed to make a token.
     The returned value will contain the state to continue from.
   - `scan_result::Complete` when a complete token is available.
     The returned value will contain the state to continue from, and the
     completed token.
   - `ScanError` when something went wrong.
     The returned value contains the error that occurred.
 */
auto scan(ScanState&& state, std::uint8_t byte) -> ScanResult;

/**
   Scans the input range specified by `[first,last)`.

   If, while scanning the input range, the scanner encounters an
   error, it stops scanning, and returns `ScanError`. The returned
   value contains the error that occurred.

   If, while scanning the input range, the scanner encounters a
   complete token, it stops scanning, and returns
   `scan_result::Complete`. The returned value will contain the state
   to continue from, and the completed token.

   If, while scanning the input range, the scanner encounters the end
   of the range before completing a token, it returns
   `scan_result::Incomplete`. The returned value will contain the
   state to continue from.

   If the input range is initially empty, the scanner returns
   `scan_result::Incomplete` with the initial state unmodified.
 */
template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
  requires std::same_as<std::uint8_t, typename Iterator::value_type>
auto scan(ScanState&& state, Iterator first, Sentinel last) -> ScanResult {
  while (first != last) {
    auto result = ::scan(std::move(state), *first++);
    if (result.is_error() or result.is_complete())
      return result;
    state = result.as_incomplete().value();
  }
  return scan_result::Incomplete{std::move(state)};
}

/**
   Scans the input range specified by `range`.

   See `auto consume(ScanState&&, Iterator, Sentinel) -> ScanResult`
 */
template <std::ranges::input_range Range>
  requires std::same_as<std::uint8_t, typename Range::value_type>
auto scan(ScanState&& state, Range range) -> ScanResult {
  return scan(std::move(state), range.begin(), range.end());
}
