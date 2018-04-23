#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_track_railstart(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "RailStart");

			instructions::track::RailStart rs;

			switch (inst.args.size()) {
				default:
				case 4:
					try {
						rs.rail_type =
						    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[3]));
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 3:
					try {
						rs.y_offset = util::parse_loose_float(inst.args[2]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 2:
					try {
						rs.x_offset = util::parse_loose_float(inst.args[1]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 1:
					rs.rail_index =
					    gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0]));
					break;
				case 0:
					// Make GCC happy
					std::abort();
					break;
			}

			return rs;
		}

		instruction create_instruction_track_rail(const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Rail");

			instructions::track::Rail r;

			switch (inst.args.size()) {
				default:
				case 4:
					try {
						r.rail_type =
						    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[3]));
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 3:
					try {
						r.y_offset = util::parse_loose_float(inst.args[2]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 2:
					try {
						r.x_offset = util::parse_loose_float(inst.args[1]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 1:
					r.rail_index =
					    gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0]));
					break;
				case 0:
					// Make GCC happy
					std::abort();
					break;
			}

			return r;
		}

		instruction create_instruction_track_railtype(
		    const line_splitting::instruction_info& inst) {
			instructions::track::RailType rt;

			switch (inst.args.size()) {
				default:
				case 2:
					rt.rail_type =
					    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1], 0));
					// fall through
				case 1:
					rt.rail_index =
					    gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0], 0));
					// fall through
				case 0:
					break;
			}

			return rt;
		}

		instruction create_instruction_track_railend(const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "RailEnd");

			instructions::track::RailEnd re;

			switch (inst.args.size()) {
				default:
				case 3:
					try {
						re.y_offset = util::parse_loose_float(inst.args[2]);
					}
					catch (const std::invalid_argument&) {
					};
					// fall through
				case 2:
					try {
						re.x_offset = util::parse_loose_float(inst.args[1]);
					}
					catch (const std::invalid_argument&) {
					};
					// fall through
				case 1:
					re.rail_index =
					    gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0]));
					break;
				case 0:
					// Make GCC happy
					std::abort();
					break;
			}

			return re;
		}

		instruction create_instruction_track_accuracy(
		    const line_splitting::instruction_info& inst) {
			// Ignored instruction
			(void) inst;
			return instructions::naked::None{};
		}

		instruction create_instruction_track_adhesion(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<instructions::track::Adhesion>(inst,
			                                                                      "Track.Adhesion");
		}

		instruction create_instruction_track_pitch(const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<instructions::track::Pitch>(inst, "Track.Pitch");
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
