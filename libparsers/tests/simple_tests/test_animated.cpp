#include "ini.hpp"
#include "parsers/animated.hpp"
#include "tests.hpp"
#include "utils.hpp"
#include <iostream>

void test_animated() {
	auto text = parsers::util::load_from_file_utf8_bom("libparsers/tests/test_files/r160.animated");

	parsers::util::remove_comments(text);

	auto const parsed = parsers::ini::parse(text);

	std::cout << parsed << '\n';

	parsers::animated_object::parse(text);
}
