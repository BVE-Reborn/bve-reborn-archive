#pragma once

#include "parsers/errors.hpp"
#include "parsers/function_scripts.hpp"
#include <boost/optional/optional.hpp>
#include <iosfwd>
#include <mapbox/recursive_wrapper.hpp>
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace parsers {
namespace function_scripts {
	namespace lexer_types {
		struct plus {};  // +
		struct minus {}; // -
		struct star {};  // *
		struct slash {}; // /

		struct double_eq {};  // ==
		struct un_eq {};      // !=
		struct less {};       // <
		struct greater {};    // >
		struct less_eq {};    // <=
		struct greater_eq {}; // >=

		struct bang {};      // !
		struct ampersand {}; // &
		struct bar {};       // |
		struct carret {};    // ^

		struct l_paren {};   // (
		struct r_paren {};   // )
		struct l_bracket {}; // {
		struct r_bracket {}; // }

		struct comma {}; // ,

		struct variable { // BLAH
			std::string name;
		};

		struct integer {
			std::intmax_t val;
		};

		struct floating {
			float val;
		};

	} // namespace lexer_types

	using lexer_token =
	    mapbox::util::variant<lexer_types::plus, lexer_types::minus, lexer_types::star, lexer_types::slash,
	                          lexer_types::double_eq, lexer_types::un_eq, lexer_types::less, lexer_types::greater,
	                          lexer_types::less_eq, lexer_types::greater_eq, lexer_types::bang, lexer_types::ampersand,
	                          lexer_types::bar, lexer_types::carret, lexer_types::l_paren, lexer_types::r_paren,
	                          lexer_types::l_bracket, lexer_types::r_bracket, lexer_types::comma, lexer_types::variable,
	                          lexer_types::integer, lexer_types::floating>;
	using lexer_token_list = std::vector<lexer_token>;

	class lexer_token_container {
	  private:
		const lexer_token_list& list;
		errors::errors_t& err;
		std::size_t index = 0;

	  public:
		lexer_token_container(const lexer_token_list& contains, errors::errors_t& errors)
		    : list(contains), err(errors){};

		template <class T>
		boost::optional<T> get_next_token() {
			if (index < list.size() && list[index].is<T>()) {
				auto ret = list[index].get<T>();
				index += 1;
				return ret;
			}
			return boost::none;
		}

		template <class T>
		bool skip_next_token() {
			if (index < list.size() && list[index].is<T>()) {
				index += 1;
				return true;
			}
			return false;
		}

		template <class T>
		bool is_next_token() {
			return index < list.size() && list[index].is<T>();
		}

		boost::optional<lexer_token> peak_next_token() {
			return index < list.size() ? boost::make_optional(list[index]) : boost::none;
		}

		void add_error(const errors::error_t& error) {
			err.emplace_back(error);
		}
	};

	namespace tree_types {
		// forward declare all structures that have nodes in them
		// recursive_wrapper transparently allocates
		struct binary_and;
		struct binary_xor;
		struct binary_or;
		struct binary_eq;
		struct binary_not_eq;
		struct binary_less;
		struct binary_greater;
		struct binary_less_eq;
		struct binary_greater_eq;
		struct binary_add;
		struct binary_subtract;
		struct binary_multiply;
		struct binary_divide;
		struct unary_not;
		struct unary_minus;
		struct function_call;
		struct integer {
			std::intmax_t num;
		};
		struct floating {
			float num;
		};
		struct name {
			std::string val;
		};

		struct none {};
	} // namespace tree_types

	using tree_node =
	    mapbox::util::variant<mapbox::util::recursive_wrapper<tree_types::binary_and>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_xor>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_or>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_eq>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_not_eq>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_less>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_greater>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_less_eq>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_greater_eq>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_add>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_subtract>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_multiply>,
	                          mapbox::util::recursive_wrapper<tree_types::binary_divide>,
	                          mapbox::util::recursive_wrapper<tree_types::unary_not>,
	                          mapbox::util::recursive_wrapper<tree_types::unary_minus>,
	                          mapbox::util::recursive_wrapper<tree_types::function_call>, tree_types::integer,
	                          tree_types::floating, tree_types::name, tree_types::none>;

	namespace tree_types {
		struct binary_and {
			tree_node left;
			tree_node right;
		};

		struct binary_xor {
			tree_node left;
			tree_node right;
		};

		struct binary_or {
			tree_node left;
			tree_node right;
		};

		struct binary_eq {
			tree_node left;
			tree_node right;
		};

		struct binary_not_eq {
			tree_node left;
			tree_node right;
		};

		struct binary_less {
			tree_node left;
			tree_node right;
		};

		struct binary_greater {
			tree_node left;
			tree_node right;
		};

		struct binary_less_eq {
			tree_node left;
			tree_node right;
		};

		struct binary_greater_eq {
			tree_node left;
			tree_node right;
		};

		struct binary_add {
			tree_node left;
			tree_node right;
		};

		struct binary_subtract {
			tree_node left;
			tree_node right;
		};

		struct binary_multiply {
			tree_node left;
			tree_node right;
		};

		struct binary_divide {
			tree_node left;
			tree_node right;
		};

		struct unary_not {
			tree_node child;
		};
		struct unary_minus {
			tree_node child;
		};

		struct function_call {
			tree_types::name name;
			std::vector<tree_node> args;
		};
	} // namespace tree_types

	lexer_token_list lex(const std::string& text, errors::errors_t& errors);
	tree_node create_tree(const lexer_token_list& list, errors::errors_t& errors);
	instruction_list build_instructions(const tree_node& head_node, const errors::errors_t& errors = {});
	instruction_list parse(const std::string& text);
} // namespace function_scripts
} // namespace parsers

std::ostream& operator<<(std::ostream& os, const parsers::function_scripts::lexer_token& lt);
std::ostream& operator<<(std::ostream& os, const parsers::function_scripts::lexer_token_list& list);
std::ostream& operator<<(std::ostream& os, const parsers::function_scripts::tree_node& node);
