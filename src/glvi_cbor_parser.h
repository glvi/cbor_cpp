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
#include "glvi_cbor_scanner.h"
#include "glvi_cbor_value.h"
#include <functional>

/**
   Terminal symbols
 */
using Term = Token;

/**
   Non-terminal symbols
 */
enum class NonTerm {
  Array,
  ArrayXSeq,
  Bstr,
  BstrXSeq,
  Map,
  MapXSeq,
  Tag,
  Tstr,
  TstrXSeq,
  Value,
};

/**
   Vector of kinds of terminal symbols
 */
using vec_kind = std::vector<Kind>;

/**
   Vector of kinds of non-terminal symbols
 */
using vec_nt = std::vector<NonTerm>;

/**
   Things related to errors that may occur during parsing
 */
namespace parse_error {
  /**
     The parser is in an invalid state.

     This is usually the case immediately after construction, and
     before the parser has been properly set up.
   */
  struct Invalid {};

  /**
     The parser is in order, but it needs to see more tokens before it
     can accept or reject its input.
   */
  struct Incomplete {};

  /**
     The parser in its current state has encountered an unexpected
     terminal symbol.
   */
  struct UnexpectedT {
    vec_kind expected;
    Term found;
  };

  /**
     The parser in its current state has encountered an unexpected
     non-terminal symbol. (LR parsers only)
   */
  struct UnexpectedNT {
    vec_nt expected;
    NonTerm found;
  };

  /**
     The parser in its current state encountered unexpected
     circumstances.
   */
  struct Unexpected {
    std::string message;
  };

  /**
     More input than necessary provided to the parser
   */
  struct TrailingInput {};

  /**
     Error propagated from the scanner
   */
  struct Scanner {
    ScanError scanError;
  };

  /**
     Parsing a token would require more stack size than is available;
     refers to context stack size for an LL parser, or state stack
     size for an LR parser.
   */
  struct InsufficientStackSize {};

  /**
     Internal error
  */
  struct Internal {};

  /**
     To do (for development purposes only)
  */
  struct Todo {};

  /**
     What could possibly go wrong with parsing?
   */
  using ParseError =
      std::variant<Invalid, Incomplete, UnexpectedT, UnexpectedNT, Unexpected,
                   TrailingInput, Scanner, InsufficientStackSize, Internal, Todo>;
} // namespace parse_error

/**
   Identifies variants of parse errors.
 */
template <typename T>
concept parse_error_type = std::constructible_from<parse_error::ParseError, T>;

/**
   What could possibly go wrong with parsing?
 */
class ParseError {
  parse_error::ParseError e;

public:
  /**
     Constructs an `invalid` error
   */
  ParseError() = default;

  /**
     Constructs a parse error from the specified parse error variant.
   */
  template <parse_error_type Error>
  ParseError(Error&& error) : e{std::forward<Error>(error)} {
  }

  auto is_invalid                 () const noexcept -> bool;
  auto is_incomplete              () const noexcept -> bool;
  auto is_unexpected_t            () const noexcept -> bool;
  auto is_unexpected_nt           () const noexcept -> bool;
  auto is_unexpected              () const noexcept -> bool;
  auto is_trailing_input          () const noexcept -> bool;
  auto is_scanner                 () const noexcept -> bool;
  auto is_insufficient_stack_size () const noexcept -> bool;
  auto is_internal                () const noexcept -> bool;
  auto is_todo                    () const noexcept -> bool;

  auto& as_unexpected_t  () { return std::get<parse_error::UnexpectedT>(e); }
  auto& as_unexpected_nt () { return std::get<parse_error::UnexpectedNT>(e); }
  auto& as_unexpected    () { return std::get<parse_error::Unexpected>(e); }
  auto& as_scanner       () { return std::get<parse_error::Scanner>(e); }
  auto& as_todo          () { return std::get<parse_error::Todo>(e); }

  auto const& as_unexpected_t  () const { return std::get<parse_error::UnexpectedT>(e); }
  auto const& as_unexpected_nt () const { return std::get<parse_error::UnexpectedNT>(e); }
  auto const& as_unexpected    () const { return std::get<parse_error::Unexpected>(e); }
  auto const& as_scanner       () const { return std::get<parse_error::Scanner>(e); }
  auto const& as_todo          () const { return std::get<parse_error::Todo>(e); }

