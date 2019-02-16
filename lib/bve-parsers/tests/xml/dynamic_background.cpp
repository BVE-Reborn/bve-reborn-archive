#include "parsers/xml/dynamic_background.hpp"
#include "sample_relative_file_func.hpp"
#include "util/parsing.hpp"
#include <doctest.h>
#include <iostream>

namespace db = bve::parsers::xml::dynamic_background;
using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - xml - dynamic_background");

TEST_CASE("libparser - xml - dynamic_background - single backgound") {
	// clang-format off
    std::string const test_single_background = 
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
            "<Background>"
                "<Time>00.00</Time>"
                "<Mode>FadeIn</Mode>"
                "<Repetitions>6</Repetitions>"
                "<Texture>Cloudy.png</Texture>"
            "</Background>"
        "</openBVE>"s;
	// clang-format on
	bve::parsers::errors::MultiError output_errors;

	auto const output = db::parse("some_file.xml"s, test_single_background, output_errors, rel_file_func);

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
	CHECK(output_errors.empty());

	REQUIRE(output.is<std::vector<db::TextureBackgroundInfo>>());
	auto const& background = output.get_unchecked<std::vector<db::TextureBackgroundInfo>>();
	REQUIRE_EQ(background.size(), 1);

	CHECK_EQ(background[0].time, 0);
	CHECK_EQ(background[0].repetitions, 6);
	CHECK_EQ(background[0].transition_mode, db::TextureBackgroundInfo::fade_in);
	CHECK_EQ(background[0].filename, "some_file.xml/Cloudy.png"s);
}

TEST_CASE("libparser - xml - dynamic_background - multiple backgounds") {
	// clang-format off
    std::string const test_multiple_backgrounds = 
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
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
        "</openBVE>"s;
	// clang-format on

	bve::parsers::errors::MultiError output_errors;

	auto const output = db::parse("some_file.xml"s, test_multiple_backgrounds, output_errors, rel_file_func);

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
	CHECK(output_errors.empty());

	// Parsed values should match.
	REQUIRE(output.is<std::vector<db::TextureBackgroundInfo>>());
	auto const& vec = output.get_unchecked<std::vector<db::TextureBackgroundInfo>>();
	REQUIRE_EQ(vec.size(), 2);

	CHECK_EQ(vec[0].time, 0);
	CHECK_EQ(vec[0].repetitions, 6);
	CHECK_EQ(vec[0].transition_mode, db::TextureBackgroundInfo::fade_in);
	CHECK_EQ(vec[0].filename, "some_file.xml/Cloudy.png"s);

	CHECK_EQ(vec[1].time, 36000);
	CHECK_EQ(vec[1].repetitions, 6);
	CHECK_EQ(vec[1].transition_mode, db::TextureBackgroundInfo::fade_out);
	CHECK_EQ(vec[1].filename, "some_file.xml/Sunny.png"s);
	CHECK_EQ(vec[1].transition_time, 10);
}

TEST_CASE("libparser - xml - dynamic_background - object_background") {
	// clang-format off
    std::string const test_object_background =
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
			"<Background>"
				"<Object>Background.csv</Object>"
			"</Background>"
		"</openBVE>"s;
	// clang-format on

	bve::parsers::errors::MultiError output_errors;

	auto const output = db::parse("some_file.xml"s, test_object_background, output_errors, rel_file_func);

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
	CHECK(output_errors.empty());

	REQUIRE(output.is<db::ObjectBackgroundInfo>());
	auto const& object_background = output.get_unchecked<db::ObjectBackgroundInfo>();

	CHECK_EQ(object_background.filename, "some_file.xml/Background.csv"s);
}

TEST_CASE("libparser - xml - dynamic_background - should allow only one object_background") {
	// clang-format off
    std::string const test_object_background =
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
			"<Background>"
				"<Object>Background.csv</Object>"
			"</Background>"
			"<Background>"
				"<Object>Background2.csv</Object>"
			"</Background>"
		"</openBVE>"s;
	// clang-format on

	bve::parsers::errors::MultiError output_errors;

	auto const output = db::parse("some_file.xml"s, test_object_background, output_errors, rel_file_func);

	CHECK(!output_errors.empty());
	CHECK_EQ(output_errors.size(), 1);
}

TEST_CASE("libparser - xml - dynamic_background - openbve node should be optional") {
	// clang-format off
    std::string const test_single_background = 
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<Background>"
			"<Time>00.00</Time>"
			"<Mode>FadeIn</Mode>"
			"<Repetitions>6</Repetitions>"
			"<Texture>Cloudy.png</Texture>"
		"</Background>"s;
	// clang-format on

	bve::parsers::errors::MultiError output_errors;

	auto const out = db::parse("some_file.xml"s, test_single_background, output_errors, rel_file_func);
	CHECK(output_errors.empty());
}

TEST_CASE("libparsers - xml - dynamic_background - improper values should add an errors") {
	// clang-format off
    std::string const test_single_background = 
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<Background>"
			"<Time>ABC</Time>"// Not a proper Time value
			"<Mode>THIS ISNT A PROPER MODE</Mode>"// Only FadeIn and FadeOut modes supported
			"<Repetitions>-1</Repetitions>"// Repetitions should be positive
			"<Texture>Cloudy.png</Texture>"
		"</Background>"s;
	// clang-format on

	bve::parsers::errors::MultiError output_errors;

	auto const out = db::parse("some_file.xml"s, test_single_background, output_errors, rel_file_func);
	CHECK(!output_errors.empty());
	CHECK_EQ(output_errors.at("some_file.xml"s).size(), 3);
}

TEST_SUITE_END();
