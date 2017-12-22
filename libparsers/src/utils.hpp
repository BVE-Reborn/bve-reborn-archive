#pragma once

#include "datatypes/datatypes.hpp"
#include <string>

namespace parsers {
namespace util {
	long long parse_loose_integer(std::string text);
	float parse_loose_float(std::string text);
	long long parse_time(std::string text);
	openbve2::datatypes::color8_rgba parse_color(std::string text);
} // namespace util
} // namespace parsers