#pragma once

#include "core/datatypes.hpp"
#include <cinttypes>
#include <string>
#include <vector>

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
	std::vector<std::string> split_text(const std::string& text, char delim = ',');
	void strip_text(std::string& text, const char* characters = "\t\n\v\f\r ");
	void remove_comments(std::string& text, char comment = ';');

	std::string load_from_file_utf8_bom(const std::string& filename);
} // namespace util
} // namespace parsers
