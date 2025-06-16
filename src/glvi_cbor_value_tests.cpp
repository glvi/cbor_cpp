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
#include "glvi_cbor_simple.h"
#include "glvi_cbor_value.h"
#include <dejagnu.h>
#include <source_location>

using namespace std::string_literals;

#define TEST_CASE(name) auto test_##name() noexcept try

class CBORValueTests : TestState, std::source_location {
  unsigned numFailed_ = 0;

  void fail(std::string msg) {
    TestState::fail(std::move(msg));
    numFailed_++;
  }

public:
  inline auto success() const noexcept { return numFailed_ == 0; }
  inline auto failure() const noexcept { return numFailed_ > 0; }

  TEST_CASE(construct_tag) {
    CBORValue x = CBORTag(0_cbor, u8"foo"_cbor_tstr);
    if (auto const &opt_tag = x.as_tag_cref()) {
      auto const &tag = opt_tag->get();
      if (tag.tag() == 0_cbor) {
	if (auto const& opt_tstr = tag.value().as_tstr_cref()) {
	  auto const& tstr = opt_tstr->get();
	  if (tstr == u8"foo"s)
	    return pass(current().function_name());
	}
      }
    }
    return fail(current().function_name());
  }
  catch (...) {
    return fail(current().function_name());
  }

  TEST_CASE(move_tag) {
    CBORValue x = CBORTag(1_cbor, u8"foo"_cbor_tstr);
    CBORTag y{0_cbor, CBORUint(0_cbor)};
    x.move_tag(y);
    if (x.is_simple() and y.tag() == 1_cbor and y.value().is_tstr()) {
      if (auto const& opt_tstr = y.value().as_tstr_cref()) {
	auto const& tstr = opt_tstr->get();
	if (tstr == u8"foo"s) {
	  return pass(current().function_name());
	}
      }
    }
    return fail(current().function_name());
  } catch (...) {
    return fail(current().function_name());
  }
};

int main(int argc, char *argv[]) {
  CBORValueTests testSuite{};
  testSuite.test_construct_tag();
  testSuite.test_move_tag();
  return testSuite.failure();
}
