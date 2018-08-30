#include "utils.hpp"
#include "ini.hpp"
#include <doctest.h>
#include "parsers/config/extentions_cfg.hpp"

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - config - extentions_cfg");

TEST_CASE("libparsers - config - extentions_cfg - empty file") {
    auto parsed = parsers::ini::parse("");

    REQUIRE_EQ(parsed.size(), 1);
    CHECK_EQ(parsed[0].key_value_pairs.size(), 0);
    CHECK_EQ(parsed[0].values.size(), 0);
    CHECK_EQ(parsed[0].name, "");
    CHECK_EQ(parsed[0].line, 0);
}

TEST_CASE("libparsers - config - extentions_cfg - empty section") {
    auto parsed = parsers::ini::parse("[sec1]");

    REQUIRE_EQ(parsed.size(), 2);
    CHECK_EQ(parsed[0].key_value_pairs.size(), 0);
    CHECK_EQ(parsed[0].values.size(), 0);
    CHECK_EQ(parsed[0].name, "");
    CHECK_EQ(parsed[0].line, 0);

    CHECK_EQ(parsed[1].key_value_pairs.size(), 0);
    CHECK_EQ(parsed[1].values.size(), 0);
    CHECK_EQ(parsed[1].name, "sec1");
    CHECK_EQ(parsed[1].line, 1);
}

TEST_CASE("libparsers - config - extentions_cfg - single Car section") {
    auto parsed = parsers::ini::parse(
        "[Car0]\n"
        "Object = locomotive.b3d\n"
        "Length = 16\n"
        "Axles = -6, 6\n"
        "Reversed = False");

    REQUIRE_EQ(parsed.size(), 2);
    CHECK_EQ(parsed[0].key_value_pairs.size(), 0);
    CHECK_EQ(parsed[0].values.size(), 0);
    CHECK_EQ(parsed[0].name, "");
    CHECK_EQ(parsed[0].line, 0);

    REQUIRE_EQ(parsed[1].key_value_pairs.size(), 4);
    CHECK_EQ(parsed[1].values.size(), 0);
    CHECK_EQ(parsed[1].name, "Car0");
    CHECK_EQ(parsed[1].line, 1);

    CHECK_EQ(parsed[1].key_value_pairs[0].line, 2);
    CHECK_EQ(parsed[1].key_value_pairs[0].key, "Object");
    CHECK_EQ(parsed[1].key_value_pairs[0].value, "locomotive.b3d");

    CHECK_EQ(parsed[1].key_value_pairs[1].line, 3);
    CHECK_EQ(parsed[1].key_value_pairs[1].key, "Length");
    CHECK_EQ(parsed[1].key_value_pairs[1].value, "16");

    CHECK_EQ(parsed[1].key_value_pairs[2].line, 4);
    CHECK_EQ(parsed[1].key_value_pairs[2].key, "Axles");
    CHECK_EQ(parsed[1].key_value_pairs[2].value, "-6, 6");

    CHECK_EQ(parsed[1].key_value_pairs[3].line, 5);
    CHECK_EQ(parsed[1].key_value_pairs[3].key, "Reversed");
    CHECK_EQ(parsed[1].key_value_pairs[3].value, "False");
}

TEST_CASE("libparsers - config - extentions_cfg - single Coupler section") {
    auto parsed = parsers::ini::parse(
            "[Coupler0]\n"
            "Distances = 0.30, 0.35");

    REQUIRE_EQ(parsed.size(), 2);
    CHECK_EQ(parsed[0].key_value_pairs.size(), 0);
    CHECK_EQ(parsed[0].values.size(), 0);
    CHECK_EQ(parsed[0].name, "");
    CHECK_EQ(parsed[0].line, 0);

    REQUIRE_EQ(parsed[1].key_value_pairs.size(), 1);
    CHECK_EQ(parsed[1].values.size(), 0);
    CHECK_EQ(parsed[1].name, "Coupler0");
    CHECK_EQ(parsed[1].line, 1);

    CHECK_EQ(parsed[1].key_value_pairs[0].line, 2);
    CHECK_EQ(parsed[1].key_value_pairs[0].key, "Distances");
    CHECK_EQ(parsed[1].key_value_pairs[0].value, "0.30, 0.35");
}

TEST_CASE("libparsers - config - extentions_cfg - Exterior section") {
    auto parsed = parsers::ini::parse(
            "[Exterior]\n"
            "0 = cars\\engine.csv\n"
            "1 = cars\\passenger_mk1.b3d\n"
            "2 = cars\\passenger_mk1.b3d\n"
            "3 = cars\\passenger_bistro.b3d\n"
            "4 = cars\\passenger_mk2.b3d\n"
            "5 = cars\\postal.x");

    REQUIRE_EQ(parsed.size(), 2);
    CHECK_EQ(parsed[0].key_value_pairs.size(), 0);
    CHECK_EQ(parsed[0].values.size(), 0);
    CHECK_EQ(parsed[0].name, "");
    CHECK_EQ(parsed[0].line, 0);

    REQUIRE_EQ(parsed[1].key_value_pairs.size(), 6);
    CHECK_EQ(parsed[1].values.size(), 0);
    CHECK_EQ(parsed[1].name, "Exterior");
    CHECK_EQ(parsed[1].line, 1);

    CHECK_EQ(parsed[1].key_value_pairs[0].line, 2);
    CHECK_EQ(parsed[1].key_value_pairs[0].key, "0");
    CHECK_EQ(parsed[1].key_value_pairs[0].value, "cars\\engine.csv");

    CHECK_EQ(parsed[1].key_value_pairs[1].line, 3);
    CHECK_EQ(parsed[1].key_value_pairs[1].key, "1");
    CHECK_EQ(parsed[1].key_value_pairs[1].value, "cars\\passenger_mk1.b3d");

    CHECK_EQ(parsed[1].key_value_pairs[2].line, 4);
    CHECK_EQ(parsed[1].key_value_pairs[2].key, "2");
    CHECK_EQ(parsed[1].key_value_pairs[2].value, "cars\\passenger_mk1.b3d");

    CHECK_EQ(parsed[1].key_value_pairs[3].line, 5);
    CHECK_EQ(parsed[1].key_value_pairs[3].key, "3");
    CHECK_EQ(parsed[1].key_value_pairs[3].value, "cars\\passenger_bistro.b3d");

    CHECK_EQ(parsed[1].key_value_pairs[4].line, 6);
    CHECK_EQ(parsed[1].key_value_pairs[4].key, "4");
    CHECK_EQ(parsed[1].key_value_pairs[4].value, "cars\\passenger_mk2.b3d");

    CHECK_EQ(parsed[1].key_value_pairs[5].line, 7);
    CHECK_EQ(parsed[1].key_value_pairs[5].key, "5");
    CHECK_EQ(parsed[1].key_value_pairs[5].value, "cars\\postal.x");
}