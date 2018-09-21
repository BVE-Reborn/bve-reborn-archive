#pragma once

#include "parsers/errors.hpp"
#include "parsers/function_scripts.hpp"
#include <iosfwd>
#include <mapbox/recursive_wrapper.hpp>
#include <mapbox/variant.hpp>
#include <string>
#include <tl/optional.hpp>
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
		tl::optional<T> getNextToken() {
			if (index_ < list_.size() && list_[index_].is<T>()) {
				auto ret = list_[index_].get<T>();
				index_ += 1;
				return ret;
			}
			return tl::nullopt;
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

		tl::optional<LexerToken> peakNextToken() const {
			return index_ < list_.size() ? tl::make_optional(list_[index_]) : tl::nullopt;
		}

		void addError(const errors::Error& error) const {
			err_.emplace_back(error);
		}
	};

	namespace tree_types {
		// forward declare all structures that have nodes in them
		// recursive_wrapper transparently allocates
		struct BinaryAnd;
		struct BinaryXor;
		struct BinaryOr;
		struct BinaryEq;
		struct BinaryNotEq;
		struct BinaryLess;
		struct BinaryGreater;
		struct BinaryLessEq;
		struct BinaryGreaterEq;
		struct BinaryAdd;
		struct BinarySubtract;
		struct BinaryMultiply;
		struct BinaryDivide;
		struct UnaryNot;
		struct UnaryMinus;
		struct FunctionCall;
		struct Integer {
			std::intmax_t num;
		};
		struct Floating {
			float num;
		};
		struct Identifier {
			std::string val;
		};

		struct None {};
	} // namespace tree_types

	using TreeNode =
	    mapbox::util::variant<mapbox::util::recursive_wrapper<tree_types::BinaryAnd>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryXor>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryOr>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryNotEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryLess>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryGreater>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryLessEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryGreaterEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryAdd>,
	                          mapbox::util::recursive_wrapper<tree_types::BinarySubtract>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryMultiply>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryDivide>,
	                          mapbox::util::recursive_wrapper<tree_types::UnaryNot>,
	                          mapbox::util::recursive_wrapper<tree_types::UnaryMinus>,
	                          mapbox::util::recursive_wrapper<tree_types::FunctionCall>,
	                          tree_types::Integer,
	                          tree_types::Floating,
	                          tree_types::Identifier,
	                          tree_types::None>;

	namespace tree_types {
		struct BinaryAnd {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryXor {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryOr {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryNotEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryLess {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryGreater {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryLessEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryGreaterEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryAdd {
			TreeNode left;
			TreeNode right;
		};

		struct BinarySubtract {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryMultiply {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryDivide {
			TreeNode left;
			TreeNode right;
		};

		struct UnaryNot {
			TreeNode child;
		};
		struct UnaryMinus {
			TreeNode child;
		};

		struct FunctionCall {
			Identifier name;
			std::vector<TreeNode> args;
		};
	} // namespace tree_types

	LexerTokenList lex(const std::string& text, errors::Errors& errors);
	TreeNode create_tree(const LexerTokenList& list, errors::Errors& errors);
	InstructionList build_instructions(const TreeNode& head_node,
	                                   const errors::Errors& errors = {});
} // namespace bve::parsers::function_scripts

std::ostream& operator<<(std::ostream& os, const bve::parsers::function_scripts::LexerToken& lt);
std::ostream& operator<<(std::ostream& os,
                         const bve::parsers::function_scripts::LexerTokenList& list);
std::ostream& operator<<(std::ostream& os, const bve::parsers::function_scripts::TreeNode& node);
