#include "parsers/xml/dynamic_background.hpp"
#include "utils.hpp"
#include <doctest.h>
#include <iostream>

using namespace parsers::xml;
using parsers::errors::error_t;
using parsers::errors::errors_t;
using namespace openbve2::datatypes;

TEST_SUITE_BEGIN("libparsers - xml - dynamic_background");

TEST_CASE("libparser - xml - dynamic_background - test single backgound") {
	// clang-format off
    std::string const test_single_background = 
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<openBVE>"
            "<Background>"
                "<Time>00.00</Time>"
                "<Mode>FadeIn</Mode>"
                "<Repetitions>6</Repetitions>"
                "<Texture>Cloudy.png</Texture>"
            "</Background>"
        "</openBVE>";
	// clang-format on
	parsers::errors::multi_error_t output_errors;
	auto lm = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
	auto const output = dynamic_background::parse("some_file.xml", test_single_background, output_errors, lm);

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
	REQUIRE(output_errors.empty());
    
    REQUIRE(output.is<std::vector<dynamic_background::texture_background_info>>());
    auto& background = output.get_unchecked<std::vector<dynamic_background::texture_background_info>>();
    CHECK_EQ(background.size(), 1);

	CHECK_EQ(background[0].time, 0);
	CHECK_EQ(background[0].repetitions, 6);
	CHECK_EQ(background[0].transition_mode, dynamic_background::texture_background_info::fade_in);
	CHECK_EQ(background[0].filename, "some_file.xml/Cloudy.png");

}

TEST_CASE("libparser - xml - dynamic_background - test multiple backgound") {
	// clang-format off
    std::string const test_multiple_backgrounds = 
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<openBVE>"
            "<Background>"
                "<Time>00.00</Time>"
                "<Mode>FadeIn</Mode>"
                "<Repetitions>6</Repetitions>"
                "<Texture>Cloudy.png</Texture>"
            "</Background>"
            "<Background>"
                "<Time>10.00</Time>"
                "<Mode>FadeOut</Mode>"
                "<Repetitions>6</Repettions>"
                "<Texture>Sunny.png</Texture>"
                "<TransitionTime>10</TransitionTime>"
            "</Background>"
        "</openBVE>";
	// clang-format on

	parsers::errors::multi_error_t output_errors;
	auto lm = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
	auto const output = dynamic_background::parse("some_file.xml", test_multiple_backgrounds, output_errors, lm);

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
	REQUIRE(output_errors.empty());

	// Parsed values should match.
	REQUIRE(output.is<std::vector<dynamic_background::texture_background_info>>());
	auto& vec = output.get_unchecked<std::vector<dynamic_background::texture_background_info>>();
	CHECK_GT(vec.size(), 0);

	CHECK_EQ(vec[0].time, 0);
	CHECK_EQ(vec[0].repetitions, 6);
	CHECK_EQ(vec[0].transition_mode, dynamic_background::texture_background_info::fade_in);
	CHECK_EQ(vec[0].filename, "some_file.xml/Cloudy.png");

	CHECK_EQ(vec[1].time, parsers::util::parse_time("10.00"));
	CHECK_EQ(vec[1].repetitions, 6);
	CHECK_EQ(vec[1].transition_mode, dynamic_background::texture_background_info::fade_out);
	CHECK_EQ(vec[1].filename, "some_file.xml/Sunny.png");
	CHECK_EQ(vec[1].transition_time, 10);


}