  template <typename Self, typename Visitor>
  constexpr auto visit(this Self&& self, Visitor&& visitor) {
    return std::visit(std::forward<Visitor>(visitor), std::forward<Self>(self));
  }

  friend auto to_u8string(ParseError const& e) -> std::u8string;
};

namespace parse_state {
  struct ContextStack;
  struct ValueStack;

  namespace context {
    struct Action {
      std::string name;
      std::function<void(ValueStack&, ContextStack&)> fun;
      Action() = delete;
    };
    struct TerminalSymbol{
      Kind kind;
      TerminalSymbol() = delete;
      TerminalSymbol(Kind kind) : kind{kind} {}
    };
    struct NonTerminalSymbol{
      NonTerm nonTerm;
      NonTerminalSymbol() = delete;
      NonTerminalSymbol(NonTerm nonTerm) : nonTerm{nonTerm} {}
    };

    using Context = std::variant<Action, TerminalSymbol, NonTerminalSymbol>;
  }

  template<typename T>
  concept context_type = std::constructible_from<context::Context, T>;

  class Context {
    context::Context c;
  public:
    Context() = delete;

    template <context_type ContextType>
    Context(ContextType&& contextType)
        : c{std::forward<ContextType>(contextType)} {
    }

    auto& __expose() { return c; }

    auto is_action() const noexcept -> bool;
    auto is_terminal_symbol() const noexcept -> bool;
    auto is_terminal_symbol(Kind) const noexcept -> bool;
    auto is_non_terminal_symbol() const noexcept -> bool;
    auto is_non_terminal_symbol(NonTerm) const noexcept -> bool;

    auto& as_action() { return std::get<context::Action>(c); }
    auto& as_terminal_symbol() { return std::get<context::TerminalSymbol>(c); }
    auto& as_non_terminal_symbol() { return std::get<context::NonTerminalSymbol>(c); }

    auto const& as_action() const { return std::get<context::Action>(c); }
    auto const& as_terminal_symbol() const { return std::get<context::TerminalSymbol>(c); }
    auto const& as_non_terminal_symbol() const { return std::get<context::NonTerminalSymbol>(c); }

    template<typename Visitor>
    constexpr auto visit(Visitor&& visitor) && {
      return std::visit(std::forward<Visitor>(visitor), std::move(c));
    }
  };

  struct ContextStack {
    using container_type = std::vector<Context>;
    container_type theStack;
    constexpr auto size() const noexcept { return theStack.size(); }
    auto pop() -> std::optional<Context>;
    void push(Context&&);
  };
  struct ValueStack {
    using container_type = std::vector<CBORValue>;
    container_type theStack;
    constexpr auto size() const noexcept { return theStack.size(); }
    auto pop() -> std::optional<CBORValue>;
    void push(CBORValue&&);
  };
  struct ParseState {
    ContextStack cxtStack;
    ValueStack valStack;
  };
} // namespace parse_state

using parse_state::ParseState;

namespace parse_result {
  struct Incomplete {};
  struct Complete {
    CBORValue value;
  };
  using ParseResult = std::variant<Incomplete, Complete, ParseError>;
} // namespace parse_result

template <typename T>
concept parse_result_type =
    std::constructible_from<parse_result::ParseResult, T>;

class ParseResult {
  parse_result::ParseResult v;

public:
  ParseResult() = delete;

  template <parse_result_type ParseResultType>
  ParseResult(ParseResultType&& parseResult)
      : v{std::forward<ParseResultType>(parseResult)} {
  }

  auto is_incomplete() const noexcept -> bool;
  auto is_complete() const noexcept -> bool;
  auto is_error() const noexcept -> bool;

  auto& as_incomplete() noexcept {
    return std::get<parse_result::Incomplete>(v);
  }
  auto& as_complete() noexcept {
    return std::get<parse_result::Complete>(v);
  }
  auto& as_error() noexcept {
    return std::get<ParseError>(v);
  }

  template <typename Visitor>
  friend constexpr auto visit(Visitor&& visitor, ParseResult&& result) {
    return std::visit(std::forward<Visitor>(visitor), std::move(result.v));
  }
};

class Parser {
  ScanState scanState;
  ParseState parseState;

public:
  auto consume(Term&& term) -> ParseResult;
};
