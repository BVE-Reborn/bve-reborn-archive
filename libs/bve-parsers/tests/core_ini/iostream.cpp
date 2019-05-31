#include "parsers/ini.hpp"
#include <doctest/doctest.h>
#include <ostream>
#include <sstream>

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - core_ini");

TEST_CASE("libparsers - core_ini - iostream") {
	bve::parsers::ini::ParsedINIObject const test_input{{
	                                                        ""s,                     //
	                                                        {{"hello"s, 1}},         //
	                                                        {{"key"s, "value"s, 2}}, //
	                                                        0                        //
	                                                    },
	                                                    {
	                                                        "sec1"s,                                          //
	                                                        {{"world"s, 3}, {"hi"s, 5}},                      //
	                                                        {{"key1"s, "value"s, 4}, {"key2"s, "value"s, 6}}, //
	                                                        3                                                 //
	                                                    }};

	std::ostringstream output;

	output << test_input;

	CHECK_EQ(output.str(),
	         "Line: 0 Section: \"\"\n"
	         "Line: 1 		\"hello\"\n"
	         "Line: 2 		\"key\" = \"value\"\n"
	         "Line: 3 Section: \"sec1\"\n"
	         "Line: 3 		\"world\"\n"
	         "Line: 5 		\"hi\"\n"
	         "Line: 4 		\"key1\" = \"value\"\n"
	         "Line: 6 		\"key2\" = \"value\"\n"s);
}

TEST_SUITE_END();
