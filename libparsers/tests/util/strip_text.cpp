#include "utils.hpp"
#include <doctest.h>

using namespace std::string_literals;

#define STRIP_TEXT_CHECK_EQ(input_string, output_string)                                           \
	{                                                                                              \
		std::string s = input_string;                                                              \
		parsers::util::strip_text(s);                                                              \
		CHECK_EQ(s, output_string);                                                                \
	}

TEST_SUITE_BEGIN("libparsers - util");

TEST_CASE("libparsers - util - strip_text") {
	STRIP_TEXT_CHECK_EQ(""s, ""s);
	STRIP_TEXT_CHECK_EQ(" \t\n\v\f\r Hello! \t\n\v\f\r "s, "Hello!"s);
	STRIP_TEXT_CHECK_EQ("       "s, ""s);
	STRIP_TEXT_CHECK_EQ("       Before"s, "Before"s);
	STRIP_TEXT_CHECK_EQ("After       "s, "After"s);
	STRIP_TEXT_CHECK_EQ("Everything"s, "Everything"s);
	STRIP_TEXT_CHECK_EQ("   But not in der Mitte   "s, "But not in der Mitte"s);
}

TEST_SUITE_END();
