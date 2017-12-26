#include "utils.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace util {
	static std::intmax_t _parse_loose_integer_impl(std::string text);
	static float _parse_loose_float_impl(std::string text);
	static std::intmax_t _parse_time_impl(std::string text);
	static openbve2::datatypes::color8_rgba _parse_color_impl(std::string text);

	/////////////////
	// Int Parsing //
	/////////////////

	static std::intmax_t _parse_loose_integer_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		// parse like normal
		return std::stoll(text);
	}

	std::intmax_t parse_loose_integer(std::string text) {
		try {
			return _parse_loose_integer_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid integer";
			throw std::invalid_argument(error_msg.str());
		}
	}

	std::intmax_t parse_loose_integer(std::string text, std::intmax_t default_value) {
		try {
			return _parse_loose_integer_impl(text);
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	///////////////////
	// Float Parsing //
	///////////////////

	static float _parse_loose_float_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		return std::stof(text);
	}

	float parse_loose_float(std::string text) {
		try {
			return _parse_loose_float_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid float";
			throw std::invalid_argument(error_msg.str());
		}
	}

	float parse_loose_float(std::string text, float default_value) {
		try {
			return _parse_loose_float_impl(text);
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	//////////////////
	// Time Parsing //
	//////////////////

	static std::intmax_t _parse_time_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		auto dot = std::find(text.begin(), text.end(), '.');
		auto right_hand_size = std::distance(dot, text.end()) - 1;

		// no dot, only hh
		if (dot == text.end() || right_hand_size <= 0) {
			return std::stoll(text) * 3600;
		}

		// only hh.mm
		if (0 < right_hand_size && right_hand_size <= 2) {
			return std::stoll(text) * 3600 + //
			       std::stoll(std::string(dot + 1, dot + 1 + right_hand_size)) * 60;
		}

		// hh.mmss
		if (right_hand_size <= 4) {
			return std::stoll(text) * 3600 + //
			       std::stoll(std::string(dot + 1, dot + 3)) * 60 +
			       std::stoll(std::string(dot + 3, dot + 1 + right_hand_size));
		}

		throw std::invalid_argument("");
	}

	std::intmax_t parse_time(std::string text) {
		try {
			return _parse_time_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid time";
			throw std::invalid_argument(error_msg.str());
		}
	}

	std::intmax_t parse_time(std::string text, std::intmax_t default_value) {
		try {
			return _parse_time_impl(text);
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	///////////////////
	// Color Parsing //
	///////////////////

	static openbve2::datatypes::color8_rgba _parse_color_impl(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		if (text.size() == 7) {
			auto value = std::stoi(std::string(text.begin() + 1, text.end()), nullptr, 16);

			openbve2::datatypes::color8_rgba color{};
			color.r = uint8_t((value & 0xFF0000) >> 16);
			color.g = uint8_t((value & 0xFF00) >> 8);
			color.b = uint8_t((value & 0xFF) >> 0);
			color.a = uint8_t(255);

			return color;
		}

		throw std::invalid_argument("");
	}

	openbve2::datatypes::color8_rgba parse_color(std::string text) {
		try {
			return _parse_color_impl(text);
		}
		catch (const std::invalid_argument&) {
			std::ostringstream error_msg;
			error_msg << "\"" << text << "\" is not a valid color";
			throw std::invalid_argument(error_msg.str());
		}
	}

	openbve2::datatypes::color8_rgba parse_color(std::string text, openbve2::datatypes::color8_rgba default_value) {
		try {
			return _parse_color_impl(text);
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	std::string lower(std::string text) {
		for (auto& c : text) {
			c = char(std::tolower(c));
		}
		return text;
	}

	std::vector<std::string> split_text(const std::string& text, char delim) {
		std::vector<std::string> vec;

		auto begin = text.begin();
		auto end = text.end();

		while (begin != end) {
			auto next_delim = std::find(begin, end, delim);

			vec.emplace_back(begin, next_delim);

			begin = next_delim == end ? end : next_delim + 1;
		}

		return vec;
	}

	bool match_text(const std::string& text, const char* match) {
		auto text_len = text.size();
		auto match_len = std::strlen(match);

		auto min_len = std::min(text_len, match_len);

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

		if (first_char == std::string::npos && last_char == std::string::npos) {
			text = "";
			return;
		}
		if (first_char == std::string::npos) {
			first_char = 0;
		}
		if (last_char == std::string::npos) {
			last_char = text.size() - 1;
		}

		text = text.substr(first_char, last_char - first_char + 1);
	}

	void remove_comments(std::string& text, char comment) {
		bool removing;
		auto remove_func = [&removing, &comment](const char& c) {
			if (c == comment) {
				removing = true;
			}
			if (c == '\n') {
				removing = false;
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

		// check for bom
		std::array<unsigned char, 3> bom = {};
		std::size_t i = 0;
		for (; i < 3 && file >> bom[i]; ++i)
			;

		bool has_bom = i == 3 && std::get<0>(bom) == 0xEF && std::get<1>(bom) == 0xBB && std::get<2>(bom) == 0xBF;
		std::size_t start_of_file = has_bom ? 3 : 0;

		// get file length
		file.seekg(0, std::ifstream::end);
		auto length = std::size_t(file.tellg()) - start_of_file;
		file.seekg(start_of_file, std::ifstream::beg);

		std::string contents;
		contents.reserve(length);

		// extract file combining \r\n into \n
		std::array<char, 4096> buf;
		bool last_char_r = false;
		while (true) {
			file.read(buf.data(), buf.size());
			std::size_t chars_read = std::size_t(file.gcount());
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
} // namespace util
} // namespace parsers