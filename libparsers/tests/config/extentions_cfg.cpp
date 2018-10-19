#include "utils.hpp"
#include <doctest.h>
#include "parsers/config/extentions_cfg.hpp"
#include "parsers/errors.hpp"

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - config - extentions_cfg");

TEST_CASE("libparsers - config - extentions_cfg - empty file") {
    std::string const file =
            ""s;
    parsers::errors::multi_error_t output_errors;
    const auto parsed = parsers::config::extensions_cfg::parse(file, "extensions.cfg"s, output_errors);

    REQUIRE_EQ(parsed.cars.size(), 0);
}

TEST_CASE("libparsers - config - extentions_cfg - empty section") {
	std::string const file =
		"[Car0]\n"s;
    parsers::errors::multi_error_t output_errors;
    const auto parsed =  parsers::config::extensions_cfg::parse(file, "extensions.cfg"s, output_errors);

    REQUIRE_EQ(parsed.cars.size(), 1);
    CHECK_EQ(parsed.cars[0].object.empty(), true);
}

TEST_CASE("libparsers - config - extentions_cfg - single [car] section") {
	std::string const file =
		"[Car0]\n"
		"Object = locomotive.b3d\n"
		"Length = 16\n"
		"Axles = -6, 6\n"
		"Reversed = False"s;
    parsers::errors::multi_error_t output_errors;
    const auto parsed =  parsers::config::extensions_cfg::parse(file, "extensions.cfg"s, output_errors);

    REQUIRE_EQ(parsed.cars.size(), 1);
    CHECK_EQ(parsed.cars[0].object, "locomotive.b3d");
    CHECK_EQ(parsed.cars[0].length, 16);
    CHECK_EQ(parsed.cars[0].axles.rear, -6);
    CHECK_EQ(parsed.cars[0].axles.front, 6);
    CHECK_EQ(parsed.cars[0].reversed, false);
}

TEST_CASE("libparsers - config - extentions_cfg - multi [car] section") {
	std::string const file =
		"[Car0]\n"
		"Object = locomotive.b3d\n"
		"Length = 16\n"
		"Axles = -6, 6\n"
		"Reversed = False\n"
		"[Car1]\n"
		"Object = locomotive.b3d\n"
		"Length = 18\n"
		"Axles = -8, 8\n"
		"Reversed = False"s;
	parsers::errors::multi_error_t output_errors;
	const auto parsed =  parsers::config::extensions_cfg::parse(file, "extensions.cfg"s, output_errors);

	REQUIRE_EQ(parsed.cars.size(), 2);
	CHECK_EQ(parsed.cars[0].object, "locomotive.b3d");
	CHECK_EQ(parsed.cars[0].length, 16);
	CHECK_EQ(parsed.cars[0].axles.rear, -6);
	CHECK_EQ(parsed.cars[0].axles.front, 6);
	CHECK_EQ(parsed.cars[0].reversed, false);
	CHECK_EQ(parsed.cars[1].object, "locomotive.b3d");
	CHECK_EQ(parsed.cars[1].length, 18);
	CHECK_EQ(parsed.cars[1].axles.rear, -8);
	CHECK_EQ(parsed.cars[1].axles.front, 8);
	CHECK_EQ(parsed.cars[1].reversed, false);
}

TEST_CASE("libparsers - config - extentions_cfg - single [coupler] section") {
	std::string const file =
		"[Coupler0]\n"
		"Distances = 0.30, 0.35"s;
    parsers::errors::multi_error_t output_errors;
    const auto parsed =  parsers::config::extensions_cfg::parse(file, "extensions.cfg"s, output_errors);

    REQUIRE_EQ(parsed.couplers.size(), 1);
    CHECK_EQ(parsed.couplers[0].distances.minimum, 0.30f);
    CHECK_EQ(parsed.couplers[0].distances.maximum, 0.35f);
}

TEST_CASE("libparsers - config - extentions_cfg - multi [coupler] section") {
	std::string const file =
		"[Coupler0]\n"
		"Distances = 0.30, 0.35\n"
		"[Coupler1]\n"
		"Distances = 0.40, 0.45"s;
	parsers::errors::multi_error_t output_errors;
	const auto parsed =  parsers::config::extensions_cfg::parse(file, "extensions.cfg"s, output_errors);

	REQUIRE_EQ(parsed.couplers.size(), 2);
	CHECK_EQ(parsed.couplers[0].distances.minimum, 0.30f);
	CHECK_EQ(parsed.couplers[0].distances.maximum, 0.35f);
	CHECK_EQ(parsed.couplers[1].distances.minimum, 0.40f);
	CHECK_EQ(parsed.couplers[1].distances.maximum, 0.45f);
}

TEST_CASE("libparsers - config - extentions_cfg - [exterior] section") {
	std::string const file =
		"[Exterior]\n"
		"0 = cars//engine.csv\n"
		"1 = cars//passenger_mk1.b3d\n"
		"2 = cars//passenger_mk1.b3d\n"
		"3 = cars//passenger_bistro.b3d\n"
		"4 = cars//passenger_mk2.b3d\n"
		"5 = cars//postal.x"s;
    parsers::errors::multi_error_t output_errors;
    const auto parsed =  parsers::config::extensions_cfg::parse(file, "extensions.cfg"s, output_errors);

    REQUIRE_EQ(parsed.exterior.car_filenames.size(), 6);
    CHECK_EQ(parsed.exterior.car_filenames[0], "cars//engine.csv");
    CHECK_EQ(parsed.exterior.car_filenames[1], "cars//passenger_mk1.b3d");
    CHECK_EQ(parsed.exterior.car_filenames[2], "cars//passenger_mk1.b3d");
    CHECK_EQ(parsed.exterior.car_filenames[3], "cars//passenger_bistro.b3d");
    CHECK_EQ(parsed.exterior.car_filenames[4], "cars//passenger_mk2.b3d");
    CHECK_EQ(parsed.exterior.car_filenames[5], "cars//postal.x");
}

TEST_SUITE_END();