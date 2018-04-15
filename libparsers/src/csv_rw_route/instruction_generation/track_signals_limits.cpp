#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_track_limit(const line_splitting::instruction_info& inst) {
			instructions::track::Limit l;

			switch (inst.args.size()) {
				default:
				case 3: {
					auto course_num = util::parse_loose_integer(inst.args[2], 0);

					switch (course_num) {
						case -1:
							l.cource = instructions::track::Limit::Cource_t::Left;
							break;
						default:
						case 0:
							l.cource = instructions::track::Limit::Cource_t::None;
							break;
						case 1:
							l.cource = instructions::track::Limit::Cource_t::Right;
							break;
					}
				}
					// fall through
				case 2: {
					auto post_num = util::parse_loose_integer(inst.args[1], 0);

					switch (post_num) {
						case -1:
							l.post = instructions::track::Limit::Post_t::Left;
							break;
						default:
						case 0:
							l.post = instructions::track::Limit::Post_t::None;
							break;
						case 1:
							l.post = instructions::track::Limit::Post_t::Right;
							break;
					}
				}
					// fall through
				case 1:
					l.speed = util::parse_loose_float(inst.args[0], 0);
					// fall through
				case 0:
					break;
			}

			return l;
		}

		instruction create_instruction_track_section(const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Track.Section");

			instructions::track::Section s;
			s.a_term.reserve(inst.args.size());

			std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(s.a_term),
			               [](const std::string& val) {
				               return gsl::narrow<std::size_t>(util::parse_loose_integer(val, 0));
			               });

			return s;
		}

		instruction create_instruction_track_sigf(const line_splitting::instruction_info& inst) {
			args_at_least(inst, 2, "Track.SigF");

			instructions::track::SigF sf;

			sf.signal_index = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[0]));
			sf.section = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1]));
			set_positions<2>(sf, inst);

			return sf;
		}
		instruction create_instruction_track_signal(const line_splitting::instruction_info& inst) {
			instructions::track::Signal s;

			if (!inst.args.empty()) {
				auto type_num = util::parse_loose_integer(inst.args[0]);

				switch (type_num) {
					case 2:
						s.type = instructions::track::Signal::R_Y;
						break;
					default:
					case -2:
						s.type = instructions::track::Signal::R_G;
						break;
					case 3:
						s.type = instructions::track::Signal::R_Y_G;
						break;
					case 4:
						s.type = instructions::track::Signal::R_YY_Y_G;
						break;
					case -4:
						s.type = instructions::track::Signal::R_Y_YG_G;
						break;
					case 5:
						s.type = instructions::track::Signal::R_YY_Y_YG_G;
						break;
					case -5:
						s.type = instructions::track::Signal::R_Y_YG_G_GG;
						break;
					case 6:
						s.type = instructions::track::Signal::R_YY_Y_YG_G_GG;
						break;
				}
			}
			else {
				s.type = instructions::track::Signal::R_G;
			}

			set_positions<2>(s, inst);

			return s;
		}

		instruction create_instruction_track_relay(const line_splitting::instruction_info& inst) {
			instructions::track::Relay r;

			set_positions<0>(r, inst);

			return r;
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
