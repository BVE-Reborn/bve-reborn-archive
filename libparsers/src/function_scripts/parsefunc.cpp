#include "function_scripts.hpp"
#include "parsers/function_scripts.hpp"

namespace parsers {
namespace function_scripts {
	instruction_list parse(const std::string& text) {
		errors::errors_t e;
		auto const tokens = lex(text, e);
		auto const tree = create_tree(tokens, e);
		auto instructions = build_instructions(tree, e);
		return instructions;
	}
} // namespace function_scripts
} // namespace parsers
