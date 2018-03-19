#include <doctest.h>
#include <parsers/errors.hpp>
#include <sstream>

TEST_SUITE_BEGIN("libparsers - error");

TEST_CASE("errors::error_t - print error") {
	std::ostringstream oss;
	parsers::errors::error_t err{59, "Hello"};

	oss << err;

	CHECK_EQ(oss.str(), "59: Hello");
}

TEST_CASE("errors::errors_t - print single error") {
	std::ostringstream oss;
	parsers::errors::errors_t err;

	parsers::errors::add_error(err, 59, "Hello");

	oss << err;

	CHECK_EQ(oss.str(), "59: Hello\n");
}

TEST_CASE("errors::errors_t - print multiple errors") {
	std::ostringstream oss;
	parsers::errors::errors_t err;

	parsers::errors::add_error(err, 59, "Hello");
	parsers::errors::add_error(err, -1, "Bello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "-1: Bello\n"
	         "59: Hello\n");
}

TEST_CASE("errors::multi_error_t - print single error") {
	std::ostringstream oss;
	parsers::errors::multi_error_t err;

	parsers::errors::add_error(err, "/path/to/file", 59, "Hello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "/path/to/file:\n"
	         "\t59: Hello\n");
}

TEST_CASE("errors::multi_error_t - print multiple errors") {
	std::ostringstream oss;
	parsers::errors::multi_error_t err;

	parsers::errors::add_error(err, "/path/to/file", 59, "Hello");
	parsers::errors::add_error(err, "/path/to/file", -1, "Bello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "/path/to/file:\n"
	         "\t-1: Bello\n"
	         "\t59: Hello\n");
}

TEST_CASE("errors::multi_error_t - print from multiple files") {
	std::ostringstream oss;
	parsers::errors::multi_error_t err;

	parsers::errors::add_error(err, "/path/to/file", 59, "Hello");
	parsers::errors::add_error(err, "/path/to/file2", -1, "Bello");

	oss << err;

	CHECK_EQ(oss.str(),
	         "/path/to/file:\n"
	         "\t59: Hello\n"
	         "/path/to/file2:\n"
	         "\t-1: Bello\n");
}

TEST_SUITE_END();
