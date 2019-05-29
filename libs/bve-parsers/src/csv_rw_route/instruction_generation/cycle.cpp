#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_cycle_ground(const line_splitting::InstructionInfo& inst) {
		indices_at_least(inst, 1, "Cycle.Ground");
		args_at_least(inst, 1, "Cycle.Ground");

		instructions::cycle::Ground g;

		g.cycle_structure_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.indices[0]));

		g.input_indices.reserve(inst.args.size());
		std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(g.input_indices),
		               [](const std::string& arg) { return gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(arg)); });

		return g;
	}

	Instruction create_instruction_cycle_rail(const line_splitting::InstructionInfo& inst) {
		indices_at_least(inst, 1, "Cycle.Rail");
		args_at_least(inst, 1, "Cycle.Rail");

		instructions::cycle::Rail r;

		r.cycle_structure_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.indices[0]));

		r.input_indices.reserve(inst.args.size());
		std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(r.input_indices),
		               [](const std::string& arg) { return gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(arg)); });

		return r;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
