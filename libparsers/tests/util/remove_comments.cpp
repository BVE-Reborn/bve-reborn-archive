#include "utils.hpp"
#include <doctest.h>
#include <limits>

using namespace std::string_literals;

#define REMOVE_COMMENTS_CHECK_EQ(input_string, comment_char, first_in_line, output_string)         \
	{                                                                                              \
		std::string s = input_string;                                                              \
		parsers::util::remove_comments(s, comment_char, first_in_line);                            \
		CHECK_EQ(s, output_string);                                                                \
	}

TEST_SUITE_BEGIN("libparsers - util");

TEST_CASE("libparsers - util - remove_comments") {
	REMOVE_COMMENTS_CHECK_EQ("hello world"s, ';', false, "hello world");
	REMOVE_COMMENTS_CHECK_EQ("hello world;"s, ';', false, "hello world");
	REMOVE_COMMENTS_CHECK_EQ(";hello world"s, ';', false, "");
	REMOVE_COMMENTS_CHECK_EQ("hello; world"s, ';', false, "hello");

	REMOVE_COMMENTS_CHECK_EQ(
	    "hello world\n"
	    "hello world"s,
	    ';', true,
	    "hello world\n"
	    "hello world");
	REMOVE_COMMENTS_CHECK_EQ(
	    "hello world\n"
	    "hello world;"s,
	    ';', true,
	    "hello world\n"
	    "hello world;");
	REMOVE_COMMENTS_CHECK_EQ(
	    "hello world\n"
	    ";hello world"s,
	    ';', true,
	    "hello world\n"
	    "");
	REMOVE_COMMENTS_CHECK_EQ(
	    "hello world\n"
	    "hello; world"s,
	    ';', true,
	    "hello world\n"
	    "hello; world");

	REMOVE_COMMENTS_CHECK_EQ("hello; world"s, ',', false, "hello; world");
	REMOVE_COMMENTS_CHECK_EQ("hello; world,"s, ',', false, "hello; world");
	REMOVE_COMMENTS_CHECK_EQ(",hello; world"s, ',', false, "");
	REMOVE_COMMENTS_CHECK_EQ("hello,; world"s, ',', false, "hello");
}

TEST_SUITE_END();
