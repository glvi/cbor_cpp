[+ autogen5 template h cpp +]
//  -*- mode: c++; coding: utf-8-unix; -*-
//
[+(dne "// ")+]
//
// cbor: Utilities for decoding Concise Binary Object Representation
// Copyright (C) 2025 GLVI Gesellschaft für Luftverkehrsinformatik mbH.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
[+ CASE (suffix) +][+ == h +]#pragma once
#include <cstdint>
#include <expected>
#include <string>
#include <utility>
#include <vector>
#include <variant>

enum class Kind {[+ FOR token "," +]
  /// [+ comment +]
  [+ kind +][+ ENDFOR token +]
};

/// Structured representation of syntax tokens, a.k.a. terminal symbols
namespace token {
  [+ FOR token +]
  /// [+ comment +]
  struct [+ kind +] {[+ IF value_type +]
    [+ value_type +] value;
    [+ ENDIF +]
    friend constexpr auto kind([+kind+]) noexcept -> Kind { return Kind::[+kind+]; }
  };
  [+ ENDFOR token +]
  /// Syntax token, a.k.a. terminal symbol
  using Token = std::variant<[+ FOR token ", " +][+ kind +][+ ENDFOR token +]>;

  constexpr auto kind(Token token) noexcept {
    return std::visit([](auto const& x) {
      return kind(x);
    }, token);
  }

  inline namespace error {[+ FOR token +]
    struct Not[+ kind +] {}; [+ ENDFOR token +]
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
  static auto make(Kind kind, std::uint64_t argument, std::vector<std::byte> payload) -> Token;

  template <token_type TokenType>
  Token(TokenType token) : token{std::move(token)} {
  }

  Token() = delete;

  constexpr auto kind() const noexcept -> Kind {
    return token::kind(token);
  }
  [+ FOR token +]
  constexpr auto is_[+ (string-downcase! (get "kind")) +]() noexcept -> bool {
    return holds<token::[+ kind +]>();
  }
  [+ ENDFOR token +][+ FOR token +][+ IF value_type +]
  constexpr auto as_[+(string-downcase! (get "kind"))+]() -> std::expected<[+value_type+], token::error::Not[+kind+]> {
    if (is_[+(string-downcase! (get "kind"))+]()) return get<token::[+kind+]>().value;
    return std::unexpected(token::error::Not[+kind+]{});
  }
  [+ ENDIF +][+ ENDFOR token +]
  template<typename Visitor>
  constexpr auto visit(Visitor&& visitor) && {
    return std::visit(std::forward<Visitor>(visitor), std::move(token));
  }

  template<typename Visitor>
  constexpr auto visit(Visitor&& visitor) const& {
    return std::visit(std::forward<Visitor>(visitor), token);
  }
};

[+ == cpp +]#include "glvi_cbor_token.h"

static_assert(not std::is_nothrow_default_constructible_v<Token>);
static_assert(std::is_nothrow_move_constructible_v<Token>);
static_assert(std::is_copy_constructible_v<Token>);
static_assert(std::is_nothrow_move_assignable_v<Token>);
static_assert(std::is_copy_assignable_v<Token>);

auto Token::make(Kind kind, std::uint64_t argument, std::vector<std::byte> payload) -> Token {
  switch (kind) {
  [+ FOR token +][+
  CASE value_type +]
  [+ == "std::uint64_t" +]case Kind::[+kind+]: return token::[+kind+]{argument};
  [+ == "std::uint8_t" +]case Kind::[+kind+]: return token::[+kind+]{static_cast<std::uint8_t>(argument)};
  [+ == "std::u8string" +]case Kind::[+kind+]: return token::[+kind+]{std::u8string{(char8_t*)payload.data(), payload.size()}};
  [+ ~~* "std::vector" +]case Kind::[+kind+]: return token::[+kind+]{std::move(payload)};
  [+ !E +]case Kind::[+kind+]: return token::[+kind+]{};
  [+ ESAC +][+ ENDFOR token +]}
}
[+ ESAC +]