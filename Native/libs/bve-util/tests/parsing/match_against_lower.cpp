#include "util/parsing.hpp"
#include <doctest/doctest.h>
#include <ostream>

using namespace std::string_literals;

TEST_SUITE_BEGIN("libutil - util");

TEST_CASE("libutil - util - match_against_lower") {
	CHECK_EQ(bve::util::parsers::match_against_lower("hello"s, "hello"), true);
	CHECK_EQ(bve::util::parsers::match_against_lower("HeLlo"s, "hello"), true);
	CHECK_EQ(bve::util::parsers::match_against_lower("Hello"s, "hell", false), true);
	CHECK_EQ(bve::util::parsers::match_against_lower("Hell"s, "hello", false), true);
	CHECK_EQ(bve::util::parsers::match_against_lower("Hello"s, "hell"), false);
	CHECK_EQ(bve::util::parsers::match_against_lower("Hell"s, "hello"), false);

	CHECK_EQ(bve::util::parsers::match_against_lower("hel1o"s, "hello"), false);
	CHECK_EQ(bve::util::parsers::match_against_lower("HeL1o"s, "hello"), false);
	CHECK_EQ(bve::util::parsers::match_against_lower("Hel1o"s, "hell"), false);
	CHECK_EQ(bve::util::parsers::match_against_lower("Hel1"s, "hello"), false);
}

TEST_SUITE_END();
