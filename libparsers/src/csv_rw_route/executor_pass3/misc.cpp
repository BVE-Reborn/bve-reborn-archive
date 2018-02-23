#include "../executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Back& inst) {
		auto background_iterator = background_mapping.find(inst.background_texture_index);

		if (background_iterator == background_mapping.end()) {
			std::ostringstream err;

			err << "Background index " << inst.background_texture_index
			    << " has not been used. Please use Texture.Background to add one. Ignoring.";

			errors::add_error(_errors, get_filename(inst.file_index), inst.line, err.str());

			return;
		}

		_route_data.backgrounds.emplace_back<background_info>({inst.absolute_position, background_iterator->second});
	}

    void pass3_executor::operator()(const instructions::track::Fog& inst) {
        fog_info fi;

        fi.position = inst.absolute_position;
        fi.starting_distance = inst.starting_distance * units_of_length[1];
        fi.ending_distance = inst.ending_distance * units_of_length[1];
        fi.color = inst.color;

        _route_data.fog.emplace_back(std::move(fi));
    }

    void pass3_executor::operator()(const instructions::track::Brightness& /*inst*/) {
        // Continue: Add brightness data
    }
	void pass3_executor::operator()(const instructions::track::Marker& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::MarkerXML& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::TextMarker& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::PointOfInterest& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::PreTrain& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::Announce& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::Doppler& /*inst*/) {}
	void pass3_executor::operator()(const instructions::track::Buffer& /*inst*/) {}
} // namespace csv_rw_route
} // namespace parsers
