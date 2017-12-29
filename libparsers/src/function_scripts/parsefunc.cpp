#include "function_scripts.hpp"
#include "parsers/function_scripts.hpp"

#include <iostream>
namespace parsers {
namespace function_scripts {
	instruction_list parse(const std::string& text) {
		errors::errors_t e;
		auto tokens = parsers::function_scripts::lex(text, e);
		auto tree = parsers::function_scripts::create_tree(tokens, e);
		auto instructions = parsers::function_scripts::build_instructions(tree, e);
		return instructions;
	}
} // namespace function_scripts
} // namespace parsers
