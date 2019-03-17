#include <doctest/doctest.h>
#include <parsers/errors.hpp>
#include <sstream>

TEST_SUITE_BEGIN("libparsers - errors");

TEST_CASE("libparsers - errors - errors::Errors - add single error") {
	bve::parsers::errors::Errors error_list;

	constexpr std::intmax_t line = 59;

	SUBCASE("String") {
		add_error(error_list, line, "Hello");

		REQUIRE_EQ(error_list.size(), 1);
		CHECK_EQ(error_list[0].line, line);
		CHECK_EQ(error_list[0].error, "Hello");
	}

	SUBCASE("StringStream") {
		std::ostringstream oss;

		oss << "Hello";

		add_error(error_list, line, oss);

		REQUIRE_EQ(error_list.size(), 1);
		CHECK_EQ(error_list[0].line, line);
		CHECK_EQ(error_list[0].error, "Hello");
	}
}

TEST_CASE("libparsers - errors - errors::MultiError - add single error") {
	bve::parsers::errors::MultiError error_list;

	constexpr char filename[] = "test.csv";
	constexpr std::intmax_t line = 59;

	SUBCASE("String") {
		add_error(error_list, filename, line, "Hello");

		REQUIRE_EQ(error_list.size(), 1);
		REQUIRE_EQ(error_list.count(filename), 1);
		REQUIRE_EQ(error_list[filename].size(), 1);
		CHECK_EQ(error_list[filename][0].line, line);
		CHECK_EQ(error_list[filename][0].error, "Hello");
	}

	SUBCASE("StringStream") {
		std::ostringstream oss;

		oss << "Hello";

		add_error(error_list, filename, line, oss);

		REQUIRE_EQ(error_list.size(), 1);
		REQUIRE_EQ(error_list.count(filename), 1);
		REQUIRE_EQ(error_list[filename].size(), 1);
		CHECK_EQ(error_list[filename][0].line, line);
		CHECK_EQ(error_list[filename][0].error, "Hello");
	}
}

TEST_SUITE_END();
