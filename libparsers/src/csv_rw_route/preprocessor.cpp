#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	static std::string::const_iterator find_matching_parens(std::string::const_iterator begin,
	                                                        std::string::const_iterator end) {
		std::size_t level = 0;
		std::string::const_iterator end_paren = end;
		for (auto i = begin; i != end; ++i) {
			if (*i == '(') {
				level++;
			}
			if (*i == ')' && level) {
				level--;
				if (level == 0) {
					end_paren = i;
					break;
				}
			}
		}

		return end_paren;
	}

	static std::string parse_sub(std::unordered_map<std::size_t, std::string>& variable_set, std::string parens) {
		auto index = util::parse_loose_integer(parens);

		return variable_set[index];
	}

	static std::string parse_sub_equality(std::unordered_map<std::size_t, std::string>& variable_set,
	                                      std::string parens, std::string after_equals) {
		auto index = util::parse_loose_integer(parens);
		variable_set[index] = after_equals;

		return ""s;
	}

	static std::string parse_rnd(const std::string& arg, openbve2::datatypes::rng& rng) {
		auto split = util::split_text(arg, ';');

		if (split.size() != 2) {
			throw std::invalid_argument("$Rnd takes two arguments");
		}

		auto start = util::parse_loose_integer(split[0]);
		auto end = util::parse_loose_integer(split[1]);

		std::uniform_int_distribution<std::intmax_t> dist(start, end);

		return std::to_string(dist(rng));
	}

	static std::string parse_char(const std::string& arg) {
		auto val = util::parse_loose_integer(arg);

		if (val == 10 || val == 13 || (val <= 20 && val <= 127)) {
			return std::string(1, char(val));
		}
		else {
			return ""s;
		}
	}

	static bool parse_if(const std::string& arg) {
		auto val = util::parse_loose_integer(arg);

		return val != 0;
	}

	struct if_status {
		enum type_t { IF_TRUE, IF_FALSE, ELSE, ENDIF, NONE } type = NONE;
		std::size_t char_start;
	};

	static std::string preprocess_pass_dispatch(std::unordered_map<std::size_t, std::string>& variable_set,
	                                            if_status& if_conditions, openbve2::datatypes::rng& rng,
	                                            std::string::const_iterator& last_used,
	                                            std::string::const_iterator arg_begin,
	                                            std::string::const_iterator arg_end,
	                                            std::string::const_iterator line_end) {
		auto begin = arg_begin;
		auto end = arg_end;

		std::string return_value;

		while (begin != end) {
			auto next_money = std::find(begin, end, '$');
			if (next_money == end) {
				return std::string(begin, end);
			}
			auto next_parens = std::find(next_money, end, '(');

			// find the matching parenthesis
			auto matched_rparens = find_matching_parens(next_parens, end);

			auto inside_value = preprocess_pass_dispatch(variable_set, if_conditions, rng, last_used, next_parens + 1,
			                                             matched_rparens, matched_rparens);
			util::strip_text(inside_value);

			auto command_text = std::string(next_money + 1, next_parens);

			util::strip_text(command_text);
			command_text = util::lower(command_text);

			if (command_text == "sub") {
				// check for an assignment
				auto equals = std::find(matched_rparens, line_end, '=');
				if (end != line_end && equals != line_end) {
					auto after_equals = preprocess_pass_dispatch(variable_set, if_conditions, rng, last_used,
					                                             equals + 1, line_end, line_end);
					parse_sub_equality(variable_set, inside_value, after_equals);

					inside_value.clear();
					last_used = line_end;
				}

				// no assignment
				else {
					parse_sub(variable_set, inside_value);
					last_used = matched_rparens;
				}
			}
			else if (command_text == "rnd") {
				inside_value = parse_rnd(inside_value, rng);
				last_used = matched_rparens;
			}
			else if (command_text == "chr") {
				inside_value = parse_char(inside_value);
				last_used = matched_rparens;
			}
			else if (command_text == "if") {
				auto enabled = parse_if(inside_value);
				inside_value = "";
				if_conditions = {enabled ? if_status::IF_TRUE : if_status::IF_FALSE,
				                 std::size_t(std::distance(arg_begin, next_money))};
				last_used = matched_rparens;
			}
			else if (command_text == "else") {
				inside_value = "";
				if_conditions = {if_status::ELSE, std::size_t(std::distance(arg_begin, next_money))};
				last_used = matched_rparens;
			}
			else if (command_text == "endif") {
				inside_value = "";
				if_conditions = {if_status::ENDIF, std::size_t(std::distance(arg_begin, next_money))};
				last_used = matched_rparens;
			}

			return_value += std::string(begin, next_money);
			return_value += inside_value;

			begin = matched_rparens + 1;
		}

		return return_value;
	}

	static void preprocess_pass(preprocessed_lines& lines, openbve2::datatypes::rng& rng, errors::multi_error& errors) {
		std::unordered_map<std::size_t, std::string> variable_storage;

		std::vector<bool> if_condition_stack(1, true);

		for (auto& line : lines.lines) {
			std::string processed_line;

			auto begin = line.contents.cbegin();
			auto end = line.contents.cend();

			while (begin != end) {
				auto next_money = std::find(begin, end, '$');
				auto next_parens = std::find(next_money, end, '(');

				auto matched_rparens = find_matching_parens(next_parens, end);

				// pre-directive characters
				if (if_condition_stack.back()) {
					std::copy(begin, next_money, std::back_inserter(processed_line));
				}

				if (next_money == end || next_parens == end || matched_rparens == end) {
					break;
				}

				if_status if_condition;
				std::string::const_iterator last_used;
				std::string directive_value;
				try {
					directive_value = preprocess_pass_dispatch(variable_storage, if_condition, rng, last_used,
					                                           next_money, matched_rparens + 1, end);
				}
				catch (const std::invalid_argument& e) {
					errors[lines.filenames[line.filename_index]].emplace_back<errors::error_t>({line.line, e.what()});
					continue;
				}

				if (if_condition.type == if_status::IF_TRUE) {
					if_condition_stack.emplace_back(true);
				}
				else if (if_condition.type == if_status::IF_FALSE) {
					if_condition_stack.emplace_back(true);
				}
				else if (if_condition.type == if_status::ELSE) {
					if_condition_stack.back() = !if_condition_stack.back();
				}
				else if (if_condition.type == if_status::ENDIF) {
					if (if_condition_stack.size() == 1) {
						// error
						throw std::invalid_argument("preprocessing wtf");
					}
					if_condition_stack.pop_back();
				}

				// copy the result of the directive, and concatinate \r\n into \n
				// if condition will be true if this is not the \n of the \r\n sequence
				if (!(directive_value == "\n" && !processed_line.empty() && processed_line.back() == '\r')) {
					processed_line += directive_value;
				}
				else {
					processed_line.back() = '\n';
				}

				begin = last_used != end ? last_used + 1 : end;
			}

			line.contents = processed_line;
		}
	}

	void split_on_commas(preprocessed_lines& lines) { 
		preprocessed_lines fixed;

		for (auto& line : lines.lines) {
			auto vec = util::split_text(line.contents, ',', true);
			for (auto& elem : vec) {
				util::strip_text(elem);
				if (!elem.empty()) {
					fixed.lines.emplace_back<preprocessed_line>({ std::move(elem), line.filename_index, line.line, line.offset });
				}
			}
		}

		fixed.filenames = std::move(lines.filenames);

		lines = fixed;
	}

	void preprocess_file(preprocessed_lines& lines, openbve2::datatypes::rng& rng, errors::multi_error& errors) {
		preprocess_pass(lines, rng, errors);

		// remove comments that have been added by preprocessing
		for (auto& line : lines.lines) {
			util::remove_comments(line.contents, ';', true);
		}

		// remove empty lines
		lines.lines.erase(std::remove_if(lines.lines.begin(), lines.lines.end(),
		                                 [](preprocessed_line& l) { return l.contents.empty(); }),
		                  lines.lines.end());

		// split lines on commas
		split_on_commas(lines);
	}
} // namespace csv_rw_route
} // namespace parsers
