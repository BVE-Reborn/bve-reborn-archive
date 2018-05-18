#include "parsers/xml/dynamic_lighting.hpp"
#include <doctest.h>

using namespace parsers::xml;
using dynamic_lighting::lighting_info;
using parsers::errors::errors_t;
using parsers::errors::error_t;
using namespace openbve2::datatypes;

TEST_SUITE_BEGIN("libparsers - xml - dynamic_lighting");

TEST_CASE("libparsers - xml - dynamic_lighting - test") {
	const std::string test_value = "<?xml version=\"1.0\" encoding=\"utf - 8\"?>"
		"<openBVE xmlns : xsi = \"http://www.w3.org/2001/XMLSchema-instance\" xmlns : xsd = \"http://www.w3.org/2001/XMLSchema\">"
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
	const std::vector<lighting_info> output = dynamic_lighting::parse("some_file.xml", test_value, output_errors);

	if (!output_errors.empty()) {
		INFO(output_errors);
		REQUIRE(output_errors.empty());
	}

	//Check that the struct values match
	CHECK_EQ(output[0].ambient, color8_rgb { 1, 1, 1 });
	CHECK_EQ(output[0].cablighting, 80);
	CHECK_EQ(output[0].directional_lighting, color8_rgb { 1, 1, 1 });
	CHECK_EQ(output[0].light_direction, glm::vec3 { 1.0, 1.0, 1.0 });
	CHECK_EQ(output[0].time, 0);

	CHECK_EQ(output[0].ambient, color8_rgb{ 160, 160, 160 });
	CHECK_EQ(output[0].cablighting, 255);
	CHECK_EQ(output[0].directional_lighting, color8_rgb{ 160, 160, 160 });
	CHECK_EQ(output[0].light_direction, glm::vec3{ -0.5, -0.5, -0.5 });
	CHECK_EQ(output[0].time, 12);
}
