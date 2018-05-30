#include "parsers/xml/dynamic_lighting.hpp"
#include <doctest.h>
#include <iostream>

using namespace parsers::xml;
using dynamic_lighting::lighting_info;
using parsers::errors::error_t;
using parsers::errors::errors_t;

TEST_SUITE_BEGIN("libparsers - xml - dynamic_lighting");

TEST_CASE("libparsers - xml - dynamic_lighting - test") {
	std::string const test_value =
	    "<?xml version=\"1.0\" encoding=\"utf - 8\"?>"
	    "<openBVE xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsd="
	    "\"http://www.w3.org/2001/XMLSchema\">"
	    "<Brightness>"
	    "<Time>10.00</Time>"
	    "<AmbientLight>1, 1, 1</AmbientLight>"
	    "<DirectionalLight>1, 1, 1</DirectionalLight>"
	    "<LightDirection>1.0, 1.0, 1.0</LightDirection>"
	    "<CabLighting>80</CabLighting>"
	    "</Brightness>"
	    "<Brightness>"
	    "<Time>12.00</Time>"
	    "<AmbientLight>160, 160, 160</AmbientLight>"
	    "<DirectionalLight>160, 160, 160</DirectionalLight>"
	    "<LightDirection>-0.5, -0.5, -0.5</LightDirection>"
	    "<CabLighting>255</CabLighting>"
	    "</Brightness>"
	    "</openBVE>";

	parsers::errors::multi_error_t output_errors;
	auto const output = dynamic_lighting::parse("some_file.xml", test_value, output_errors);

	// The if here is needed to check if there's any errors to report
	// Because otherwise it would print the filename
	if (!output_errors.empty()) {
		std::cout << output_errors;
	}
	REQUIRE(output_errors.empty());

	// Check that the struct values match
	CHECK_EQ(output[0].ambient, openbve2::datatypes::color8_rgb{1, 1, 1});
	CHECK_EQ(output[0].cablighting, 80);
	CHECK_EQ(output[0].directional_lighting, openbve2::datatypes::color8_rgb{1, 1, 1});
	CHECK_EQ(output[0].light_direction, glm::vec3{1.0, 1.0, 1.0});
	CHECK_EQ(output[0].time, 3600 * 10);

	CHECK_EQ(output[1].ambient, openbve2::datatypes::color8_rgb{160, 160, 160});
	CHECK_EQ(output[1].cablighting, 255);
	CHECK_EQ(output[1].directional_lighting, openbve2::datatypes::color8_rgb{160, 160, 160});
	CHECK_EQ(output[1].light_direction, glm::vec3{-0.5f, -0.5f, -0.5f});
	CHECK_EQ(output[1].time, 3600 * 12);
}

TEST_CASE("libparsers - xml - dynamic_lighting - insane_values") {
	std::string const test_value =
	    "<?xml version=\"1.0\" encoding=\"utf - 8\"?>"
	    "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
	    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
	    "<SomeOtherTag>" // This is not <Brightness>, so that the parser doesn't read this by
	                     // accident
	    "<Time>sghkjthj</Time>"
	    "<AmbientLight>text</AmbientLight>"
	    "<DirectionalLight>1,</DirectionalLight>"
	    "<LightDirection>-15gfajhk</LightDirection>"
	    "<CabLighting>5000000000</CabLighting>"
	    "<UselessTag>someuselessvalue</UselessTag>"
	    "</SomeOtherTag>"
	    "<Brightness>"
	    "<Time>sghkjthj</Time>"
	    "<AmbientLight>text</AmbientLight>"
	    "<DirectionalLight>1,</DirectionalLight>"
	    "<LightDirection>sdjgh57nfd, hfjidr, dsguah</LightDirection>"
	    "<CabLighting>5000000000</CabLighting>"
	    "<UselessTag>someuselessvalue</UselessTag>"
	    "</Brightness>"
	    "</openBVE>";

	parsers::errors::multi_error_t output_errors;
	auto const output = dynamic_lighting::parse("some_file.xml", test_value, output_errors);

	REQUIRE_EQ(output_errors["some_file.xml"].size(), 4);

	CHECK_EQ(output[0].ambient, openbve2::datatypes::color8_rgb{160, 160, 160});
	CHECK_EQ(output[0].cablighting, 0);
	CHECK_EQ(output[0].directional_lighting, openbve2::datatypes::color8_rgb{160, 160, 160});
	CHECK_EQ(output[0].light_direction, glm::vec3{-0.223645424f, -0.866025404f, 0.44719428f});
	CHECK_EQ(output[0].time, 0);
}

TEST_SUITE_END();
