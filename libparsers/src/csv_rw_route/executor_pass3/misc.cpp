#include "../executor_pass3.hpp"
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
		auto issuer_filename = get_filename(inst.file_index);

		xml::route_marker::image_marker im;

		im.on_time_filename = inst.filename;
		im.distance = inst.distance;

		marker_info mi;
		mi.marker = im;
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
			mi.marker = xml::route_marker::parse(filename, std::move(str), _errors[filename],
			                                     _get_relative_file);
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

	void pass3_executor::operator()(const instructions::track::TextMarker& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::PointOfInterest& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::PreTrain& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::Announce& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::Doppler& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::Buffer& /*inst*/) {}
} // namespace csv_rw_route
} // namespace parsers
