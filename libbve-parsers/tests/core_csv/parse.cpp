#include "csv.hpp"
#include <doctest.h>

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - core_csv");

TEST_CASE("libparsers - core_csv - empty file") {
	parsers::csv::parsed_csv result;

	const std::string text;

	SUBCASE("nosplit_first, delim comma, split on space") {
		result = parse(text, parsers::csv::split_first_column::no, ',', ' ');
	}
	SUBCASE("split_first, delim comma, split on space") {
		result = parse(text, parsers::csv::split_first_column::yes, ',', ' ');
	}
	SUBCASE("nosplit_first, delim tab, split on space") {
		result = parse(text, parsers::csv::split_first_column::no, '\t', ' ');
	}
	SUBCASE("nosplit_first, delim comma, split on tab") {
		result = parse(text, parsers::csv::split_first_column::no, ',', '\t');
	}

	REQUIRE_EQ(result.size(), 1);
	REQUIRE_EQ(result[0].size(), 1);
	CHECK_EQ(result[0][0].text.size(), 0);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, text.size());
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);
}

TEST_CASE("libparsers - core_csv - single item") {
	parsers::csv::parsed_csv result;

	const std::string text = "hello";

	SUBCASE("nosplit_first, delim comma, split on space") {
		result = parse(text, parsers::csv::split_first_column::no, ',', ' ');
	}
	SUBCASE("split_first, delim comma, split on space") {
		result = parse(text, parsers::csv::split_first_column::yes, ',', ' ');
	}
	SUBCASE("nosplit_first, delim tab, split on space") {
		result = parse(text, parsers::csv::split_first_column::no, '\t', ' ');
	}
	SUBCASE("nosplit_first, delim comma, split on tab") {
		result = parse(text, parsers::csv::split_first_column::no, ',', '\t');
	}

	REQUIRE_EQ(result.size(), 1);
	REQUIRE_EQ(result[0].size(), 1);
	CHECK_EQ(result[0][0].text, text);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, text.size());
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);
}

TEST_CASE("libparsers - core_csv - two items two lines") {
	parsers::csv::parsed_csv result;

	SUBCASE("nosplit_first, delim comma, split on space") {
		result = parse("hello\nworld"s, parsers::csv::split_first_column::no, ',', ' ');
	}
	SUBCASE("split_first, delim comma, split on space") {
		result = parse("hello\nworld"s, parsers::csv::split_first_column::yes, ',', ' ');
	}
	SUBCASE("nosplit_first, delim tab, split on space") {
		result = parse("hello\nworld"s, parsers::csv::split_first_column::no, '\t', ' ');
	}
	SUBCASE("nosplit_first, delim comma, split on tab") {
		result = parse("hello\nworld"s, parsers::csv::split_first_column::no, ',', '\t');
	}

	REQUIRE_EQ(result.size(), 2);
	REQUIRE_EQ(result[0].size(), 1);
	CHECK_EQ(result[0][0].text, "hello"s);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, 5);
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);

	REQUIRE_EQ(result[1].size(), 1);
	CHECK_EQ(result[1][0].text, "world"s);
	CHECK_EQ(result[1][0].char_begin, 0);
	CHECK_EQ(result[1][0].char_end, 5);
	CHECK_EQ(result[1][0].line_begin, 2);
	CHECK_EQ(result[1][0].line_end, 2);
}

TEST_CASE("libparsers - core_csv - four items two lines") {
	parsers::csv::parsed_csv result;

	SUBCASE("nosplit_first, delim comma, split on space") {
		result = parse("hello, world\non-another, line"s, parsers::csv::split_first_column::no, ',',
		               ' ');
	}
	SUBCASE("split_first, delim comma, split on space") {
		result = parse("hello, world\non-another, line"s, parsers::csv::split_first_column::yes,
		               ',', ' ');
	}
	SUBCASE("nosplit_first, delim tab, split on space") {
		result = parse("hello\t world\non-another\t line"s, parsers::csv::split_first_column::no,
		               '\t', ' ');
	}
	SUBCASE("nosplit_first, delim comma, split on tab") {
		result = parse("hello, world\non-another, line"s, parsers::csv::split_first_column::no, ',',
		               '\t');
	}

	REQUIRE_EQ(result.size(), 2);
	REQUIRE_EQ(result[0].size(), 2);
	CHECK_EQ(result[0][0].text, "hello"s);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, 5);
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);
	CHECK_EQ(result[0][1].text, "world"s);
	CHECK_EQ(result[0][1].char_begin, 6);
	CHECK_EQ(result[0][1].char_end, 12);
	CHECK_EQ(result[0][1].line_begin, 1);
	CHECK_EQ(result[0][1].line_end, 1);

	REQUIRE_EQ(result[1].size(), 2);
	CHECK_EQ(result[1][0].text, "on-another"s);
	CHECK_EQ(result[1][0].char_begin, 0);
	CHECK_EQ(result[1][0].char_end, 10);
	CHECK_EQ(result[1][0].line_begin, 2);
	CHECK_EQ(result[1][0].line_end, 2);
	CHECK_EQ(result[1][1].text, "line"s);
	CHECK_EQ(result[1][1].char_begin, 11);
	CHECK_EQ(result[1][1].char_end, 16);
	CHECK_EQ(result[1][1].line_begin, 2);
	CHECK_EQ(result[1][1].line_end, 2);
}

