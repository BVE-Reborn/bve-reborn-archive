#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_track_back(const line_splitting::InstructionInfo& inst) {
		return create_single_uint_instruction<instructions::track::Back>(inst, "Track.Back");
	}

	Instruction create_instruction_track_fog(const line_splitting::InstructionInfo& inst) {
		instructions::track::Fog f;

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

	Instruction create_instruction_track_brightness(const line_splitting::InstructionInfo& inst) {
		instructions::track::Brightness b;

		if (!inst.args.empty()) {
			b.value = gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[0], 255));
		}

		return b;
	}

	Instruction create_instruction_track_marker(const line_splitting::InstructionInfo& inst) {
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

	Instruction create_instruction_track_text_marker(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Track.TextMarker");

		if (inst.args.size() >= 2) {
			instructions::track::TextMarker m;

			m.text = inst.args[0];
			m.distance = util::parse_loose_float(inst.args[1]);

			if (inst.args.size() >= 3) {
				static std::map<std::string, instructions::track::TextMarker::FontColor>
				    text_mapping{
				        //
				        {"black", instructions::track::TextMarker::FontColor::black},
				        {"gray", instructions::track::TextMarker::FontColor::gray},
				        {"grey", instructions::track::TextMarker::FontColor::gray},
				        {"white", instructions::track::TextMarker::FontColor::white},
				        {"red", instructions::track::TextMarker::FontColor::red},
				        {"orange", instructions::track::TextMarker::FontColor::orange},
				        {"green", instructions::track::TextMarker::FontColor::green},
				        {"blue", instructions::track::TextMarker::FontColor::blue},
				        {"magenta", instructions::track::TextMarker::FontColor::magenta}
				        //
				    };

				auto const text_mapping_iter = text_mapping.find(util::lower_copy(inst.args[2]));

				if (text_mapping_iter != text_mapping.end()) {
					m.font_color = text_mapping_iter->second;
				}
				else {
					m.font_color = instructions::track::TextMarker::FontColor::black;
				}
			}
			else {
				m.font_color = instructions::track::TextMarker::FontColor::black;
			}

			return m;
		}

		instructions::track::MarkerXML mxml;

		mxml.filename = inst.args[0];

		return mxml;
	}

	Instruction create_instruction_track_pointofinterest(
	    const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Track.PointOfInterest");

		instructions::track::PointOfInterest poi;

		poi.rail_index = gsl::narrow_cast<std::size_t>(util::parse_loose_integer(inst.args[0]));
		set_positions<1>(poi, inst);
		if (inst.args.size() >= 7) {
			poi.text = inst.args[6];
		}

		return poi;
	}

	Instruction create_instruction_track_pretrain(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Track.PreTrain");

		instructions::track::PreTrain pt;

		pt.time = util::parse_time(inst.args[0]);

		return pt;
	}

	Instruction create_instruction_track_announce(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Track.Announce");

		instructions::track::Announce a;

		a.filename = inst.args[0];
		if (inst.args.size() >= 2) {
			a.speed = util::parse_loose_float(inst.args[1], 0);
		}

		return a;
	}

	Instruction create_instruction_track_doppler(const line_splitting::InstructionInfo& inst) {
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

	Instruction create_instruction_track_buffer(const line_splitting::InstructionInfo& inst) {
		(void) inst;
		return instructions::track::Buffer{};
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
