#include "utils.hpp"
#include <algorithm>
#include <iterator>

namespace parsers {

namespace util {
	long long parse_loose_integer(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		// parse like normal
		return std::stoll(text);
	}

	float parse_loose_float(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		return std::stof(text);
	}

	long long parse_time(std::string text) {
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

		// TODO: Error
		return 0;
	}

	openbve2::datatypes::color8_rgba parse_color(std::string text) {
		// strip whitespace
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());

		if (text.size() == 7) {
			auto value = std::stoi(std::string(text.begin() + 1, text.end()), nullptr, 16);

			openbve2::datatypes::color8_rgba color;
			color.r = uint8_t((value & 0xFF0000) >> 16);
			color.g = uint8_t((value & 0xFF00) >> 8);
			color.b = uint8_t((value & 0xFF) >> 0);
			color.a = uint8_t(255);

			return color;
		}

		// TODO: Error
		return openbve2::datatypes::color8_rgba{};
	}

	void strip_text(std::string& text, const char* characters) {
		auto first_char = text.find_first_not_of(characters);
		auto last_char = text.find_last_not_of(characters);

		text = text.substr(first_char, last_char - first_char + 1);
	}
} // namespace util
} // namespace parsers