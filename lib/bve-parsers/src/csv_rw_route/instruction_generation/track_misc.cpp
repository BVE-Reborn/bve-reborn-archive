#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	instruction create_instruction_track_back(const line_splitting::instruction_info& inst) {
		return create_single_sizet_instruction<instructions::track::back>(inst, "Track.Back");
	}

	instruction create_instruction_track_fog(const line_splitting::instruction_info& inst) {
		instructions::track::fog f;

		switch (inst.args.size()) {
			default:
			case 5:
				f.color.b = gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[4], 128));
				// fall through
			case 4:
				f.color.g = gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[3], 128));
				// fall through
			case 3:
				f.color.r = gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[2], 128));
				// fall through
			case 2:
				f.ending_distance = util::parse_loose_float(inst.args[1], 0);
				// fall through
			case 1:
				f.starting_distance = util::parse_loose_float(inst.args[0], 0);
				// fall through
			case 0:
				break;
		}

		return f;
	}

	instruction create_instruction_track_brightness(const line_splitting::instruction_info& inst) {
		instructions::track::brightness b;

		if (!inst.args.empty()) {
			b.value = gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[0], 255));
		}

		return b;
	}

	instruction create_instruction_track_marker(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Track.Marker");

		if (inst.args.size() >= 2) {
			instructions::track::marker m;

			m.filename = inst.args[0];
			m.distance = util::parse_loose_float(inst.args[1]);

			return m;
		}

		instructions::track::marker_xml mxml;

		mxml.filename = inst.args[0];

		return mxml;
	}

	instruction create_instruction_track_text_marker(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Track.TextMarker");

		if (inst.args.size() >= 2) {
			instructions::track::text_marker m;

			m.text = inst.args[0];
			m.distance = util::parse_loose_float(inst.args[1]);

			if (inst.args.size() >= 3) {
				static std::map<std::string, decltype(decltype(m)::font_color)> text_mapping{
				    //
				    {"black", instructions::track::text_marker::black},
				    {"gray", instructions::track::text_marker::gray},
				    {"grey", instructions::track::text_marker::gray},
				    {"white", instructions::track::text_marker::white},
				    {"red", instructions::track::text_marker::red},
				    {"orange", instructions::track::text_marker::orange},
				    {"green", instructions::track::text_marker::green},
				    {"blue", instructions::track::text_marker::blue},
				    {"magenta", instructions::track::text_marker::magenta}
				    //
				};

				auto const text_mapping_iter = text_mapping.find(util::lower_copy(inst.args[2]));

				if (text_mapping_iter != text_mapping.end()) {
					m.font_color = text_mapping_iter->second;
				}
				else {
					m.font_color = instructions::track::text_marker::black;
				}
			}
			else {
				m.font_color = instructions::track::text_marker::black;
			}

			return m;
		}

		instructions::track::marker_xml mxml;

		mxml.filename = inst.args[0];

		return mxml;
	}

	instruction create_instruction_track_pointofinterest(
	    const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Track.PointOfInterest");

		instructions::track::point_of_interest poi;

		poi.rail_index = gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0]));
		set_positions<1>(poi, inst);
		if (inst.args.size() >= 7) {
			poi.text = inst.args[6];
		}

		return poi;
	}

	instruction create_instruction_track_pretrain(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Track.PreTrain");

		instructions::track::pre_train pt;

		pt.time = util::parse_time(inst.args[0]);

		return pt;
	}

	instruction create_instruction_track_announce(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Track.Announce");

		instructions::track::announce a;

		a.filename = inst.args[0];
		if (inst.args.size() >= 2) {
			a.speed = util::parse_loose_float(inst.args[1], 0);
		}

		return a;
	}

	instruction create_instruction_track_doppler(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Track.Doppler");

		instructions::track::doppler d;

		d.filename = inst.args[0];
		switch (inst.args.size()) {
			default:
			case 3:
				d.y_offset = util::parse_loose_float(inst.args[2]);
				// fall through
			case 2:
				d.x_offset = util::parse_loose_float(inst.args[1]);
				// fall through
			case 1:
				break;
		}

		return d;
	}

	instruction create_instruction_track_buffer(const line_splitting::instruction_info& inst) {
		(void) inst;
		return instructions::track::buffer{};
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
