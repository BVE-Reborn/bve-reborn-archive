#include "function_scripts.hpp"

namespace parsers {
namespace function_scripts {
	namespace {
		// See function_script_grammar.bnf for the grammar this parser is following
		static tree_node parse_expression(lexer_token_container& list);
		static tree_node parse_equal_expression(lexer_token_container& list);
		static tree_node parse_plus_expression(lexer_token_container& list);
		static tree_node parse_times_expression(lexer_token_container& list);
		static tree_node parse_not_expression(lexer_token_container& list);
		static tree_node parse_function_call_expression(lexer_token_container& list);
		static tree_node parse_term(lexer_token_container& list);

		static tree_node parse_expression(lexer_token_container& list) {
			auto left = parse_equal_expression(list);

			if (list.skip_next_token<lexer_types::ampersand>()) {
				auto right = parse_expression(list);

				return tree_types::binary_and{left, right};
			}
			else if (list.skip_next_token<lexer_types::carret>()) {
				auto right = parse_expression(list);

				return tree_types::binary_xor{left, right};
			}
			else if (list.skip_next_token<lexer_types::bar>()) {
				auto right = parse_expression(list);

				return tree_types::binary_or{left, right};
			}
			else {
				return left;
			}
		}
		static tree_node parse_equal_expression(lexer_token_container& list) {
			auto left = parse_plus_expression(list);

			if (list.skip_next_token<lexer_types::double_eq>()) {
				auto right = parse_expression(list);

				return tree_types::binary_eq{left, right};
			}
			else if (list.skip_next_token<lexer_types::un_eq>()) {
				auto right = parse_expression(list);

				return tree_types::binary_not_eq{left, right};
			}
			else if (list.skip_next_token<lexer_types::less>()) {
				auto right = parse_expression(list);

				return tree_types::binary_less{left, right};
			}
			else if (list.skip_next_token<lexer_types::greater>()) {
				auto right = parse_expression(list);

				return tree_types::binary_greater{left, right};
			}
			else if (list.skip_next_token<lexer_types::less_eq>()) {
				auto right = parse_expression(list);

				return tree_types::binary_less_eq{left, right};
			}
			else if (list.skip_next_token<lexer_types::greater_eq>()) {
				auto right = parse_expression(list);

				return tree_types::binary_greater_eq{left, right};
			}
			else {
				return left;
			}
		}

		static tree_node parse_plus_expression(lexer_token_container& list) {
			auto left = parse_times_expression(list);

			if (list.skip_next_token<lexer_types::plus>()) {
				auto right = parse_expression(list);

				return tree_types::binary_add{left, right};
			}
			else if (list.skip_next_token<lexer_types::minus>()) {
				auto right = parse_expression(list);

				return tree_types::binary_subtract{left, right};
			}
			else {
				return left;
			}
		}

		static tree_node parse_times_expression(lexer_token_container& list) {
			auto left = parse_not_expression(list);

			if (list.skip_next_token<lexer_types::star>()) {
				auto right = parse_expression(list);

				return tree_types::binary_multiply{left, right};
			}
			else if (list.skip_next_token<lexer_types::slash>()) {
				auto right = parse_expression(list);

				return tree_types::binary_divide{left, right};
			}
			else {
				return left;
			}
		}

		static tree_node parse_not_expression(lexer_token_container& list) {
			if (list.skip_next_token<lexer_types::bang>()) {
				auto right = parse_function_call_expression(list);

				return tree_types::unary_not{right};
			}
			else if (list.skip_next_token<lexer_types::minus>()) {
				auto right = parse_function_call_expression(list);

				return tree_types::unary_minus{right};
			}
			else {
				return parse_function_call_expression(list);
			}
		}

		static tree_node parse_function_call_expression(lexer_token_container& list) {
			auto variable = list.get_next_token<lexer_types::variable>();
			if (variable) {
				tree_types::name name_node{variable->name};
				if (list.skip_next_token<lexer_types::l_bracket>()) {
					tree_types::function_call func_node{name_node, {}};
					while (true) {
						func_node.args.emplace_back(parse_expression(list));
						if (!list.skip_next_token<lexer_types::comma>()) {
							break;
						}
					}
					if (!list.skip_next_token<lexer_types::r_bracket>()) {
						// Todo(sirflankalot): Error
					}
					return func_node;
				}
				else {
					return name_node;
				}
			}
			else {
				return parse_term(list);
			}
		}

		static tree_node parse_term(lexer_token_container& list) {
			boost::optional<lexer_types::floating> f;
			boost::optional<lexer_types::integer> i;
			boost::optional<lexer_types::variable> v;

			if (list.skip_next_token<lexer_types::l_paren>()) {
				auto inside = parse_expression(list);

				if (!list.skip_next_token<lexer_types::r_paren>()) {
					// Todo(sirflankalot): error
				}

				return inside;
			}
			else if ((f = list.get_next_token<lexer_types::floating>())) {
				return tree_types::floating{f->val};
			}
			else if ((i = list.get_next_token<lexer_types::integer>())) {
				return tree_types::integer{i->val};
			}
			else if ((v = list.get_next_token<lexer_types::variable>())) {
				return tree_types::name{v->name};
			}
			else {
				// Todo(sirflankalot): error
				return tree_types::none{};
			}
		}
	} // namespace

	tree_node create_tree(const lexer_token_list& list) {
		auto container = lexer_token_container(list);

		return parse_expression(container);
	}
} // namespace function_scripts
} // namespace parsers