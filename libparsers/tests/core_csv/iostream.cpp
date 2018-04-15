#include "csv.hpp"
#include <doctest.h>
#include <sstream>

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - core_csv");

TEST_CASE("libparsers - core_csv - iostream - parsers::csv::csv_token") {
	parsers::csv::csv_token token{"hello"s, 1, 2, 3, 4};

	std::ostringstream output;

	output << token;

	CHECK_EQ(output.str(), "(\"hello\", 1, 2, 3, 4)"s);
}

TEST_CASE("libparsers - core_csv - iostream - single item") {
	parsers::csv::parsed_csv result{{
	    {{"hello"s, 1, 2, 3, 4}}, //
	}};

	std::ostringstream output;

	output << result;

	CHECK_EQ(output.str(), "(\"hello\", 1, 2, 3, 4)\n"s);
}

TEST_CASE("libparsers - core_csv - iostream - two items") {
	parsers::csv::parsed_csv result{{
	    {{"hello"s, 1, 2, 3, 4}, {"world"s, 5, 6, 7, 8}}, //
	}};

	std::ostringstream output;

	output << result;

	CHECK_EQ(output.str(), "(\"hello\", 1, 2, 3, 4), (\"world\", 5, 6, 7, 8)\n"s);
}

TEST_SUITE_END();
