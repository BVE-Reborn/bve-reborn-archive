#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace parsers::csv_rw_route::instruction_generation {
	instruction create_instruction_track_freeobj(const line_splitting::instruction_info& inst) {
		instructions::track::free_obj fobj;

		switch (inst.args.size()) {
			default:
			case 3:
				set_positions<2>(fobj, inst);
				// fall through
			case 2:
				fobj.free_obj_structure_index =
				    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1], 0));
				// fall through
			case 1:
				fobj.rail_index =
				    gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0], 0));
				// fall through
			case 0:
				break;
		}

		return fobj;
	}

	instruction create_instruction_track_wall(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 2, "Track.Wall");

		instructions::track::wall w;

		w.rail_index = gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0], 0));
		auto const direction_num = util::parse_loose_integer(inst.args[1]);
		if (inst.args.size() >= 3) {
			w.wall_structure_index =
			    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[2], 0));
		}

		switch (direction_num) {
			case -1:
				w.direction = instructions::track::wall::left;
				break;
			default:
			case 0:
				w.direction = instructions::track::wall::both;
				break;
			case 1:
				w.direction = instructions::track::wall::right;
				break;
		}

		return w;
	}

	instruction create_instruction_track_wallend(const line_splitting::instruction_info& inst) {
		return create_single_sizet_instruction<instructions::track::wall_end>(inst,
		                                                                      "Track.WallEnd");
	}

	instruction create_instruction_track_dike(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 2, "Track.Dike");

		instructions::track::dike d;

		d.rail_index = gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0], 0));
		auto const direction_num = util::parse_loose_integer(inst.args[1]);
		if (inst.args.size() >= 3) {
			d.dike_structure_index =
			    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[2], 0));
		}

		switch (direction_num) {
			case -1:
				d.direction = instructions::track::dike::left;
				break;
			default:
			case 0:
				d.direction = instructions::track::dike::both;
				break;
			case 1:
				d.direction = instructions::track::dike::right;
				break;
		}

		return d;
	}

	instruction create_instruction_track_dikeend(const line_splitting::instruction_info& inst) {
		return create_single_sizet_instruction<instructions::track::dike_end>(inst,
		                                                                      "Track.DikeEnd");
	}

	instruction create_instruction_track_pole(const line_splitting::instruction_info& inst) {
		instructions::track::pole p;

		switch (inst.args.size()) {
			default:
			case 5:
				p.pole_structure_index =
				    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[4], 0));
				// fall through
			case 4:
				p.interval = util::parse_loose_integer(inst.args[3], 1);
				// fall through
			case 3:
				p.location = util::parse_loose_integer(inst.args[2], 0);
				// fall through
			case 2:
				p.additional_rails =
				    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1], 0));
				// fall through
			case 1:
				p.rail_index =
				    gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0], 0));
				// fall through
			case 0:
				break;
		}

		return p;
	}

	instruction create_instruction_track_poleend(const line_splitting::instruction_info& inst) {
		return create_single_sizet_instruction<instructions::track::pole_end>(inst,
		                                                                      "Track.PoleEnd");
	}

	instruction create_instruction_track_crack(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 2, "Track.Crack");

		instructions::track::crack c;

		c.rail_index_1 = gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0]));
		c.rail_index_2 = gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[1]));
		if (inst.args.size() >= 3) {
			c.crack_structure_index =
			    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[2], 0));
		}

		return c;
	}

	instruction create_instruction_track_ground(const line_splitting::instruction_info& inst) {
		return create_single_sizet_instruction<instructions::track::ground>(inst, "Track.Ground");
	}
} // namespace parsers::csv_rw_route::instruction_generation
