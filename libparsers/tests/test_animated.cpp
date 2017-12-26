#include "ini.hpp"
#include "tests.hpp"
#include "utils.hpp"
#include <iostream>

void test_animated() {
	auto text = parsers::util::load_from_file_utf8_bom("libparsers/tests/r160.animated");

	parsers::util::remove_comments(text);

	auto parsed = parsers::ini::parse(text);

	std::cout << parsed << '\n';
}