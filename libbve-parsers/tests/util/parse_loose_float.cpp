#include "utils.hpp"
#include <doctest.h>
#include <limits>

using namespace std::string_literals;

#define CHECK_THROWS_AND_DEFAULTS(value)                                                           \
	CHECK_THROWS_AS(parsers::util::parse_loose_float(value), std::invalid_argument);               \
	CHECK_EQ(parsers::util::parse_loose_float(value, std::numeric_limits<float>::max()),           \
	         std::numeric_limits<float>::max());

TEST_SUITE_BEGIN("libparsers - util");

TEST_CASE("libparsers - util - parse_loose_float") {
	CHECK_EQ(parsers::util::parse_loose_float("0"s), 0.0f);
	CHECK_EQ(parsers::util::parse_loose_float("-0"s), 0.0f);
	CHECK_EQ(parsers::util::parse_loose_float("0.2"s), 0.2f);
	CHECK_EQ(parsers::util::parse_loose_float("-0.2"s), -0.2f);
	CHECK_EQ(parsers::util::parse_loose_float("50.1"s), 50.1f);
	CHECK_EQ(parsers::util::parse_loose_float("-50.1"s), -50.1f);
	CHECK_EQ(parsers::util::parse_loose_float("1024.3"s), 1024.3f);
	CHECK_EQ(parsers::util::parse_loose_float("-1024.3"s), -1024.3f);
	CHECK_EQ(parsers::util::parse_loose_float("12345678901234567890.1"s), 12345678901234567890.1f);
	CHECK_EQ(parsers::util::parse_loose_float("-12345678901234567890.1"s),
	         -12345678901234567890.1f);

	CHECK_EQ(parsers::util::parse_loose_float("- 0"s), 0.0f);
	CHECK_EQ(parsers::util::parse_loose_float("0. 2"s), 0.2f);
	CHECK_EQ(parsers::util::parse_loose_float("-0 .2"s), -0.2f);
	CHECK_EQ(parsers::util::parse_loose_float("50 .1"s), 50.1f);
	CHECK_EQ(parsers::util::parse_loose_float("-5 0.1"s), -50.1f);
	CHECK_EQ(parsers::util::parse_loose_float("10 24 .3"s), 1024.3f);
	CHECK_EQ(parsers::util::parse_loose_float("-10 24 .3"s), -1024.3f);
	CHECK_EQ(parsers::util::parse_loose_float("12 34 56 78 90 12 34 56 78 90 .1 "s),
	         12345678901234567890.1f);
	CHECK_EQ(parsers::util::parse_loose_float("-1 23 45 67 89 01 23 45 67 89 0. 1"s),
	         -12345678901234567890.1f);

	CHECK_EQ(parsers::util::parse_loose_float("0!24@.231"s), 0.0f);
	CHECK_EQ(parsers::util::parse_loose_float("-0!24@.231"s), 0.0f);
	CHECK_EQ(parsers::util::parse_loose_float("0.2!24@.231"s), 0.2f);
	CHECK_EQ(parsers::util::parse_loose_float("-0.2!24@.231"s), -0.2f);
	CHECK_EQ(parsers::util::parse_loose_float("50.1!24@.231"s), 50.1f);
	CHECK_EQ(parsers::util::parse_loose_float("-50.1!24@.231"s), -50.1f);
	CHECK_EQ(parsers::util::parse_loose_float("1024.3!24@.231"s), 1024.3f);
	CHECK_EQ(parsers::util::parse_loose_float("-1024.3!24@.231"s), -1024.3f);
	CHECK_EQ(parsers::util::parse_loose_float("12345678901234567890.1!24@.231"s),
	         12345678901234567890.1f);
	CHECK_EQ(parsers::util::parse_loose_float("-12345678901234567890.1!24@.231"s),
	         -12345678901234567890.1f);

	CHECK_THROWS_AND_DEFAULTS("!0!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!-0!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!0.2!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!-0.2!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!50.1!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!-50.1!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!1024.3!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!-1024.3!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!12345678901234567890.1!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS("!-12345678901234567890.1!24@.231"s);

	CHECK_THROWS_AND_DEFAULTS(".-0!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS(".-0.2!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS(".-50.1!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS(".-1024.3!24@.231"s);
	CHECK_THROWS_AND_DEFAULTS(".-12345678901234567890.1!24@.231"s);

	CHECK_EQ(parsers::util::parse_loose_float("82.21.21"), 82.21f);
}

TEST_SUITE_END();
