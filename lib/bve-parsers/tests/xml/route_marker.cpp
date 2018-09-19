#include "sample_relative_file_func.hpp"
#include "utils.hpp"
#include <doctest.h>
#include <parsers/xml/route_marker.hpp>

using namespace std::string_literals;
namespace rm = bve::parsers::xml::route_marker;

TEST_SUITE_BEGIN("libparser - xml - route_maker");

TEST_CASE("libparsers - xml - route_marker - image based marker") {
	// clang-format off
    std::string const image_based_marker =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
            "<ImageMarker>"
                    "<Early>"
                        "<Time>12:00</Time>"
                        "<Image>Early.png</Image>"
                    "</Early>"
                    "<OnTime>"
                        "<Image>Marker.png</Image>"
                    "</OnTime>"
                    "<Late>"
                        "<Time>12:10</Time>"
                        "<Image>Late.png</Image>"
                    "</Late>"
                    "<Distance>200</Distance>"
                    "<Timeout>12:00</Timeout>"
                    "<trains>81xx_2DCab</trains>"
            "</ImageMarker>"
        "</openBVE>"s;
	// clang-format on
	bve::parsers::errors::MultiError output_errors;
	auto const output =
	    rm::parse("some_file.xml"s, image_based_marker, output_errors, rel_file_func);
	REQUIRE(output.is<rm::image_marker>());
	auto const& image_marker = output.get_unchecked<rm::image_marker>();
	CHECK_EQ(image_marker.early_time, 43200);
	CHECK_EQ(image_marker.early_filename, "some_file.xml/Early.png"s);

	CHECK_EQ(image_marker.on_time_filename, "some_file.xml/Marker.png"s);

	CHECK_EQ(image_marker.late_time, 43800);
	CHECK_EQ(image_marker.late_filename, "some_file.xml/Late.png"s);

	CHECK_EQ(image_marker.distance, 200);
	CHECK_EQ(image_marker.timeout, 12);

	REQUIRE(!image_marker.allowed_trains.empty());
	CHECK_EQ(image_marker.allowed_trains[0], "81xx_2DCab"s);
	CHECK(image_marker.using_early);
	CHECK(image_marker.using_ontime);
	CHECK(image_marker.using_late);
}

TEST_CASE(
    "libparsers - xml - router_marker - incorrect inputs to image based markers should add "
    "errors") {
	// clang-format off
    std::string const image_based_marker =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
            "<ImageMarker>"
                    "<Early>"
                        "<Time>A</Time>"
                        "<Image>Cloudy.png</Image>"
                    "</Early>"
                    "<OnTime>"
                        "<Image>Sunny.png</Image>"
                    "</OnTime>"
                    "<Late>"
                        "<Time>A</Time>"
                        "<Image>Foggy.png</Image>"
                    "</Late>"
                    "<Distance>-1</Distance>"
                    "<Timeout>-1</Timeout>"
                    "<trains></trains>"
            "</ImageMarker>"
        "</openBVE>"s;
	// clang-format on
	bve::parsers::errors::MultiError output_errors;
	auto const output =
	    rm::parse("some_file.xml"s, image_based_marker, output_errors, rel_file_func);
	REQUIRE(output.is<rm::image_marker>());
	auto const& image_marker = output.get_unchecked<rm::image_marker>();
	CHECK_EQ(image_marker.early_time, 0);
	CHECK(!output_errors.empty());
	CHECK_EQ(output_errors["some_file.xml"].size(), 3);
	CHECK(!image_marker.using_early);
	CHECK(image_marker.using_ontime);
	CHECK(!image_marker.using_late);
}

