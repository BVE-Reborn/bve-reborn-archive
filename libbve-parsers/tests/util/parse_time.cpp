#include "utils.hpp"
#include <doctest.h>
#include <stdexcept>

using namespace std::string_literals;

#define CHECK_THROWS_AND_DEFAULTS(value)                                                           \
	CHECK_THROWS_AS(parsers::util::parse_time(value), std::invalid_argument);                      \
	CHECK_EQ(parsers::util::parse_time(value, std::numeric_limits<core::datatypes::time>::max()),  \
	         std::numeric_limits<core::datatypes::time>::max());

TEST_SUITE_BEGIN("libparsers - util");

// NOLINTNEXTLINE(google-readability-function-size,readability-function-size)
TEST_CASE("libparsers - util - parse_time") {
	CHECK_EQ(parsers::util::parse_time("0"), 0 * 3600);
	CHECK_EQ(parsers::util::parse_time("5"), 5 * 3600);
	CHECK_EQ(parsers::util::parse_time("1200"), 1200 * 3600);

	CHECK_EQ(parsers::util::parse_time("0.6"), 0 * 3600 + 6 * 60);
	CHECK_EQ(parsers::util::parse_time("5.6"), 5 * 3600 + 6 * 60);
	CHECK_EQ(parsers::util::parse_time("1200.6"), 1200 * 3600 + 6 * 60);

	CHECK_EQ(parsers::util::parse_time("0.26"), 0 * 3600 + 26 * 60);
	CHECK_EQ(parsers::util::parse_time("5.26"), 5 * 3600 + 26 * 60);
	CHECK_EQ(parsers::util::parse_time("1200.26"), 1200 * 3600 + 26 * 60);

	CHECK_EQ(parsers::util::parse_time("0.265"), 0 * 3600 + 26 * 60 + 5);
	CHECK_EQ(parsers::util::parse_time("5.265"), 5 * 3600 + 26 * 60 + 5);
	CHECK_EQ(parsers::util::parse_time("1200.265"), 1200 * 3600 + 26 * 60 + 5);

	CHECK_EQ(parsers::util::parse_time("0.2655"), 0 * 3600 + 26 * 60 + 55);
	CHECK_EQ(parsers::util::parse_time("5.2655"), 5 * 3600 + 26 * 60 + 55);
	CHECK_EQ(parsers::util::parse_time("1200.2655"), 1200 * 3600 + 26 * 60 + 55);

	CHECK_EQ(parsers::util::parse_time("  0  "), 0 * 3600);
	CHECK_EQ(parsers::util::parse_time("  5  "), 5 * 3600);
	CHECK_EQ(parsers::util::parse_time("  12 00  "), 1200 * 3600);

	CHECK_EQ(parsers::util::parse_time("  0.6  "), 0 * 3600 + 6 * 60);
	CHECK_EQ(parsers::util::parse_time("  5.6  "), 5 * 3600 + 6 * 60);
	CHECK_EQ(parsers::util::parse_time("  12 00.6  "), 1200 * 3600 + 6 * 60);

	CHECK_EQ(parsers::util::parse_time("  0. 26  "), 0 * 3600 + 26 * 60);
	CHECK_EQ(parsers::util::parse_time("  5. 26  "), 5 * 3600 + 26 * 60);
	CHECK_EQ(parsers::util::parse_time("  12 00.26  "), 1200 * 3600 + 26 * 60);

	CHECK_EQ(parsers::util::parse_time("  0. 26 5  "), 0 * 3600 + 26 * 60 + 5);
	CHECK_EQ(parsers::util::parse_time("  5. 26 5  "), 5 * 3600 + 26 * 60 + 5);
	CHECK_EQ(parsers::util::parse_time("  12 00 .2 65  "), 1200 * 3600 + 26 * 60 + 5);

	CHECK_EQ(parsers::util::parse_time("  0 .2 65 5  "), 0 * 3600 + 26 * 60 + 55);
	CHECK_EQ(parsers::util::parse_time("  5 .2 65 5  "), 5 * 3600 + 26 * 60 + 55);
	CHECK_EQ(parsers::util::parse_time("  12 00. 26 55  "), 1200 * 3600 + 26 * 60 + 55);

	CHECK_THROWS_AND_DEFAULTS("hello");

	CHECK_THROWS_AND_DEFAULTS("!0");
	CHECK_THROWS_AND_DEFAULTS("!5");
	CHECK_THROWS_AND_DEFAULTS("!1200");
	CHECK_THROWS_AND_DEFAULTS("!0.6");
	CHECK_THROWS_AND_DEFAULTS("!5.6");
	CHECK_THROWS_AND_DEFAULTS("!1200.6");
	CHECK_THROWS_AND_DEFAULTS("!0.26");
	CHECK_THROWS_AND_DEFAULTS("!5.26");
	CHECK_THROWS_AND_DEFAULTS("!1200.26");
	CHECK_THROWS_AND_DEFAULTS("!0.265");
	CHECK_THROWS_AND_DEFAULTS("!5.265");
	CHECK_THROWS_AND_DEFAULTS("!1200.265");
	CHECK_THROWS_AND_DEFAULTS("!0.2655");
	CHECK_THROWS_AND_DEFAULTS("!5.2655");
	CHECK_THROWS_AND_DEFAULTS("!1200.2655");

	CHECK_THROWS_AND_DEFAULTS(".0");
	CHECK_THROWS_AND_DEFAULTS(".5");
	CHECK_THROWS_AND_DEFAULTS(".1200");
	CHECK_THROWS_AND_DEFAULTS(".0.6");
	CHECK_THROWS_AND_DEFAULTS(".5.6");
	CHECK_THROWS_AND_DEFAULTS(".1200.6");
	CHECK_THROWS_AND_DEFAULTS(".0.26");
	CHECK_THROWS_AND_DEFAULTS(".5.26");
	CHECK_THROWS_AND_DEFAULTS(".1200.26");
	CHECK_THROWS_AND_DEFAULTS(".0.265");
	CHECK_THROWS_AND_DEFAULTS(".5.265");
	CHECK_THROWS_AND_DEFAULTS(".1200.265");
	CHECK_THROWS_AND_DEFAULTS(".0.2655");
	CHECK_THROWS_AND_DEFAULTS(".5.2655");
	CHECK_THROWS_AND_DEFAULTS(".1200.2655");
}

TEST_SUITE_END();
