#include "instruction_generator.hpp"

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_track_curve(const line_splitting::InstructionInfo& inst) {
		instructions::track::Curve c;

		switch (inst.args.size()) {
			default:
			case 2:
				c.cant = util::parse_loose_float(inst.args[1], 0);
				// fall through
			case 1:
				c.radius = util::parse_loose_float(inst.args[0], 0);
				// fall through
			case 0:
				break;
		}

		return c;
	}

	Instruction create_instruction_track_turn(const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::track::Turn>(inst, "Track.Turn", 0);
	}

	Instruction create_instruction_track_height(const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::track::Height>(inst, "Track.Height");
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
