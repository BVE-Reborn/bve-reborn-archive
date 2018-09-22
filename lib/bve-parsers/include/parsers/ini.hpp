#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace bve::parsers::ini {
	struct Value {
		std::string value;
		std::size_t line;
	};

	struct KeyValuePair {
		std::string key;
		std::string value;
		std::size_t line;
	};

	struct INISection {
		std::string name;
		std::vector<Value> values;
		std::vector<KeyValuePair> key_value_pairs;
		std::size_t line;
	};

	using ParsedINIObject = std::vector<INISection>;
	std::ostream& operator<<(std::ostream& os, const ParsedINIObject& rhs);

	ParsedINIObject parse(const std::string& file_string);
} // namespace bve::parsers::ini
