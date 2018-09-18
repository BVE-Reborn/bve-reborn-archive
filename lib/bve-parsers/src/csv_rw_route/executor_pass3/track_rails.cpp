#include "executor_pass3.hpp"
#include <sstream>

namespace bve::parsers::csv_rw_route {
	void pass3_executor::add_rail_objects_up_to_position(rail_state& state,
	                                                     float const position) const {
		if (!state.active) {
			return;
		}

		for (auto pos = static_cast<std::size_t>(state.position_last_updated);
		     pos < static_cast<std::size_t>(position); pos += 25) {
			auto const track_position = track_position_at(float(pos));
			auto const object_location =
			    bve::core::math::position_from_offsets(track_position.position,
			                                           track_position.tangent, state.x_offset,
			                                           state.y_offset);

			auto filename_iter_optional =
			    get_cycle_filename_index(cycle_rail_mapping_, object_rail_mapping_,
			                             state.rail_structure_index,
			                             static_cast<std::size_t>(position));

			if (!filename_iter_optional) {
				return;
			}

			rail_object_info i;
			i.filename = *filename_iter_optional;
			i.position = object_location;
			i.rotation = glm::vec3(0);
			route_data_.objects.emplace_back(std::move(i));
		}

		state.position_last_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::rail_start& inst) {
		auto& state = get_rail_state(inst.rail_index);

		add_rail_objects_up_to_position(state, inst.absolute_position);

		if (state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " is still active. Please use Track.Rail to update.";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}

		state.x_offset = inst.x_offset.value_or(state.x_offset);
		state.y_offset = inst.y_offset.value_or(state.y_offset);
		state.rail_structure_index = inst.rail_type.value_or(state.rail_structure_index);
		state.active = true;

		if (object_rail_mapping_.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::rail& inst) {
		auto& state = get_rail_state(inst.rail_index);

		add_rail_objects_up_to_position(state, inst.absolute_position);

		state.x_offset = inst.x_offset.value_or(state.x_offset);
		state.y_offset = inst.y_offset.value_or(state.y_offset);
		state.rail_structure_index = inst.rail_type.value_or(state.rail_structure_index);
		state.active = true;

		if (object_rail_mapping_.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::rail_type& inst) {
		auto& state = get_rail_state(inst.rail_index);

		add_rail_objects_up_to_position(state, inst.absolute_position);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " isn't active. Use Track.RailStart to start the track.";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}

		state.rail_structure_index = inst.rail_type_number;
		state.active = true;

		if (object_rail_mapping_.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::rail_end& inst) {
		auto& state = get_rail_state(inst.rail_index);

		add_rail_objects_up_to_position(state, inst.absolute_position);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " was already inactive. Did you mean Track.RailStart?";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}

		state.active = false;

		if (object_rail_mapping_.count(state.rail_structure_index) == 0) {
			std::ostringstream err;

			err << "Rail Structure " << state.rail_structure_index
			    << " has not been declared. Ignoring.";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::track::adhesion& inst) const {
		route_data_.adheason.emplace_back<rail_adheason_info>({inst.absolute_position, inst.value});
	}
} // namespace bve::parsers::csv_rw_route
