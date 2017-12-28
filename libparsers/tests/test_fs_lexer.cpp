#include "function_scripts.hpp"
#include "tests.hpp"
#include <iostream>

using namespace std::string_literals;

void test_fs() {
	// value 2 3 + 4 + * 15 2 4 / / == cameraMode 2 < c d & ?
	// auto test_string = "if[value * -Plus[2, 3, 4] == 15 / 2 / 4, cameraMode < 2, c & d]"s;
	// speed delta * 2.7632 mod 2.7632 / 3.14 2 * * value +
	auto test_string = "value + mod[(speed * delta), 2.7632] / -2.7632 * 3.14 * 2"s;

	auto output = parsers::function_scripts::lex(test_string);

	std::cout << output;

	auto tree = parsers::function_scripts::create_tree(output);

	std::cout << tree;
}