#include <doctest.h>
#include <parsers/errors.hpp>
#include <sstream>

TEST_SUITE_BEGIN("libparsers - errors");

TEST_CASE("libparsers - errors - errors::Error - print error") {
	std::ostringstream oss;
	bve::parsers::errors::Error err{59, "Hello"};

	oss << err;

	CHECK_EQ(oss.str(), "59: Hello");
}

TEST_CASE("libparsers - errors - errors::Errors - print single error") {
	std::ostringstream oss;
	bve::parsers::errors::Errors err;

	add_error(err, 59, "Hello");

	oss << err;

	CHECK_EQ(oss.str(), "59: Hello\n");
}

TEST_CASE("libparsers - errors - errors::Errors - print multiple errors") {
	std::ostringstream oss;
	bve::parsers::errors::Errors err;

	add_error(err, 59, "Hello");
	add_error(err, -1, "Bello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "-1: Bello\n"
	         "59: Hello\n");
}

TEST_CASE("libparsers - errors - errors::MultiError - print single error") {
	std::ostringstream oss;
	bve::parsers::errors::MultiError err;

	add_error(err, "/path/to/file", 59, "Hello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "/path/to/file:\n"
	         "\t59: Hello\n");
}

TEST_CASE("libparsers - errors - errors::MultiError - print multiple errors") {
	std::ostringstream oss;
	bve::parsers::errors::MultiError err;

	add_error(err, "/path/to/file", 59, "Hello");
	add_error(err, "/path/to/file", -1, "Bello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "/path/to/file:\n"
	         "\t-1: Bello\n"
	         "\t59: Hello\n");
}

TEST_CASE("libparsers - errors - errors::MultiError - print from multiple files") {
	std::ostringstream oss;
	bve::parsers::errors::MultiError err;

	add_error(err, "/path/to/file", 59, "Hello");
	add_error(err, "/path/to/file2", -1, "Bello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "/path/to/file:\n"
	         "\t59: Hello\n"
	         "/path/to/file2:\n"
	         "\t-1: Bello\n");
}

TEST_SUITE_END();
