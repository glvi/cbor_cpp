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
#include "glvi_cbor_scanner_helper.h"
#include <cstdlib>
#include <expected>
#include <optional>
#include <variant>

auto unexpected_head_error(std::byte octet) -> ScanResult {
  return ScanError{scan_error::UnexpectedHead{octet}};
}

auto make_token(Kind kind, std::uint64_t argument = 0,
                std::vector<std::byte> payload = {}) -> ScanResult {
  return scan_result::Complete{
      scan_state::Head{},
      Token::make(kind, argument, std::move(payload)),
  };
}

auto token_uint(std::uint64_t arg) -> ScanResult {
  return make_token(Kind::Uint, arg);
}

auto token_nint(std::uint64_t arg) -> ScanResult {
  return make_token(Kind::Nint, arg);
}

auto token_bstr_empty() -> ScanResult {
  return make_token(Kind::Bstr);
}

auto token_bstr_indef() -> ScanResult {
  return make_token(Kind::BstrX, 0xff);
}

auto token_tstr_empty() -> ScanResult {
  return make_token(Kind::Tstr);
}

auto token_tstr_indef() -> ScanResult {
  return make_token(Kind::TstrX, 0xff);
}

auto token_array_empty() -> ScanResult {
  return make_token(Kind::Array);
}

auto token_array(std::uint64_t arg) -> ScanResult {
  return make_token(Kind::Array, arg);
}

auto token_array_indef() -> ScanResult {
  return make_token(Kind::ArrayX, 0xff);
}

auto token_map_empty() -> ScanResult {
  return make_token(Kind::Map);
}

auto token_map(std::uint64_t arg) -> ScanResult {
  return make_token(Kind::Map, arg);
}

auto token_map_indef() -> ScanResult {
  return make_token(Kind::MapX, 0xff);
}

auto token_tag(std::uint64_t arg) -> ScanResult {
  return make_token(Kind::Tag, arg);
}

auto token_simple(std::uint64_t arg) -> ScanResult {
  return make_token(Kind::Simple, arg);
}

auto token_break() -> ScanResult {
  return make_token(Kind::Break, 0xff);
}

struct Argc {
  std::size_t count;
  operator std::size_t() const noexcept {
    return count;
  }
  constexpr Argc(std::size_t n) : count{n} {
  }
};

constexpr Argc ARGC_N1{1};
constexpr Argc ARGC_N2{2};
constexpr Argc ARGC_N4{4};
constexpr Argc ARGC_N8{8};

auto gather_argument(Kind kind, Argc count) -> ScanResult {
  return scan_result::Incomplete(scan_state::Arg{kind, 0, count});
}

auto gather_bytes(Kind kind, std::uint64_t count) {
  return scan_result::Incomplete{scan_state::Pay{kind, {}, count}};
}

