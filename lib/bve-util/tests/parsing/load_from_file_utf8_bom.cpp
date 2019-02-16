#include "util/parsing.hpp"
#include <doctest.h>
#include <sstream>

using namespace std::string_literals;

#define LOAD_FROM_FILE_COMPARE(input, output)                                                                                              \
	{                                                                                                                                      \
		std::istringstream i(input);                                                                                                       \
		CHECK_EQ(bve::util::parsers::load_from_file_utf8_bom(i), output);                                                                  \
	}

TEST_SUITE_BEGIN("libparsers - util");

TEST_CASE("libparsers - util - load_from_file_utf8_bom - empty file") {
	LOAD_FROM_FILE_COMPARE(""s, ""s);
	LOAD_FROM_FILE_COMPARE("\xEF\xBB\xBF"s, ""s);

	LOAD_FROM_FILE_COMPARE("Hello"s, "Hello"s);
	LOAD_FROM_FILE_COMPARE("\xEF\xBB\xBFHello"s, "Hello"s);

	LOAD_FROM_FILE_COMPARE(
	    "Hello\n"
	    "\xEF\xBB\xBF"s,
	    "Hello\n"
	    "\xEF\xBB\xBF"s);
	LOAD_FROM_FILE_COMPARE(
	    "\xEF\xBB\xBFHello\n"
	    "\xEF\xBB\xBF"s,
	    "Hello\n"
	    "\xEF\xBB\xBF"s);

	LOAD_FROM_FILE_COMPARE(
	    "Hello\r\n"
	    "Bello"s,
	    "Hello\n"
	    "Bello"s);
}

TEST_SUITE_END();
