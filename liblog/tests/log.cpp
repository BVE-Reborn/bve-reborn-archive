#include "log.hpp"
#include "doctest.h"
#include "log_test.hpp"
#include "utils.hpp"
#include <boost/regex.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
using namespace std::string_literals;
namespace bf = boost::filesystem;

TEST_SUITE_BEGIN("liblog");

TEST_CASE("liblog - write to ostream") {
	auto file = std::make_shared<std::ostringstream>();
	logger::set_output_location(file);

	LOG(note, "{}", "hello");
	boost::smatch m;
	std::string logged = file->str();
	CHECK(boost::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "note"s);
	// LIBLOG_DEBUG is not defined time and file location should be empty strings.
	CHECK_EQ(m[2], ""s);
	CHECK_EQ(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
}

TEST_CASE("liblog - should be able to change current severity") {
	auto file = std::make_shared<std::ostringstream>();
	logger::set_output_location(file);
	// Default severity is note.
	CHECK_EQ(logger::current_severity.get(), logger::severity::note);
	logger::current_severity.set(logger::severity::fatal_error);
	CHECK_EQ(logger::current_severity.get(), logger::severity::fatal_error);

	// Reset current severity to note.
	logger::current_severity.set(logger::severity::note);
}

TEST_CASE("liblog - log should not print if given severitiy is lower than current severity") {
	auto file = std::make_shared<std::ostringstream>();
	logger::set_output_location(file);

	logger::severity s = logger::severity::note;

	SUBCASE("debug") {
		s = logger::severity::debug;
	}
	SUBCASE("info") {
		s = logger::severity::info;
	}
	SUBCASE("note") {
		s = logger::severity::note;
	}
	SUBCASE("warning") {
		s = logger::severity::warning;
	}
	SUBCASE("error") {
		s = logger::severity::error;
	}
	SUBCASE("fatal_error") {
		s = logger::severity::fatal_error;
	}

	logger::current_severity.set(s);

	LOG(warning, "{}", "hello");
	std::string logged = file->str();
	boost::smatch m;
	if (s == logger::severity::debug || s == logger::severity::info || s == logger::severity::note
	    || s == logger::severity::warning) {
		CHECK(boost::regex_match(logged, m, log_regex));
		REQUIRE_EQ(m.size(), 5);
	}
	else {
		CHECK_EQ(logged, ""s);
	}
	// Reset current severity to note.
	logger::current_severity.set(logger::severity::note);
}

TEST_CASE("liblog - should be able to write to a file.") {
	std::string file_name = create_temp_file();
	logger::set_output_location(file_name);
	LOG(fatal_error, "{}", "hello");
	// Close file by opening a different stream.
	auto temp_loc = std::make_shared<std::ostringstream>();
	logger::set_output_location(temp_loc);

	bf::fstream file{file_name};
	std::string logged = parsers::util::load_from_file_utf8_bom(file);
	boost::smatch m;
	CHECK(boost::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "fatal_error"s);
	// LIBLOG_DEBUG is not defined time and file location should be empty strings.
	CHECK_EQ(m[2], ""s);
	CHECK_EQ(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
}

TEST_CASE("liblog - should not write to a file if the severity is lower than current severity.") {
	std::string file_name = create_temp_file();
	logger::set_output_location(file_name);

	logger::severity s = logger::severity::note;
	SUBCASE("debug") {
		s = logger::severity::debug;
	}
	SUBCASE("info") {
		s = logger::severity::info;
	}
	SUBCASE("note") {
		s = logger::severity::note;
	}
	SUBCASE("warning") {
		s = logger::severity::warning;
	}
	SUBCASE("error") {
		s = logger::severity::error;
	}
	SUBCASE("fatal_error") {
		s = logger::severity::fatal_error;
	}

	logger::current_severity.set(s);

	LOG(warning, "{}", "hello");

	auto temp_loc = std::make_shared<std::ostringstream>();
	logger::set_output_location(temp_loc);

	std::fstream file(file_name);
	std::string logged = parsers::util::load_from_file_utf8_bom(file);
	boost::smatch m;
	if (s == logger::severity::debug || s == logger::severity::info || s == logger::severity::note
	    || s == logger::severity::warning) {
		CHECK(boost::regex_match(logged, m, log_regex));
		REQUIRE_EQ(m.size(), 5);
	}
	else {
		CHECK_EQ(logged, ""s);
	}
	// Reset current severity to note.
	logger::current_severity.set(logger::severity::note);
}

TEST_CASE("liblog - logging should be thread safe.") {
	auto file = std::make_shared<std::stringstream>();
	logger::set_output_location(file);
	// Changes the global severity and LOGs str, the LOG serverity will always be note.
	auto change_sev_and_log = [&](std::string const& str, logger::severity s) {
		logger::current_severity.set(s);
		LOG(note, "{}", str);
	};

	auto t1 = std::async(std::launch::async, change_sev_and_log, "t1"s, logger::severity::info);
	auto t2 = std::async(std::launch::async, change_sev_and_log, "t2"s, logger::severity::note);
	// t3-5 shouldn't LOG because they change the global severity to one that is higher than note.
	auto t3 = std::async(std::launch::async, change_sev_and_log, "t3"s, logger::severity::warning);
	auto t4 = std::async(std::launch::async, change_sev_and_log, "t4"s, logger::severity::error);
	auto t5 =
	    std::async(std::launch::async, change_sev_and_log, "t5"s, logger::severity::fatal_error);

	t1.get();
	t2.get();
	t3.get();
	t4.get();
	t5.get();
	std::string logged;

	while (getline(*file, logged)) {
		boost::smatch m;
		CHECK(boost::regex_match(logged, m, log_regex));
		REQUIRE_EQ(m.size(), 5);
		// t3-5 shouldn't LOG because they change the global severity to one that is higher than
		// note.
		CHECK_NE(m[4], "t3"s);
		CHECK_NE(m[4], "t4"s);
		CHECK_NE(m[4], "t5"s);
	}
	// Reset current severity to note.
	logger::current_severity.set(logger::severity::note);
}

TEST_CASE("liblog - changes to current severity should be thread safe.") {
	auto change_sev = [](logger::severity s) noexcept { logger::current_severity.set(s); };
	auto t1 = std::async(std::launch::async, change_sev, logger::severity::info);
	auto t2 = std::async(std::launch::async, change_sev, logger::severity::note);
	auto t3 = std::async(std::launch::async, change_sev, logger::severity::warning);
	auto t4 = std::async(std::launch::async, change_sev, logger::severity::error);
	auto t5 = std::async(std::launch::async, change_sev, logger::severity::fatal_error);

	// Need threads to set their severity before resetting it to note.
	t1.get();
	t2.get();
	t3.get();
	t4.get();
	t5.get();

	// Reset current severity to note.
	logger::current_severity.set(logger::severity::note);
}
