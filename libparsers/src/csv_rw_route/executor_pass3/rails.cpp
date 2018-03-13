#include "../executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::add_rail_objects_up_to_position(rail_state& state, float position) {
		if (!state.active) {
			return;
		}

		for (auto pos = std::size_t(state.position_last_updated); pos < std::size_t(position);
		     pos += 25) {
			auto track_position = track_position_at(float(pos));
			auto object_location =
			    openbve2::math::position_from_offsets(track_position.position,
			                                          track_position.tangent, state.x_offset,
			                                          state.y_offset);

			auto filename_iter_optional =
			    get_cycle_filename_index(cycle_rail_mapping, object_rail_mapping,
			                             state.rail_structure_index, std::size_t(position));

			if (!filename_iter_optional) {
				return;
			}

			rail_object_info i;
			i.filename = *filename_iter_optional.get_ptr();
			i.position = object_location;
			i.rotation = glm::vec3(0);
			_route_data.objects.emplace_back(std::move(i));
		}

		state.position_last_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::RailStart& inst) {
		auto& state = current_rail_state[inst.rail_index];

		add_rail_objects_up_to_position(state, inst.absolute_position);

		if (state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " is still active. Please use Track.Rail to update.";
			add_error(_errors, get_filename(inst.file_index), inst.line, err);
		}

		state.x_offset = inst.x_offset.get_value_or(0);
		state.y_offset = inst.y_offset.get_value_or(0);
		state.rail_structure_index = inst.rail_type.get_value_or(0);
		state.active = true;

		if (object_rail_mapping.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(_errors, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::Rail& inst) {
		auto& state = current_rail_state[inst.rail_index];

		add_rail_objects_up_to_position(state, inst.absolute_position);

		state.x_offset = inst.x_offset.get_value_or(0);
		state.y_offset = inst.y_offset.get_value_or(0);
		state.rail_structure_index = inst.rail_type.get_value_or(0);
		state.active = true;

		if (object_rail_mapping.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(_errors, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::RailType& inst) {
		auto& state = current_rail_state[inst.rail_index];

		add_rail_objects_up_to_position(state, inst.absolute_position);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, get_filename(inst.file_index), inst.line, err);
		}

		state.rail_structure_index = inst.rail_type;

		if (object_rail_mapping.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(_errors, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::RailEnd& inst) {
		auto& state = current_rail_state[inst.rail_index];

		add_rail_objects_up_to_position(state, inst.absolute_position);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " was already inactive. Did you mean Track.RailStart?";
			add_error(_errors, get_filename(inst.file_index), inst.line, err);
		}

		state.active = true;

		if (object_rail_mapping.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(_errors, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::Adhesion& inst) {
		_route_data.adheason.emplace_back<rail_adheason_info>({inst.absolute_position, inst.value});
	}
} // namespace csv_rw_route
} // namespace parsers
