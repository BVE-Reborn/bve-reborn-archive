#if 0

#	include "log.hpp"
#	include "doctest.h"
#	include "log_test.hpp"
#	include "utils.hpp"
#	include <cppfs/FilePath.h>
#	include <fstream>
#	include <future>
#	include <iostream>
#	include <sstream>

using namespace std::string_literals;

TEST_SUITE_BEGIN("liblog");

TEST_CASE("liblog - write to ostream") {
	auto file = std::make_shared<std::ostringstream>();
	log::set_output_location(file);

	LOG(note, "{}", "hello");
	std::smatch m;
	std::string logged = file->str();
	CHECK(std::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "note"s);
	// LIBBVE_LOG_DEBUG is not defined time and file location should be empty strings.
	CHECK_EQ(m[2], ""s);
	CHECK_EQ(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
}

TEST_CASE("liblog - change current Severity") {
	auto file = std::make_shared<std::ostringstream>();
	log::set_output_location(file);
	// Default Severity is note.
	CHECK_EQ(log::current_severity.get(), log::Severity::note);
	log::current_severity.set(log::Severity::fatal_error);
	CHECK_EQ(log::current_severity.get(), log::Severity::fatal_error);

	// Reset current Severity to note.
	log::current_severity.set(log::Severity::note);
}

TEST_CASE("liblog - only print if Severity high enough") {
	auto file = std::make_shared<std::ostringstream>();
	log::set_output_location(file);

	log::Severity s = log::Severity::note;

	SUBCASE("debug") {
		s = log::Severity::debug;
	}
	SUBCASE("info") {
		s = log::Severity::info;
	}
	SUBCASE("note") {
		s = log::Severity::note;
	}
	SUBCASE("warning") {
		s = log::Severity::warning;
	}
	SUBCASE("error") {
		s = log::Severity::error;
	}
	SUBCASE("fatal_error") {
		s = log::Severity::fatal_error;
	}

	log::current_severity.set(s);

	LOG(warning, "{}", "hello");
	std::string logged = file->str();
	std::smatch m;
	if (s == log::Severity::debug || s == log::Severity::info || s == log::Severity::note
	    || s == log::Severity::warning) {
		CHECK(std::regex_match(logged, m, log_regex));
		REQUIRE_EQ(m.size(), 5);
	}
	else {
		CHECK_EQ(logged, ""s);
	}
	// Reset current Severity to note.
	log::current_severity.set(log::Severity::note);
}

TEST_CASE("liblog - write to file") {
	std::string file_name = create_temp_file();
	log::set_output_location(file_name);
	LOG(fatal_error, "{}", "hello");
	// Close file by opening a different stream.
	auto temp_loc = std::make_shared<std::ostringstream>();
	log::set_output_location(temp_loc);

	std::fstream file{file_name};
	std::string logged = parsers::util::load_from_file_utf8_bom(file);
	std::smatch m;
	CHECK(std::regex_match(logged, m, log_regex));
	REQUIRE_EQ(m.size(), 5);
	CHECK_EQ(m[1], "fatal_error"s);
	// LIBBVE_LOG_DEBUG is not defined time and file location should be empty strings.
	CHECK_EQ(m[2], ""s);
	CHECK_EQ(m[3], ""s);
	CHECK_EQ(m[4], "hello\n"s);
	file.close();
	std::filesystem::remove(file_name);
}

TEST_CASE("liblog - only write to file if Severity high enough") {
	std::string file_name = create_temp_file();
	log::set_output_location(file_name);

	log::Severity s = log::Severity::note;
	SUBCASE("debug") {
		s = log::Severity::debug;
	}
	SUBCASE("info") {
		s = log::Severity::info;
	}
	SUBCASE("note") {
		s = log::Severity::note;
	}
	SUBCASE("warning") {
		s = log::Severity::warning;
	}
	SUBCASE("error") {
		s = log::Severity::error;
	}
	SUBCASE("fatal_error") {
		s = log::Severity::fatal_error;
	}

	log::current_severity.set(s);

	LOG(warning, "{}", "hello");

	auto temp_loc = std::make_shared<std::ostringstream>();
	log::set_output_location(temp_loc);

	std::fstream file(file_name);
	std::string logged = parsers::util::load_from_file_utf8_bom(file);
	std::smatch m;
	if (s == log::Severity::debug || s == log::Severity::info || s == log::Severity::note
	    || s == log::Severity::warning) {
		CHECK(std::regex_match(logged, m, log_regex));
		REQUIRE_EQ(m.size(), 5);
	}
	else {
		CHECK_EQ(logged, ""s);
	}
	// Reset current Severity to note.
	log::current_severity.set(log::Severity::note);
	file.close();
	std::filesystem::remove(file_name);
}

TEST_CASE("liblog - thread safety") {
	auto file = std::make_shared<std::stringstream>();
	log::set_output_location(file);
	// Changes the global Severity and LOGs str, the LOG severity will always be note.
	auto change_sev_and_log = [&](std::string const& str, log::Severity s) {
		log::current_severity.set(s);
		LOG(note, "{}", str);
	};

	auto t1 = std::async(std::launch::async, change_sev_and_log, "t1"s, log::Severity::info);
	auto t2 = std::async(std::launch::async, change_sev_and_log, "t2"s, log::Severity::note);
	// t3-5 shouldn't LOG because they change the global Severity to one that is higher than note.
	auto t3 = std::async(std::launch::async, change_sev_and_log, "t3"s, log::Severity::warning);
	auto t4 = std::async(std::launch::async, change_sev_and_log, "t4"s, log::Severity::error);
	auto t5 =
	    std::async(std::launch::async, change_sev_and_log, "t5"s, log::Severity::fatal_error);

	t1.get();
	t2.get();
	t3.get();
	t4.get();
	t5.get();
	std::string logged;

	while (std::getline(*file, logged)) {
		std::smatch m;
		CHECK(std::regex_match(logged, m, log_regex));
		REQUIRE_EQ(m.size(), 5);
		// t3-5 shouldn't LOG because they change the global Severity to one that is higher than
		// note.
		CHECK_NE(m[4], "t3"s);
		CHECK_NE(m[4], "t4"s);
		CHECK_NE(m[4], "t5"s);
	}
	// Reset current Severity to note.
	log::current_severity.set(log::Severity::note);
}

TEST_CASE("liblog - changes to current Severity should be thread safe.") {
	auto change_sev = [](log::Severity s) noexcept {
		log::current_severity.set(s);
	};
	auto t1 = std::async(std::launch::async, change_sev, log::Severity::info);
	auto t2 = std::async(std::launch::async, change_sev, log::Severity::note);
	auto t3 = std::async(std::launch::async, change_sev, log::Severity::warning);
	auto t4 = std::async(std::launch::async, change_sev, log::Severity::error);
	auto t5 = std::async(std::launch::async, change_sev, log::Severity::fatal_error);

	// Need threads to set their Severity before resetting it to note.
	t1.get();
	t2.get();
	t3.get();
	t4.get();
	t5.get();

	// Reset current Severity to note.
	log::current_severity.set(log::Severity::note);
}

#endif
