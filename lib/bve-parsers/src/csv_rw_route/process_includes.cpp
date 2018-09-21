#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace bve::parsers::csv_rw_route {
	namespace {
		using LineBreakList = std::vector<std::string::const_iterator>;

		LineBreakList list_line_breaks(const std::string& str) {
			LineBreakList breaks;
			for (auto i = str.cbegin(); i != str.cend(); ++i) {
				if (*i == '\n') {
					breaks.emplace_back(i);
				}
			}
			return breaks;
		}

		std::size_t line_number(const LineBreakList& breaks,
		                        std::string::const_iterator const pos) {
			auto const iter = std::upper_bound(breaks.begin(), breaks.end(), pos);
			return std::distance(breaks.begin(), iter);
		}

		void add_error(const LineBreakList& line_br_list,
		               errors::Errors errors,
		               std::string::const_iterator const pos,
		               const char* msg) {
			auto const num = line_number(line_br_list, pos);
			errors::add_error(errors, num, msg);
		}

		void remove_duplicate_filenames(PreprocessedLines& lines) {
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
		std::regex include_finder(R"(\$Include\(([\w\-. \\/]+\s*)(?::\s*(\d+))*([\w\s;]*)\))",
		                          std::regex_constants::icase | std::regex_constants::ECMAScript
		                              | std::regex_constants::optimize);
	} // namespace

	struct IncludePos {
		std::string filename;
		float offset;
		std::size_t line;
		std::string::const_iterator start;
		std::string::const_iterator end;
	};

	static IncludePos parse_weighted_include(const LineBreakList& breaks,
	                                         const std::smatch& match,
	                                         core::datatypes::RNG& rng) {
		auto const string = match[1].str() + match[3].str();

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
		// ReSharper disable once CppLocalVariableMayBeConst
		std::uniform_real_distribution<float> dist(0, weights.back());

		auto const chosen_value = dist(rng);

		auto const chosen_iter = std::lower_bound(weights.begin(), weights.end(), chosen_value);
		auto const chosen_offset = std::distance(weights.begin(), chosen_iter);

		return IncludePos{split_includes[chosen_offset * 2], 0, line_number(breaks, match[0].first),
		                  match[0].first, match[0].second};
	}

	static IncludePos parse_offset_include(const LineBreakList& breaks, const std::smatch& match) {
		auto const filename = match[1].str();
		auto const offset_str = match[2].str();

		auto const offset = util::parse_loose_float(offset_str, 0);

		return IncludePos{filename, offset, line_number(breaks, match[0].first), match[0].first,
		                  match[0].second};
	}

	static IncludePos parse_naked_include(const LineBreakList& breaks, const std::smatch& match) {
		auto const filename = match[1].str();

		return IncludePos{filename, 0, line_number(breaks, match[0].first), match[0].first,
		                  match[0].second};
	}

	static std::vector<IncludePos> parse_include_directives(const std::string& contents,
	                                                        core::datatypes::RNG& rng,
	                                                        errors::Errors& errors) {
		std::vector<IncludePos> includes;

		auto const line_br_list = list_line_breaks(contents);

		auto const regex_start =
		    std::sregex_iterator(contents.begin(), contents.end(), include_finder);
		auto const regex_end = std::sregex_iterator();

		for (auto i = regex_start; i != regex_end; ++i) {
			auto const match = *i;

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

	static PreprocessedLines recursive_process_includes(const std::set<std::string>& past_files,
	                                                    const std::string& current_filename,
	                                                    core::datatypes::RNG& rng,
	                                                    errors::MultiError& errors,
	                                                    FileType ft,
	                                                    const RelativeFileFunc& get_abs_path) {
		auto& current_file_errors = errors[current_filename];

		// Read file
		auto file_contents = util::load_from_file_utf8_bom(current_filename);
		util::remove_comments(file_contents, ';', ft == FileType::csv);

		// Extract array of lines
		auto file_line_array = util::split_text(file_contents, '\n');

		// Find all include directives
		auto include_list = parse_include_directives(file_contents, rng, current_file_errors);

		// Resolve absolute filenames
		std::transform(include_list.begin(), include_list.end(), include_list.begin(),
		               [&get_abs_path, &current_filename](IncludePos include) {
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
				errors::add_error(current_file_errors, include.line, err);
				include.filename = ""s;
			}
		}

		// Run include file processing on each of this file's includes
		std::vector<PreprocessedLines> include_texts;
		include_texts.reserve(include_list.size());

		std::transform(include_list.begin(), include_list.end(), std::back_inserter(include_texts),
		               [&](const IncludePos& include) -> PreprocessedLines {
			               // Error in file name handling/circular dependency
			               if (include.filename.empty()) {
				               return {};
			               }

			               // Add the included file to include chain
			               auto include_chain_list = past_files;
			               include_chain_list.insert(include.filename);

			               try {
				               return recursive_process_includes(include_chain_list,
				                                                 include.filename, rng, errors, ft,
				                                                 get_abs_path);
			               }
			               catch (std::invalid_argument& e) {
				               errors::add_error(current_file_errors, include.line, e.what());
				               return {};
			               }
		               });

		PreprocessedLines output;

		std::vector<std::size_t> include_file_index_mapping;

		// Add current filename to filename mapping
		output.filenames.emplace_back(current_filename);
		include_file_index_mapping.emplace_back(0);

		// Compute filename indices
		for (auto& include : include_list) {
			auto const insert_iter = std::lower_bound(output.filenames.begin(),
			                                          output.filenames.end(), include.filename);
			if (insert_iter == output.filenames.end() || *insert_iter != include.filename) {
				output.filenames.insert(insert_iter, include.filename);
			}
		}

		for (auto& include : include_list) {
			auto const insert_iter = std::lower_bound(output.filenames.begin(),
			                                          output.filenames.end(), include.filename);
			auto insert_index = std::distance(output.filenames.begin(), insert_iter);
			include_file_index_mapping.emplace_back(insert_index);
		}

		std::size_t last_line = 0;
		// Concatenate includes
		for (std::size_t i = 0; i < include_list.size(); ++i) {
			auto include = include_list[i];
			auto contents = include_texts[i];
			auto name_index = include_file_index_mapping[i + 1];

			// Add all lines before the current include and after the last one
			// ReSharper disable once CppUseAuto
			for (std::size_t j = last_line; j < include.line; ++j) {
				output.lines.emplace_back<PreprocessedLine>(
				    {std::move(file_line_array[j]), 0, j + 1, 0});
			}

			// Copy contents of the include
			std::transform(contents.lines.begin(), contents.lines.end(),
			               std::back_inserter(output.lines), [&](PreprocessedLine l) {
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
			std::copy(contents.filenames.begin(), contents.filenames.end(),
			          std::back_inserter(output.filenames));

			// Start at next line
			last_line = include.line + 1;
		}

		// Add all remaining lines
		for (auto j = last_line; j < file_line_array.size(); ++j) {
			output.lines.emplace_back<PreprocessedLine>({std::move(file_line_array[j]), 0, j, 0});
		}

		return output;
	}

	PreprocessedLines process_include_directives(const std::string& filename,
	                                             core::datatypes::RNG& rng,
	                                             errors::MultiError& errors,
	                                             FileType const ft,
	                                             const RelativeFileFunc& get_abs_path) {
		auto lines = recursive_process_includes(std::set<std::string>{filename}, filename, rng,
		                                        errors, ft, get_abs_path);
		remove_duplicate_filenames(lines);
		lines.lines.erase(std::remove_if(lines.lines.begin(), lines.lines.end(),
		                                 [](const PreprocessedLine& l) {
			                                 return l.contents.empty();
		                                 }),
		                  lines.lines.end());
		return lines;
	}
} // namespace bve::parsers::csv_rw_route
