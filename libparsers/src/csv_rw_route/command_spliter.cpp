#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	namespace line_splitting {
		instruction_info csv(const preprocessed_line& l) {
			const std::string& text = l.contents;
			auto first_break = std::find_if(text.begin(), text.end(),
			                                [](const char c) { return c == ' ' || c == '('; });

			std::string command_name(text.begin(), first_break);

			// if there is a : or text has no letters, is a position declaration
			bool has_colon = std::count(command_name.begin(), command_name.end(), ':') > 0;
			bool has_letters =
			    std::count_if(command_name.begin(), command_name.end(),
			                  [](const char c) { return std::isdigit(c) == 0 && c != '.'; })
			    > 0;
			if (has_colon || !has_letters) {
				auto list = util::split_text(command_name, ':');

				return instruction_info{"", {}, std::move(list), {}, l.offset, true};
			}

			// all text is stripped by the preprocessor, so if there is a break
			// we need to parse a parenthesized statement
			if (first_break == text.end()) {
				return instruction_info{command_name, {}, {}, {}, l.offset};
			}

			// skip passed all whitespace
			first_break = std::find_if(first_break + 1, text.end(),
			                           [](const char c) { return !(c == ' ' || c == '('); })
			              - 1;

			auto after_first_break = first_break + 1;
			std::string::const_iterator start_of_arg_list;
			std::vector<std::string> indices_set;
			if (*first_break == '(') {
				start_of_arg_list = std::find(after_first_break, text.end(), ')');
				indices_set =
				    util::split_text(std::string(after_first_break, start_of_arg_list), ';');
			}
			else {
				// if there isn't a parethesis here we know that's it and there
				// are only arguments
				start_of_arg_list = text.end();
				indices_set = util::split_text(std::string(after_first_break, text.end()), ';');
			}

			std::for_each(indices_set.begin(), indices_set.end(),
			              [](std::string& s) { return util::strip_text(s); });

			// no indices, we were just parsing arguments
			if (start_of_arg_list == text.end() || start_of_arg_list + 1 == text.end()) {
				return instruction_info{command_name, {}, indices_set, {}, l.offset};
			}
			start_of_arg_list += 1;
			bool has_suffix = *start_of_arg_list == '.';

			std::string suffix;
			if (has_suffix) {
				auto suffix_end = std::find_if(start_of_arg_list, text.end(),
				                               [](const char c) { return c == ' ' || c == '('; });
				suffix = std::string(start_of_arg_list + 1, suffix_end);
				util::lower(suffix);
				start_of_arg_list = suffix_end;
			}

			if (start_of_arg_list == text.end()) {
				return instruction_info{command_name, {}, indices_set, suffix, l.offset};
			}

			auto end_of_arg_list = *start_of_arg_list == '('
			                           ? std::find(start_of_arg_list + 1, text.end(), ')')
			                           : text.end();
			start_of_arg_list += 1;

			auto arg_list = util::split_text(std::string(start_of_arg_list, end_of_arg_list), ';');

			std::for_each(arg_list.begin(), arg_list.end(),
			              [](std::string& s) { return util::strip_text(s); });

			return instruction_info{command_name, indices_set, arg_list, suffix, l.offset};
		}

		instruction_info rw(const preprocessed_line& l) {
			const std::string& text = l.contents;
			auto first_break = std::find_if(text.begin(), text.end(), [](const char c) {
				return c == ' ' || c == '(' || c == '=';
			});

			std::string command_name(text.begin(), first_break);
			util::lower(command_name);

			// if there is a : or text has no letters, is a position declaration
			bool has_colon = std::count(command_name.begin(), command_name.end(), ':') > 0;
			bool has_letters =
			    std::count_if(command_name.begin(), command_name.end(),
			                  [](const char c) { return std::isdigit(c) == 0 && c != '.'; })
			    > 0;
			if (has_colon || !has_letters) {
				auto list = util::split_text(command_name, ':');

				return instruction_info{"", {}, std::move(list), {}, l.offset, true};
			}

			// This is a section header
			if (first_break == text.end()) {
				util::strip_text(command_name, "\t\n\v\f\r []");

				return instruction_info{"with"s, {}, {command_name}, {}, l.offset};
			}

			// skip passed all whitespace
			first_break =
			    std::find_if(first_break + 1, text.end(),
			                 [](const char c) { return !(c == ' ' || c == '(' || c == '='); })
			    - 1;

			std::vector<std::string> parens_list;
			if (*first_break == '(') {
				auto rparen = std::find(first_break + 1, text.end(), ')');
				parens_list = util::split_text(std::string(first_break + 1, rparen), ',');
				first_break = rparen;
			}

			if (first_break + 1 == text.end()) {
				return instruction_info{command_name, {}, std::move(parens_list), {}, l.offset};
			}

			bool has_suffix = *first_break + 1 == '.' && first_break + 2 != text.end();

			std::string suffix;
			if (has_suffix) {
				auto suffix_end = std::find_if(first_break + 1, text.end(),
				                               [](const char c) { return c == '='; });
				suffix = std::string(first_break + 2, suffix_end);
				util::lower(suffix);
				first_break = suffix_end;
			}
			else {
				first_break += 1;
			}

			if (first_break == text.end() && first_break + 1 == text.end()) {
				return instruction_info{command_name, {}, std::move(parens_list), suffix, l.offset};
			}

			return instruction_info{command_name,
			                        std::move(parens_list),
			                        {std::string(first_break + 1, text.end())},
			                        suffix,
			                        l.offset};
		}
	} // namespace line_splitting
} // namespace csv_rw_route
} // namespace parsers
