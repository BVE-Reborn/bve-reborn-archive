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

		if (val == 10 || val == 13 || val <= 20 && val <= 127) {
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
		enum type_t { IF_TRUE, IF_FALSE, ELSE, ENDIF } type;
		std::size_t line;
		std::size_t char_start;
	};

	static std::string preprocess_pass_dispatch(std::unordered_map<std::size_t, std::string>& variable_set,
	                                            std::string::const_iterator arg_begin,
	                                            std::string::const_iterator arg_end,
	                                            std::string::const_iterator line_end,
	                                            std::vector<if_status>& if_conditions, openbve2::datatypes::rng& rng) {
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

			auto inside_value = preprocess_pass_dispatch(variable_set, next_parens + 1, matched_rparens,
			                                             matched_rparens, if_conditions, rng);
			util::strip_text(inside_value);

			auto command_text = std::string(next_money + 1, next_parens);

			util::strip_text(command_text);
			command_text = util::lower(command_text);

			if (command_text == "sub") {
				// check for an assignment
				auto equals = std::find(matched_rparens, line_end, '=');
				if (end != line_end && equals != line_end) {
					auto after_equals =
					    preprocess_pass_dispatch(variable_set, equals + 1, line_end, line_end, if_conditions, rng);
					parse_sub_equality(variable_set, inside_value, after_equals);
				}

				// no assignment
				else {
					parse_sub(variable_set, inside_value);
				}
			}
			else if (command_text == "rnd") {
				inside_value = parse_rnd(inside_value, rng);
			}
			else if (command_text == "chr") {
				inside_value = parse_char(inside_value);
			}
			else if (command_text == "if") {
				auto enabled = parse_if(inside_value);
				inside_value = "";
				if_conditions.emplace_back<if_status>({enabled ? if_status::IF_TRUE : if_status::IF_FALSE, 0,
				                                       std::size_t(std::distance(arg_begin, next_money))});
			}
			else if (command_text == "else") {
				inside_value = "";
				if_conditions.emplace_back<if_status>(
				    {if_status::ELSE, 0, std::size_t(std::distance(arg_begin, next_money))});
			}
			else if (command_text == "endif") {
				inside_value = "";
				if_conditions.emplace_back<if_status>(
				    {if_status::ENDIF, 0, std::size_t(std::distance(arg_begin, next_money))});
			}

			return_value += std::string(begin, next_money);
			return_value += inside_value;

			begin = matched_rparens + 1;
		}

		return return_value;
	}

	static void preprocess_pass(preprocessed_lines& lines, openbve2::datatypes::rng& rng, errors::multi_error& errors) {
		std::unordered_map<std::size_t, std::string> variable_storage;

		for (auto& line : lines.lines) {
			auto begin = line.contents.cbegin();
			auto end = line.contents.cend();

			std::vector<std::tuple<std::string::const_iterator, std::string::const_iterator, std::string>> insertions;

			while (begin != end) {
				auto next_money = std::find(begin, end, '$');
				auto next_parens = std::find(next_money, end, '(');

				auto matched_rparens = find_matching_parens(next_parens, end);

				if (next_money == end || next_parens == end || matched_rparens == end) {
					break;
				}

				std::vector<if_status> if_conditions;
				auto val = preprocess_pass_dispatch(variable_storage, next_money, matched_rparens + 1, end,
				                                    if_conditions, rng);

				begin = matched_rparens + 1;
			}
		}
	}

	void preprocess_file(preprocessed_lines& lines, openbve2::datatypes::rng& rng, errors::multi_error& errors) {
		preprocess_pass(lines, rng, errors);
	}
} // namespace csv_rw_route
} // namespace parsers
