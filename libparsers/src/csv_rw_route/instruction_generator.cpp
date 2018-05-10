#include "instruction_generation/instruction_generator.hpp"
#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <algorithm>
#include <parsers/errors.hpp>
#include <sstream>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		// ReSharper disable once CyclomaticComplexity
		instruction generate_instruction(const preprocessed_lines& lines,
		                                 const preprocessed_line& line,
		                                 errors::multi_error_t& errors,
		                                 std::string& with_value,
		                                 file_type const ft) {
			instruction i;

			auto parsed =
			    ft == file_type::csv ? line_splitting::csv(line) : line_splitting::rw(line);

			if (parsed.track_position) {
				return create_instruction_location_statement(parsed);
			}

			util::lower(parsed.name);

			if (ft == file_type::csv) {
				auto const dot_iter = std::find(parsed.name.begin(), parsed.name.end(), '.');
				auto const has_dot = dot_iter != parsed.name.end();

				// get fully qualified name
				if (has_dot && dot_iter == parsed.name.begin()) {
					parsed.name = with_value + parsed.name;
				}
			}
			else if (parsed.name != "with"s) {
				// Deal with special cases
				if (with_value.empty() && parsed.name != "with"s) {
					return instructions::route::comment{line.contents};
				}
				if (with_value == "signal") {
					parsed.indices.emplace_back(std::move(parsed.name));
					parsed.name = "@@signal@@signalindex"s;
				}
				else if (with_value == "cycle") {
					parsed.indices.emplace_back(std::move(parsed.name));
					parsed.name = "@@cycle@@groundstructureindex"s;
				}
				// Get normal qualitifed name
				else {
					parsed.name = "@@"s + with_value + "@@" + parsed.name;
				}
			}

			// lookup function
			auto const func_iter = function_mapping.find(parsed.name);
			if (func_iter == function_mapping.end()) {
				if (parsed.name == "with") {
					with_value = util::lower_copy(parsed.args[0]);
				}
				else {
					bool ignored;
					if (ft == file_type::csv) {
						ignored = parsed.name == "route.developerid"s
						          || parsed.name == "train.acceleration"s
						          || parsed.name == "train.station"s;
					}
					else {
						ignored = parsed.name == "@@route@@developerid"s
						          || parsed.name == "@@train@@acceleration"s
						          || parsed.name == "@@train@@station"s;
					}

					if (!ignored) {
						std::ostringstream oss;
						oss << "\"" << parsed.name << "\" is not a known function in a "
						    << (ft == file_type::csv ? "csv" : "rw") << " file";
						add_error(errors, lines.filenames[line.filename_index], line.line, oss);
					}
				}
				return instructions::naked::none{};
			}

			try {
				i = func_iter->second(parsed);
			}
			catch (const std::exception& e) {
				add_error(errors, lines.filenames[line.filename_index], line.line, e.what());
				return instructions::naked::none{};
			}

			return i;
		}
	} // namespace instruction_generation

	instruction_list generate_instructions(const preprocessed_lines& lines,
	                                       errors::multi_error_t& errors,
	                                       file_type const ft) {
		instruction_list i_list;
		i_list.instructions.reserve(lines.lines.size());

		std::string with_value;
		for (auto& line : lines.lines) {
			auto i =
			    instruction_generation::generate_instruction(lines, line, errors, with_value, ft);

			apply_visitor(
			    [&line](auto& inst) {
				    inst.file_index = line.filename_index;
				    inst.line = line.line;
			    },
			    i);

			i_list.instructions.emplace_back(i);
		}

		i_list.filenames = lines.filenames;

		return i_list;
	}
} // namespace csv_rw_route
} // namespace parsers
