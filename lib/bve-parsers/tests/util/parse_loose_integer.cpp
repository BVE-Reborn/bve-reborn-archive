#include "parsers/utils.hpp"
#include "test_macro_util.hpp"
#include <doctest.h>
#include <limits>

using namespace std::string_literals;

#define CHECK_THROWS_AND_DEFAULTS(value)                                                           \
	CHECK_THROWS_AS(bve::parsers::util::parse_loose_integer(value), std::invalid_argument);        \
	CHECK_EQ(bve::parsers::util::parse_loose_integer(value,                                        \
	                                                 std::numeric_limits<std::intmax_t>::max()),   \
	         std::numeric_limits<std::intmax_t>::max());

TEST_SUITE_BEGIN("libparsers - util");

TEST_CASE("libparsers - util - parse_loose_integer") {
	CHECK_EQ(bve::parsers::util::parse_loose_integer("0"s), 0);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-0"s), 0);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("50"s), 50);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-50"s), -50);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("1024"s), 1024);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-1024"s), -1024);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("1234567890123456789"s), 1234567890123456789);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-1234567890123456789"s),
	         -1234567890123456789);

	CHECK_EQ(bve::parsers::util::parse_loose_integer("- 0"s), 0);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("5 0"s), 50);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-5 0"s), -50);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("10 24"s), 1024);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-10 24"s), -1024);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("12 34 56 78 90 12 34 56 78 9"s),
	         1234567890123456789);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-12 34 56 78 90 12 34 56 78 9"s),
	         -1234567890123456789);

	CHECK_EQ(bve::parsers::util::parse_loose_integer("0!2231"s), 0);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-0!2231"s), 0);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("50!2231"s), 50);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-50!2231"s), -50);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("1024!2231"s), 1024);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-1024!2231"s), -1024);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("1234567890123456789!2231"s),
	         1234567890123456789);
	CHECK_EQ(bve::parsers::util::parse_loose_integer("-1234567890123456789!2231"s),
	         -1234567890123456789);

	CHECK_THROWS_AND_DEFAULTS("!0!2231"s);
	CHECK_THROWS_AND_DEFAULTS("!-0!2231"s);
	CHECK_THROWS_AND_DEFAULTS("!50!2231"s);
	CHECK_THROWS_AND_DEFAULTS("!-50!2231"s);
	CHECK_THROWS_AND_DEFAULTS("!1024!2231"s);
	CHECK_THROWS_AND_DEFAULTS("!-1024!2231"s);
	CHECK_THROWS_AND_DEFAULTS("!1234567890123456789!2231"s);
	CHECK_THROWS_AND_DEFAULTS("!-1234567890123456789!2231"s);

	CHECK_THROWS_AND_DEFAULTS(".0!2231"s);
	CHECK_THROWS_AND_DEFAULTS(".-0!2231"s);
	CHECK_THROWS_AND_DEFAULTS(".50!2231"s);
	CHECK_THROWS_AND_DEFAULTS(".-50!2231"s);
	CHECK_THROWS_AND_DEFAULTS(".1024!2231"s);
	CHECK_THROWS_AND_DEFAULTS(".-1024!2231"s);
	CHECK_THROWS_AND_DEFAULTS(".1234567890123456789!2231"s);
	CHECK_THROWS_AND_DEFAULTS(".-1234567890123456789!2231"s);
}

TEST_SUITE_END();
