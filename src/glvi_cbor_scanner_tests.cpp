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
#include "glvi_cbor_scanner.h"
#include <concepts>
#include <cstdint>
#include <dejagnu.h>
#include <initializer_list>
#include <ranges>
#include <source_location>

using namespace std::string_literals;

template <std::integral integral_type>
  requires(sizeof(integral_type) == sizeof(std::byte))
auto to_vec_byte(std::vector<integral_type>& vec) -> std::vector<std::byte> {
  return reinterpret_cast<std::vector<std::byte>&>(vec);
}

using vec_u8 = std::vector<std::uint8_t>;

using vec_byte = std::vector<std::byte>;

#define TEST_CASE(name) auto test_##name() noexcept try

class CBORScannerTests : TestState, std::source_location {
  unsigned numFailed_ = 0;

  void fail(std::string msg) {
    TestState::fail(std::move(msg));
    numFailed_++;
  }

  static constexpr auto to_byte = [](auto n) { return std::byte(n); };

  static constexpr auto to_bytes = std::views::transform(to_byte);

  template <typename Assessor>
  auto expect(std::source_location loc, Assessor assess, vec_u8 from) noexcept
      try {
    auto result = consume(ScanState{}, std::move(from));
    auto [_, token] = result.as_complete().value();
    if ((token.*assess)()) {
      return pass(loc.function_name());
    }
    return fail(loc.function_name());
  } catch (...) {
    return fail(loc.function_name());
  }

  template <typename Extractor, typename Expected>
  auto expect(std::source_location loc, Extractor extract, Expected expected,
              vec_u8 from) noexcept try {
    auto result = consume(ScanState{}, std::move(from));
    auto [_, token] = result.as_complete().value();
    if ((token.*extract)().value() == expected) {
      return pass(loc.function_name());
    }
    return fail(loc.function_name());
  } catch (...) {
    return fail(loc.function_name());
  }

  inline auto expect_uint(std::source_location loc, std::uint64_t expected,
                          vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_uint, expected, std::move(from));
  }

  inline auto expect_nint(std::source_location loc, std::uint64_t expected,
                          vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_nint, expected, std::move(from));
  }

  inline auto expect_bstrx(std::source_location loc, vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::is_bstrx, std::move(from));
  }

  inline auto expect_bstr(std::source_location loc,
                          std::vector<std::byte> expected,
                          vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_bstr, std::move(expected),
                  std::move(from));
  }

