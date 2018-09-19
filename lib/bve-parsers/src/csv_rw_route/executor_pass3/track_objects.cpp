#include "executor_pass3.hpp"
#include <sstream>

namespace bve::parsers::csv_rw_route {
	void pass3_executor::operator()(const instructions::track::FreeObj& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(inst.rail_index);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " isn't active. Use Track.RailStart to start the track.";
			add_error(errors_, issuer_filename, inst.line, err);
		}

		auto const structure_index_iter =
		    object_freeobj_mapping_.find(inst.free_obj_structure_index);

		if (structure_index_iter == object_freeobj_mapping_.end()) {
			std::ostringstream err;

			err << "FreeObj Structure #" << inst.free_obj_structure_index
			    << " isn't mapped. Ignoring call. Use Structure.FreeObj to "
			       "declare it.";
			add_error(errors_, issuer_filename, inst.line, err);
			return;
		}

		auto const structure_filename_iter = structure_index_iter->second;

		rail_object_info roi;
		roi.filename = structure_filename_iter;
		roi.position = position_relative_to_rail(inst.rail_index, inst.absolute_position,
		                                         inst.x_offset, inst.y_offset);
		/*roi.rotation = */ // TODO(sirflankalot): convert Yaw/Pitch/Roll to
		                    // rotation vector

