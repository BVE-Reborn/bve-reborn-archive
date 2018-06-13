#define LIBLOG_DEBUG
#include "doctest.h"
#include "log.hpp"
#include "log_test.hpp"
#include "utils.hpp"
#include <boost/regex.hpp>
#include <fstream>
#include <sstream>

using namespace std::string_literals;

TEST_SUITE_BEGIN("liblog_debug");

TEST_CASE("liblog_debug - log should write correct output when liblog_debug is defined.") {
	auto file = std::make_shared<std::ostringstream>();
	logger::set_output_location(file);

	LOG(note, "{}", "hello");
	boost::smatch m;
	std::string logged = file->str();
	CHECK(boost::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "note"s);
	// time and file should not be empty strings because LIBLOG_DEBUG is defined.
	CHECK_NE(m[2], ""s);
	CHECK_NE(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
}

TEST_CASE(
    "liblog_debug - log should write correct output to a file when liblog_debug is defined.") {
	std::string file_name = create_temp_file();
	logger::set_output_location(file_name);
	LOG(fatal_error, "{}", "hello");
	// Close file by opening a different stream.
	auto temp_loc = std::make_shared<std::ostringstream>();
	logger::set_output_location(temp_loc);

	std::fstream file(file_name);
	std::string logged = parsers::util::load_from_file_utf8_bom(file);
	boost::smatch m;
	CHECK(boost::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "fatal_error"s);
	// time and file should not be empty strings because LIBLOG_DEBUG is defined.
	CHECK_NE(m[2], ""s);
	CHECK_NE(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
}

#undef LIBLOG_DEBUG
