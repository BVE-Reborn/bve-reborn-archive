#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace parsers {
namespace csv {
	struct csv_token {
		std::string text;
		std::size_t line_begin = 0;
		std::size_t line_end = 0;
		std::size_t char_begin = 0;
		std::size_t char_end = 0;
	};

	using parsed_csv = std::vector<std::vector<csv_token>>;

	parsed_csv parse_csv(const std::string& file);
} // namespace csv
} // namespace parsers