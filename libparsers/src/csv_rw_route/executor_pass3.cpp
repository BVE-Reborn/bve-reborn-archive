#include "executor_pass3.hpp"
#include "csv_rw_route.hpp"

namespace parsers {
	namespace csv_rw_route {
		void execute_instructions_pass3(parsed_route_data& rd, instruction_list& list,
			errors::multi_error& errors,
			const find_relative_file_func& get_abs_path) {
			pass3_executor p3e(rd, errors, list.filenames, get_abs_path);

			for (auto& inst : list.instructions) {
				mapbox::util::apply_visitor(p3e, inst);
			}

			float largest_position = mapbox::util::apply_visitor([](auto& inst) -> float { return inst.absolute_position; },
				list.instructions.back());

			p3e.finalize(largest_position);
		}
	}
}
