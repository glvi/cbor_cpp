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
#include "glvi_cbor_parser.h"
#include "glvi_cbor_simple.h"
#include "glvi_cbor_token.h"
#include "glvi_cbor_uint.h"
#include "glvi_cbor_value.h"
#include <dejagnu.h>

#include <source_location>

namespace {
  template <typename... Ts> struct adhoc : Ts... {
    using Ts::operator()...;
  };
  template <typename... Ts> adhoc(Ts...) -> adhoc<Ts...>;
} // namespace

using namespace std::string_literals;

using vec_tok = std::vector<Token>;

using vec_cbor = std::vector<CBORValue>;

class CBORParserTests : TestState, std::source_location {
  enum class test { passed, failed };

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

  void test_parse_uint() noexcept try {
    static auto loc = current();
    auto expected = CBORUint(0_cbor);
    Parser parser;
    auto result = parser.consume(token::Uint(0));
    visit(adhoc{
              [&](parse_result::Incomplete&& incomplete) -> void {
		note("Incomplete");
                fail(loc.function_name());
              },
              [&](parse_result::Complete&& complete) -> void {
		note("Complete");
                fail(loc.function_name());
              },
              [&](ParseError&& error) -> void {
		note("Parse error: %s", to_u8string(error).c_str());
                fail(loc.function_name());
              },
          },
          std::move(result));
  } catch (...) {
    note("Exception");
    return fail(current().function_name());
  }

  void test_parse_null() noexcept try {
    static auto loc = current();
    auto expected = CBOR_Null;
    Parser parser;
    auto result = parser.consume(token::Simple(0x16));
    visit(adhoc{
              [&](parse_result::Incomplete&& incomplete) -> void {
		note("Incomplete");
                fail(loc.function_name());
              },
              [&](parse_result::Complete&& complete) -> void {
		note("Complete");
                fail(loc.function_name());
              },
              [&](ParseError&& error) -> void {
		note("Parse error: %s", to_u8string(error).c_str());
                fail(loc.function_name());
              },
          },
          std::move(result));
  } catch (...) {
    note("Exception");
    return fail(current().function_name());
  }

  void test_parse_nil() noexcept try {
    static auto loc = current();
    auto expected = CBOR_Nil;
    Parser parser;
    auto result = parser.consume(token::Bstr());
    visit(adhoc{
              [&](parse_result::Incomplete&& incomplete) -> void {
		note("Incomplete");
                fail(loc.function_name());
              },
              [&](parse_result::Complete&& complete) -> void {
		note("Complete");
                fail(loc.function_name());
              },
              [&](ParseError&& error) -> void {
		note("Parse error: %s", to_u8string(error).c_str());
                fail(loc.function_name());
              },
          },
          std::move(result));
  } catch (...) {
    note("Exception");
    return fail(current().function_name());
  }
};

int main(int argc, char *argv[]) {
  CBORParserTests testSuite{};
  testSuite.test_parse_uint();
  testSuite.test_parse_null();
  testSuite.test_parse_nil();
  return testSuite.failure();
}
