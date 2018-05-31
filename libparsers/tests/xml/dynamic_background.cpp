#include "parsers/xml/dynamic_background.hpp"
#include "utils.hpp"
#include <doctest.h>
#include <iostream>

<<<<<<< HEAD
using namespace parsers::xml;
using parsers::errors::error_t;
using parsers::errors::errors_t;
using namespace openbve2::datatypes;

TEST_SUITE_BEGIN("libparsers - xml - dynamic_background");

TEST_CASE("libparser - xml - dynamic_background - test single backgound") {
=======
namespace db = parsers::xml::dynamic_background;
using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - xml - dynamic_background");

TEST_CASE("libparser - xml - dynamic_background - single backgound") {
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
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
<<<<<<< HEAD
        "</openBVE>";
=======
        "</openBVE>"s;
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
	// clang-format on
	parsers::errors::multi_error_t output_errors;
	auto rel_file_func = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
<<<<<<< HEAD
	auto const output = dynamic_background::parse("some_file.xml", test_single_background,
	                                              output_errors, rel_file_func);
=======
	auto const output =
	    db::parse("some_file.xml"s, test_single_background, output_errors, rel_file_func);
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
<<<<<<< HEAD
	REQUIRE(output_errors.empty());

	REQUIRE(output.is<std::vector<dynamic_background::texture_background_info>>());
	auto& background =
	    output.get_unchecked<std::vector<dynamic_background::texture_background_info>>();
	CHECK_EQ(background.size(), 1);

	CHECK_EQ(background[0].time, 0);
	CHECK_EQ(background[0].repetitions, 6);
	CHECK_EQ(background[0].transition_mode, dynamic_background::texture_background_info::fade_in);
	CHECK_EQ(background[0].filename, "some_file.xml/Cloudy.png");
}

TEST_CASE("libparser - xml - dynamic_background - test multiple backgound") {
=======
	CHECK(output_errors.empty());

	REQUIRE(output.is<std::vector<db::texture_background_info>>());
	auto const& background = output.get_unchecked<std::vector<db::texture_background_info>>();
	REQUIRE_EQ(background.size(), 1);

	CHECK_EQ(background[0].time, 0);
	CHECK_EQ(background[0].repetitions, 6);
	CHECK_EQ(background[0].transition_mode, db::texture_background_info::fade_in);
	CHECK_EQ(background[0].filename, "some_file.xml/Cloudy.png"s);
}

TEST_CASE("libparser - xml - dynamic_background - multiple backgounds") {
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
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
<<<<<<< HEAD
        "</openBVE>";
=======
        "</openBVE>"s;
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
	// clang-format on

	parsers::errors::multi_error_t output_errors;
	auto rel_file_func = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
<<<<<<< HEAD
	auto const output = dynamic_background::parse("some_file.xml", test_multiple_backgrounds,
	                                              output_errors, rel_file_func);
=======
	auto const output =
	    db::parse("some_file.xml"s, test_multiple_backgrounds, output_errors, rel_file_func);
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
<<<<<<< HEAD
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
=======
	CHECK(output_errors.empty());

	// Parsed values should match.
	REQUIRE(output.is<std::vector<db::texture_background_info>>());
	auto const& vec = output.get_unchecked<std::vector<db::texture_background_info>>();
	REQUIRE_EQ(vec.size(), 2);

	CHECK_EQ(vec[0].time, 0);
	CHECK_EQ(vec[0].repetitions, 6);
	CHECK_EQ(vec[0].transition_mode, db::texture_background_info::fade_in);
	CHECK_EQ(vec[0].filename, "some_file.xml/Cloudy.png"s);

	CHECK_EQ(vec[1].time, 36000);
	CHECK_EQ(vec[1].repetitions, 6);
	CHECK_EQ(vec[1].transition_mode, db::texture_background_info::fade_out);
	CHECK_EQ(vec[1].filename, "some_file.xml/Sunny.png"s);
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
	CHECK_EQ(vec[1].transition_time, 10);
}

TEST_CASE("libparser - xml - dynamic_background - object_background") {
	// clang-format off
    std::string const test_object_background =
<<<<<<< HEAD
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
                "<Background>"
                    "<Object>Background.csv</Object>"
                "</Background>"
            "</openBVE>";
=======
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
			"<Background>"
				"<Object>Background.csv</Object>"
			"</Background>"
		"</openBVE>"s;
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
	// clang-format on

	parsers::errors::multi_error_t output_errors;
	auto rel_file_func = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
<<<<<<< HEAD
	auto const output = dynamic_background::parse("some_file.xml", test_object_background,
	                                              output_errors, rel_file_func);
=======
	auto const output =
	    db::parse("some_file.xml"s, test_object_background, output_errors, rel_file_func);
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9

	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
<<<<<<< HEAD
	REQUIRE(output_errors.empty());

	REQUIRE(output.is<dynamic_background::object_background_info>());
	auto& object_background = output.get_unchecked<dynamic_background::object_background_info>();

	CHECK_EQ(object_background.filename, "some_file.xml/Background.csv");
=======
	CHECK(output_errors.empty());

	REQUIRE(output.is<db::object_background_info>());
	auto const& object_background = output.get_unchecked<db::object_background_info>();

	CHECK_EQ(object_background.filename, "some_file.xml/Background.csv"s);
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
}

TEST_CASE("libparser - xml - dynamic_background - should allow only one object_background") {
	// clang-format off
    std::string const test_object_background =
<<<<<<< HEAD
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
                "<Background>"
                    "<Object>Background.csv</Object>"
                "</Background>"
                "<Background>"
                    "<Object>Background2.csv</Object>"
                "</Background>"
            "</openBVE>";
=======
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

	parsers::errors::multi_error_t output_errors;
	auto rel_file_func = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
	auto const output =
	    db::parse("some_file.xml"s, test_object_background, output_errors, rel_file_func);

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
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
	// clang-format on

	parsers::errors::multi_error_t output_errors;
	auto rel_file_func = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
<<<<<<< HEAD
	auto const output = dynamic_background::parse("some_file.xml", test_object_background,
	                                              output_errors, rel_file_func);

	REQUIRE(!output_errors.empty());
	CHECK_EQ(output_errors["some_file.xml"][0].error,
	         "Multiple Object backgrounds: only one object background is allowed.");
=======
	auto const out =
	    db::parse("some_file.xml"s, test_single_background, output_errors, rel_file_func);
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

	parsers::errors::multi_error_t output_errors;
	auto rel_file_func = [](std::string const& base, std::string const& relative) {
		return base + "/" + relative;
	};
	auto const out =
	    db::parse("some_file.xml"s, test_single_background, output_errors, rel_file_func);
	CHECK(!output_errors.empty());
	CHECK_EQ(output_errors.at("some_file.xml"s).size(), 3);
>>>>>>> 998cf3ef5be7d6ca66c60e58d6e8d851f25427b9
}
