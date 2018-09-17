#include "instruction_generator.hpp"

namespace parsers::csv_rw_route::instruction_generation {
	instruction create_instruction_track_curve(const line_splitting::instruction_info& inst) {
		instructions::track::curve c;

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

	instruction create_instruction_track_turn(const line_splitting::instruction_info& inst) {
		return create_single_float_instruction<instructions::track::turn>(inst, "Track.Turn", 0);
	}

	instruction create_instruction_track_height(const line_splitting::instruction_info& inst) {
		return create_single_float_instruction<instructions::track::height>(inst, "Track.Height");
	}
} // namespace parsers::csv_rw_route::instruction_generation
