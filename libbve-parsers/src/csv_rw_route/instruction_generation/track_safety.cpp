#include "instruction_generator.hpp"

namespace parsers::csv_rw_route::instruction_generation {
	instruction create_instruction_track_beacon(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 4, "Track.Beacon");

		instructions::track::beacon b;

		b.type = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[0]));
		b.beacon_structure_index =
		    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1]));
		b.section = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[2]));
		b.data = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[3]));

		set_positions<4>(b, inst);

		return b;
	}

	instruction create_instruction_track_transponder(const line_splitting::instruction_info& inst) {
		instructions::track::transponder t;

		switch (inst.args.size()) {
			default:
			case 3:
				t.switch_system = util::parse_loose_integer(inst.args[2], 0) == 0;
				// fall through
			case 2:
				t.signal = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1], 0));
				// fall through
			case 1: {
				auto const type_num = util::parse_loose_integer(inst.args[0], 0);

				switch (type_num) {
					default:
					case 0:
						t.type = instructions::track::transponder::s_type;
						break;
					case 1:
						t.type = instructions::track::transponder::sn_type;
						break;
					case 2:
						t.type = instructions::track::transponder::departure;
						break;
					case 3:
						t.type = instructions::track::transponder::ats_p_renewal;
						break;
					case 4:
						t.type = instructions::track::transponder::ats_p_stop;
						break;
				}
			}
				// fall through
			case 0:
				break;
		}

		set_positions<3>(t, inst);

		return t;
	}

	instruction create_instruction_track_atssn(const line_splitting::instruction_info& inst) {
		(void) inst;

		instructions::track::transponder t;

		t.type = instructions::track::transponder::s_type;
		t.signal = 0;
		t.switch_system = true;

		return t;
	}

	instruction create_instruction_track_atsp(const line_splitting::instruction_info& inst) {
		(void) inst;

		instructions::track::transponder t;

		t.type = instructions::track::transponder::ats_p_renewal;
		t.signal = 0;
		t.switch_system = true;

		return t;
	}

	instruction create_instruction_track_pattern(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 2, "Track.Pattern");

		instructions::track::pattern p;

		auto const type_num = util::parse_loose_integer(inst.args[0]);
		p.type = type_num == 0 ? instructions::track::pattern::permanent
		                       : instructions::track::pattern::temporary;
		p.speed = util::parse_loose_float(inst.args[1]);

		return p;
	}

	instruction create_instruction_track_plimit(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Track.PLimit");

		instructions::track::pattern p;

		p.type = instructions::track::pattern::temporary;
		p.speed = util::parse_loose_float(inst.args[0]);

		return p;
	}
} // namespace parsers::csv_rw_route::instruction_generation
