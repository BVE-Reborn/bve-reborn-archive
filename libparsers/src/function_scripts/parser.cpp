#include "function_scripts.hpp"
#include <sstream>

namespace parsers {
namespace function_scripts {
	namespace {
		// See function_script_grammar.bnf for the grammar this parser is
		// following
		tree_node parse_expression(lexer_token_container& list);
		tree_node parse_xor_expression(lexer_token_container& list);
		tree_node parse_and_expression(lexer_token_container& list);
		tree_node parse_not_expression(lexer_token_container& list);
		tree_node parse_equal_expression(lexer_token_container& list);
		tree_node parse_plus_expression(lexer_token_container& list);
		tree_node parse_times_expression(lexer_token_container& list);
		tree_node parse_divide_expression(lexer_token_container& list);
		tree_node parse_minus_expression(lexer_token_container& list);
		tree_node parse_function_call_expression(lexer_token_container& list);
		tree_node parse_term(lexer_token_container& list);

		tree_node parse_expression(lexer_token_container& list) {
			auto left = parse_xor_expression(list);

			if (list.skip_next_token<lexer_types::bar>()) {
				auto const right = parse_expression(list);

				return tree_types::binary_or{left, right};
			}

			return left;
		}

		tree_node parse_xor_expression(lexer_token_container& list) {
			auto left = parse_and_expression(list);

			if (list.skip_next_token<lexer_types::carret>()) {
				auto const right = parse_xor_expression(list);

				return tree_types::binary_xor{left, right};
			}

			return left;
		}

		tree_node parse_and_expression(lexer_token_container& list) {
			auto left = parse_not_expression(list);

			if (list.skip_next_token<lexer_types::ampersand>()) {
				auto const right = parse_and_expression(list);

				return tree_types::binary_and{left, right};
			}

			return left;
		}

		tree_node parse_not_expression(lexer_token_container& list) {
			if (list.skip_next_token<lexer_types::bang>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::unary_not{right};
			}

			return parse_equal_expression(list);
		}

		tree_node parse_equal_expression(lexer_token_container& list) {
			auto left = parse_plus_expression(list);

			if (list.skip_next_token<lexer_types::double_eq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::binary_eq{left, right};
			}
			if (list.skip_next_token<lexer_types::un_eq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::binary_not_eq{left, right};
			}
			if (list.skip_next_token<lexer_types::less>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::binary_less{left, right};
			}
			if (list.skip_next_token<lexer_types::greater>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::binary_greater{left, right};
			}
			if (list.skip_next_token<lexer_types::less_eq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::binary_less_eq{left, right};
			}
			if (list.skip_next_token<lexer_types::greater_eq>()) {
				auto const right = parse_equal_expression(list);

				return tree_types::binary_greater_eq{left, right};
			}

			return left;
		}

		tree_node parse_plus_expression(lexer_token_container& list) {
			auto left = parse_times_expression(list);

			if (list.skip_next_token<lexer_types::plus>()) {
				auto const right = parse_plus_expression(list);

				return tree_types::binary_add{left, right};
			}
			if (list.skip_next_token<lexer_types::minus>()) {
				auto const right = parse_plus_expression(list);

				return tree_types::binary_subtract{left, right};
			}

			return left;
		}

		tree_node parse_times_expression(lexer_token_container& list) {
			auto left = parse_divide_expression(list);

			if (list.skip_next_token<lexer_types::star>()) {
				auto const right = parse_times_expression(list);

				return tree_types::binary_multiply{left, right};
			}

			return left;
		}

		tree_node parse_divide_expression(lexer_token_container& list) {
			auto left = parse_minus_expression(list);

			if (list.skip_next_token<lexer_types::slash>()) {
				auto const right = parse_divide_expression(list);

				return tree_types::binary_divide{left, right};
			}

			return left;
		}

		tree_node parse_minus_expression(lexer_token_container& list) {
			if (list.skip_next_token<lexer_types::minus>()) {
				auto const right = parse_function_call_expression(list);

				return tree_types::unary_minus{right};
			}

			return parse_function_call_expression(list);
		}

		tree_node parse_function_call_expression(lexer_token_container& list) {
			auto variable = list.get_next_token<lexer_types::variable>();
			if (variable) {
				tree_types::identifier name_node{variable->name};
				if (list.skip_next_token<lexer_types::l_bracket>()) {
					tree_types::function_call func_node{name_node, {}};
					while (true) {
						func_node.args.emplace_back(parse_expression(list));
						if (!list.skip_next_token<lexer_types::comma>()) {
							break;
						}
					}
					if (!list.skip_next_token<lexer_types::r_bracket>()) {
						list.add_error({0, "Missing right bracket"});
					}
					return func_node;
				}

				return name_node;
			}

			return parse_term(list);
		}

		tree_node parse_term(lexer_token_container& list) {
			boost::optional<lexer_types::floating> f;
			boost::optional<lexer_types::integer> i;

			if (list.skip_next_token<lexer_types::l_paren>()) {
				auto inside = parse_expression(list);

				if (!list.skip_next_token<lexer_types::r_paren>()) {
					list.add_error({0, "Missing right parethesis"});
				}

				return inside;
			}
			if ((f = list.get_next_token<lexer_types::floating>())) {
				return tree_types::floating{f->val};
			}
			if ((i = list.get_next_token<lexer_types::integer>())) {
				return tree_types::integer{i->val};
			}

			auto next_token = list.peak_next_token();

			std::ostringstream err;
			if (next_token) {
				::operator<<(err << "Unexpected ", *next_token);
			}
			else {
				err << "Unexpected end of function script";
			}
			list.add_error({0, err.str()});
			return tree_types::none{};
		}
	} // namespace

	tree_node create_tree(const lexer_token_list& list, errors::errors_t& errors) {
		auto container = lexer_token_container(list, errors);

		auto ret_tree = parse_expression(container);

		boost::optional<lexer_token> next_token;
		while ((next_token = container.peak_next_token())) {
			std::ostringstream err;

			::operator<<(err << "Unexpected ", *next_token) << ", expected end of function script";
			add_error(errors, 0, err);

			container.advance_one_token();
		}
		return ret_tree;
	}
} // namespace function_scripts
} // namespace parsers
