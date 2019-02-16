#include "parsers/animated.hpp"
#include "parsers/ini.hpp"
#include "tests.hpp"
#include "util/parsing.hpp"
#include <iostream>

void test_animated() {
	auto text = bve::util::parsers::load_from_file_utf8_bom(TESTLOCATION "/r160.animated");

	bve::util::parsers::remove_comments(text);

	auto const parsed = bve::parsers::ini::parse(text);

	std::cout << parsed << '\n';

	bve::parsers::animated_object::parse(text);
}
