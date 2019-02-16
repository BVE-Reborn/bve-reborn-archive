#include "csv_rw_route/instruction_generation/instruction_generator.hpp"
#include "parsers/csv_rw_route.hpp"
#include "util/parsing.hpp"
#include <algorithm>
#include <parsers/errors.hpp>
#include <sstream>

using namespace std::string_literals;

namespace bve::parsers::csv_rw_route {
	namespace instruction_generation {
		// ReSharper disable once CyclomaticComplexity
		Instruction generate_instruction(const PreprocessedLines& lines,
		                                 const PreprocessedLine& line,
		                                 errors::MultiError& errors,
		                                 std::string& with_value,
		                                 FileType const ft) {
			Instruction i;

			auto parsed = ft == FileType::csv ? line_splitting::csv(line) : line_splitting::rw(line);

			if (parsed.track_position) {
				return create_instruction_location_statement(parsed);
			}

			util::parsers::lower(parsed.name);

			if (ft == FileType::csv) {
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
					return instructions::route::Comment{line.contents};
				}
				if (with_value == "signal") {
					parsed.indices.emplace_back(std::move(parsed.name));
					parsed.name = "@@signal@@signalindex"s;
				}
				else if (with_value == "cycle") {
					parsed.indices.emplace_back(std::move(parsed.name));
					parsed.name = "@@cycle@@groundstructureindex"s;
				}
				// Get normal qualified name
				else {
					parsed.name = "@@"s + with_value + "@@" + parsed.name;
				}
			}

			// lookup function
			auto const func_iter = function_mapping.find(parsed.name);
			if (func_iter == function_mapping.end()) {
				if (parsed.name == "with") {
					with_value = util::parsers::lower_copy(parsed.args[0]);
				}
				else {
					bool ignored;
					if (ft == FileType::csv) {
						ignored =
						    parsed.name == "route.developerid"s || parsed.name == "train.acceleration"s || parsed.name == "train.station"s;
					}
					else {
						ignored = parsed.name == "@@route@@developerid"s || parsed.name == "@@train@@acceleration"s
						          || parsed.name == "@@train@@station"s;
					}

					if (!ignored) {
						std::ostringstream oss;
						oss << "\"" << parsed.name << "\" is not a known function in a " << (ft == FileType::csv ? "csv" : "rw") << " file";
						add_error(errors, lines.filenames[line.filename_index], line.line, oss);
					}
				}
				return instructions::naked::None{};
			}

			try {
				i = func_iter->second(parsed);
			}
			catch (const std::exception& e) {
				add_error(errors, lines.filenames[line.filename_index], line.line, e.what());
				return instructions::naked::None{};
			}

			return i;
		}
	} // namespace instruction_generation

	InstructionList generate_instructions(const PreprocessedLines& lines, errors::MultiError& errors, FileType const ft) {
		InstructionList i_list;
		i_list.instructions.reserve(lines.lines.size());

		std::string with_value;
		for (auto& line : lines.lines) {
			auto i = instruction_generation::generate_instruction(lines, line, errors, with_value, ft);

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
} // namespace bve::parsers::csv_rw_route
