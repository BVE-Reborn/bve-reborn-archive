//
// Created by kevin Cybura on 9/1/18.
//

#include <doctest.h>
#include <include/config/panel2_cfg.h>
#include <include/parsers/errors.hpp>

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - panel2_cfg");

TEST_CASE("libparsers - panel2_cfg - this section - should parse without errors") {
	std::string file =
	    "[This] \n"
	    "Resolution = 32.2\n"
	    "Left = 32.2\n"
	    "Right = 32.2\n"
	    "Top = 32.2\n"
	    "Bottom = 32.2\n"
	    "DaytimeImage = day.png\n"
	    "NighttimeImage = night.png\n"
	    "TransparentColor = #000000\n"
	    "Center = 10.2, 10\n"
	    "Origin = .1, 0"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["This"].size(), 0);
	CHECK_EQ(panel2.section_this.resolution, 32.2f);
	CHECK_EQ(panel2.section_this.left, 32.2f);
	CHECK_EQ(panel2.section_this.right, 32.2f);
	CHECK_EQ(panel2.section_this.top, 32.2f);
	CHECK_EQ(panel2.section_this.bottom, 32.2f);
	CHECK_EQ(panel2.section_this.day_time_image, "day.png"s);
	CHECK_EQ(panel2.section_this.night_time_image, "night.png"s);
	CHECK_EQ(panel2.section_this.transparent_color,
	         parsers::panel2::sections::color(0, 0, 0, 0xFF));
	CHECK_EQ(panel2.section_this.center.first, 10.2f);
	CHECK_EQ(panel2.section_this.center.second, 10);
	CHECK_EQ(panel2.section_this.origin.first, .1f);
	CHECK_EQ(panel2.section_this.origin.second, 0);
}

TEST_CASE("libparsers - panel2_cfg - this section - should parse with all errors") {
	std::string file =
	    "[This] \n"
	    "Resolution = A\n"
	    "Left = B\n"
	    "Right = C\n"
	    "Top = D\n"
	    "Bottom = E\n"
	    "DaytimeImage = \n"
	    "NighttimeImage = night.png\n"
	    "TransparentColor = a\n"
	    "Center = c, a\n"
	    "Origin = c, b"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["This"].size(), 9);
}

TEST_CASE("libparsers - panel2_cfg - pilotlamp section - should parse without errors") {
	std::string file =
	    "[PilotLamp] \n"
	    "Subject = Subject\n"
	    "Location = 212.2, 21.2\n"
	    "DaytimeImage = day.png\n"
	    "NighttimeImage = night.png\n"
	    "TransparentColor = #000000\n"
	    "Layer = 2"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["panel2.cfg"].size(), 0);
	CHECK_EQ(panel2.pilot_lamp.location.first, 212.2f);
	CHECK_EQ(panel2.pilot_lamp.location.second, 21.2f);
	CHECK_EQ(panel2.pilot_lamp.day_time_image, "day.png"s);
	CHECK_EQ(panel2.pilot_lamp.night_time_image, "night.png"s);
	CHECK_EQ(panel2.pilot_lamp.transparent_color, parsers::panel2::sections::color(0, 0, 0, 0xFF));
	CHECK_EQ(panel2.pilot_lamp.layer, 2);
}

TEST_CASE("libparsers - panel2_cfg - pilotlamp section - should parse with all errors") {
	std::string file =
	    "[PilotLamp] \n"
	    "Subject = Subject\n"
	    "Location = Left, Top\n"
	    "DaytimeImage = \n"
	    "NighttimeImage = day.png\n"
	    "TransparentColor = HexColor\n"
	    "Layer = LayerIndex"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["PilotLamp"].size(), 4);
}

TEST_CASE("libparsers - panel2_cfg - needle section - should parse without errors") {
	std::string file =
	    "[Needle] \n"
	    "Subject = Subject\n"
	    "Location = 32.2, 32.2\n"
	    "Radius = 32.2\n"
	    "DaytimeImage = day.png\n"
	    "NighttimeImage = night.png\n"
	    "Color = #000000\n"
	    "TransparentColor = #000000\n"
	    "Origin = 32.2, 32.2\n"
	    "InitialAngle = 32.2\n"
	    "LastAngle = 32.2\n"
	    "Minimum = 32.2\n"
	    "Maximum = 32.2\n"
	    "NaturalFreq = 32.2\n"
	    "DampingRatio = 32.2\n"
	    "Layer = 15"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["Needle"].size(), 0);
	CHECK_EQ(panel2.needle.location.first, 32.2f);
	CHECK_EQ(panel2.needle.location.second, 32.2f);
	CHECK_EQ(panel2.needle.radius, 32.2f);
	CHECK_EQ(panel2.needle.day_time_image, "day.png"s);
	CHECK_EQ(panel2.needle.night_time_image, "night.png"s);
	CHECK_EQ(panel2.needle.color_, parsers::panel2::sections::color(0, 0, 0, 0xFF));
	CHECK_EQ(panel2.needle.transparent_color, parsers::panel2::sections::color(0, 0, 0, 0xFF));
	CHECK_EQ(panel2.needle.origin.first, 32.2f);
	CHECK_EQ(panel2.needle.origin.second, 32.2f);
	CHECK_EQ(panel2.needle.initial_angle, 32.2f);
	CHECK_EQ(panel2.needle.last_angle, 32.2f);
	CHECK_EQ(panel2.needle.layer, 15);
}