TEST_CASE("libparsers - core_csv - split first line, single item") {
	auto result = parse("first second"s, parsers::csv::split_first_column::yes);

	REQUIRE_EQ(result.size(), 1);
	REQUIRE_EQ(result[0].size(), 2);
	CHECK_EQ(result[0][0].text, "first"s);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, 5);
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);
	CHECK_EQ(result[0][1].text, "second"s);
	CHECK_EQ(result[0][1].char_begin, 6);
	CHECK_EQ(result[0][1].char_end, 12);
	CHECK_EQ(result[0][1].line_begin, 1);
	CHECK_EQ(result[0][1].line_end, 1);
}

TEST_CASE("libparsers - core_csv - split first line, two items") {
	auto result = parse("first second, third forth"s, parsers::csv::split_first_column::yes);

	REQUIRE_EQ(result.size(), 1);
	REQUIRE_EQ(result[0].size(), 3);
	CHECK_EQ(result[0][0].text, "first"s);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, 5);
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);
	CHECK_EQ(result[0][1].text, "second"s);
	CHECK_EQ(result[0][1].char_begin, 6);
	CHECK_EQ(result[0][1].char_end, 12);
	CHECK_EQ(result[0][1].line_begin, 1);
	CHECK_EQ(result[0][1].line_end, 1);
	CHECK_EQ(result[0][2].text, "third forth"s);
	CHECK_EQ(result[0][2].char_begin, 13);
	CHECK_EQ(result[0][2].char_end, 25);
	CHECK_EQ(result[0][2].line_begin, 1);
	CHECK_EQ(result[0][2].line_end, 1);
}

TEST_CASE("libparsers - core_csv - split first line, four items") {
	auto result =
	    parse("first second, third forth\nfifth, sixth"s, parsers::csv::split_first_column::yes);

	REQUIRE_EQ(result.size(), 2);
	REQUIRE_EQ(result[0].size(), 3);
	CHECK_EQ(result[0][0].text, "first"s);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, 5);
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);
	CHECK_EQ(result[0][1].text, "second"s);
	CHECK_EQ(result[0][1].char_begin, 6);
	CHECK_EQ(result[0][1].char_end, 12);
	CHECK_EQ(result[0][1].line_begin, 1);
	CHECK_EQ(result[0][1].line_end, 1);
	CHECK_EQ(result[0][2].text, "third forth"s);
	CHECK_EQ(result[0][2].char_begin, 13);
	CHECK_EQ(result[0][2].char_end, 25);
	CHECK_EQ(result[0][2].line_begin, 1);
	CHECK_EQ(result[0][2].line_end, 1);

	REQUIRE_EQ(result[1].size(), 2);
	CHECK_EQ(result[1][0].text, "fifth"s);
	CHECK_EQ(result[1][0].char_begin, 0);
	CHECK_EQ(result[1][0].char_end, 5);
	CHECK_EQ(result[1][0].line_begin, 2);
	CHECK_EQ(result[1][0].line_end, 2);
	CHECK_EQ(result[1][1].text, "sixth"s);
	CHECK_EQ(result[1][1].char_begin, 6);
	CHECK_EQ(result[1][1].char_end, 12);
	CHECK_EQ(result[1][1].line_begin, 2);
	CHECK_EQ(result[1][1].line_end, 2);
}

TEST_CASE("libparsers - core_csv - split first line, four items split") {
	auto result =
	    parse("first second, third forth\nfifth sixth"s, parsers::csv::split_first_column::yes);

	REQUIRE_EQ(result.size(), 2);
	REQUIRE_EQ(result[0].size(), 3);
	CHECK_EQ(result[0][0].text, "first"s);
	CHECK_EQ(result[0][0].char_begin, 0);
	CHECK_EQ(result[0][0].char_end, 5);
	CHECK_EQ(result[0][0].line_begin, 1);
	CHECK_EQ(result[0][0].line_end, 1);
	CHECK_EQ(result[0][1].text, "second"s);
	CHECK_EQ(result[0][1].char_begin, 6);
	CHECK_EQ(result[0][1].char_end, 12);
	CHECK_EQ(result[0][1].line_begin, 1);
	CHECK_EQ(result[0][1].line_end, 1);
	CHECK_EQ(result[0][2].text, "third forth"s);
	CHECK_EQ(result[0][2].char_begin, 13);
	CHECK_EQ(result[0][2].char_end, 25);
	CHECK_EQ(result[0][2].line_begin, 1);
	CHECK_EQ(result[0][2].line_end, 1);

	REQUIRE_EQ(result[1].size(), 2);
	CHECK_EQ(result[1][0].text, "fifth"s);
	CHECK_EQ(result[1][0].char_begin, 0);
	CHECK_EQ(result[1][0].char_end, 5);
	CHECK_EQ(result[1][0].line_begin, 2);
	CHECK_EQ(result[1][0].line_end, 2);
	CHECK_EQ(result[1][1].text, "sixth"s);
	CHECK_EQ(result[1][1].char_begin, 6);
	CHECK_EQ(result[1][1].char_end, 11);
	CHECK_EQ(result[1][1].line_begin, 2);
	CHECK_EQ(result[1][1].line_end, 2);
}

TEST_SUITE_END();
