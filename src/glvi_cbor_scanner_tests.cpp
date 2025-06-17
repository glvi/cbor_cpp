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
#include <dejagnu.h>
#include <source_location>
#include <variant>

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
    ScanState scanState;
    auto scanResult = consume(std::move(scanState), 0x01);
    if (std::holds_alternative<scan_result::Complete>(scanResult)) {
      auto& complete = std::get<scan_result::Complete>(scanResult);
      scanState = std::move(complete.state);
      if (std::holds_alternative<token::Uint>(complete.token)) {
        auto& uintToken = std::get<token::Uint>(complete.token);
        if (uintToken.value == 0x01) {
          return pass(current().function_name());
        }
      }
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(decode_nint) {
    ScanState state;
    auto result = consume(std::move(state), 0x21);
    if (std::holds_alternative<scan_result::Complete>(result)) {
      auto& complete = std::get<scan_result::Complete>(result);
      state = std::move(complete.state);
      if (std::holds_alternative<token::Nint>(complete.token)) {
        auto& token = std::get<token::Nint>(complete.token);
        if (token.value == 0x01) {
          return pass(current().function_name());
        }
      }
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
  return testSuite.failure();
}
