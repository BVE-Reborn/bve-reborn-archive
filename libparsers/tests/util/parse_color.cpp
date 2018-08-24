#include "utils.hpp"
#include <doctest.h>
#include <stdexcept>

using namespace std::string_literals;
using color = bvereborn::datatypes::color8_rgba;

#define CHECK_THROWS_AND_DEFAULTS(value)                                                           \
	CHECK_THROWS_AS(parsers::util::parse_color(value), std::invalid_argument);                     \
	CHECK_EQ(parsers::util::parse_color(value,                                                     \
	                                    color(std::numeric_limits<decltype(color::b)>::max())),    \
	         color(std::numeric_limits<decltype(color::b)>::max()));

TEST_SUITE_BEGIN("libparsers - util");

TEST_CASE("libparsers - util - parse_color") {
	CHECK_EQ(parsers::util::parse_color("#000000"), color(0, 0, 0, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#770000"), color(0x77, 0, 0, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#FF0000"), color(0xFF, 0, 0, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#007700"), color(0, 0x77, 0, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#00FF00"), color(0, 0xFF, 0, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#000077"), color(0, 0, 0x77, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#0000FF"), color(0, 0, 0xFF, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#777777"), color(0x77, 0x77, 0x77, 0xFF));
	CHECK_EQ(parsers::util::parse_color("#FFFFFF"), color(0xFF, 0xFF, 0xFF, 0xFF));

	CHECK_THROWS_AND_DEFAULTS("#00000");
	CHECK_THROWS_AND_DEFAULTS("#77000");
	CHECK_THROWS_AND_DEFAULTS("#FF000");
	CHECK_THROWS_AND_DEFAULTS("#00770");
	CHECK_THROWS_AND_DEFAULTS("#00FF0");
	CHECK_THROWS_AND_DEFAULTS("#00007");
	CHECK_THROWS_AND_DEFAULTS("#0000F");
	CHECK_THROWS_AND_DEFAULTS("#77777");
	CHECK_THROWS_AND_DEFAULTS("#FFFFF");

	CHECK_THROWS_AND_DEFAULTS("000000");
	CHECK_THROWS_AND_DEFAULTS("770000");
	CHECK_THROWS_AND_DEFAULTS("FF0000");
	CHECK_THROWS_AND_DEFAULTS("007700");
	CHECK_THROWS_AND_DEFAULTS("00FF00");
	CHECK_THROWS_AND_DEFAULTS("000077");
	CHECK_THROWS_AND_DEFAULTS("0000FF");
	CHECK_THROWS_AND_DEFAULTS("777777");
	CHECK_THROWS_AND_DEFAULTS("FFFFFF");

	CHECK_THROWS_AND_DEFAULTS("#KK0000");
	CHECK_THROWS_AND_DEFAULTS("#KK0000");
	CHECK_THROWS_AND_DEFAULTS("#KK0000");
	CHECK_THROWS_AND_DEFAULTS("#KK7700");
	CHECK_THROWS_AND_DEFAULTS("#KKFF00");
	CHECK_THROWS_AND_DEFAULTS("#KK0077");
	CHECK_THROWS_AND_DEFAULTS("#KK00FF");
	CHECK_THROWS_AND_DEFAULTS("#KK7777");
	CHECK_THROWS_AND_DEFAULTS("#KKFFFF");
}

TEST_SUITE_END();
