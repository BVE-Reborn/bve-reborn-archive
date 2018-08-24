#pragma once

#include "core/datatypes.hpp"
#include <cinttypes>
#include <iosfwd>
#include <string>
#include <vector>

namespace parsers {
namespace util {
	std::intmax_t parse_loose_integer(const std::string& text);
	std::intmax_t parse_loose_integer(std::string text, std::intmax_t default_value);

	float parse_loose_float(const std::string& text);
	float parse_loose_float(std::string text, float default_value);

	bvereborn::datatypes::time parse_time(const std::string& text);
	bvereborn::datatypes::time parse_time(std::string text, std::intmax_t default_value);

	bvereborn::datatypes::color8_rgba parse_color(const std::string& text);
	bvereborn::datatypes::color8_rgba parse_color(std::string text,
	                                              bvereborn::datatypes::color8_rgba default_value);

	void lower(std::string& text);
	std::string lower_copy(std::string text);
	bool match_against_lower(const std::string& text, const char* match);
	std::vector<std::string> split_text(const std::string& text,
	                                    char delim = ',',
	                                    bool remove_blanks = false);
	void strip_text(std::string& text, const char* characters = "\t\n\v\f\r ");
	void remove_comments(std::string& text, char comment = ';', bool first_in_line = false);

	std::string load_from_file_utf8_bom(const std::string& filename);
	std::string load_from_file_utf8_bom(std::istream& file);
} // namespace util
} // namespace parsers
