#include "parsers/csv_rw_route.hpp"
#include "util/parsing.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace bve::parsers::csv_rw_route::line_splitting {
	InstructionInfo csv(const PreprocessedLine& l) {
		const auto& text = l.contents;
		auto first_break = std::find_if(text.begin(), text.end(), [](const char c) { return c == ' ' || c == '('; });

		std::string command_name(text.begin(), first_break);

		// if there is a : or text has no letters, is a position declaration
		auto const has_colon = std::count(command_name.begin(), command_name.end(), ':') > 0;
		auto const has_letters =
		    std::count_if(command_name.begin(), command_name.end(), [](const char c) { return std::isdigit(c) == 0 && c != '.'; }) > 0;
		if (has_colon || !has_letters) {
			auto list = util::parsers::split_text(command_name, ':');

			return InstructionInfo{"", {}, std::move(list), {}, l.offset, true};
		}

		// all text is stripped by the preprocessor, so if there is a break
		// we need to parse a parenthesized statement
		if (first_break == text.end()) {
			return InstructionInfo{command_name, {}, {}, {}, l.offset};
		}

		// skip passed all whitespace
		first_break = std::find_if(first_break + 1, text.end(), [](const char c) { return !(c == ' ' || c == '('); }) - 1;

		auto const after_first_break = first_break + 1;
		std::string::const_iterator start_of_arg_list;
		std::vector<std::string> indices_set;
		if (*first_break == '(') {
			start_of_arg_list = std::find(after_first_break, text.end(), ')');
			indices_set = util::parsers::split_text(std::string(after_first_break, start_of_arg_list), ';');
		}
		else {
			// if there isn't a parenthesis here we know that's it and there
			// are only arguments
			start_of_arg_list = text.end();
			indices_set = util::parsers::split_text(std::string(after_first_break, text.end()), ';');
		}

		std::for_each(indices_set.begin(), indices_set.end(), [](std::string& s) { return util::parsers::strip_text(s); });

		// no indices, we were just parsing arguments
		if (start_of_arg_list == text.end() || start_of_arg_list + 1 == text.end()) {
			return InstructionInfo{command_name, {}, indices_set, {}, l.offset};
		}
		start_of_arg_list += 1;
		auto const has_suffix = *start_of_arg_list == '.';

		std::string suffix;
		if (has_suffix) {
			auto const suffix_end = std::find_if(start_of_arg_list, text.end(), [](const char c) { return c == ' ' || c == '('; });
			suffix = std::string(start_of_arg_list + 1, suffix_end);
			util::parsers::lower(suffix);
			start_of_arg_list = suffix_end;
		}

		if (start_of_arg_list == text.end()) {
			return InstructionInfo{command_name, {}, indices_set, suffix, l.offset};
		}

		auto const end_of_arg_list = *start_of_arg_list == '(' ? std::find(start_of_arg_list + 1, text.end(), ')') : text.end();
		start_of_arg_list += 1;

		auto arg_list = util::parsers::split_text(std::string(start_of_arg_list, end_of_arg_list), ';');

		std::for_each(arg_list.begin(), arg_list.end(), [](std::string& s) { return util::parsers::strip_text(s); });

		return InstructionInfo{command_name, indices_set, arg_list, suffix, l.offset};
	}

	InstructionInfo rw(const PreprocessedLine& l) {
		auto const& text = l.contents;
		auto first_break = std::find_if(text.begin(), text.end(), [](const char c) { return c == ' ' || c == '(' || c == '='; });

		std::string command_name(text.begin(), first_break);
		util::parsers::lower(command_name);

		// if there is a : or text has no letters, is a position declaration
		auto const has_colon = std::count(command_name.begin(), command_name.end(), ':') > 0;
		auto const has_letters =
		    std::count_if(command_name.begin(), command_name.end(), [](const char c) { return std::isdigit(c) == 0 && c != '.'; }) > 0;
		if (has_colon || !has_letters) {
			auto list = util::parsers::split_text(command_name, ':');

			return InstructionInfo{"", {}, std::move(list), {}, l.offset, true};
		}

		// This is a section header
		if (first_break == text.end()) {
			util::parsers::strip_text(command_name, "\t\n\v\f\r []");

			return InstructionInfo{"with"s, {}, {command_name}, {}, l.offset};
		}

		// skip passed all whitespace
		first_break = std::find_if(first_break + 1, text.end(), [](const char c) { return !(c == ' ' || c == '(' || c == '='); }) - 1;

		std::vector<std::string> parens_list;
		if (*first_break == '(') {
			auto const rparen = std::find(first_break + 1, text.end(), ')');
			parens_list = util::parsers::split_text(std::string(first_break + 1, rparen), ',');
			first_break = rparen;
		}

		if (first_break + 1 == text.end()) {
			return InstructionInfo{command_name, {}, std::move(parens_list), {}, l.offset};
		}

		auto const has_suffix = *first_break + 1 == '.' && first_break + 2 != text.end();

		std::string suffix;
		if (has_suffix) {
			auto const suffix_end = std::find_if(first_break + 1, text.end(), [](const char c) { return c == '='; });
			suffix = std::string(first_break + 2, suffix_end);
			util::parsers::lower(suffix);
			first_break = suffix_end;
		}
		else {
			first_break += 1;
		}

		if (first_break == text.end() && first_break + 1 == text.end()) {
			return InstructionInfo{command_name, {}, std::move(parens_list), suffix, l.offset};
		}

		return InstructionInfo{command_name, std::move(parens_list), {std::string(first_break + 1, text.end())}, suffix, l.offset};
	}
} // namespace bve::parsers::csv_rw_route::line_splitting