		route_data_.objects.emplace_back(std::move(roi));
	}

	void pass3_executor::add_wall_objects_up_to_position(rail_state& state,
	                                                     float const position,
	                                                     uint8_t const type) {
		std::unordered_map<std::size_t, filename_set_iterator>* object_mapping;
		std::size_t index;
		float* last_updated;
		bool* enabled;

		switch (type) {
			case 0:
				object_mapping = &object_wall_l_mapping_;
				index = state.wall_l_index;
				last_updated = &state.position_wall_l_updated;
				enabled = &state.wall_l_active;
				break;
			case 1:
				object_mapping = &object_wall_r_mapping_;
				index = state.wall_r_index;
				last_updated = &state.position_wall_r_updated;
				enabled = &state.wall_r_active;
				break;
			case 2:
				object_mapping = &object_dike_l_mapping_;
				index = state.dike_l_index;
				last_updated = &state.position_dike_l_updated;
				enabled = &state.dike_l_active;
				break;
			case 3:
				object_mapping = &object_dike_r_mapping_;
				index = state.dike_r_index;
				last_updated = &state.position_dike_r_updated;
				enabled = &state.dike_r_active;
				break;
			default:
				return;
		}

		auto const object_mapping_iter = object_mapping->find(index);

		if (object_mapping_iter == object_mapping->end() || !state.active || !*enabled) {
			return;
		}

		for (auto pos = static_cast<std::size_t>(*last_updated);
		     pos < static_cast<std::size_t>(position); pos += 25) {
			auto const track_position = track_position_at(float(pos));
			auto const object_location =
			    bve::core::math::position_from_offsets(track_position.position,
			                                           track_position.tangent, state.x_offset,
			                                           state.y_offset);

			rail_object_info i;
			i.filename = object_mapping_iter->second;
			i.position = object_location;
			i.rotation = glm::vec3(0);
			route_data_.objects.emplace_back(std::move(i));
		}

		*last_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::Wall& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(inst.rail_index);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " isn't active. Use Track.RailStart to start the track.";
			add_error(errors_, issuer_filename, inst.line, err);
		}

		auto const left = inst.direction == instructions::track::Wall::Direction::left
		                  || inst.direction == instructions::track::Wall::Direction::both;
		auto const right = inst.direction == instructions::track::Wall::Direction::right
		                   || inst.direction == instructions::track::Wall::Direction::both;

		if (left) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 0);

			if (object_wall_l_mapping_.count(inst.wall_structure_index) == 0) {
				std::ostringstream err;

				err << "WallL Structure #" << inst.wall_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.WallL to "
				       "declare it.";
				add_error(errors_, issuer_filename, inst.line, err);
				goto right_wall;
			}

			state.wall_l_active = true;
			state.wall_l_index = inst.wall_structure_index;
		}
	right_wall:
		if (right) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 1);

			if (object_wall_r_mapping_.count(inst.wall_structure_index) == 0) {
				std::ostringstream err;

				err << "WallR Structure #" << inst.wall_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.WallR to "
				       "declare it.";
				add_error(errors_, issuer_filename, inst.line, err);
				add_error(errors_, issuer_filename, inst.line, err);
				return;
			}

			state.wall_r_active = true;
			state.wall_r_index = inst.wall_structure_index;
		}
	}

	void pass3_executor::operator()(const instructions::track::WallEnd& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(inst.rail_index);

		// Don't check if the rail is active as people can call .RailEnd before calling .WallEnd
		// if (!state.active) {}

		add_wall_objects_up_to_position(state, inst.absolute_position, 0);
		add_wall_objects_up_to_position(state, inst.absolute_position, 1);

		state.wall_l_active = false;
		state.wall_r_active = false;
	}

	void pass3_executor::operator()(const instructions::track::Dike& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(inst.rail_index);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " isn't active. Use Track.RailStart to start the track.";
			add_error(errors_, issuer_filename, inst.line, err);
		}

		auto const left = inst.direction == instructions::track::Dike::Direction::left
		                  || inst.direction == instructions::track::Dike::Direction::both;
		auto const right = inst.direction == instructions::track::Dike::Direction::right
		                   || inst.direction == instructions::track::Dike::Direction::both;

		if (left) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 2);

			if (object_dike_l_mapping_.count(inst.dike_structure_index) == 0) {
				std::ostringstream err;

				err << "DikeL Structure #" << inst.dike_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.DikeL to "
				       "declare it.";
				add_error(errors_, issuer_filename, inst.line, err);
				add_error(errors_, issuer_filename, inst.line, err);
				goto right_dike;
			}

			state.dike_l_active = true;
			state.dike_l_index = inst.dike_structure_index;
		}
	right_dike:
		if (right) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 3);

			if (object_dike_r_mapping_.count(inst.dike_structure_index) == 0) {
				std::ostringstream err;

				err << "DikeR Structure #" << inst.dike_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.DikeR to "
				       "declare it.";
				add_error(errors_, issuer_filename, inst.line, err);
				add_error(errors_, issuer_filename, inst.line, err);
				return;
			}

			state.dike_r_active = true;
			state.dike_r_index = inst.dike_structure_index;
		}
	}

	void pass3_executor::operator()(const instructions::track::DikeEnd& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(inst.rail_index);

		// Don't check if the rail is active as people can call .RailEnd before calling .DikeEnd
		// if (!state.active) {}

		add_wall_objects_up_to_position(state, inst.absolute_position, 2);
		add_wall_objects_up_to_position(state, inst.absolute_position, 3);

		state.dike_l_active = false;
		state.dike_r_active = false;
	}

	void pass3_executor::add_poll_objects_up_to_position(std::size_t const rail_number,
	                                                     rail_state& state,
	                                                     float const position) {
		auto const object_mapping_iter =
		    object_pole_mapping_.find({state.pole_additional_rails, state.pole_structure_index});

		if (object_mapping_iter == object_pole_mapping_.end() || !state.active
		    || !state.pole_active) {
			return;
		}

		for (auto pos = static_cast<std::size_t>(state.position_pole_updated);
		     pos < static_cast<std::size_t>(position); pos += 25) {
			auto const add_object = pos % state.pole_interval == 0;

			if (!add_object) {
				continue;
			}
			rail_object_info i;

			// see the Track.Pole doc for more info on this abserd routine
			glm::vec3 object_location;

			if (state.pole_additional_rails == 0) {
				if (state.pole_location > 0) {
					i.flip_x = true;
				}

				object_location = position_relative_to_rail(rail_number, position, 0, 0);
			}
			else {
				object_location =
				    position_relative_to_rail(rail_number, position,
				                              static_cast<float>(state.pole_location) * 3.8f, 0);
			}

			i.filename = object_mapping_iter->second;
			i.position = object_location;
			i.rotation = glm::vec3(0);
			route_data_.objects.emplace_back(std::move(i));
		}

		state.position_pole_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::Pole& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(inst.rail_index);

		add_poll_objects_up_to_position(inst.rail_index, state, inst.absolute_position);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index
			    << " isn't active. Use Track.RailStart to start the track.";
			add_error(errors_, issuer_filename, inst.line, err);
		}

		auto const pole_structure_iter =
		    object_pole_mapping_.find({inst.additional_rails, inst.pole_structure_index});

		if (pole_structure_iter == object_pole_mapping_.end()) {
			std::ostringstream err;

			err << "Pole Structure (" << inst.additional_rails << ", " << inst.pole_structure_index
			    << ") isn't mapped. Ignoring call. Use Structure.Pole to "
			       "declare it.";
			add_error(errors_, issuer_filename, inst.line, err);
			return;
		}

		state.pole_additional_rails = inst.additional_rails;
		state.pole_location = inst.location;
		state.pole_interval = inst.interval;
		state.pole_structure_index = inst.pole_structure_index;
		state.pole_active = true;
	}

	void pass3_executor::operator()(const instructions::track::PoleEnd& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(inst.rail_index);

		// Don't check if the rail is active as people can call .RailEnd before calling .PoleEnd
		// if (!state.active) {}

		add_poll_objects_up_to_position(inst.rail_index, state, inst.absolute_position);

		state.pole_active = false;
	}

	void pass3_executor::operator()(const instructions::track::Crack& inst) const {
		(void) inst;
		// TODO(sirflankalot): crack
	}

	void pass3_executor::add_ground_objects_up_to_position(rail_state& state,
	                                                       float const position) const {
		for (auto pos = static_cast<std::size_t>(state.position_ground_updated);
		     pos < static_cast<std::size_t>(position); pos += 25) {
			auto const track_location = track_position_at(float(pos));
			auto const ground_height = ground_height_at(float(pos));

			auto filename_iter_optional =
			    get_cycle_filename_index(cycle_ground_mapping_, object_ground_mapping_,
			                             state.ground_index, pos);

			if (!filename_iter_optional) {
				return;
			}

			rail_object_info roi;
			roi.filename = *filename_iter_optional;
			roi.position =
			    bve::core::math::position_from_offsets(track_location.position,
			                                           track_location.tangent, 0, -ground_height);
			roi.rotation = glm::vec3(0);
			route_data_.objects.emplace_back(roi);
		}

		state.position_ground_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::Ground& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto& state = getRailState(0);

		add_ground_objects_up_to_position(state, inst.absolute_position);

		if (object_ground_mapping_.count(inst.ground_structure_index) == 0) {
			std::ostringstream err;

			err << "Ground Structure #" << inst.ground_structure_index
			    << " isn't mapped. Ignoring call. Use Structure.Ground to "
			       "declare it.";
			add_error(errors_, issuer_filename, inst.line, err);
			return;
		}

		state.ground_index = inst.ground_structure_index;
	}
} // namespace bve::parsers::csv_rw_route
