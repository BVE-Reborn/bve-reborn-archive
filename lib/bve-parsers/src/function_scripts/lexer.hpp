#pragma once

#include "parsers/errors.hpp"
#include "parsers/function_scripts.hpp"
#include <iosfwd>
#include <mapbox/recursive_wrapper.hpp>
#include <mapbox/variant.hpp>
#include <string>
#include <absl/types/optional.h>
#include <vector>

namespace bve::parsers::function_scripts {
	namespace lexer_types {
		struct Plus {};  // +
		struct Minus {}; // -
		struct Star {};  // *
		struct Slash {}; // /

		struct DoubleEq {};  // ==
		struct UnEq {};      // !=
		struct Less {};      // <
		struct Greater {};   // >
		struct LessEq {};    // <=
		struct GreaterEq {}; // >=

		struct Bang {};      // !
		struct Ampersand {}; // &
		struct Bar {};       // |
		struct Caret {};     // ^

		struct LParen {};   // (
		struct RParen {};   // )
		struct LBracket {}; // {
		struct RBracket {}; // }

		struct Comma {}; // ,
		struct Dot {};   // .

		struct Variable { // BLAH
			std::string name;
		};

		struct Integer {
			std::intmax_t val;
		};

		struct Floating {
			float val;
		};

	} // namespace lexer_types

	using LexerToken = mapbox::util::variant<lexer_types::Plus,
	                                         lexer_types::Minus,
	                                         lexer_types::Star,
	                                         lexer_types::Slash,
	                                         lexer_types::DoubleEq,
	                                         lexer_types::UnEq,
	                                         lexer_types::Less,
	                                         lexer_types::Greater,
	                                         lexer_types::LessEq,
	                                         lexer_types::GreaterEq,
	                                         lexer_types::Bang,
	                                         lexer_types::Ampersand,
	                                         lexer_types::Bar,
	                                         lexer_types::Caret,
	                                         lexer_types::LParen,
	                                         lexer_types::RParen,
	                                         lexer_types::LBracket,
	                                         lexer_types::RBracket,
	                                         lexer_types::Comma,
	                                         lexer_types::Dot,
	                                         lexer_types::Variable,
	                                         lexer_types::Integer,
	                                         lexer_types::Floating>;
	using LexerTokenList = std::vector<LexerToken>;

	class LexerTokenProvider {
	  private:
		const LexerTokenList& list_;
		errors::Errors& err_;
		std::size_t index_ = 0;

	  public:
		LexerTokenProvider(const LexerTokenList& contains, errors::Errors& errors) :
		    list_(contains),
		    err_(errors) {}

		template <class T>
		absl::optional<T> getNextToken() {
			if (index_ < list_.size() && list_[index_].is<T>()) {
				auto ret = list_[index_].get<T>();
				index_ += 1;
				return ret;
			}
			return absl::nullopt;
		}

		template <class T>
		bool skipNextToken() {
			if (index_ < list_.size() && list_[index_].is<T>()) {
				index_ += 1;
				return true;
			}
			return false;
		}

		template <class T>
		bool isNextToken() const {
			return index_ < list_.size() && list_[index_].is<T>();
		}

		void advanceOneToken() {
			if (index_ < list_.size()) {
				index_ += 1;
			}
		}

		absl::optional<LexerToken> peakNextToken() const {
			return index_ < list_.size() ? absl::make_optional(list_[index_]) : absl::nullopt;
		}

		void addError(const errors::Error& error) const {
			err_.emplace_back(error);
		}
	};
	LexerTokenList lex(const std::string& text, errors::Errors& errors);
} // namespace bve::parsers::function_scripts

std::ostream& operator<<(std::ostream& os, const bve::parsers::function_scripts::LexerToken& lt);