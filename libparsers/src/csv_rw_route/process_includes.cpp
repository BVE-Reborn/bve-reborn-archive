#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <numeric>
#include <regex>
#include <set>
#include <sstream>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	namespace {
		using line_break_list = std::vector<std::string::const_iterator>;

		static line_break_list list_line_breaks(const std::string& str) {
			line_break_list breaks;
			for (auto i = str.cbegin(); i != str.cend(); ++i) {
				if (*i == '\n') {
					breaks.emplace_back(i);
				}
			}
			return breaks;
		}

		static std::size_t line_number(const line_break_list& breaks, std::string::const_iterator pos) {
			auto iter = std::upper_bound(breaks.begin(), breaks.end(), pos);
			return std::distance(breaks.begin(), iter);
		}

		static void add_error(const line_break_list& line_br_list,
		                      errors::errors_t errors,
		                      std::string::const_iterator pos,
		                      const char* msg) {
			auto num = line_number(line_br_list, pos);
			errors.emplace_back(errors::error_t{num, msg});
		}

		static void remove_duplicate_filenames(preprocessed_lines& lines) {
			if (lines.filenames.empty()) {
				return;
			}

			// Pairs of filename and original index
			std::vector<std::pair<std::string, std::size_t>> line_pairs;
			line_pairs.reserve(lines.filenames.size());

			for (std::size_t i = 0; i < lines.filenames.size(); ++i) {
				line_pairs.emplace_back(std::move(lines.filenames[i]), i);
			}

			// Positional mapping from old index to new;
			std::vector<std::size_t> mapping(lines.filenames.size(), 0);
			std::vector<std::string> new_file_list;

			std::sort(line_pairs.begin(), line_pairs.end(),
			          [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });

			mapping[line_pairs.front().second] = 0;
			new_file_list.emplace_back(line_pairs.front().first);

			// Copy the unique filenames (as they are sorted) and create the
			// mapping
			std::size_t new_index = 0;
			for (std::size_t i = 1; i < line_pairs.size(); ++i) {
				if (line_pairs[i - 1].first != line_pairs[i].first) {
					new_file_list.emplace_back(line_pairs[i].first);
					new_index++;
				}
				mapping[line_pairs[i].second] = new_index;
			}

			lines.filenames = std::move(new_file_list);

			// Apply the new mapping
			for (auto& line : lines.lines) {
				line.filename_index = mapping[line.filename_index];
			}
		}

		// Capture group 1 is filename
		// Capture group 2 is position offset
		// Capture group 3 is everything after the first filename
		static std::regex include_finder(
		    "\\$Include\\(([\\w\\-. "
		    "\\\\/]+\\s*)(?::\\s*(\\d+))*([\\w\\s;]*)\\)",
		    std::regex_constants::icase | std::regex_constants::ECMAScript | std::regex_constants::optimize);
	} // namespace

	struct include_pos {
		std::string filename;
		float offset;
		std::size_t line;
		std::string::const_iterator start;
		std::string::const_iterator end;
	};

	static include_pos parse_weighted_include(const line_break_list& breaks,
	                                          const std::smatch& match,
	                                          openbve2::datatypes::rng& rng) {
		auto string = match[1].str() + match[3].str();

		auto split_includes = util::split_text(string, ';');
		if (split_includes.size() % 2 != 0) {
			throw std::invalid_argument("Weighted includes must have an even amount of arguments");
		}

		// list of weights
		std::vector<float> weights;
		weights.reserve(split_includes.size() / 2);

		for (std::size_t i = 1; i < split_includes.size(); i += 2) {
			weights.emplace_back(util::parse_loose_float(split_includes[i], 1));
		}

		// turn the list of weights into partial sums
		std::partial_sum(weights.begin(), weights.end(), weights.begin());

		// pick a random value in in the possible weights
		std::uniform_real_distribution<float> dist(0, weights.back());

		auto choosen_value = dist(rng);

		auto choosen_iter = std::lower_bound(weights.begin(), weights.end(), choosen_value);
		auto choosen_offset = std::distance(weights.begin(), choosen_iter);

		return include_pos{split_includes[choosen_offset * 2], 0, line_number(breaks, match[0].first), match[0].first,
		                   match[0].second};
	}

	static include_pos parse_offset_include(const line_break_list& breaks, const std::smatch& match) {
		auto filename = match[1].str();
		auto offset_str = match[2].str();

		auto offset = util::parse_loose_float(offset_str, 0);

		return include_pos{filename, offset, line_number(breaks, match[0].first), match[0].first, match[0].second};
	}

	static include_pos parse_naked_include(const line_break_list& breaks, const std::smatch& match) {
		auto filename = match[1].str();

		return include_pos{filename, 0, line_number(breaks, match[0].first), match[0].first, match[0].second};
	}

	static std::vector<include_pos> parse_include_directives(const std::string& contents,
	                                                         openbve2::datatypes::rng& rng,
	                                                         errors::errors_t& errors) {
		std::vector<include_pos> includes;

		auto line_br_list = list_line_breaks(contents);

		auto regex_start = std::sregex_iterator(contents.begin(), contents.end(), include_finder);
		auto regex_end = std::sregex_iterator();

		for (std::sregex_iterator i = regex_start; i != regex_end; ++i) {
			std::smatch match = *i;

			try {
				// Weighted include
				if (match[3].length() != 0 && *match[3].first == ';') {
					includes.emplace_back(parse_weighted_include(line_br_list, match, rng));
				}

				// Offset include
				else if (match[2].length() != 0) {
					includes.emplace_back(parse_offset_include(line_br_list, match));
				}

				// Naked include
				else {
					includes.emplace_back(parse_naked_include(line_br_list, match));
				}
			}
			catch (const std::invalid_argument& e) {
				add_error(line_br_list, errors, match[0].first, e.what());
			}
		}

		return includes;
	}

	static preprocessed_lines recursive_process_includes(const std::set<std::string>& past_files,
	                                                     const std::string& current_filename,
	                                                     openbve2::datatypes::rng& rng,
	                                                     errors::multi_error& errors,
	                                                     file_type ft,
	                                                     const find_relative_file_func& get_abs_path) {
		auto& current_file_errors = errors[current_filename];

		// Read file
		auto file_contents = util::load_from_file_utf8_bom(current_filename);
		util::remove_comments(file_contents, ';', ft == file_type::csv);

		// Extract array of lines
		auto file_line_array = util::split_text(file_contents, '\n');

		// Find all include directives
		auto include_list = parse_include_directives(file_contents, rng, current_file_errors);

		// Resolve absolute filenames
		std::transform(include_list.begin(), include_list.end(), include_list.begin(),
		               [&get_abs_path, &current_filename](include_pos include) {
			               include.filename = get_abs_path(current_filename, include.filename);
			               return include;
		               });

		// Check for circular dependencies
		for (auto& include : include_list) {
			if (past_files.find(include.filename) != past_files.end()) {
				std::ostringstream err;
				err << "File \"" << include.filename
				    << "\" has included this file. There is a circular chain "
				       "of includes. Including nothing.";
				current_file_errors.emplace_back<errors::error_t>({include.line, err.str()});
				include.filename = ""s;
			}
		}

		// Run include file processing on each of this file's includes
		std::vector<preprocessed_lines> include_texts;
		include_texts.reserve(include_list.size());

		std::transform(include_list.begin(), include_list.end(), std::back_inserter(include_texts),
		               [&](const include_pos& include) -> preprocessed_lines {
			               // Error in file name handling/circular depedency
			               if (include.filename.empty()) {
				               return {};
			               }

			               // Add the included file to include chain
			               auto include_chain_list = past_files;
			               include_chain_list.insert(include.filename);

			               try {
				               return recursive_process_includes(include_chain_list, include.filename, rng, errors, ft,
				                                                 get_abs_path);
			               }
			               catch (std::invalid_argument& e) {
				               current_file_errors.emplace_back<errors::error_t>({include.line, e.what()});
				               return {};
			               }
		               });

		preprocessed_lines output;

		std::vector<std::size_t> include_file_index_mapping;

		// Add current filename to filename mapping
		output.filenames.emplace_back(current_filename);
		include_file_index_mapping.emplace_back(0);

		// Compute filename indices
		for (auto& include : include_list) {
			auto insert_iter = std::lower_bound(output.filenames.begin(), output.filenames.end(), include.filename);
			if (insert_iter == output.filenames.end() || *insert_iter != include.filename) {
				output.filenames.insert(insert_iter, include.filename);
			}
		}

		for (auto& include : include_list) {
			auto insert_iter = std::lower_bound(output.filenames.begin(), output.filenames.end(), include.filename);
			auto insert_index = std::distance(output.filenames.begin(), insert_iter);
			include_file_index_mapping.emplace_back(insert_index);
		}

		std::size_t last_line = 0;
		// Concatinate includes
		for (std::size_t i = 0; i < include_list.size(); ++i) {
			auto include = include_list[i];
			auto contents = include_texts[i];
			auto name_index = include_file_index_mapping[i + 1];

			// Add all lines before the current include and after the last one
			for (std::size_t j = last_line; j < include.line; ++j) {
				output.lines.emplace_back<preprocessed_line>({std::move(file_line_array[j]), 0, j + 1, 0});
			}

			// Copy contents of the include
			std::transform(contents.lines.begin(), contents.lines.end(), std::back_inserter(output.lines),
			               [&](preprocessed_line l) {
				               l.offset += include.offset;
				               // Original filename index of 0 means it's the
				               // file we're including
				               if (l.filename_index == 0) {
					               l.filename_index = name_index;
				               }
				               // Duplicate file indices will be removed later
				               else {
					               l.filename_index += output.filenames.size();
				               }
				               return l;
			               });

			// Copy filenames
			std::copy(contents.filenames.begin(), contents.filenames.end(), std::back_inserter(output.filenames));

			// Start at next line
			last_line = include.line + 1;
		}

		// Add all remaining lines
		for (std::size_t j = last_line; j < file_line_array.size(); ++j) {
			output.lines.emplace_back<preprocessed_line>({std::move(file_line_array[j]), 0, j, 0});
		}

		return output;
	}

	preprocessed_lines process_include_files(const std::string& filename,
	                                         openbve2::datatypes::rng& rng,
	                                         errors::multi_error& errors,
	                                         file_type ft,
	                                         const find_relative_file_func& get_abs_path) {
		auto lines =
		    recursive_process_includes(std::set<std::string>{filename}, filename, rng, errors, ft, get_abs_path);
		remove_duplicate_filenames(lines);
		lines.lines.erase(std::remove_if(lines.lines.begin(), lines.lines.end(),
		                                 [](const preprocessed_line& l) { return l.contents.empty(); }),
		                  lines.lines.end());
		return lines;
	}
} // namespace csv_rw_route
} // namespace parsers
