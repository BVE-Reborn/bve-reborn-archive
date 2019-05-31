#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_track_limit(const line_splitting::InstructionInfo& inst) {
		instructions::track::Limit l;

		switch (inst.args.size()) {
			default:
			case 3: {
				auto const course_num = util::parsers::parse_loose_integer(inst.args[2], 0);

				switch (course_num) {
					case -1:
						l.course = instructions::track::Limit::Course::left;
						break;
					default:
					case 0:
						l.course = instructions::track::Limit::Course::none;
						break;
					case 1:
						l.course = instructions::track::Limit::Course::right;
						break;
				}
			}
				[[fallthrough]];
			case 2: {
				auto const post_num = util::parsers::parse_loose_integer(inst.args[1], 0);

				switch (post_num) {
					case -1:
						l.post = instructions::track::Limit::Post::left;
						break;
					default:
					case 0:
						l.post = instructions::track::Limit::Post::none;
						break;
					case 1:
						l.post = instructions::track::Limit::Post::right;
						break;
				}
			}
				[[fallthrough]];
			case 1:
				l.speed = util::parsers::parse_loose_float(inst.args[0], 0);
				[[fallthrough]];
			case 0:
				break;
		}

		return l;
	}

	Instruction create_instruction_track_section(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Track.Section");

		instructions::track::Section s;
		s.a_term.reserve(inst.args.size());

		std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(s.a_term),
		               [](const std::string& val) { return gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(val, 0)); });

		return s;
	}

	Instruction create_instruction_track_sigf(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 2, "Track.SigF");

		instructions::track::SigF sf;

		sf.signal_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[0]));
		sf.section = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.args[1]));
		set_positions<2>(sf, inst);

		return sf;
	}
	Instruction create_instruction_track_signal(const line_splitting::InstructionInfo& inst) {
		instructions::track::Signal s;

		if (!inst.args.empty()) {
			auto const type_num = util::parsers::parse_loose_integer(inst.args[0]);

			switch (type_num) {
				case 2:
					s.type = instructions::track::Signal::Aspect::r_y;
					break;
				default:
				case -2:
					s.type = instructions::track::Signal::Aspect::r_g;
					break;
				case 3:
					s.type = instructions::track::Signal::Aspect::r_y_g;
					break;
				case 4:
					s.type = instructions::track::Signal::Aspect::r_yy_y_g;
					break;
				case -4:
					s.type = instructions::track::Signal::Aspect::r_y_yg_g;
					break;
				case 5:
					s.type = instructions::track::Signal::Aspect::r_yy_y_yg_g;
					break;
				case -5:
					s.type = instructions::track::Signal::Aspect::r_y_yg_g_gg;
					break;
				case 6:
					s.type = instructions::track::Signal::Aspect::r_yy_y_yg_g_gg;
					break;
			}
		}
		else {
			s.type = instructions::track::Signal::Aspect::r_g;
		}

		set_positions<2>(s, inst);

		return s;
	}

	Instruction create_instruction_track_relay(const line_splitting::InstructionInfo& inst) {
		instructions::track::Relay r;

		set_positions<0>(r, inst);

		return r;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
