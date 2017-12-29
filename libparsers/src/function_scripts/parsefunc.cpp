#include "function_scripts.hpp"
#include "parsers/function_scripts.hpp"

#include <iostream>
namespace parsers {
namespace function_scripts {
	instruction_list parse(const std::string& text) {
		auto tokens = parsers::function_scripts::lex(text);
		auto tree = parsers::function_scripts::create_tree(tokens);
		auto instructions = parsers::function_scripts::build_instructions(tree);
		::operator<<(std::cout, tokens);
		::operator<<(std::cout, tree);
		std::cout << instructions;
		return instructions;
	}
} // namespace function_scripts
} // namespace parsers