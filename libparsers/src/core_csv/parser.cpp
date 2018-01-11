#include "csv.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>

namespace parsers {
namespace csv {
	parsed_csv parse(const std::string& file, split_first_column sfc, char delim, char split_char) {
		// start with at least one row
		std::vector<std::vector<csv_token>> token_list{1, std::vector<csv_token>{}};

		auto begin = file.begin();
		auto end = file.end();

		std::string::const_iterator last_newline_iterator = begin;
		std::size_t current_line = 1;

		bool last_loop = false;
		bool newline = true;

		while (!last_loop) {
			// find next comma/newline
			auto next_delim = std::find_if(begin, end, [&](char c) {
				return ((sfc == split_first_column::yes && newline) ? c == split_char : false) || c == delim
				       || c == '\n';
			});

			newline = false;

			std::string s;
			s = std::string{begin, next_delim};

			util::strip_text(s);

			csv_token token{std::move(s), current_line, current_line,
			                std::size_t(std::distance(last_newline_iterator, begin)),
			                std::size_t(std::distance(last_newline_iterator, next_delim))};

			token_list.back().emplace_back(std::move(token));

			if (next_delim != end) {
				if (*next_delim == '\n') {
					newline = true;
					last_newline_iterator = next_delim + 1;
					current_line++;
					token_list.emplace_back();
				}
			}
			if (last_loop) {
				break;
			}

			if (next_delim != end) {
				begin = next_delim + 1;
			}
			else {
				last_loop = true;
			}
		}

		return token_list;
	}
} // namespace csv
} // namespace parsers
