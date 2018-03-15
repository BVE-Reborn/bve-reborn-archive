#include "instruction_generator.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_track_back(const line_splitting::instruction_info& inst) {
			return create_single_sizet_instruction<instructions::track::Back>(inst, "Track.Back");
		}

		instruction create_instruction_track_fog(const line_splitting::instruction_info& inst) {
			instructions::track::Fog f;

			switch (inst.args.size()) {
				default:
				case 5:
					f.color.b = std::uint8_t(util::parse_loose_integer(inst.args[4], 128));
					// fall through
				case 4:
					f.color.g = std::uint8_t(util::parse_loose_integer(inst.args[3], 128));
					// fall through
				case 3:
					f.color.r = std::uint8_t(util::parse_loose_integer(inst.args[2], 128));
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

		instruction create_instruction_track_brightness(
		    const line_splitting::instruction_info& inst) {
			instructions::track::Brightness b;

			if (!inst.args.empty()) {
				b.value = std::uint8_t(util::parse_loose_integer(inst.args[0], 255));
			}

			return b;
		}

		instruction create_instruction_track_marker(const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Track.Marker");

			if (inst.args.size() >= 2) {
				instructions::track::Marker m;

				m.filename = inst.args[0];
				m.distance = util::parse_loose_float(inst.args[1]);

				return m;
			}

			instructions::track::MarkerXML mxml;

			mxml.filename = inst.args[0];

			return mxml;
		}

		instruction create_instruction_track_text_marker(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Track.TextMarker");

			if (inst.args.size() >= 2) {
				instructions::track::TextMarker m;

				m.text = inst.args[0];
				m.distance = util::parse_loose_float(inst.args[1]);

				if (inst.args.size() >= 3) {
					static std::map<std::string, decltype(decltype(m)::font_color)> text_mapping{
					    //
					    {"black", instructions::track::TextMarker::Black},
					    {"gray", instructions::track::TextMarker::Gray},
					    {"grey", instructions::track::TextMarker::Gray},
					    {"white", instructions::track::TextMarker::White},
					    {"red", instructions::track::TextMarker::Red},
					    {"orange", instructions::track::TextMarker::Orange},
					    {"green", instructions::track::TextMarker::Green},
					    {"blue", instructions::track::TextMarker::Blue},
					    {"magenta", instructions::track::TextMarker::Magenta}
					    //
					};

					auto text_mapping_iter = text_mapping.find(util::lower_copy(inst.args[2]));

					if (text_mapping_iter != text_mapping.end()) {
						m.font_color = text_mapping_iter->second;
					}
					else {
						m.font_color = instructions::track::TextMarker::Black;
					}
				}
				else {
					m.font_color = instructions::track::TextMarker::Black;
				}

				return m;
			}

			instructions::track::MarkerXML mxml;

			mxml.filename = inst.args[0];

			return mxml;
		}

		instruction create_instruction_track_pointofinterest(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Track.PointOfInterest");

			instructions::track::PointOfInterest poi;

			poi.rail_index = std::size_t(util::parse_loose_integer(inst.args[0]));
			set_positions<1>(poi, inst);
			if (inst.args.size() >= 7) {
				poi.text = inst.args[6];
			}

			return poi;
		}

		instruction create_instruction_track_pretrain(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Track.PreTrain");

			instructions::track::PreTrain pt;

			pt.time = util::parse_time(inst.args[0]);

			return pt;
		}

		instruction create_instruction_track_announce(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Track.Announce");

			instructions::track::Announce a;

			a.filename = inst.args[0];
			if (inst.args.size() >= 2) {
				a.speed = util::parse_loose_float(inst.args[1], 0);
			}

			return a;
		}

		instruction create_instruction_track_doppler(const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Track.Doppler");

			instructions::track::Doppler d;

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
			return instructions::track::Buffer{};
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
