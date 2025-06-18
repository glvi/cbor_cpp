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
#include <cstdint>
#include <dejagnu.h>
#include <source_location>

using namespace std::string_literals;

#define TEST_CASE(name) auto test_##name() noexcept try

class CBORScannerTests : TestState, std::source_location {
  unsigned numFailed_ = 0;

  void fail(std::string msg) {
    TestState::fail(std::move(msg));
    numFailed_++;
  }

public:
  inline auto success() const noexcept {
    return numFailed_ == 0;
  }
  inline auto failure() const noexcept {
    return numFailed_ > 0;
  }

  TEST_CASE(decode_uint) {
    auto result = consume(ScanState{}, 0x01);
    auto [_, token] = result.as_complete().value();
    if (token.as_uint().value() == 0x01) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_nint) {
    auto result = consume(ScanState{}, 0x21);
    auto [_, token] = result.as_complete().value();
    if (token.as_nint().value() == 0x01) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_bstrx) {
    auto result = consume(ScanState{}, 0x5f);
    auto [_, token] = result.as_complete().value();
    if (token.is_bstrx()) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_bstr0) {
    auto result = consume(ScanState{}, 0x40);
    auto [_, token] = result.as_complete().value();
    if (token.as_bstr().value() == std::vector<std::byte>{}) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_bstr) {
    std::vector<std::uint8_t> test_vector{0x41, 0x01};
    auto [_, token] = consume(ScanState{}, test_vector).as_complete().value();
    if (token.as_bstr().value() == std::vector{std::byte{0x01}}) {
      return pass(current().function_name());
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
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

  TEST_CASE(decode_array) {
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
  testSuite.test_decode_uint();
  testSuite.test_decode_nint();
  testSuite.test_decode_bstrx();
  testSuite.test_decode_bstr0();
  testSuite.test_decode_bstr();
  testSuite.test_decode_tstrx();
  testSuite.test_decode_tstr0();
  testSuite.test_decode_tstr();
  testSuite.test_decode_arrayx();
  testSuite.test_decode_array0();
  testSuite.test_decode_array();
  testSuite.test_decode_mapx();
  testSuite.test_decode_map0();
  testSuite.test_decode_map();
  testSuite.test_decode_tag();
  testSuite.test_decode_simple();
  testSuite.test_decode_float();
  return testSuite.failure();
}
