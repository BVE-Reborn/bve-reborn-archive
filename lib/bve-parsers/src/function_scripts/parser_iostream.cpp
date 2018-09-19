#include "function_scripts.hpp"
#include <ostream>

namespace bve::parsers::function_scripts {
	class FunctionScriptParseTreePrinter {
	  private:
		std::ostream& os_;
		std::size_t depth_ = 0;

		void startPrint() const {
			for (std::size_t i = 0; i < depth_; ++i) {
				os_ << "| ";
			}
		}

		void printNextNode(const tree_node& next) {
			depth_ += 1;
			apply_visitor(*this, next);
			depth_ -= 1;
		}

		template <class T>
		void printBinary(const T& node, const char* name) {
			startPrint();
			os_ << name << '\n';
			printNextNode(node.left);
			printNextNode(node.right);
		}

		template <class T>
		void printUnary(const T& node, const char* name) {
			startPrint();
			os_ << name << '\n';
			printNextNode(node.child);
		}

	  public:
		explicit FunctionScriptParseTreePrinter(std::ostream& os) : os_(os) {}

		void operator()(const tree_types::binary_and& node) {
			printBinary(node, "AND");
		}

		void operator()(const tree_types::binary_xor& node) {
			printBinary(node, "XOR");
		}

		void operator()(const tree_types::binary_or& node) {
			printBinary(node, "OR");
		}

		void operator()(const tree_types::binary_eq& node) {
			printBinary(node, "EQ");
		}

		void operator()(const tree_types::binary_not_eq& node) {
			printBinary(node, "NEQ");
		}

		void operator()(const tree_types::binary_less& node) {
			printBinary(node, "LESS");
		}

		void operator()(const tree_types::binary_greater& node) {
			printBinary(node, "GREATER");
		}

		void operator()(const tree_types::binary_less_eq& node) {
			printBinary(node, "LESS_EQ");
		}

		void operator()(const tree_types::binary_greater_eq& node) {
			printBinary(node, "GREATER_EQ");
		}

		void operator()(const tree_types::binary_add& node) {
			printBinary(node, "ADD");
		}

		void operator()(const tree_types::binary_subtract& node) {
			printBinary(node, "SUB");
		}

		void operator()(const tree_types::binary_multiply& node) {
			printBinary(node, "MULTIPLY");
		}

		void operator()(const tree_types::binary_divide& node) {
			printBinary(node, "DIVIDE");
		}

		void operator()(const tree_types::unary_not& node) {
			printUnary(node, "NOT");
		}

		void operator()(const tree_types::unary_minus& node) {
			printUnary(node, "MINUS");
		}

		void operator()(const tree_types::function_call& node) {
			startPrint();
			os_ << "FUNC_CALL: " << node.name.val << '\n';
			for (auto& n : node.args) {
				printNextNode(n);
			}
		}
		void operator()(const tree_types::integer& node) const {
			startPrint();
			os_ << node.num << '\n';
		}

		void operator()(const tree_types::floating& node) const {
			startPrint();
			os_ << node.num << '\n';
		}

		void operator()(const tree_types::identifier& node) const {
			startPrint();
			os_ << "VARIABLE: " << node.val << '\n';
		}

		void operator()(const tree_types::none& node) const {
			(void) node;
			startPrint();
			os_ << "NONE\n";
		}
	};
} // namespace bve::parsers::function_scripts

std::ostream& operator<<(std::ostream& os, const bve::parsers::function_scripts::tree_node& node) {
	bve::parsers::function_scripts::FunctionScriptParseTreePrinter parse_tree_printer{os};

	apply_visitor(parse_tree_printer, node);

	return os;
}
