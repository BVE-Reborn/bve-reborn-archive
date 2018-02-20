#include "function_scripts.hpp"
#include <ostream>

namespace parsers {
namespace function_scripts {
	class function_script_parse_tree_printer {
	  private:
		std::ostream& _os;
		std::size_t depth = 0;

		void start_print() {
			for (std::size_t i = 0; i < depth; ++i) {
				_os << "| ";
			}
		}

		void print_next_node(const tree_node& next) {
			depth += 1;
			mapbox::util::apply_visitor(*this, next);
			depth -= 1;
		}

		template <class T>
		void print_binary(const T& node, const char* name) {
			start_print();
			_os << name << '\n';
			print_next_node(node.left);
			print_next_node(node.right);
		}

		template <class T>
		void print_unary(const T& node, const char* name) {
			start_print();
			_os << name << '\n';
			print_next_node(node.child);
		}

	  public:
		explicit function_script_parse_tree_printer(std::ostream& os) : _os(os){};

		void operator()(const tree_types::binary_and& node) {
			print_binary(node, "AND");
		}

		void operator()(const tree_types::binary_xor& node) {
			print_binary(node, "XOR");
		}

		void operator()(const tree_types::binary_or& node) {
			print_binary(node, "OR");
		}

		void operator()(const tree_types::binary_eq& node) {
			print_binary(node, "EQ");
		}

		void operator()(const tree_types::binary_not_eq& node) {
			print_binary(node, "NEQ");
		}

		void operator()(const tree_types::binary_less& node) {
			print_binary(node, "LESS");
		}

		void operator()(const tree_types::binary_greater& node) {
			print_binary(node, "GREATER");
		}

		void operator()(const tree_types::binary_less_eq& node) {
			print_binary(node, "LESS_EQ");
		}

		void operator()(const tree_types::binary_greater_eq& node) {
			print_binary(node, "GREATER_EQ");
		}

		void operator()(const tree_types::binary_add& node) {
			print_binary(node, "ADD");
		}

		void operator()(const tree_types::binary_subtract& node) {
			print_binary(node, "SUB");
		}

		void operator()(const tree_types::binary_multiply& node) {
			print_binary(node, "MULTIPLY");
		}

		void operator()(const tree_types::binary_divide& node) {
			print_binary(node, "DIVIDE");
		}

		void operator()(const tree_types::unary_not& node) {
			print_unary(node, "NOT");
		}

		void operator()(const tree_types::unary_minus& node) {
			print_unary(node, "MINUS");
		}

		void operator()(const tree_types::function_call& node) {
			start_print();
			_os << "FUNC_CALL: " << node.name.val << '\n';
			for (auto& n : node.args) {
				print_next_node(n);
			}
		}
		void operator()(const tree_types::integer& node) {
			start_print();
			_os << node.num << '\n';
		}

		void operator()(const tree_types::floating& node) {
			start_print();
			_os << node.num << '\n';
		}

		void operator()(const tree_types::name& node) {
			start_print();
			_os << "VARIABLE: " << node.val << '\n';
		}

		void operator()(const tree_types::none& node) {
			(void) node;
			start_print();
			_os << "NONE\n";
		}
	};
} // namespace function_scripts
} // namespace parsers

std::ostream& operator<<(std::ostream& os, const parsers::function_scripts::tree_node& node) {
	parsers::function_scripts::function_script_parse_tree_printer fsptp{os};

	mapbox::util::apply_visitor(fsptp, node);

	return os;
}
