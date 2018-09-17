#include "ini.hpp"
#include "parsers/animated.hpp"
#include "tests.hpp"
#include "utils.hpp"
#include <iostream>

void test_animated() {
	auto text = bve::parsers::util::load_from_file_utf8_bom(TESTLOCATION "/r160.animated");

	bve::parsers::util::remove_comments(text);

	auto const parsed = bve::parsers::ini::parse(text);

	std::cout << parsed << '\n';

	bve::parsers::animated_object::parse(text);
}
