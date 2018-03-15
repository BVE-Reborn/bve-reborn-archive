#include "executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Back& inst) {
		auto background_iterator = background_mapping.find(inst.background_texture_index);

		if (background_iterator == background_mapping.end()) {
			std::ostringstream err;

			err << "Background index " << inst.background_texture_index
			    << " has not been used. Please use Texture.Background to add "
			       "one. Ignoring.";

			errors::add_error(_errors, get_filename(inst.file_index), inst.line, err.str());

			return;
		}

		_route_data.backgrounds.emplace_back<background_info>(
		    {inst.absolute_position, background_iterator->second});
	}

	void pass3_executor::operator()(const instructions::track::Fog& inst) {
		fog_info fi;

		fi.position = inst.absolute_position;
		fi.starting_distance = inst.starting_distance * units_of_length[1];
		fi.ending_distance = inst.ending_distance * units_of_length[1];
		fi.color = inst.color;

		_route_data.fog.emplace_back(std::move(fi));
	}

	void pass3_executor::operator()(const instructions::track::Brightness& inst) {
		_route_data.brightness_levels.emplace_back<brightness_level_info>(
		    {inst.absolute_position, inst.value});
	}

	void pass3_executor::operator()(const instructions::track::Marker& inst) {
		xml::route_marker::image_marker im;

		im.on_time_filename = inst.filename;
		im.distance = inst.distance;

		marker_info mi;
		mi.marker = std::move(im);
		if (inst.distance < 0) {
			mi.start = inst.absolute_position;
			mi.end = inst.absolute_position - inst.distance; // subtracting a negative
		}
		else {
			mi.start = inst.absolute_position - inst.distance;
			mi.end = inst.absolute_position;
		}
		_route_data.markers.emplace_back(mi);
	}

	void pass3_executor::operator()(const instructions::track::TextMarker& inst) {
		xml::route_marker::text_marker tm;

		tm.on_time_text = inst.text;
		tm.distance = inst.distance;

		switch (inst.font_color) {
			default:
			case instructions::track::TextMarker::Black:
				tm.on_time_color = xml::route_marker::text_marker::color::Black;
				break;
			case instructions::track::TextMarker::Gray:
				tm.on_time_color = xml::route_marker::text_marker::color::Gray;
				break;
			case instructions::track::TextMarker::White:
				tm.on_time_color = xml::route_marker::text_marker::color::White;
				break;
			case instructions::track::TextMarker::Red:
				tm.on_time_color = xml::route_marker::text_marker::color::Red;
				break;
			case instructions::track::TextMarker::Orange:
				tm.on_time_color = xml::route_marker::text_marker::color::Orange;
				break;
			case instructions::track::TextMarker::Green:
				tm.on_time_color = xml::route_marker::text_marker::color::Green;
				break;
			case instructions::track::TextMarker::Blue:
				tm.on_time_color = xml::route_marker::text_marker::color::Blue;
				break;
			case instructions::track::TextMarker::Magenta:
				tm.on_time_color = xml::route_marker::text_marker::color::Magenta;
				break;
		}

		marker_info mi;
		mi.marker = std::move(tm);
		if (inst.distance < 0) {
			mi.start = inst.absolute_position;
			mi.end = inst.absolute_position - inst.distance; // subtracting a negative
		}
		else {
			mi.start = inst.absolute_position - inst.distance;
			mi.end = inst.absolute_position;
		}
		_route_data.markers.emplace_back(mi);
	}

	void pass3_executor::operator()(const instructions::track::MarkerXML& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		marker_info mi;

		auto filename = _get_relative_file(issuer_filename, inst.filename);

		try {
			auto str = util::load_from_file_utf8_bom(filename);
			mi.marker = xml::route_marker::parse(filename, std::move(str), _get_relative_file);
		}
		catch (std::invalid_argument& e) {
			errors::add_error(_errors, issuer_filename, inst.line, e.what());
		}

		float distance =
		    mapbox::util::apply_visitor([](const auto& m) -> float { return m.distance; },
		                                mi.marker);

		if (distance < 0) {
			mi.start = inst.absolute_position;
			mi.end = inst.absolute_position - distance; // subtracting a negative
		}
		else {
			mi.start = inst.absolute_position - distance;
			mi.end = inst.absolute_position;
		}
		_route_data.markers.emplace_back(mi);
	}

	void pass3_executor::operator()(const instructions::track::PointOfInterest& inst) {
		point_of_interest_info poii;

		if (!current_rail_state[inst.rail_index].active) {
			std::ostringstream err;

			err << "Track Index " << inst.rail_index
			    << " is not activated. Please use Track.RailStart or Track.Rail to activate";

			errors::add_error(_errors[get_filename(inst.file_index)], inst.line, err);
			return;
		}

		auto location = position_relative_to_rail(inst.rail_index, inst.absolute_position,
		                                          inst.x_offset, inst.y_offset);

		poii.position = location;
		// TODO(sirflankalot): Rotation
		poii.text = inst.text;
	}

	void pass3_executor::operator()(const instructions::track::PreTrain& inst) {
		if (!_route_data.pretrain_points.empty()
		    && _route_data.pretrain_points.back().value > inst.time) {
			std::ostringstream err;

			err << "Pretrain point at location " << _route_data.pretrain_points.back().position
			    << " has a later time " << _route_data.pretrain_points.back().value
			    << " than current point at " << inst.absolute_position << " and time " << inst.time
			    << ". Ignoring.";

			errors::add_error(_errors[get_filename(inst.file_index)], inst.line, err);
		}

		pretrain_info pti;

		pti.position = inst.absolute_position;
		pti.value = inst.time;

		_route_data.pretrain_points.emplace_back(std::move(pti));
	}

	void pass3_executor::operator()(const instructions::track::Announce& inst) {
		announcement_info ai;

		ai.filename = add_sound_filename(inst.filename);
		ai.position = inst.absolute_position;
		ai.speed = inst.speed * unit_of_speed;

		_route_data.announcments.emplace_back(std::move(ai));
	}

	void pass3_executor::operator()(const instructions::track::Doppler& inst) {
		sound_info si;

		si.position =
		    position_relative_to_rail(0, inst.absolute_position, inst.x_offset, inst.y_offset);
		si.filename = add_sound_filename(inst.filename);

		_route_data.sounds.emplace_back(std::move(si));
	}

	void pass3_executor::operator()(const instructions::track::Buffer& inst) {
		_route_data.bumpers.emplace_back(inst.absolute_position);
	}
} // namespace csv_rw_route
} // namespace parsers
