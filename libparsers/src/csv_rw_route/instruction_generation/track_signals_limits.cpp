#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_track_limit(const line_splitting::instruction_info& inst) {
			instructions::track::limit l;

			switch (inst.args.size()) {
				default:
				case 3: {
					auto const course_num = util::parse_loose_integer(inst.args[2], 0);

					switch (course_num) {
						case -1:
							l.cource = instructions::track::limit::cource_t::left;
							break;
						default:
						case 0:
							l.cource = instructions::track::limit::cource_t::none;
							break;
						case 1:
							l.cource = instructions::track::limit::cource_t::right;
							break;
					}
				}
					// fall through
				case 2: {
					auto const post_num = util::parse_loose_integer(inst.args[1], 0);

					switch (post_num) {
						case -1:
							l.post = instructions::track::limit::post_t::left;
							break;
						default:
						case 0:
							l.post = instructions::track::limit::post_t::none;
							break;
						case 1:
							l.post = instructions::track::limit::post_t::right;
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

			instructions::track::section s;
			s.a_term.reserve(inst.args.size());

			std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(s.a_term),
			               [](const std::string& val) {
				               return gsl::narrow<std::size_t>(util::parse_loose_integer(val, 0));
			               });

			return s;
		}

		instruction create_instruction_track_sigf(const line_splitting::instruction_info& inst) {
			args_at_least(inst, 2, "Track.SigF");

			instructions::track::sig_f sf;

			sf.signal_index = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[0]));
			sf.section = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[1]));
			set_positions<2>(sf, inst);

			return sf;
		}
		instruction create_instruction_track_signal(const line_splitting::instruction_info& inst) {
			instructions::track::signal s;

			if (!inst.args.empty()) {
				auto const type_num = util::parse_loose_integer(inst.args[0]);

				switch (type_num) {
					case 2:
						s.type = instructions::track::signal::r_y;
						break;
					default:
					case -2:
						s.type = instructions::track::signal::r_g;
						break;
					case 3:
						s.type = instructions::track::signal::r_y_g;
						break;
					case 4:
						s.type = instructions::track::signal::r_yy_y_g;
						break;
					case -4:
						s.type = instructions::track::signal::r_y_yg_g;
						break;
					case 5:
						s.type = instructions::track::signal::r_yy_y_yg_g;
						break;
					case -5:
						s.type = instructions::track::signal::r_y_yg_g_gg;
						break;
					case 6:
						s.type = instructions::track::signal::r_yy_y_yg_g_gg;
						break;
				}
			}
			else {
				s.type = instructions::track::signal::r_g;
			}

			set_positions<2>(s, inst);

			return s;
		}

		instruction create_instruction_track_relay(const line_splitting::instruction_info& inst) {
			instructions::track::relay r;

			set_positions<0>(r, inst);

			return r;
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