auto consume(ScanState&& state, std::uint8_t byte) -> ScanResult {
  struct ByteConsumer {
    std::uint8_t byte;
    auto operator()(scan_state::Head) -> ScanResult {
      switch (byte) {
      case CASES_0x00_0x17: return token_uint(byte);
      case 0x18           : return gather_argument(Kind::Uint, ARGC_N1);
      case 0x19           : return gather_argument(Kind::Uint, ARGC_N2);
      case 0x1a           : return gather_argument(Kind::Uint, ARGC_N4);
      case 0x1b           : return gather_argument(Kind::Uint, ARGC_N8);

      case CASES_0x20_0x37: return token_nint(byte - 0x20);
      case 0x38           : return gather_argument(Kind::Nint, ARGC_N1);
      case 0x39           : return gather_argument(Kind::Nint, ARGC_N2);
      case 0x3a           : return gather_argument(Kind::Nint, ARGC_N4);
      case 0x3b           : return gather_argument(Kind::Nint, ARGC_N8);

      case 0x40           : return token_bstr_empty();
      case CASES_0x41_0x57: return gather_bytes(Kind::Bstr, (byte - 0x40));
      case 0x58           : return gather_argument(Kind::Bstr, ARGC_N1);
      case 0x59           : return gather_argument(Kind::Bstr, ARGC_N2);
      case 0x5a           : return gather_argument(Kind::Bstr, ARGC_N4);
      case 0x5b           : return gather_argument(Kind::Bstr, ARGC_N8);
      case 0x5f           : return token_bstr_indef();

      case 0x60           : return token_tstr_empty();
      case CASES_0x61_0x77: return gather_bytes(Kind::Tstr, (byte - 0x60));
      case 0x78           : return gather_argument(Kind::Tstr, ARGC_N1);
      case 0x79           : return gather_argument(Kind::Tstr, ARGC_N2);
      case 0x7a           : return gather_argument(Kind::Tstr, ARGC_N4);
      case 0x7b           : return gather_argument(Kind::Tstr, ARGC_N8);
      case 0x7f           : return token_tstr_indef();

      case 0x80           : return token_array_empty();
      case CASES_0x81_0x97: return token_array(byte - 0x80);
      case 0x98           : return gather_argument(Kind::Array, ARGC_N1);
      case 0x99           : return gather_argument(Kind::Array, ARGC_N2);
      case 0x9a           : return gather_argument(Kind::Array, ARGC_N4);
      case 0x9b           : return gather_argument(Kind::Array, ARGC_N8);
      case 0x9f           : return token_array_indef();

      case 0xa0           : return token_map_empty();
      case CASES_0xa1_0xb7: return token_map(byte - 0xa0);
      case 0xb8           : return gather_argument(Kind::Map, ARGC_N1);
      case 0xb9           : return gather_argument(Kind::Map, ARGC_N2);
      case 0xba           : return gather_argument(Kind::Map, ARGC_N4);
      case 0xbb           : return gather_argument(Kind::Map, ARGC_N8);
      case 0xbf           : return token_map_indef();

      case CASES_0xc0_0xd7: return token_tag(byte - 0xc0);
      case 0xd8           : return gather_argument(Kind::Tag, ARGC_N1);
      case 0xd9           : return gather_argument(Kind::Tag, ARGC_N2);
      case 0xda           : return gather_argument(Kind::Tag, ARGC_N4);
      case 0xdb           : return gather_argument(Kind::Tag, ARGC_N8);

      case CASES_0xe0_0xf7: return token_simple(byte - 0xe0);
      case 0xf8           : return gather_argument(Kind::Simple, ARGC_N1);

      case 0xf9           : return gather_argument(Kind::Float, ARGC_N2);
      case 0xfa           : return gather_argument(Kind::Float, ARGC_N4);
      case 0xfb           : return gather_argument(Kind::Float, ARGC_N8);

      case 0xff           : return token_break();

      default             : return unexpected_head_error(std::byte{byte});
      }
    }
    auto operator()(scan_state::Arg&& arg) -> ScanResult {
      arg.arg <<= 8;
      arg.arg |= std::uint64_t(byte);
      arg.pending -= 1;
      if (arg.pending > 0) {
        return scan_result::Incomplete{std::move(arg)};
      } else if (arg.arg == 0) {
        switch (arg.kind) {
        case Kind::Bstr : return token_bstr_empty();
        case Kind::Tstr : return token_tstr_empty();
        case Kind::Array: return token_array_empty();
        case Kind::Map  : return token_map_empty();
        default         : return make_token(arg.kind, arg.arg, {});
        }
      } else {
        switch (arg.kind) {
        case Kind::Bstr:
        case Kind::Tstr: return gather_bytes(arg.kind, arg.arg);
        default        : return make_token(arg.kind, arg.arg, {});
        }
      }
    }
    auto operator()(scan_state::Pay&& pay) -> ScanResult {
      pay.bytes.push_back(std::byte{byte});
      pay.pending -= 1;
      if (pay.pending > 0) {
        return scan_result::Incomplete{std::move(pay)};
      } else {
        switch (pay.kind) {
        case Kind::Bstr:
        case Kind::Tstr:
          return make_token(pay.kind, pay.bytes.size(), std::move(pay.bytes));
        default:
          // TODO: ERROR MESSAGE
          ::abort();
        }
      }
    }
  };
  return std::visit(ByteConsumer{byte}, std::move(state));
}

struct Scanner {
  ScanState state;

  auto consume(std::uint8_t octet) -> std::expected<std::optional<Token>, ScanError> {
    auto scan_result = ::consume(std::move(state), octet);
    struct ResultProcessor {
      ScanState& state;
      auto operator()(scan_result::Incomplete&& i) -> std::expected<std::optional<Token>, ScanError> {
	state = std::move(i.state);
	return std::nullopt;
      }
      auto operator()(scan_result::Complete&& c) -> std::expected<std::optional<Token>, ScanError> {
	state = std::move(c.state);
	return std::optional {std::move(c.token)};
      }
      auto operator()(ScanError&& e) -> std::expected<std::optional<Token>, ScanError> {
        return std::unexpected(std::move(e));
      }
    };
    return visit(ResultProcessor {state}, std::move(scan_result));
  }
};