TEST_CASE("libparsers - panel2_cfg - needle section - should parse with all errors") {
	std::string file =
	    "[Needle] \n"
	    "Subject = Subject\n"
	    "Location = CenterX, CenterY\n"
	    "Radius = ValueInPixels\n"
	    "DaytimeImage = \n"
	    "NighttimeImage = day.png\n"
	    "Color = HexColor\n"
	    "TransparentColor = HexColor\n"
	    "Origin = X, Y\n"
	    "InitialAngle = ValueInDegrees\n"
	    "LastAngle = ValueInDegrees\n"
	    "Minimum = Value\n"
	    "Maximum = Value\n"
	    "NaturalFreq = Value\n"
	    "DampingRatio = Value\n"
	    "Layer = LayerIndex"s;

	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["Needle"].size(), 12);
}

TEST_CASE("libparsers - panel2_cfg - digital_number section - should parse without errors") {
	std::string file =
	    "[DigitalNumber]\n"
	    "Subject = Subject\n"
	    "Location = 32.2, 32.2\n"
	    "DaytimeImage = day.png\n"
	    "NighttimeImage = night.png\n"
	    "TransparentColor = #000000\n"
	    "Interval = 32\n"
	    "Layer = 15"s;

	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["DigitalNumber"].size(), 0);
	CHECK_EQ(panel2.number.location.first, 32.2f);
	CHECK_EQ(panel2.number.location.second, 32.2f);
	CHECK_EQ(panel2.number.day_time_image, "day.png"s);
	CHECK_EQ(panel2.number.night_time_image, "night.png"s);
	CHECK_EQ(panel2.number.transparent_color, parsers::panel2::sections::color(0, 0, 0, 0xFF));
	CHECK_EQ(panel2.number.interval, 32);
	CHECK_EQ(panel2.number.layer, 15);
}

TEST_CASE("libparsers - panel2_cfg - digital_number section - should parse with all errors") {
	std::string file =
	    "[DigitalNumber] \n"
	    "Subject = Subject\n"
	    "Location = Left, Top\n"
	    "DaytimeImage = \n"
	    "NighttimeImage = day.png\n"
	    "TransparentColor = HexColor\n"
	    "Interval = Height\n"
	    "Layer = LayerIndex"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["DigitalNumber"].size(), 5);
}

TEST_CASE("libparsers - panel2_cfg - digital_gauge section - should parse with out errors") {
	std::string file =
	    "[DigitalGauge] \n"
	    "Subject = Subject\n"
	    "Location = 32.2, 32.2\n"
	    "Radius = 32.2\n"
	    "Color = #000000\n"
	    "InitialAngle = 32.2\n"
	    "LastAngle = 32.2\n"
	    "Minimum = 32.2\n"
	    "Maximum = 32.2\n"
	    "Step = 32.2\n"
	    "Layer = 1"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["DigitalGauge"].size(), 0);
	CHECK_EQ(panel2.gauge.location.first, 32.2f);
	CHECK_EQ(panel2.gauge.location.second, 32.2f);
	CHECK_EQ(panel2.gauge.radius, 32.2f);
	CHECK_EQ(panel2.gauge.color_, parsers::panel2::sections::color(0, 0, 0, 0xFF));
	CHECK_EQ(panel2.gauge.initial_angle, 32.2f);
	CHECK_EQ(panel2.gauge.last_angle, 32.2f);
	CHECK_EQ(panel2.gauge.max, 32.2f);
	CHECK_EQ(panel2.gauge.min, 32.2f);
	CHECK_EQ(panel2.gauge.step, 32.2f);
	CHECK_EQ(panel2.gauge.layer, 1);
}

TEST_CASE("libparser - panel2_cfg - digital_gauge section - should parse with all errors") {
	std::string file =
	    "[DigitalGauge] \n"
	    "Subject = Subject\n"
	    "Location = hi, no\n"
	    "Radius = k\n"
	    "Color = \n"
	    "InitialAngle = angle\n"
	    "LastAngle = angle\n"
	    "Minimum = min\n"
	    "Maximum = max\n"
	    "Step = step\n"
	    "Layer = -1"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["DigitalGauge"].size(), 8);
}

TEST_CASE("libparsers - panel2_cfg - linear_gauge section - should parse with out errors") {
	std::string file =
	    "[LinearGauge] \n"
	    "Subject = Subject\n"
	    "Location = 32.2, 32.2\n"
	    "Minimum = 32.2\n"
	    "Maximum = 32.2\n"
	    "Direction = 32.2, 32.2\n"
	    "Width = 1\n"
	    "Layer = 1"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["LinearGauge"].size(), 0);
	CHECK_EQ(panel2.linear.location.first, 32.2f);
	CHECK_EQ(panel2.linear.location.second, 32.2f);
	CHECK_EQ(panel2.linear.max, 32.2f);
	CHECK_EQ(panel2.linear.min, 32.2f);
	CHECK_EQ(panel2.linear.direction.first, 32.2f);
	CHECK_EQ(panel2.linear.direction.second, 32.2f);
	CHECK_EQ(panel2.linear.width, 1);
	CHECK_EQ(panel2.linear.layer, 1);
}

TEST_CASE("libparser - panel2_cfg - linear_gauge section - should parse with all errors") {
	std::string file =
	    "[LinearGauge] \n"
	    "Subject = Subject\n"
	    "Location = hi, no\n"
	    "Minimum = min\n"
	    "Maximum = max\n"
	    "Direction = hi, no\n"
	    "Width = step\n"
	    "Layer = -1"s;
	parsers::errors::multi_error_t errors;
	auto panel2 = parsers::panel2::parse_panel2_cfg(file, "panel2.cfg"s, errors);
	CHECK_EQ(errors["LinearGauge"].size(), 5);
}
TEST_SUITE_END();