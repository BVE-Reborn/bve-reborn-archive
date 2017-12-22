#pragma once

#include <string>

namespace parsers {
namespace util {
	long long parse_loose_integer(std::string text);
	float parse_loose_float(std::string text);
} // namespace util
} // namespace parsers