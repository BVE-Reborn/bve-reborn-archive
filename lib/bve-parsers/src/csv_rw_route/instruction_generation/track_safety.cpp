#include "instruction_generator.hpp"

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_track_beacon(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 4, "Track.Beacon");

		instructions::track::Beacon b;

		b.type = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[0]));
		b.beacon_structure_index =
		    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1]));
		b.section = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[2]));
		b.data = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[3]));

		set_positions<4>(b, inst);

		return b;
	}

	Instruction create_instruction_track_transponder(const line_splitting::InstructionInfo& inst) {
		instructions::track::Transponder t;

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
						t.type = instructions::track::Transponder::Type::s_type;
						break;
					case 1:
						t.type = instructions::track::Transponder::Type::sn_type;
						break;
					case 2:
						t.type = instructions::track::Transponder::Type::departure;
						break;
					case 3:
						t.type = instructions::track::Transponder::Type::ats_p_renewal;
						break;
					case 4:
						t.type = instructions::track::Transponder::Type::ats_p_stop;
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

	Instruction create_instruction_track_atssn(const line_splitting::InstructionInfo& inst) {
		(void) inst;

		instructions::track::Transponder t;

		t.type = instructions::track::Transponder::Type::s_type;
		t.signal = 0;
		t.switch_system = true;

		return t;
	}

	Instruction create_instruction_track_atsp(const line_splitting::InstructionInfo& inst) {
		(void) inst;

		instructions::track::Transponder t;

		t.type = instructions::track::Transponder::Type::ats_p_renewal;
		t.signal = 0;
		t.switch_system = true;

		return t;
	}

	Instruction create_instruction_track_pattern(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 2, "Track.Pattern");

		instructions::track::Pattern p;

		auto const type_num = util::parse_loose_integer(inst.args[0]);
		p.type = type_num == 0 ? instructions::track::Pattern::Type::permanent
		                       : instructions::track::Pattern::Type::temporary;
		p.speed = util::parse_loose_float(inst.args[1]);

		return p;
	}

	Instruction create_instruction_track_plimit(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Track.PLimit");

		instructions::track::Pattern p;

		p.type = instructions::track::Pattern::Type::temporary;
		p.speed = util::parse_loose_float(inst.args[0]);

		return p;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
