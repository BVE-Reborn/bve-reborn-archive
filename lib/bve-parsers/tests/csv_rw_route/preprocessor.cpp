#include "cppfs/FileHandle.h"
#include "cppfs/fs.h"
#include "parsers/utils.hpp"
#include "sample_relative_file_func.hpp"
#include "parsers/csv_rw_route.hpp"
#include <cppfs/FilePath.h>
#include <doctest.h>
#include <fstream>

using namespace std::string_literals;
namespace cs = bve::parsers::csv_rw_route;
namespace p_util = bve::parsers::util;

namespace {

	void write_to_file(std::string const& directive) {
		cppfs::FileHandle file = cppfs::fs::open("directive.csv"s);
		auto const ofs = file.createOutputStream();
		*ofs << directive;
	}

	cs::PreprocessedLines setup(std::string const& test,
	                            bve::parsers::errors::MultiError& output_errors) {
		write_to_file(test);
		std::uint32_t const seed = 1;
		auto rng = bve::core::datatypes::RNG{seed};
		auto processed = process_include_directives("directive.csv"s, rng, output_errors,
		                                            cs::FileType::csv, rel_file_func);
		preprocess_file(processed, rng, output_errors, cs::FileType::csv);
		cppfs::fs::open("directive.csv"s).remove();
		return processed;
	}

} // namespace

TEST_SUITE_BEGIN("libparsers - csv_rw_route - preprocessor");

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Chr") {
	std::string const test_command =
	    "$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78),\n" //
	    "$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78)"s;

	bve::parsers::errors::MultiError output_errors;

	auto const processed = setup(test_command, output_errors);

	CHECK_EQ(processed.lines.size(), 10);

	std::string value;
	for (auto& chr : processed.lines) {
		value += chr.contents;
	}

	CHECK_EQ(value, "KEVINKEVIN");
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Chr - invalid input") {
	std::string const test_command = "$Cr(75),$Ch(69),$Chr(75)"s;

	bve::parsers::errors::MultiError output_errors;

	auto const processed = setup(test_command, output_errors);
	CHECK_EQ(processed.lines.size(), 1);

	std::string value;
	for (auto& chr : processed.lines) {
		value += chr.contents;
	}

	CHECK_EQ(value, "K");
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Rnd") {
	std::string const test_command = "$Rnd(3;5),$Rnd(-100;100),$Rnd(0;0),$Rnd(5;3)"s;

	bve::parsers::errors::MultiError output_errors;

	auto const processed = setup(test_command, output_errors);

	REQUIRE_EQ(processed.lines.size(), 4);

	CHECK_LE(p_util::parse_loose_integer(processed.lines[0].contents), 5);
	CHECK_GE(p_util::parse_loose_integer(processed.lines[0].contents), 3);

	CHECK_LE(p_util::parse_loose_integer(processed.lines[1].contents), 100);
	CHECK_GE(p_util::parse_loose_integer(processed.lines[1].contents), -100);

	CHECK_EQ(p_util::parse_loose_integer(processed.lines[2].contents), 0);

	CHECK_LE(p_util::parse_loose_integer(processed.lines[3].contents), 5);
	CHECK_GE(p_util::parse_loose_integer(processed.lines[3].contents), 3);
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Rnd - invalid input") {
	std::string const test_command = "$Rd(3;5),$nd(-100;100),$d(0;0),$Rnd(;3),$Rnd(0),$Rnd(s:a)"s;
	bve::parsers::errors::MultiError output_errors;

	auto const processed = setup(test_command, output_errors);

	CHECK_EQ(processed.lines.size(), 0);
	CHECK_EQ(output_errors["directive.csv"].size(), 6);
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $Sub") {
	std::string const test_command =
	    "$Sub(0) = $Rnd(3;5)\n"
	    "$Sub(1) = $Chr(75) \n"
	    ",$Sub(0),$Sub(1)"s;

	bve::parsers::errors::MultiError output_errors;
	auto const processed = setup(test_command, output_errors);
	REQUIRE_EQ(processed.lines.size(), 2);
	CHECK_GE(p_util::parse_loose_integer(processed.lines[0].contents), 3);
	CHECK_LE(p_util::parse_loose_integer(processed.lines[0].contents), 5);

	CHECK_EQ(processed.lines[1].contents, "K"s);
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $If") {
	std::string const test_command = "$If(1),$Chr(75),$EndIf()"s;

	bve::parsers::errors::MultiError output_errors;
	// Single sub expression.
	auto const processed1 = setup(test_command, output_errors);
	REQUIRE_EQ(processed1.lines.size(), 1);
	CHECK_EQ(processed1.lines[0].contents, "K"s);

	std::string const test_command2 = "$If(0),$Chr(75),$Else(),$Chr(69),$EndIf()"s;
	auto const processed2 = setup(test_command2, output_errors);
	REQUIRE_EQ(processed2.lines.size(), 1);
	CHECK_EQ(processed2.lines[0].contents, "E");
}

TEST_CASE("libparser - csv_rw_route - preprocessor - $If - multiple sub expressions") {
	std::string const test_input =
	    "$If(1),$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78),$Else(),$Chr(69),$EndIf()"s;

	bve::parsers::errors::MultiError output_errors;

	auto const processed = setup(test_input, output_errors);

	REQUIRE_EQ(processed.lines.size(), 5);

	std::string value;
	for (auto const& line : processed.lines) {
		value += line.contents;
	}
	CHECK_EQ(value, "KEVIN"s);

	std::string const test_input2 = "$If(0),$Chr(75),$Else(),$Chr(75),$Chr(69),$Chr(86),$EndIf()"s;

	auto const processed2 = setup(test_input2, output_errors);

	REQUIRE_EQ(processed2.lines.size(), 3);

	value.clear();
	for (auto const& line : processed2.lines) {
		value += line.contents;
	}
	CHECK_EQ(value, "KEV"s);
}

TEST_CASE("libparsers - csv_rw_route - preprocessor - $If - multiple conditionals") {
	std::string const test_input =
	    "$If(1),$Chr(75),$If(1),$Chr(69),$If(1),$Chr(86),$Else(),$Chr(69),$EndIf()"s;

	bve::parsers::errors::MultiError output_errors;

	auto const processed = setup(test_input, output_errors);

	REQUIRE_EQ(processed.lines.size(), 3);

	std::string value = ""s;
	for (auto const& line : processed.lines) {
		value += line.contents;
	}

	CHECK_EQ(value, "KEV"s);
}

TEST_SUITE_END();
