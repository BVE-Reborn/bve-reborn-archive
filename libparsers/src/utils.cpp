#include "utils.hpp"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <cctype>

namespace parsers {

namespace util {
	std::intmax_t parse_loose_integer(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		// parse like normal
		return std::stoll(text);
	}

	std::intmax_t parse_loose_integer(std::string text, std::intmax_t default_value) {
		try {
			return parse_loose_integer(std::move(text));
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	float parse_loose_float(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		return std::stof(text);
	}

	float parse_loose_float(std::string text, float default_value) {
		try {
			return parse_loose_float(std::move(text));
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	std::intmax_t parse_time(std::string text) {
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

		// TODO (sirflankalot): Error
		return 0;
	}

	std::intmax_t parse_time(std::string text, std::intmax_t default_value) {
		try {
			return parse_time(std::move(text));
		}
		catch (const std::invalid_argument&) {
			return default_value;
		}
	}

	openbve2::datatypes::color8_rgba parse_color(std::string text) {
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

		// TODO (sirflankalot): Error
		return openbve2::datatypes::color8_rgba{};
	}

	openbve2::datatypes::color8_rgba parse_color(std::string text, openbve2::datatypes::color8_rgba default_value) {
		try {
			return parse_color(std::move(text));
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
} // namespace util
} // namespace parsers