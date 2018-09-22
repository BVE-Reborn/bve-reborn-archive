#include "parse_tree.hpp"
#include <sstream>
#include <tl/optional.hpp>

namespace bve::parsers::function_scripts {
	namespace {
		// See function_script_grammar.bnf for the grammar this parser is
		// following
		TreeNode parse_expression(LexerTokenProvider& list);
		TreeNode parse_xor_expression(LexerTokenProvider& list);
		TreeNode parse_and_expression(LexerTokenProvider& list);
		TreeNode parse_not_expression(LexerTokenProvider& list);
		TreeNode parse_equal_expression(LexerTokenProvider& list);
		TreeNode parse_plus_expression(LexerTokenProvider& list);
		TreeNode parse_times_expression(LexerTokenProvider& list);
		TreeNode parse_divide_expression(LexerTokenProvider& list);
		TreeNode parse_minus_expression(LexerTokenProvider& list);
		TreeNode parse_function_call_expression(LexerTokenProvider& list);
		TreeNode parse_term(LexerTokenProvider& list);

		TreeNode parse_expression(LexerTokenProvider& list) {
			auto left = parse_xor_expression(list);

			if (list.skipNextToken<lexer_types::Bar>()) {
				auto const right = parse_expression(list);

				return tree_types::BinaryOr{left, right};
			}

			return left;
		}

		TreeNode parse_xor_expression(LexerTokenProvider& list) {
			auto left = parse_and_expression(list);

			if (list.skipNextToken<lexer_types::Caret>()) {
				auto const right = parse_xor_expression(list);

				return tree_types::BinaryXor{left, right};
			}

			return left;
		}

		TreeNode parse_and_expression(LexerTokenProvider& list) {
			auto left = parse_not_expression(list);

			if (list.skipNextToken<lexer_types::Ampersand>()) {
				auto const right = parse_and_expression(list);

				return tree_types::BinaryAnd{left, right};
			}

			return left;
		}

		TreeNode parse_not_expression(LexerTokenProvider& list) {
			if (list.skipNextToken<lexer_types::Bang>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::UnaryNot{right};
			}

			return parse_equal_expression(list);
		}

		TreeNode parse_equal_expression(LexerTokenProvider& list) {
			auto left = parse_plus_expression(list);

			if (list.skipNextToken<lexer_types::DoubleEq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::BinaryEq{left, right};
			}
			if (list.skipNextToken<lexer_types::UnEq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::BinaryNotEq{left, right};
			}
			if (list.skipNextToken<lexer_types::Less>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::BinaryLess{left, right};
			}
			if (list.skipNextToken<lexer_types::Greater>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::BinaryGreater{left, right};
			}
			if (list.skipNextToken<lexer_types::LessEq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::BinaryLessEq{left, right};
			}
			if (list.skipNextToken<lexer_types::GreaterEq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::BinaryGreaterEq{left, right};
			}

			return left;
		}

		TreeNode parse_plus_expression(LexerTokenProvider& list) {
			auto left = parse_times_expression(list);

			if (list.skipNextToken<lexer_types::Plus>()) {
				auto const right = parse_plus_expression(list);

				return tree_types::BinaryAdd{left, right};
			}
			if (list.skipNextToken<lexer_types::Minus>()) {
				auto const right = parse_plus_expression(list);

				return tree_types::BinarySubtract{left, right};
			}

			return left;
		}

		TreeNode parse_times_expression(LexerTokenProvider& list) {
			auto left = parse_divide_expression(list);

			if (list.skipNextToken<lexer_types::Star>()) {
				auto const right = parse_times_expression(list);

				return tree_types::BinaryMultiply{left, right};
			}

			return left;
		}

		TreeNode parse_divide_expression(LexerTokenProvider& list) {
			auto left = parse_minus_expression(list);

			if (list.skipNextToken<lexer_types::Slash>()) {
				auto const right = parse_divide_expression(list);

				return tree_types::BinaryDivide{left, right};
			}

			return left;
		}

		TreeNode parse_minus_expression(LexerTokenProvider& list) {
			if (list.skipNextToken<lexer_types::Minus>()) {
				auto const right = parse_function_call_expression(list);

				return tree_types::UnaryMinus{right};
			}

			return parse_function_call_expression(list);
		}

		TreeNode parse_function_call_expression(LexerTokenProvider& list) {
			auto variable = list.getNextToken<lexer_types::Variable>();
			if (variable) {
				tree_types::Identifier name_node{variable->name};
				if (list.skipNextToken<lexer_types::LBracket>()) {
					tree_types::FunctionCall func_node{name_node, {}};
					while (true) {
						func_node.args.emplace_back(parse_expression(list));
						if (!list.skipNextToken<lexer_types::Comma>()) {
							break;
						}
					}
					if (!list.skipNextToken<lexer_types::RBracket>()) {
						list.addError({0, "Missing right bracket"});
					}
					return func_node;
				}

				return name_node;
			}

			return parse_term(list);
		}

		TreeNode parse_term(LexerTokenProvider& list) {
			tl::optional<lexer_types::Floating> f;
			tl::optional<lexer_types::Integer> i;

			if (list.skipNextToken<lexer_types::LParen>()) {
				auto inside = parse_expression(list);

				if (!list.skipNextToken<lexer_types::RParen>()) {
					list.addError({0, "Missing right parenthesis"});
				}

				return inside;
			}
			if ((f = list.getNextToken<lexer_types::Floating>())) {
				return tree_types::Floating{f->val};
			}
			if ((i = list.getNextToken<lexer_types::Integer>())) {
				return tree_types::Integer{i->val};
			}

			auto next_token = list.peakNextToken();

			std::ostringstream err;
			if (next_token) {
				::operator<<(err << "Unexpected ", *next_token);
			}
			else {
				err << "Unexpected end of function script";
			}
			list.addError({0, err.str()});
			return tree_types::None{};
		}
	} // namespace

	TreeNode create_tree(const LexerTokenList& list, errors::Errors& errors) {
		auto container = LexerTokenProvider(list, errors);

		auto ret_tree = parse_expression(container);

		tl::optional<LexerToken> next_token;
		while ((next_token = container.peakNextToken())) {
			std::ostringstream err;

			::operator<<(err << "Unexpected ", *next_token) << ", expected end of function script";
			add_error(errors, 0, err);

			container.advanceOneToken();
		}
		return ret_tree;
	}
} // namespace bve::parsers::function_scripts
