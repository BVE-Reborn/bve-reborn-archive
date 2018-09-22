#include "parsers/utils.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <fstream>
#include <gsl/gsl_util>
#include <iterator>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>

using namespace std::string_literals;

namespace bve::parsers::util {
	static std::intmax_t parse_loose_integer_impl(std::string text);
	static float parse_loose_float_impl(std::string text);
	static std::intmax_t parse_time_impl(std::string text);
	static core::datatypes::Color8RGBA parse_color_impl(std::string text);

	/////////////////
	// Int Parsing //
	/////////////////

	static std::intmax_t parse_loose_integer_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		// parse like normal
		return std::stoll(text);
	}

	std::intmax_t parse_loose_integer(const std::string& text) {
		try {
			return parse_loose_integer_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid integer";
			throw std::invalid_argument(error_msg.str());
		}
	}

	std::intmax_t parse_loose_integer(std::string text, std::intmax_t const default_value) {
		try {
			return parse_loose_integer_impl(std::move(text));
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	bool is_loose_integer(const std::string& text) {
		try {
			parse_loose_integer_impl(text);
			return true;
		}
		catch (const std::invalid_argument&) {
			return false;
		}
	}

	///////////////////
	// Float Parsing //
	///////////////////

	static float parse_loose_float_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		return std::stof(text);
	}

	float parse_loose_float(const std::string& text) {
		try {
			return parse_loose_float_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid float";
			throw std::invalid_argument(error_msg.str());
		}
	}

	float parse_loose_float(std::string text, float const default_value) {
		try {
			return parse_loose_float_impl(std::move(text));
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	//////////////////
	// time Parsing //
	//////////////////

	static core::datatypes::Time parse_time_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		auto const deliminator = std::find_if(text.begin(), text.end(),
		                                      [](char const c) { return c == '.' || c == ':'; });
		auto const right_hand_size = std::distance(deliminator, text.end()) - 1;

		// no deliminator, only hh
		if (deliminator == text.end() || right_hand_size <= 0) {
			return std::stoll(text) * 3600;
		}

		// only hh.mm
		if (0 < right_hand_size && right_hand_size <= 2) {
			return std::stoll(text) * 3600 + //
			       std::stoll(std::string(deliminator + 1, deliminator + 1 + right_hand_size)) * 60;
		}

		// hh.mmss
		if (right_hand_size <= 4) {
			return std::stoll(text) * 3600 + //
			       std::stoll(std::string(deliminator + 1, deliminator + 3)) * 60
			       + std::stoll(std::string(deliminator + 3, deliminator + 1 + right_hand_size));
		}

		throw std::invalid_argument("");
	}

	core::datatypes::Time parse_time(const std::string& text) {
		try {
			return parse_time_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid time";
			throw std::invalid_argument(error_msg.str());
		}
	}

	core::datatypes::Time parse_time(std::string text, std::intmax_t const default_value) {
		try {
			return parse_time_impl(std::move(text));
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	///////////////////
	// Color Parsing //
	///////////////////

	static core::datatypes::Color8RGBA parse_color_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		if (text.size() == 7) {
			auto const value = std::stoi(std::string(text.begin() + 1, text.end()), nullptr, 16);

			core::datatypes::Color8RGBA color{uint8_t((value & 0xFF0000) >> 16),
			                                  uint8_t((value & 0xFF00) >> 8),
			                                  uint8_t((value & 0xFF) >> 0), uint8_t(255)};

			return color;
		}

		throw std::invalid_argument("");
	}

	core::datatypes::Color8RGBA parse_color(const std::string& text) {
		try {
			return parse_color_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid color";
			throw std::invalid_argument(error_msg.str());
		}
	}

	core::datatypes::Color8RGBA parse_color(std::string text,
	                                        core::datatypes::Color8RGBA default_value) {
		try {
			return parse_color_impl(std::move(text));
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	void lower(std::string& text) {
		for (auto& c : text) {
			c = gsl::narrow<char>(std::tolower(c));
		}
	}

	std::string lower_copy(std::string text) {
		lower(text);
		return text;
	}

	std::vector<std::string> split_text(const std::string& text,
	                                    char const delim,
	                                    bool const remove_blanks) {
		std::vector<std::string> vec;

		auto begin = text.begin();
		auto const end = text.end();

		while (begin != end) {
			auto next_delim = std::find(begin, end, delim);
			if (remove_blanks) {
				if (std::distance(begin, next_delim) > 0) {
					vec.emplace_back(begin, next_delim);
				}
			}
			else {
				vec.emplace_back(begin, next_delim);
			}

			if (next_delim == end) {
				begin = end;
			}
			else {
				begin = next_delim + 1;
				// generate empty string if comma at end of the string.
				if (begin == end) {
					vec.emplace_back();
				}
			}
		}

		return vec;
	}

	bool match_against_lower(const std::string& text, char const* const match, bool const exact) {
		auto const text_len = text.size();
		auto const match_len = std::strlen(match);

		if (text_len == 0 && match_len != 0) {
			return false;
		}

		if (match_len == 0) {
			return true;
		}

		if (exact == true && text_len != match_len) {
			return false;
		}

		auto const min_len = std::min(text_len, match_len);

		for (std::size_t i = 0; i < min_len; ++i) {
			if (std::tolower(text[i]) != match[i]) {
				return false;
			}
		}
		return true;
	}

	void strip_text(std::string& text, const char* characters) {
		if (text.empty()) {
			return;
		}

		auto first_char = text.find_first_not_of(characters);
		auto last_char = text.find_last_not_of(characters);

		// These will only both happen, or neither will happen
		if (first_char == std::string::npos && last_char == std::string::npos) {
			text = "";
			return;
		}

		std::size_t i = 0;
		text.erase(std::remove_if(text.begin(), text.end(),
		                          [&i, &first_char, &last_char](char) {
			                          auto const in_range = first_char <= i && i <= last_char;
			                          ++i;
			                          return !in_range;
		                          }),
		           text.end());
	}

	void remove_comments(std::string& text, char comment, bool first_in_line) {
		auto removing = false;
		auto newline = true;
		auto const remove_func = [&](const char& c) {
			if (c == comment && (first_in_line ? newline : true)) {
				removing = true;
			}
			if (c == '\n') {
				removing = false;
				newline = true;
			}
			else {
				newline = false;
			}
			return removing;
		};

		text.erase(std::remove_if(text.begin(), text.end(), remove_func), text.end());
	}

	std::string load_from_file_utf8_bom(const std::string& filename) {
		// open file
		std::ifstream file(filename, std::ifstream::binary);

		if (!file) {
			throw std::invalid_argument("file "s + filename + " not found"s);
		}

		return load_from_file_utf8_bom(file);
	}

	std::string load_from_file_utf8_bom(std::istream& file) {
		// check for bom
		std::array<unsigned char, 3> bom = {};
		std::size_t i = 0;
		for (; i < 3 && file >> bom[i]; ++i) {
		}

		auto const has_bom = i == 3 && std::get<0>(bom) == 0xEF && std::get<1>(bom) == 0xBB
		                     && std::get<2>(bom) == 0xBF;
		std::size_t const start_of_file = has_bom ? 3 : 0;

		std::string contents;

		// get file length
		if (typeid(file) == typeid(std::ifstream)) {
			file.seekg(0, std::istream::end);
			auto const length = gsl::narrow<std::size_t>(file.tellg()) - start_of_file;

			contents.reserve(length);
		}

		file.seekg(start_of_file, std::istream::beg);

		// extract file combining \r\n into \n
		std::array<char, 4096> buf{};
		auto last_char_r = false;
		while (true) {
			file.read(buf.data(), buf.size());
			auto const chars_read = gsl::narrow<std::size_t>(file.gcount());
			for (std::size_t j = 0; j < chars_read; ++j) {
				if (last_char_r && buf[j] == '\n') {
					contents.back() = '\n';
				}
				else {
					contents.push_back(buf[j]);
				}
				last_char_r = buf[j] == '\r';
			}
			if (chars_read != buf.size()) {
				break;
			}
		}

		return contents;
	}

} // namespace bve::parsers::util
