#pragma once

#include "util/datatypes.hpp"
#include <cinttypes>
#include <iosfwd>
#include <string>
#include <vector>

namespace bve::util::parsers {
	std::intmax_t parse_loose_integer(const std::string& text);
	std::intmax_t parse_loose_integer(std::string text, std::intmax_t default_value);

	bool is_loose_integer(const std::string& text);

	float parse_loose_float(const std::string& text);
	float parse_loose_float(std::string text, float default_value);

	datatypes::Time parse_time(const std::string& text);
	datatypes::Time parse_time(std::string text, std::intmax_t default_value);

	datatypes::Color8RGBA parse_color(const std::string& text);
	datatypes::Color8RGBA parse_color(std::string text, datatypes::Color8RGBA default_value);

	void lower(std::string& text);
	std::string lower_copy(std::string text);
	bool match_against_lower(const std::string& text, const char* match, bool exact = true);
	std::vector<std::string> split_text(const std::string& text, char delim = ',', bool remove_blanks = false);
	void strip_text(std::string& text, const char* characters = "\t\n\v\f\r ");
	void remove_comments(std::string& text, char comment = ';', bool first_in_line = false);

	std::string load_from_file_utf8_bom(const std::string& filename);
	std::string load_from_file_utf8_bom(std::istream& file);

} // namespace bve::util::parsers
