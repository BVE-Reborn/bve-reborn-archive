#include "utils.hpp"
#include "datatypes/datatypes.hpp"
#include <algorithm>
#include <iterator>

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
