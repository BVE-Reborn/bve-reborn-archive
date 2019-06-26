#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_track_freeobj(const line_splitting::InstructionInfo& inst) {
		instructions::track::FreeObj obj;

		switch (inst.args.size()) {
			default:
			case 3:
				set_positions<2>(obj, inst);
				[[fallthrough]];
			case 2:
				obj.free_obj_structure_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[1], 0));
				[[fallthrough]];
			case 1:
				obj.rail_index = gsl::narrow_cast<std::size_t>(util::parsers::parse_loose_integer(inst.args[0], 0));
				[[fallthrough]];
			case 0:
				break;
		}

		return obj;
	}

	Instruction create_instruction_track_wall(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 2, "Track.Wall");

		instructions::track::Wall w;

		w.rail_index = gsl::narrow_cast<std::size_t>(util::parsers::parse_loose_integer(inst.args[0], 0));
		auto const direction_num = util::parsers::parse_loose_integer(inst.args[1]);
		if (inst.args.size() >= 3) {
			w.wall_structure_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[2], 0));
		}

		switch (direction_num) {
			case -1:
				w.direction = instructions::track::Wall::Direction::left;
				break;
			default:
			case 0:
				w.direction = instructions::track::Wall::Direction::both;
				break;
			case 1:
				w.direction = instructions::track::Wall::Direction::right;
				break;
		}

		return w;
	}

	Instruction create_instruction_track_wallend(const line_splitting::InstructionInfo& inst) {
		return create_single_uint_instruction<instructions::track::WallEnd>(inst, "Track.WallEnd");
	}

	Instruction create_instruction_track_dike(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 2, "Track.Dike");

		instructions::track::Dike d;

		d.rail_index = gsl::narrow_cast<std::size_t>(util::parsers::parse_loose_integer(inst.args[0], 0));
		auto const direction_num = util::parsers::parse_loose_integer(inst.args[1]);
		if (inst.args.size() >= 3) {
			d.dike_structure_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[2], 0));
		}

		switch (direction_num) {
			case -1:
				d.direction = instructions::track::Dike::Direction::left;
				break;
			default:
			case 0:
				d.direction = instructions::track::Dike::Direction::both;
				break;
			case 1:
				d.direction = instructions::track::Dike::Direction::right;
				break;
		}

		return d;
	}

	Instruction create_instruction_track_dikeend(const line_splitting::InstructionInfo& inst) {
		return create_single_uint_instruction<instructions::track::DikeEnd>(inst, "Track.DikeEnd");
	}

	Instruction create_instruction_track_pole(const line_splitting::InstructionInfo& inst) {
		instructions::track::Pole p;

		switch (inst.args.size()) {
			default:
			case 5:
				p.pole_structure_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[4], 0));
				[[fallthrough]];
			case 4:
				p.interval = util::parsers::parse_loose_integer(inst.args[3], 1);
				[[fallthrough]];
			case 3:
				p.location = util::parsers::parse_loose_integer(inst.args[2], 0);
				[[fallthrough]];
			case 2:
				p.additional_rails = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[1], 0));
				[[fallthrough]];
			case 1:
				p.rail_index = gsl::narrow_cast<std::size_t>(util::parsers::parse_loose_integer(inst.args[0], 0));
				[[fallthrough]];
			case 0:
				break;
		}

		return p;
	}

	Instruction create_instruction_track_poleend(const line_splitting::InstructionInfo& inst) {
		return create_single_uint_instruction<instructions::track::PoleEnd>(inst, "Track.PoleEnd");
	}

	Instruction create_instruction_track_crack(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 2, "Track.Crack");

		instructions::track::Crack c;

		c.rail_index_1 = gsl::narrow_cast<std::size_t>(util::parsers::parse_loose_integer(inst.args[0]));
		c.rail_index_2 = gsl::narrow_cast<std::size_t>(util::parsers::parse_loose_integer(inst.args[1]));
		if (inst.args.size() >= 3) {
			c.crack_structure_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[2], 0));
		}

		return c;
	}

	Instruction create_instruction_track_ground(const line_splitting::InstructionInfo& inst) {
		return create_single_uint_instruction<instructions::track::Ground>(inst, "Track.Ground");
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
