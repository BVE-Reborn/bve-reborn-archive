#include "../executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::add_rail_objects_up_to_postion(rail_state& state, float position) {
		auto object_mapping_iter = object_rail_mapping.find(state.rail_structure_type);

		if (object_mapping_iter == object_rail_mapping.end() || state.active == false) {
			return;
		}

		for (auto pos = std::size_t(state.postion_last_updated); pos < std::size_t(position); pos += 25) {
			auto track_position = find_track_position_at(float(pos));
			auto object_location = openbve2::math::postion_from_offsets(track_position.position, track_position.tangent,
			                                                            state.x_offset, state.y_offset);

			rail_object_info i;
			i.filename = object_mapping_iter->second;
			i.position = object_location;
			i.rotation = glm::vec3(0);
			_route_data.objects.emplace_back(std::move(i));
		}
	}

	void pass3_executor::operator()(const instructions::track::RailStart& instr) {
		auto& state = current_rail_state[instr.rail_index];

		add_rail_objects_up_to_postion(state, instr.absolute_offset);

		if (state.active == true) {
			std::ostringstream err;

			err << "Rail number " << instr.rail_index << " is still active. Please use Track.Rail to update.";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}

		state.x_offset = instr.x_offset.get_value_or(0);
		state.y_offset = instr.y_offset.get_value_or(0);
		state.rail_structure_type = instr.rail_type.get_value_or(0);
		state.active = true;

		if (object_rail_mapping.count(state.rail_structure_type) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_type << " has not been declared. Ignoring.";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}
	}

	void pass3_executor::operator()(const instructions::track::Rail& instr) {
		auto& state = current_rail_state[instr.rail_index];

		add_rail_objects_up_to_postion(state, instr.absolute_offset);

		state.x_offset = instr.x_offset.get_value_or(0);
		state.y_offset = instr.y_offset.get_value_or(0);
		state.rail_structure_type = instr.rail_type.get_value_or(0);
		state.active = true;

		if (object_rail_mapping.count(state.rail_structure_type) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_type << " has not been declared. Ignoring.";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}
	}

	void pass3_executor::operator()(const instructions::track::RailType& instr) {
		auto& state = current_rail_state[instr.rail_index];

		add_rail_objects_up_to_postion(state, instr.absolute_offset);

		if (state.active == false) {
			std::ostringstream err;

			err << "Rail number " << instr.rail_index << " isn't active. Use Track.RailStart to start the track.";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}

		state.rail_structure_type = instr.rail_type;

		if (object_rail_mapping.count(state.rail_structure_type) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_type << " has not been declared. Ignoring.";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}
	}

	void pass3_executor::operator()(const instructions::track::RailEnd& instr) {
		auto& state = current_rail_state[instr.rail_index];

		add_rail_objects_up_to_postion(state, instr.absolute_offset);

		if (state.active == false) {
			std::ostringstream err;

			err << "Rail number " << instr.rail_index << " was already inactive. Did you mean Track.RailStart?";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}

		state.active = true;

		if (object_rail_mapping.count(state.rail_structure_type) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_type << " has not been declared. Ignoring.";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}
	}

	void pass3_executor::operator()(const instructions::track::Adhesion& instr) {
		_route_data.adheason.emplace_back<rail_adheason_info>({instr.absolute_offset, instr.value});
	}
} // namespace csv_rw_route
} // namespace parsers
