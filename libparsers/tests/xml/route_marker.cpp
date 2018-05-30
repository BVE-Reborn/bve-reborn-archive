#include "utils.hpp"
#include <doctest.h>
#include <parsers/xml/route_marker.hpp>

using namespace std::string_literals;
namespace rm = parsers::xml::route_marker;

TEST_SUITE_BEGIN("libparser - xml - route_maker");

TEST_CASE("libparsers - xml - route_marker - Image Based Marker test") {
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
    // clang-format off
    auto rel_file_func = [](std::string base, std::string rel){ return base + "/" + rel;}; 
    auto const output = rm::parse("some_file.xml"s, image_based_marker,rel_file_func);
	REQUIRE(output.is<rm::image_marker>());
	auto& image_marker =
	    output.get_unchecked<rm::image_marker>();
    CHECK_EQ(image_marker.early_time,43200);
    CHECK_EQ(image_marker.early_filename, "some_file.xml/Early.png"s);

    CHECK_EQ(image_marker.on_time_filename, "some_file.xml/Marker.png"s);

    CHECK_EQ(image_marker.late_time, 43800);
    CHECK_EQ(image_marker.late_filename, "some_file.xml/Late.png"s);

    CHECK_EQ(image_marker.distance, 200);
    CHECK_EQ(image_marker.timeout, 12);

    CHECK(!image_marker.allowed_trains.empty());
}
