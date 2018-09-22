#include "parsers/utils.hpp"
#include <doctest.h>

using namespace std::string_literals;

#define SPLIT_TEXT_COMPARE(lhs, ...) CHECK_EQ(lhs, std::vector<std::string>{__VA_ARGS__})

TEST_SUITE_BEGIN("libparsers - util");

TEST_CASE("libparsers - util - split_text") {
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello"s), "Hello"s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello,"s), "Hello"s, ""s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello, World"s), "Hello"s, " World"s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello, ,World"s), "Hello"s, " "s, "World"s);

	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello"s, ';'), "Hello"s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello;,"s, ';'), "Hello"s, ","s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello; World"s, ';'), "Hello"s, " World"s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello;,;World"s, ';'), "Hello"s, ","s,
	                   "World"s);

	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello"s, ';', true), "Hello"s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello;,"s, ';', true), "Hello"s, ","s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello;; World"s, ';', true), "Hello"s,
	                   " World"s);
	SPLIT_TEXT_COMPARE(bve::parsers::util::split_text("Hello;,;;World"s, ';', true), "Hello"s, ","s,
	                   "World"s);
}

TEST_SUITE_END();
