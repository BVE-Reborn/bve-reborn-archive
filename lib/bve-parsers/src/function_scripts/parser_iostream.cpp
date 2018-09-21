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

		void printNextNode(const TreeNode& next) {
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

		void operator()(const tree_types::BinaryAnd& node) {
			printBinary(node, "AND");
		}

		void operator()(const tree_types::BinaryXor& node) {
			printBinary(node, "XOR");
		}

		void operator()(const tree_types::BinaryOr& node) {
			printBinary(node, "OR");
		}

		void operator()(const tree_types::BinaryEq& node) {
			printBinary(node, "EQ");
		}

		void operator()(const tree_types::BinaryNotEq& node) {
			printBinary(node, "NEQ");
		}

		void operator()(const tree_types::BinaryLess& node) {
			printBinary(node, "LESS");
		}

		void operator()(const tree_types::BinaryGreater& node) {
			printBinary(node, "GREATER");
		}

		void operator()(const tree_types::BinaryLessEq& node) {
			printBinary(node, "LESS_EQ");
		}

		void operator()(const tree_types::BinaryGreaterEq& node) {
			printBinary(node, "GREATER_EQ");
		}

		void operator()(const tree_types::BinaryAdd& node) {
			printBinary(node, "ADD");
		}

		void operator()(const tree_types::BinarySubtract& node) {
			printBinary(node, "SUB");
		}

		void operator()(const tree_types::BinaryMultiply& node) {
			printBinary(node, "MULTIPLY");
		}

		void operator()(const tree_types::BinaryDivide& node) {
			printBinary(node, "DIVIDE");
		}

		void operator()(const tree_types::UnaryNot& node) {
			printUnary(node, "NOT");
		}

		void operator()(const tree_types::UnaryMinus& node) {
			printUnary(node, "MINUS");
		}

		void operator()(const tree_types::FunctionCall& node) {
			startPrint();
			os_ << "FUNC_CALL: " << node.name.val << '\n';
			for (auto& n : node.args) {
				printNextNode(n);
			}
		}
		void operator()(const tree_types::Integer& node) const {
			startPrint();
			os_ << node.num << '\n';
		}

		void operator()(const tree_types::Floating& node) const {
			startPrint();
			os_ << node.num << '\n';
		}

		void operator()(const tree_types::Identifier& node) const {
			startPrint();
			os_ << "VARIABLE: " << node.val << '\n';
		}

		void operator()(const tree_types::None& node) const {
			(void) node;
			startPrint();
			os_ << "NONE\n";
		}
	};
} // namespace bve::parsers::function_scripts

std::ostream& operator<<(std::ostream& os, const bve::parsers::function_scripts::TreeNode& node) {
	bve::parsers::function_scripts::FunctionScriptParseTreePrinter parse_tree_printer{os};

	apply_visitor(parse_tree_printer, node);

	return os;
}
