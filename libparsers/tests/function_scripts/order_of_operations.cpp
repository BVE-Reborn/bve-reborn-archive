#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("order-of-operations") {
	parsers::function_scripts::parse("1 + 1");
}

TEST_SUITE_END();
