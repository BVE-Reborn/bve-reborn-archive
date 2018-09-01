//
// Created by kevin Cybura on 9/1/18.
//

#include "panel2_cfg.h"
#include <doctest.h>

using namespace std::string_literals;

TEST_SUITE_BEGIN("libparsers - panel2_cfg");

TEST_CASE("libparsers - panel2_cfg - commands - mesh builder") {
    std::string file= "[This] \n"
                  "Resolution = 32.2\n"
                  "Left = 32.2\n"
                  "Right = 32.2\n"
                  "Top = 32.2\n"
                  "Bottom = 32.2\n"
                  "DaytimeImage = FileName\n"
                  "NighttimeImage = FileName\n"
                  "TransparentColor = HexColor\n"
                  "Center = X, Y\n"
                  "Origin = X, Y";
    auto x = parsers::panel2::parse_panel2_cfg(file);
}

TEST_SUITE_END();