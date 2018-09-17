#if 0

#	define LIBLOG_DEBUG
#	include "doctest.h"
#	include "log.hpp"
#	include "log_test.hpp"
#	include "utils.hpp"
#	include <filesystem>
#	include <fstream>
#	include <sstream>

using namespace std::string_literals;

TEST_SUITE_BEGIN("liblog-debug");

TEST_CASE("liblog-debug - LIBLOG_DEBUG output") {
	auto file = std::make_shared<std::ostringstream>();
	logger::set_output_location(file);

	LOG(note, "{}", "hello");
	std::smatch m;
	std::string logged = file->str();
	CHECK(std::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "note"s);
	// time and file should not be empty strings because LIBLOG_DEBUG is defined.
	CHECK_NE(m[2], ""s);
	CHECK_NE(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
}

TEST_CASE("liblog-debug - LIBLOG_DEBUG file output") {
	std::string file_name = create_temp_file();
	logger::set_output_location(file_name);
	LOG(fatal_error, "{}", "hello");
	// Close file by opening a different stream.
	auto temp_loc = std::make_shared<std::ostringstream>();
	logger::set_output_location(temp_loc);

	std::fstream file(file_name);
	std::string logged = parsers::util::load_from_file_utf8_bom(file);
	std::smatch m;
	CHECK(std::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "fatal_error"s);
	// time and file should not be empty strings because LIBLOG_DEBUG is defined.
	CHECK_NE(m[2], ""s);
	CHECK_NE(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
	file.close();
	std::filesystem::remove(file_name);
}

TEST_CASE("liblog-debug - get_output_stream") {
	std::string file_name = create_temp_file();
	logger::set_output_location(file_name);
	auto file = logger::get_output_stream();
	REQUIRE_NE(file.get(), nullptr);
}

#	undef LIBLOG_DEBUG

#endif
