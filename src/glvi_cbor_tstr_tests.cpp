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
#include "glvi_cbor_tstr.h"
#include <dejagnu.h>
#include <source_location>

#define TEST_CASE(name) auto test_##name() noexcept try

class CBORTstrTests : TestState {
  unsigned numFailed_ = 0;

  void fail(std::string msg) {
    TestState::fail(std::move(msg));
    numFailed_++;
  }

public:
  inline auto success() const noexcept { return numFailed_ == 0; }
  inline auto failure() const noexcept { return numFailed_ > 0; }

  TEST_CASE(direct_init)
  {
    CBORTstr x{};
    if (x.size() == 0) {
      return pass(std::source_location::current().function_name());
    }
    return fail(std::source_location::current().function_name());
  } catch (...) {
    return fail(std::source_location::current().function_name());
  }

  TEST_CASE(list_init)
  {
    CBORTstr x{
        std::byte{0x31},
        std::byte{0x32},
    };
    if (x.size() == 2 and x.at(0) == std::byte{0x31} and
        x.at(1) == std::byte{0x32}) {
      return pass(std::source_location::current().function_name());
    }
    return fail(std::source_location::current().function_name());
  } catch (...) {
    return fail(std::source_location::current().function_name());
  }

  TEST_CASE(vector_init_move)
  {
    auto y = std::vector{
        std::byte{0x31},
        std::byte{0x32},
    };
    CBORTstr x{std::move(y)};
    if (y.size() == 0 and x.size() == 2 and x.at(0) == std::byte{0x31} and
        x.at(1) == std::byte{0x32}) {
      return pass(std::source_location::current().function_name());
    }
    return fail(std::source_location::current().function_name());
  } catch (...) {
    return fail(std::source_location::current().function_name());
  }

  TEST_CASE(vector_init_copy)
  {
    auto y = std::vector{
        std::byte{0x31},
        std::byte{0x32},
    };
    CBORTstr x{y};
    if (y.size() == 2 and x.size() == 2 and x.at(0) == std::byte{0x31} and
        x.at(1) == std::byte{0x32}) {
      return pass(std::source_location::current().function_name());
    }
    return fail(std::source_location::current().function_name());
  } catch (...) {
    return fail(std::source_location::current().function_name());
  }

  TEST_CASE(at_out_of_bounds)
  {
    CBORTstr x{};
    x.at(1);
    return fail(std::source_location::current().function_name());
  } catch (std::out_of_range&) {
    return pass(std::source_location::current().function_name());
  } catch (...) {
    return fail(std::source_location::current().function_name());
  }
};

int main(int argc, char *argv[]) {
  CBORTstrTests testSuite{};
  testSuite.test_direct_init();
  testSuite.test_list_init();
  testSuite.test_vector_init_move();
  testSuite.test_vector_init_copy();
  testSuite.test_at_out_of_bounds();
  return testSuite.failure();
}
