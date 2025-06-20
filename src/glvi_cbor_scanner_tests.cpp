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

  inline auto expect_tstrx(std::source_location loc, vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::is_tstrx, std::move(from));
  }

  inline auto expect_tstr(std::source_location loc,
                          std::vector<std::byte> expected,
                          vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_tstr, std::move(expected),
                  std::move(from));
  }

  inline auto expect_arrayx(std::source_location loc, vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::is_arrayx, std::move(from));
  }

  inline auto expect_array(std::source_location loc, std::uint64_t expected,
                           vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_array, expected, std::move(from));
  }

  inline auto expect_mapx(std::source_location loc, vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::is_mapx, std::move(from));
  }

  inline auto expect_map(std::source_location loc, std::uint64_t expected,
                         vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_map, expected, std::move(from));
  }

  inline auto expect_tag(std::source_location loc, std::uint64_t expected,
                         vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_tag, expected, std::move(from));
  }

  inline auto expect_simple(std::source_location loc, std::uint8_t expected,
                            vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_simple, expected, std::move(from));
  }

  inline auto expect_float(std::source_location loc, std::uint64_t expected,
                           vec_u8 from) noexcept {
    return expect(std::move(loc), &Token::as_float, expected, std::move(from));
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

  auto test_decode_tstrx() noexcept {
    return expect_tstrx(current(), {0x7f});
  }

  auto test_decode_tstr0() noexcept {
    return expect_tstr(current(), {}, {0x60});
  }

  auto test_decode_tstr1() noexcept {
    return expect_tstr(current(), {}, {0x78, 0x00});
  }

  auto test_decode_tstr2() noexcept {
    return expect_tstr(current(), {}, {0x79, 0x00, 0x00});
  }

  auto test_decode_tstr4() noexcept {
    return expect_tstr(current(), {}, {0x7a, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_tstr8() noexcept {
    return expect_tstr(current(), {},
                       {0x7b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_tstr0_with_payload() noexcept {
    auto test_vector = vec_u8{0x77, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                              0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
                              0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    auto expected = vec_u8{0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                           0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
                           0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    return expect_tstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_tstr1_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x78, 0x17, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
               0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
               0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    auto expected = vec_u8{0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                           0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
                           0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    return expect_tstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_tstr2_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x79, 0x00, 0x17, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
               0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
               0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    auto expected = vec_u8{0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                           0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
                           0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    return expect_tstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_tstr4_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x7a, 0x00, 0x00, 0x00, 0x17, 0x41, 0x42, 0x43, 0x44, 0x45,
               0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
               0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    auto expected = vec_u8{0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                           0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
                           0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    return expect_tstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_tstr8_with_payload() noexcept {
    auto test_vector =
        vec_u8{0x7b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x41, 0x42,
               0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d,
               0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    auto expected = vec_u8{0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                           0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
                           0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57};
    return expect_tstr(current(), to_vec_byte(expected), test_vector);
  }

  auto test_decode_arrayx() noexcept {
    return expect_arrayx(current(), {0x9f});
  }

  auto test_decode_array0() noexcept {
    return expect_array(current(), 0, {0x80});
  }

  auto test_decode_array1() noexcept {
    return expect_array(current(), 0, {0x98, 0x00});
  }

  auto test_decode_array2() noexcept {
    return expect_array(current(), 0, {0x99, 0x00, 0x00});
  }

  auto test_decode_array4() noexcept {
    return expect_array(current(), 0, {0x9a, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_array8() noexcept {
    return expect_array(current(), 0,
                        {0x9b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_array0_with_payload() noexcept {
    return expect_array(current(), 0x17, {0x97});
  }

  auto test_decode_array1_with_payload() noexcept {
    return expect_array(current(), 0x17, {0x98, 0x17});
  }

  auto test_decode_array2_with_payload() noexcept {
    return expect_array(current(), 0x17, {0x99, 0x00, 0x17});
  }

  auto test_decode_array4_with_payload() noexcept {
    return expect_array(current(), 0x17, {0x9a, 0x00, 0x00, 0x00, 0x17});
  }

  auto test_decode_array8_with_payload() noexcept {
    return expect_array(current(), 0x17,
                        {0x9b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17});
  }

  auto test_decode_mapx() noexcept {
    return expect_mapx(current(), {0xbf});
  }

  auto test_decode_map0() noexcept {
    return expect_map(current(), 0, {0xa0});
  }

  auto test_decode_map1() noexcept {
    return expect_map(current(), 0, {0xb8, 0x00});
  }

  auto test_decode_map2() noexcept {
    return expect_map(current(), 0, {0xb9, 0x00, 0x00});
  }

  auto test_decode_map4() noexcept {
    return expect_map(current(), 0, {0xba, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_map8() noexcept {
    return expect_map(current(), 0,
                      {0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_map0_with_payload() noexcept {
    return expect_map(current(), 0x17, {0xb7});
  }

  auto test_decode_map1_with_payload() noexcept {
    return expect_map(current(), 0x17, {0xb8, 0x17});
  }

  auto test_decode_map2_with_payload() noexcept {
    return expect_map(current(), 0x17, {0xb9, 0x00, 0x17});
  }

  auto test_decode_map4_with_payload() noexcept {
    return expect_map(current(), 0x17, {0xba, 0x00, 0x00, 0x00, 0x17});
  }

  auto test_decode_map8_with_payload() noexcept {
    return expect_map(current(), 0x17,
                      {0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17});
  }

  auto test_decode_tag0() noexcept {
    return expect_tag(current(), 0, {0xc0});
  }

  auto test_decode_tag1() noexcept {
    return expect_tag(current(), 0, {0xd8, 0x00});
  }

  auto test_decode_tag2() noexcept {
    return expect_tag(current(), 0, {0xd9, 0x00, 0x00});
  }

  auto test_decode_tag4() noexcept {
    return expect_tag(current(), 0, {0xda, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_tag8() noexcept {
    return expect_tag(current(), 0,
                      {0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_simple0() noexcept {
    return expect_simple(current(), 0, {0xe0});
  }

  auto test_decode_simple1() noexcept {
    return expect_simple(current(), 0, {0xf8, 0x00});
  }

  auto test_decode_float2() noexcept {
    return expect_float(current(), 0, {0xf9, 0x00, 0x00});
  }

  auto test_decode_float4() noexcept {
    return expect_float(current(), 0, {0xfa, 0x00, 0x00, 0x00, 0x00});
  }

  auto test_decode_float8() noexcept {
    return expect_float(current(), 0,
                        {0xfb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
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
  testSuite.test_decode_tstr1();
  testSuite.test_decode_tstr2();
  testSuite.test_decode_tstr4();
  testSuite.test_decode_tstr8();
  testSuite.test_decode_tstr0_with_payload();
  testSuite.test_decode_tstr1_with_payload();
  testSuite.test_decode_tstr2_with_payload();
  testSuite.test_decode_tstr4_with_payload();
  testSuite.test_decode_tstr8_with_payload();
  testSuite.test_decode_arrayx();
  testSuite.test_decode_array0();
  testSuite.test_decode_array1();
  testSuite.test_decode_array2();
  testSuite.test_decode_array4();
  testSuite.test_decode_array8();
  testSuite.test_decode_array0_with_payload();
  testSuite.test_decode_array1_with_payload();
  testSuite.test_decode_array2_with_payload();
  testSuite.test_decode_array4_with_payload();
  testSuite.test_decode_array8_with_payload();
  testSuite.test_decode_mapx();
  testSuite.test_decode_map0();
  testSuite.test_decode_map1();
  testSuite.test_decode_map2();
  testSuite.test_decode_map4();
  testSuite.test_decode_map8();
  testSuite.test_decode_map0_with_payload();
  testSuite.test_decode_map1_with_payload();
  testSuite.test_decode_map2_with_payload();
  testSuite.test_decode_map4_with_payload();
  testSuite.test_decode_map8_with_payload();
  testSuite.test_decode_tag0();
  testSuite.test_decode_tag1();
  testSuite.test_decode_tag2();
  testSuite.test_decode_tag4();
  testSuite.test_decode_tag8();
  testSuite.test_decode_simple0();
  testSuite.test_decode_simple1();
  testSuite.test_decode_float2();
  testSuite.test_decode_float4();
  testSuite.test_decode_float8();
  return testSuite.failure();
}
