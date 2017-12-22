#pragma once

#include "core/datatypes.hpp"
#include <cinttypes>
#include <string>
#include <utility>

namespace parsers {
namespace util {
	std::intmax_t parse_loose_integer(std::string text);
	std::intmax_t parse_loose_integer(std::string text, std::intmax_t default_value);

	float parse_loose_float(std::string text);
	float parse_loose_float(std::string text, float default_value);

	std::intmax_t parse_time(std::string text);
	std::intmax_t parse_time(std::string text, std::intmax_t default_value);

	openbve2::datatypes::color8_rgba parse_color(std::string text);
	openbve2::datatypes::color8_rgba parse_color(std::string text, openbve2::datatypes::color8_rgba default_value);

	std::string lower(std::string text);
	bool match_text(const std::string& text, const char* match);
	void strip_text(std::string& text, const char* characters = "\t\n\v\f\r ");

} // namespace util
} // namespace parsers