public:
  inline auto success() const noexcept {
    return numFailed_ == 0;
  }
  inline auto failure() const noexcept {
    return numFailed_ > 0;
  }

  auto test_decode_uint0() noexcept {
    return expect_uint(current(), 0x01, {0x01});
  }

  auto test_decode_uint1() noexcept {
    return expect_uint(current(), 0x01, {0x18, 0x01});
  }

  auto test_decode_uint2() noexcept {
    return expect_uint(current(), 0x0102, {0x19, 0x01, 0x02});
  }

  auto test_decode_uint4() noexcept {
    return expect_uint(current(), 0x01020304, {0x1a, 0x01, 0x02, 0x03, 0x04});
  }

  auto test_decode_uint8() noexcept {
    return expect_uint(current(), 0x0102030405060708,
                       {0x1b, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});
  }

  auto test_decode_nint0() noexcept {
    return expect_nint(current(), 0x01, {0x21});
  }

  auto test_decode_nint1() noexcept {
    return expect_nint(current(), 0x01, {0x38, 0x01});
  }

  auto test_decode_nint2() noexcept {
    return expect_nint(current(), 0x0102, {0x39, 0x01, 0x02});
  }

  auto test_decode_nint4() noexcept {
    return expect_nint(current(), 0x01020304, {0x3a, 0x01, 0x02, 0x03, 0x04});
  }

  auto test_decode_nint8() noexcept {
    return expect_nint(current(), 0x0102030405060708,
                       {0x3b, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});
  }

  auto test_decode_bstrx() noexcept {
    return expect_bstrx(current(), {0x5f});
  }

  auto test_decode_bstr0() noexcept {
    return expect_bstr(current(), {}, {0x40});
  }

  auto test_decode_bstr1() noexcept {
    return expect_bstr(current(), {}, {0x58, 0x00});
  }

  auto test_decode_bstr2() noexcept {
    return expect_bstr(current(), {}, {0x59, 0x00, 0x00});
  }

  auto test_decode_bstr4() noexcept {
    return expect_bstr(current(), {}, {0x5a, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_bstr8() noexcept {
    return expect_bstr(current(), {},
                       {0x5b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_bstr0_with_payload() noexcept {
    auto test_vector = vec_u8{0x57, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                              0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    auto expected = vec_u8{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                           0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                           0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    return expect_bstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_bstr1_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x58, 0x17, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
               0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
               0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    auto expected = vec_u8{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                           0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                           0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    return expect_bstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_bstr2_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x59, 0x00, 0x17, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
               0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
               0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    auto expected = vec_u8{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                           0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                           0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    return expect_bstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_bstr4_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x5a, 0x00, 0x00, 0x00, 0x17, 0x01, 0x02, 0x03, 0x04, 0x05,
               0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
               0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    auto expected = vec_u8{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                           0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                           0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    return expect_bstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_bstr8_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x5b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x01, 0x02,
               0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
               0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    auto expected = vec_u8{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                           0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                           0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    return expect_bstr(current(), to_vec_byte(expected), test_vector);
  }

  TEST_CASE(decode_tstrx) {
    auto result = consume(ScanState{}, 0x7f);
    auto [_, token] = result.as_complete().value();
    if (token.is_tstrx()) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_tstr0) {
    auto result = consume(ScanState{}, 0x60);
    auto [_, token] = result.as_complete().value();
    if (token.as_tstr().value() == std::vector<std::byte>{}) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_tstr) {
    std::vector<std::uint8_t> test_vector{0x61, 0x01};
    auto [_, token] = consume(ScanState{}, test_vector).as_complete().value();
    if (token.as_tstr().value() == std::vector{std::byte{0x01}}) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_arrayx) {
    auto result = consume(ScanState{}, 0x9f);
    auto [_, token] = result.as_complete().value();
    if (token.is_arrayx()) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_array0) {
    auto result = consume(ScanState{}, 0x80);
    auto [_, token] = result.as_complete().value();
    if (token.as_array().value() == 0x00) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_array1) {
    std::vector<std::uint8_t> test_vector{0x81, 0x01};
    auto [_, token] = consume(ScanState{}, test_vector).as_complete().value();
    if (token.as_array().value() == 0x01) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_array2) {
    std::vector<std::uint8_t> test_vector{0x99, 0x01};
    auto result = consume(ScanState{}, test_vector);
    if (result.is_incomplete()) {
      return pass(current().function_name());
    } else if (result.is_error()) {
      note("%s:%d: Result is <ERROR>", current().file_name(), current().line());
    } else if (result.is_complete()) {
      note("%s:%d: Result is <COMPLETE>", current().file_name(),
           current().line());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_mapx) {
    auto result = consume(ScanState{}, 0xbf);
    auto [_, token] = result.as_complete().value();
    if (token.is_mapx()) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_map0) {
    auto result = consume(ScanState{}, 0xa0);
    auto [_, token] = result.as_complete().value();
    if (token.as_map().value() == 0x00) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_map) {
    std::vector<std::uint8_t> test_vector{0xa1, 0x01};
    auto [_, token] = consume(ScanState{}, test_vector).as_complete().value();
    if (token.as_map().value() == 0x01) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_tag) {
    auto [_, token] = consume(ScanState{}, 0xc1).as_complete().value();
    if (token.as_tag().value() == 0x01) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_simple) {
    auto result = consume(ScanState{}, 0xe1);
    auto [_, token] = result.as_complete().value();
    if (token.as_simple().value() == 0x01) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_float) {
    std::vector<std::uint8_t> test_vector{0xf9, 0x01, 0x02};
    auto result = consume(ScanState{}, test_vector);
    auto [_, token] = result.as_complete().value();
    if (token.as_float().value() == 0x0102) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }
};

int main(int argc, char *argv[]) {
  CBORScannerTests testSuite{};
  testSuite.test_decode_uint0();
  testSuite.test_decode_uint1();
  testSuite.test_decode_uint2();
  testSuite.test_decode_uint4();
  testSuite.test_decode_uint8();
  testSuite.test_decode_nint0();
  testSuite.test_decode_nint1();
  testSuite.test_decode_nint2();
  testSuite.test_decode_nint4();
  testSuite.test_decode_nint8();
  testSuite.test_decode_bstrx();
  testSuite.test_decode_bstr0();
  testSuite.test_decode_bstr1();
  testSuite.test_decode_bstr2();
  testSuite.test_decode_bstr4();
  testSuite.test_decode_bstr8();
  testSuite.test_decode_bstr0_with_payload();
  testSuite.test_decode_bstr1_with_payload();
  testSuite.test_decode_bstr2_with_payload();
  testSuite.test_decode_bstr4_with_payload();
  testSuite.test_decode_bstr8_with_payload();
  testSuite.test_decode_tstrx();
  testSuite.test_decode_tstr0();
  testSuite.test_decode_tstr();
  testSuite.test_decode_arrayx();
  testSuite.test_decode_array0();
  testSuite.test_decode_array1();
  testSuite.test_decode_array2();
  testSuite.test_decode_mapx();
  testSuite.test_decode_map0();
  testSuite.test_decode_map();
  testSuite.test_decode_tag();
  testSuite.test_decode_simple();
  testSuite.test_decode_float();
  return testSuite.failure();
}