TEST_CASE(
    "libparsers - xml - router_marker - early/late nodes reuqire time and image nodes, ontime "
    "nodes require a image node") {
	// clang-format off
    std::string const image_based_marker =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
                "<ImageMarker>"
                    "<Early>"
                    // No time or image nodes.
                    "</Early>"
                    "<OnTime>"
                    // No nodes.
                    "</OnTime>"
                    "<Late>"
                    // No time or image nodes
                    "</Late>"
                    "<Distance>-1</Distance>" // Isn't an error
                    "<Timeout>-1</Timeout>"
                    "<trains></trains>"
                "</ImageMarker>"
            "</openBVE>"s;
	// clang-format on
	bve::parsers::errors::MultiError output_errors;
	auto const output =
	    rm::parse("some_file.xml"s, image_based_marker, output_errors, rel_file_func);
	REQUIRE(output.is<rm::image_marker>());
	auto const& image_marker = output.get_unchecked<rm::image_marker>();
	CHECK(!output_errors.empty());
	CHECK_EQ(output_errors["some_file.xml"].size(), 4);
	CHECK_EQ(image_marker.early_time, 0);
	CHECK_EQ(image_marker.late_time, 0);
	CHECK_EQ(image_marker.allowed_trains.size(), 0);
	CHECK(!image_marker.using_early);
	CHECK(!image_marker.using_ontime);
	CHECK(!image_marker.using_late);
}

TEST_CASE("libparsers - xml - router_marker - text base marker") {
	// clang-format off
    std::string const text_based_marker =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
            "<TextMarker>"
                "<Early>"
                    "<Time>12:00</Time>"
                    "<Text>Early!</Text>"
                "</Early>"
                "<OnTime>"
                    "<Text>On Time.</Text>"
                "</OnTime>"
                "<Late>"
                    "<Time>12:10</Time>"
                    "<Text>Late...</Text>"
                "</Late>"
                "<Distance>200</Distance>"  // Isn't an error
                "<trains>81xx_2DCab</trains>"
                "<Color>White</Color>"
            "</TextMarker>"
        "</openBVE>"s;
	// clang-format on
	bve::parsers::errors::MultiError output_errors;
	auto const output =
	    rm::parse("some_file.xml"s, text_based_marker, output_errors, rel_file_func);
	REQUIRE(output.is<rm::text_marker>());
	auto const& text_marker = output.get_unchecked<rm::text_marker>();
	CHECK_EQ(text_marker.early_time, 43200);
	CHECK_EQ(text_marker.early_text, "Early!"s);
	CHECK_EQ(text_marker.on_time_text, "On Time."s);
	CHECK_EQ(text_marker.late_time, 43800);
	CHECK_EQ(text_marker.late_text, "Late..."s);
	CHECK_EQ(text_marker.distance, 200);
	CHECK(text_marker.using_early);
	CHECK(text_marker.using_ontime);
	CHECK(text_marker.using_late);
}

TEST_CASE(
    "libparsers - xml - router_marker - early/late nodes require time and text nodes, ontime nodes "
    "require a text node") {
	// clang-format off
    std::string const text_based_marker =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<openBVE xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
                "<TextMarker>"
                    "<Early>"
                    "</Early>"
                    "<OnTime>"
                    "</OnTime>"
                    "<Late>"
                    "</Late>"
                    "<Distance>200</Distance>"
                    "<trains></trains>"
                    "<Color>K</Color>"
                "</TextMarker>"
            "</openBVE>"s;
	// clang-format on
	bve::parsers::errors::MultiError output_errors;
	auto const output =
	    rm::parse("some_file.xml"s, text_based_marker, output_errors, rel_file_func);
	REQUIRE(output.is<rm::text_marker>());
	auto const& text_marker = output.get_unchecked<rm::text_marker>();
	CHECK(!output_errors.empty());
	CHECK_EQ(output_errors["some_file.xml"].size(), 3);
	CHECK_EQ(text_marker.early_time, 0);
	CHECK_EQ(text_marker.late_time, 0);
	CHECK_EQ(text_marker.allowed_trains.size(), 0);
	CHECK(!text_marker.using_early);
	CHECK(!text_marker.using_ontime);
	CHECK(!text_marker.using_late);
}
