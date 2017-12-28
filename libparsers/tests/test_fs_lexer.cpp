#include "function_scripts.hpp"
#include "tests.hpp"
#include <iostream>

using namespace std::string_literals;

void test_fs() {
	auto test_string = "if[value * -Add2[2, 3, 4] == 15 / 2 + 4, cameraMode < 2, c & d]"s;

	auto output = parsers::function_scripts::lex(test_string);

	std::cout << output;

	auto tree = parsers::function_scripts::create_tree(output);

	std::cout << tree;
}