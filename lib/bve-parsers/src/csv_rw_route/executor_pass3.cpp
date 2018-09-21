#include "src/csv_rw_route/executor_pass3/executor_pass3.hpp"
#include "csv_rw_route.hpp"
#include <parsers/errors.hpp>

namespace bve::parsers::csv_rw_route {
	void execute_instructions_pass3(ParsedRoute& rd,
	                                InstructionList& list,
	                                errors::MultiError& errors,
	                                const RelativeFileFunc& get_abs_path) {
		Pass3Executor p3_e(rd, errors, list.filenames, get_abs_path);

		for (auto& inst : list.instructions) {
			apply_visitor(p3_e, inst);
		}

		auto const largest_position =
		    apply_visitor([](auto& inst) -> float { return inst.absolute_position; },
		                  list.instructions.back());

		p3_e.finalize(largest_position);
	}
} // namespace bve::parsers::csv_rw_route
