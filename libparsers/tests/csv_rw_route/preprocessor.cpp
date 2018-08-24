#include "csv_rw_route.hpp"
#include <doctest.h>
#include <fstream>

using namespace std::string_literals;
namespace cs = parsers::csv_rw_route;
namespace {
std::string file_func(std::string const& base, std::string const& rel) {
	return base + "/" + rel;
}

void write_to_file(std::string const& directive) {
	std::ofstream file;
	file.open("directive.csv", std::ios::trunc);
	file.write(directive.c_str(), sizeof(char) * directive.size());
}

cs::preprocessed_lines setup(std::string const& test,
                             parsers::errors::multi_error_t& output_errors) {
	write_to_file(test);
	int const seed = 1;
	auto rng = bvereborn::datatypes::rng{seed};
	auto processed = cs::process_include_directives("directive.csv", rng, output_errors,
	                                                cs::file_type::csv, file_func);
	cs::preprocess_file(processed, rng, output_errors, cs::file_type::csv);
	return processed;
}

} // namespace
TEST_SUITE_BEGIN("libparsers - csv_rw_route - preprocessor");

TEST_CASE("libparsers - csv_rw_route - preprocessor - $chr") {
	std::string test_command =
	    "$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78),\n$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78)"s;
	parsers::errors::multi_error_t output_errors;
	auto processed = setup(test_command, output_errors);
	CHECK_EQ(processed.lines.size(), 10);
	std::string value{};
	for (auto& chr : processed.lines) {
		value += chr.contents;
	}
	CHECK_EQ(value, "KEVINKEVIN");
}

TEST_CASE(
    "libparsers - csv_rw_route - preprocessor - incorrect $Chr() expression should return "
    "nothing") {
	std::string test_command = "$Cr(75),$Ch(69),$Chr(75)"s;
	parsers::errors::multi_error_t output_errors;
	auto processed = setup(test_command, output_errors);
	CHECK_EQ(processed.lines.size(), 1);
	std::string value{};
	for (auto& chr : processed.lines) {
		value += chr.contents;
	}
	CHECK_EQ(value, "K");
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Rnd") {
	std::string test_command = "$Rnd(3;5),$Rnd(-100;100),$Rnd(0;0),$Rnd(5;3)"s;
	parsers::errors::multi_error_t output_errors;
	auto processed = setup(test_command, output_errors);
	REQUIRE_EQ(processed.lines.size(), 4);
	CHECK_LE(processed.lines[0].contents, "5"s);
	CHECK_GE(processed.lines[0].contents, "3"s);

	CHECK_LE(processed.lines[1].contents, "100"s);
	CHECK_GE(processed.lines[1].contents, "-100"s);

	CHECK_EQ(processed.lines[2].contents, "0"s);

	CHECK_LE(processed.lines[3].contents, "5"s);
	CHECK_GE(processed.lines[3].contents, "3"s);
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Rnd bad inputs should add errors") {
	std::string test_command = "$Rd(3;5),$nd(-100;100),$d(0;0),$Rnd(;3),$Rnd(0),$Rnd(s:a)"s;
	parsers::errors::multi_error_t output_errors;
	auto processed = setup(test_command, output_errors);
	CHECK_EQ(processed.lines.size(), 0);
	CHECK_EQ(output_errors["directive.csv"].size(), 6);
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Sub") {
	std::string test_command = "$Sub(0) = $Rnd(3;5)\n$Sub(1) = $Chr(75) \n,$Sub(0),$Sub(1)"s;
	parsers::errors::multi_error_t output_errors;
	auto processed = setup(test_command, output_errors);
	REQUIRE_EQ(processed.lines.size(), 2);
	CHECK_GE(processed.lines[0].contents, "3"s);
	CHECK_LE(processed.lines[0].contents, "5"s);

	CHECK_EQ(processed.lines[1].contents, "K"s);
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $If") {
	parsers::errors::multi_error_t output_errors;
	// Single sub expression.
	auto processed = setup("$If(1),$Chr(75),$EndIf()"s, output_errors);
	REQUIRE_EQ(processed.lines.size(), 1);
	CHECK_EQ(processed.lines[0].contents, "K"s);

	processed = setup("$If(0),$Chr(75),$Else(),$Chr(69),$EndIf()"s, output_errors);
	REQUIRE_EQ(processed.lines.size(), 1);
	CHECK_EQ(processed.lines[0].contents, "E");
}
TEST_CASE("libparser - csv_rw_route - preprocessor - $If multiple sub expressions") {
	parsers::errors::multi_error_t output_errors;
	auto processed =
	    setup("$If(1),$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78),$Else(),$Chr(69),$EndIf()"s,
	          output_errors);
	REQUIRE_EQ(processed.lines.size(), 5);
	std::string value{};
	for (auto const& line : processed.lines) {
		value += line.contents;
	}
	CHECK_EQ(value, "KEVIN"s);

	processed =
	    setup("$If(0),$Chr(75),$Else(),$Chr(75),$Chr(69),$Chr(86),$EndIf()"s, output_errors);
	REQUIRE_EQ(processed.lines.size(), 3);
	value = ""s;
	for (auto const& line : processed.lines) {
		value += line.contents;
	}
	CHECK_EQ(value, "KEV"s);
}
TEST_CASE("libparsers - csv_rw_route - preprocessor - $If mutliple conditionals") {
	parsers::errors::multi_error_t output_errors;
	auto processed =
	    setup("$If(1),$Chr(75),$If(1),$Chr(69),$If(1),$Chr(86),$Else(),$Chr(69),$EndIf()"s,
	          output_errors);
	REQUIRE_EQ(processed.lines.size(), 3);

	std::string value = ""s;
	for (auto const& line : processed.lines) {
		value += line.contents;
	}
	CHECK_EQ(value, "KEV"s);
}

TEST_SUITE_END();