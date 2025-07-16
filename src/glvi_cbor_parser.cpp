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

namespace {
  template <typename... Ts> struct adhoc : Ts... {
    using Ts::operator()...;
  };
  template <typename... Ts> adhoc(Ts...) -> adhoc<Ts...>;
} // namespace

namespace parse_state {
  static auto do_flush(ValueStack& valStack, ContextStack& cxtStack)
      -> std::expected<void, ParseError>;

  static auto do_flush(ValueStack& valStack, ContextStack& cxtStack,
                       Context&& context) -> std::expected<void, ParseError>;

  static auto do_consume(ValueStack& valStack, ContextStack& cxtStack,
                         Term&& input) -> std::expected<void, ParseError>;

  static auto do_consume(ValueStack& valStack, ContextStack& cxtStack,
                         Context&& context, Term&& input)
      -> std::expected<void, ParseError>;
} // namespace parse_state

static auto make_value(Term&& term) -> std::optional<CBORValue>;

auto Parser::consume(Term&& term) -> ParseResult {
  auto result =
      do_consume(parseState.valStack, parseState.cxtStack, std::move(term));
  if (not result.has_value()) {
    return result.error();
  } else if (parseState.cxtStack.size() > 0) {
    return parse_result::Incomplete{};
  } else if (parseState.valStack.size() > 1) {
    return parse_error::Internal{};
  } else if (auto opt_value = parseState.valStack.pop()) {
    return parse_result::Complete{*std::move(opt_value)};
  } else {
    return parse_error::Invalid{};
  }
}

static auto parse_state::do_flush(ValueStack& valStack, ContextStack& cxtStack)
    -> std::expected<void, ParseError> {
  if (auto opt_context = cxtStack.pop()) {
    return do_flush(valStack, cxtStack, *std::move(opt_context));
  } else {
    return {};
  }
}

static auto parse_state::do_flush(ValueStack& valStack, ContextStack& cxtStack,
                                  Context&& context)
    -> std::expected<void, ParseError> {
  using namespace context;
  return std::move(context).visit(adhoc{
      [&](Action&& arg) -> std::expected<void, ParseError> {
        auto&& [name, action] = std::move(arg);
        std::invoke(std::move(action), valStack, cxtStack);
        return do_flush(valStack, cxtStack);
      },
      [&](auto&& arg) -> std::expected<void, ParseError> {
        cxtStack.push(std::move(arg));
        return {};
      },
  });
}

static auto parse_state::do_consume(ValueStack& valStack,
                                    ContextStack& cxtStack, Term&& input)
    -> std::expected<void, ParseError> {
  if (auto opt_context = cxtStack.pop()) {
    return do_consume(valStack, cxtStack, *std::move(opt_context),
                      std::move(input));
  } else {
    return std::unexpected(parse_error::TrailingInput{});
  }
}

static auto parse_state::do_consume(ValueStack& valStack,
                                    ContextStack& cxtStack, Context&& context,
                                    Term&& input)
    -> std::expected<void, ParseError> {
  if (context.is_action()) {
    auto [name, action] = std::move(context.as_action());
    std::invoke(std::move(action), valStack, cxtStack);
    return do_consume(valStack, cxtStack, std::move(input));
  } else if (context.is_terminal_symbol(input.kind())) {
    if (auto optValue = make_value(std::move(input))) {
      valStack.push(*std::move(optValue));
    }
    return do_flush(valStack, cxtStack);
  } else if (context.is_terminal_symbol()) {
    auto [kind] = context.as_terminal_symbol();
    cxtStack.push(context::TerminalSymbol{kind});
    return std::unexpected(parse_error::UnexpectedT{{kind}, input});
  } else if (context.is_non_terminal_symbol(NonTerm::Value)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::Array)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::ArrayXSeq)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::Bstr)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::BstrXSeq)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::Map)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::MapXSeq)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::Tag)) {
    // TODO:
  } else if (context.is_non_terminal_symbol(NonTerm::Tstr)) {
    // TODO:e
  } else if (context.is_non_terminal_symbol(NonTerm::TstrXSeq)) {
    // TODO:
  }
  return std::unexpected(parse_error::Internal{});
}

auto parse_state::ContextStack::pop() -> std::optional<Context> {
  if (theStack.empty())
    return {};
  Context cxt{std::move(theStack.back())};
  theStack.pop_back();
  return cxt;
}

static auto make_value(Term&& term) -> std::optional<CBORValue> {
  return std::move(term).visit(adhoc{
      [](token::Uint&& uint) -> std::optional<CBORValue> {
	return CBORUint { CBOR_U64 { uint.value } };
      },
      [](token::Nint&& nint) -> std::optional<CBORValue> {
	return CBORNint { CBOR_U64 { nint.value } };
      },
      [](token::BstrX&&) -> std::optional<CBORValue> {
	return CBORBstr {};
      },
      [](token::Bstr&& bstr) -> std::optional<CBORValue> {
	return CBORBstr {bstr.value};
      },
      [](token::TstrX&&) -> std::optional<CBORValue> {
	return CBORTstr {};
      },
      [](token::Tstr&& tstr) -> std::optional<CBORValue> {
	return CBORTstr {tstr.value};
      },
      [](token::ArrayX&&) -> std::optional<CBORValue> {
	return CBORArray {};
      },
      [](token::MapX&&) -> std::optional<CBORValue> {
	return CBORMap {};
      },
      [](token::Simple&& simple) -> std::optional<CBORValue> {
	return CBORSimple { simple.value };
      },
      [](token::Float&& f) -> std::optional<CBORValue> {
	// TODO:
	return CBORFloat { };
      },
      [](auto&&) -> std::optional<CBORValue> { return {}; },
  });
}
