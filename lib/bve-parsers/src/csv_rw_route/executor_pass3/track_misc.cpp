#include "executor_pass3.hpp"
#include <sstream>

namespace bve::parsers::csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Back& inst) {
		auto const background_iterator = background_mapping_.find(inst.background_texture_index);

		if (background_iterator == background_mapping_.end()) {
			std::ostringstream err;

			err << "Background index " << inst.background_texture_index
			    << " has not been used. Please use Texture.Background to add "
			       "one. Ignoring.";

			add_error(errors_, get_filename(inst.file_index), inst.line, err.str());

			return;
		}

		route_data_.backgrounds.emplace_back<background_info>(
		    {inst.absolute_position, background_iterator->second});
	}

	void pass3_executor::operator()(const instructions::track::Fog& inst) {
		fog_info fi;

		fi.position = inst.absolute_position;
		fi.starting_distance = inst.starting_distance * units_of_length_[1];
		fi.ending_distance = inst.ending_distance * units_of_length_[1];
		fi.color = inst.color;

		route_data_.fog.emplace_back(std::move(fi));
	}

	void pass3_executor::operator()(const instructions::track::Brightness& inst) const {
		route_data_.brightness_levels.emplace_back<brightness_level_info>(
		    {inst.absolute_position, inst.value});
	}

	void pass3_executor::operator()(const instructions::track::Marker& inst) const {
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
		route_data_.markers.emplace_back(mi);
	}

	void pass3_executor::operator()(const instructions::track::TextMarker& inst) const {
		xml::route_marker::text_marker tm;

		tm.on_time_text = inst.text;
		tm.distance = inst.distance;

		switch (inst.font_color) {
			default:
			case instructions::track::TextMarker::FontColor::black:
				tm.on_time_color = xml::route_marker::text_marker::color::black;
				break;
			case instructions::track::TextMarker::FontColor::gray:
				tm.on_time_color = xml::route_marker::text_marker::color::gray;
				break;
			case instructions::track::TextMarker::FontColor::white:
				tm.on_time_color = xml::route_marker::text_marker::color::white;
				break;
			case instructions::track::TextMarker::FontColor::red:
				tm.on_time_color = xml::route_marker::text_marker::color::red;
				break;
			case instructions::track::TextMarker::FontColor::orange:
				tm.on_time_color = xml::route_marker::text_marker::color::orange;
				break;
			case instructions::track::TextMarker::FontColor::green:
				tm.on_time_color = xml::route_marker::text_marker::color::green;
				break;
			case instructions::track::TextMarker::FontColor::blue:
				tm.on_time_color = xml::route_marker::text_marker::color::blue;
				break;
			case instructions::track::TextMarker::FontColor::magenta:
				tm.on_time_color = xml::route_marker::text_marker::color::magenta;
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
		route_data_.markers.emplace_back(mi);
	}

	void pass3_executor::operator()(const instructions::track::marker_xml& inst) const {
		auto const issuer_filename = get_filename(inst.file_index);

		marker_info mi;

		auto const filename = get_relative_file_(issuer_filename, inst.filename);

		try {
			auto str = util::load_from_file_utf8_bom(filename);
			mi.marker =
			    xml::route_marker::parse(filename, std::move(str), errors_, get_relative_file_);
		}
		catch (std::invalid_argument& e) {
			add_error(errors_, issuer_filename, inst.line, e.what());
		}

		auto const distance =
		    apply_visitor([](const auto& m) -> float { return m.distance; }, mi.marker);

		if (distance < 0) {
			mi.start = inst.absolute_position;
			mi.end = inst.absolute_position - distance; // subtracting a negative
		}
		else {
			mi.start = inst.absolute_position - distance;
			mi.end = inst.absolute_position;
		}
		route_data_.markers.emplace_back(mi);
	}

	void pass3_executor::operator()(const instructions::track::PointOfInterest& inst) {
		point_of_interest_info poii;

		if (!getRailState(inst.rail_index).active) {
			std::ostringstream err;

			err << "Track Index " << inst.rail_index
			    << " is not activated. Please use Track.RailStart or Track.Rail to activate";

			add_error(errors_, get_filename(inst.file_index), inst.line, err);
			return;
		}

		auto const location = position_relative_to_rail(inst.rail_index, inst.absolute_position,
		                                                inst.x_offset, inst.y_offset);

		poii.position = location;
		// TODO(sirflankalot): Rotation
		poii.text = inst.text;
	}

	void pass3_executor::operator()(const instructions::track::PreTrain& inst) const {
		if (!route_data_.pretrain_points.empty()
		    && route_data_.pretrain_points.back().value > inst.time) {
			std::ostringstream err;

			err << "Pretrain point at location " << route_data_.pretrain_points.back().position
			    << " has a later time " << route_data_.pretrain_points.back().value
			    << " than current point at " << inst.absolute_position << " and time " << inst.time
			    << ". Ignoring.";

			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}

		pretrain_info pti;

		pti.position = inst.absolute_position;
		pti.value = inst.time;

		route_data_.pretrain_points.emplace_back(std::move(pti));
	}

	void pass3_executor::operator()(const instructions::track::Announce& inst) const {
		announcement_info ai;

		ai.filename = add_sound_filename(inst.filename);
		ai.position = inst.absolute_position;
		ai.speed = inst.speed * unit_of_speed_;

		route_data_.announcments.emplace_back(std::move(ai));
	}

	void pass3_executor::operator()(const instructions::track::Doppler& inst) {
		sound_info si;

		si.position =
		    position_relative_to_rail(0, inst.absolute_position, inst.x_offset, inst.y_offset);
		si.filename = add_sound_filename(inst.filename);

		route_data_.sounds.emplace_back(std::move(si));
	}

	void pass3_executor::operator()(const instructions::track::Buffer& inst) const {
		route_data_.bumpers.emplace_back(inst.absolute_position);
	}
} // namespace bve::parsers::csv_rw_route
