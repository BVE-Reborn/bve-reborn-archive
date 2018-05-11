#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace parsers {
namespace ini {
	struct value_t {
		std::string value;
		std::size_t line;
	};

	struct kvp_t {
		std::string key;
		std::string value;
		std::size_t line;
	};

	struct ini_section_t {
		std::string name;
		std::vector<value_t> values;
		std::vector<kvp_t> key_value_pairs;
		std::size_t line;
	};

	using parsed_ini_object = std::vector<ini_section_t>;
	std::ostream& operator<<(std::ostream& os, const parsed_ini_object& rhs);

	parsed_ini_object parse(const std::string& file_string);
} // namespace ini
} // namespace parsers
