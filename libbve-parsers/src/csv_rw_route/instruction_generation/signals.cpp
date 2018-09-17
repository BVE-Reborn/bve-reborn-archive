#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	instruction create_instruction_signal(const line_splitting::instruction_info& inst) {
		indices_at_least(inst, 1, "Signal");
		args_at_least(inst, 1, "Signal");

		auto const signal_index =
		    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.indices[0]));

		// Signal File version
		if (inst.args.size() >= 2) {
			instructions::naked::signal s;
			s.signal_index = signal_index;
			s.signal_filename = inst.args[0];
			s.glow_filename = inst.args[1];
			return s;
		}

		// Animated File version
		instructions::naked::signal_animated sa;
		sa.signal_index = signal_index;
		sa.filename = inst.args[0];
		return sa;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
