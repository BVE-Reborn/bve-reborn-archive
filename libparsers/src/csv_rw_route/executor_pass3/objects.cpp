#include "../executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::FreeObj& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = current_rail_state[inst.rail_index];

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, issuer_filename, inst.line, err);
		}

		auto structure_index_iter = object_freeobj_mapping.find(inst.free_obj_structure_index);

		if (structure_index_iter == object_freeobj_mapping.end()) {
			std::ostringstream err;

			err << "FreeObj Structure #" << inst.free_obj_structure_index
			    << " isn't mapped. Ignoring call. Use Structure.FreeObj to "
			       "declare it.";
			add_error(_errors, issuer_filename, inst.line, err);
			return;
		}

		auto structure_filename_iter = structure_index_iter->second;

		rail_object_info roi;
		roi.filename = structure_filename_iter;
		roi.position = position_relative_to_rail(inst.rail_index, inst.absolute_position, inst.x_offset, inst.y_offset);
		/*roi.rotation = */ // TODO(sirflankalot): convert Yaw/Pitch/Roll to
		                    // rotation vector

		_route_data.objects.emplace_back(std::move(roi));
	}

	void pass3_executor::add_wall_objects_up_to_position(rail_state& state, float position, uint8_t type) {
		std::unordered_map<std::size_t, filename_set_iterator>* object_mapping;
		std::size_t index;
		float* last_updated;
		bool* enabled;

		switch (type) {
			case 0:
				object_mapping = &object_wallL_mapping;
				index = state.wallL_index;
				last_updated = &state.position_wallL_updated;
				enabled = &state.wallL_active;
				break;
			case 1:
				object_mapping = &object_wallR_mapping;
				index = state.wallR_index;
				last_updated = &state.position_wallR_updated;
				enabled = &state.wallR_active;
				break;
			case 2:
				object_mapping = &object_dikeL_mapping;
				index = state.dikeL_index;
				last_updated = &state.position_dikeL_updated;
				enabled = &state.dikeL_active;
				break;
			case 3:
				object_mapping = &object_dikeR_mapping;
				index = state.dikeR_index;
				last_updated = &state.position_dikeR_updated;
				enabled = &state.dikeR_active;
				break;
			default:
				return;
		}

		auto object_mapping_iter = object_mapping->find(index);

		if (object_mapping_iter == object_mapping->end() || !state.active || !*enabled) {
			return;
		}

		for (auto pos = std::size_t(*last_updated); pos < std::size_t(position); pos += 25) {
			auto track_position = track_position_at(float(pos));
			auto object_location =
			    openbve2::math::position_from_offsets(track_position.position, track_position.tangent, state.x_offset,
			                                          state.y_offset);

			rail_object_info i;
			i.filename = object_mapping_iter->second;
			i.position = object_location;
			i.rotation = glm::vec3(0);
			_route_data.objects.emplace_back(std::move(i));
		}

		*last_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::Wall& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = current_rail_state[inst.rail_index];

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, issuer_filename, inst.line, err);
		}

		bool left =
		    inst.direction == instructions::track::Wall::Left || inst.direction == instructions::track::Wall::Both;
		bool right =
		    inst.direction == instructions::track::Wall::Right || inst.direction == instructions::track::Wall::Both;

		if (left) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 0);

			if (object_wallL_mapping.count(inst.wall_structure_index) == 0) {
				std::ostringstream err;

				err << "WallL Structure #" << inst.wall_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.WallL to "
				       "declare it.";
				add_error(_errors, issuer_filename, inst.line, err);
				_errors[issuer_filename].emplace_back<errors::error_t>({inst.line, err.str()});
				goto right_wall;
			}

			state.wallL_active = true;
			state.wallL_index = inst.wall_structure_index;
		}
	right_wall:
		if (right) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 1);

			if (object_wallR_mapping.count(inst.wall_structure_index) == 0) {
				std::ostringstream err;

				err << "WallR Structure #" << inst.wall_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.WallR to "
				       "declare it.";
				add_error(_errors, issuer_filename, inst.line, err);
				_errors[issuer_filename].emplace_back<errors::error_t>({inst.line, err.str()});
				return;
			}

			state.wallR_active = true;
			state.wallR_index = inst.wall_structure_index;
		}
	}

	void pass3_executor::operator()(const instructions::track::WallEnd& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = current_rail_state[inst.rail_index];

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, issuer_filename, inst.line, err);
		}

		add_wall_objects_up_to_position(state, inst.absolute_position, 0);
		add_wall_objects_up_to_position(state, inst.absolute_position, 1);

		state.wallL_active = false;
		state.wallR_active = false;
	}

	void pass3_executor::operator()(const instructions::track::Dike& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = current_rail_state[inst.rail_index];

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, issuer_filename, inst.line, err);
		}

		bool left =
		    inst.direction == instructions::track::Dike::Left || inst.direction == instructions::track::Dike::Both;
		bool right =
		    inst.direction == instructions::track::Dike::Right || inst.direction == instructions::track::Dike::Both;

		if (left) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 2);

			if (object_dikeL_mapping.count(inst.dike_structure_index) == 0) {
				std::ostringstream err;

				err << "DikeL Structure #" << inst.dike_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.DikeL to "
				       "declare it.";
				add_error(_errors, issuer_filename, inst.line, err);
				_errors[issuer_filename].emplace_back<errors::error_t>({inst.line, err.str()});
				goto right_dike;
			}

			state.dikeL_active = true;
			state.dikeL_index = inst.dike_structure_index;
		}
	right_dike:
		if (right) {
			add_wall_objects_up_to_position(state, inst.absolute_position, 3);

			if (object_dikeR_mapping.count(inst.dike_structure_index) == 0) {
				std::ostringstream err;

				err << "DikeR Structure #" << inst.dike_structure_index
				    << " isn't mapped. Ignoring call. Use Structure.DikeR to "
				       "declare it.";
				add_error(_errors, issuer_filename, inst.line, err);
				_errors[issuer_filename].emplace_back<errors::error_t>({inst.line, err.str()});
				return;
			}

			state.dikeR_active = true;
			state.dikeR_index = inst.dike_structure_index;
		}
	}

	void pass3_executor::operator()(const instructions::track::DikeEnd& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = current_rail_state[inst.rail_index];

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, issuer_filename, inst.line, err);
		}

		add_wall_objects_up_to_position(state, inst.absolute_position, 2);
		add_wall_objects_up_to_position(state, inst.absolute_position, 3);

		state.dikeL_active = false;
		state.dikeR_active = false;
	}

	void pass3_executor::add_poll_objects_up_to_position(std::size_t rail_number, rail_state& state, float position) {
		auto object_mapping_iter = object_pole_mapping.find({state.pole_additional_rails, state.pole_structure_index});

		if (object_mapping_iter == object_pole_mapping.end() || !state.active || !state.pole_active) {
			return;
		}

		for (auto pos = std::size_t(state.position_pole_updated); pos < std::size_t(position); pos += 25) {
			bool add_object = pos % state.pole_interval == 0;

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
				    position_relative_to_rail(rail_number, position, float(state.pole_location) * 3.8f, 0);
			}

			i.filename = object_mapping_iter->second;
			i.position = object_location;
			i.rotation = glm::vec3(0);
			_route_data.objects.emplace_back(std::move(i));
		}

		state.position_pole_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::Pole& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = current_rail_state[inst.rail_index];

		add_poll_objects_up_to_position(inst.rail_index, state, inst.absolute_position);

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, issuer_filename, inst.line, err);
		}

		auto pole_structure_iter = object_pole_mapping.find({inst.additional_rails, inst.pole_structure_index});

		if (pole_structure_iter == object_pole_mapping.end()) {
			std::ostringstream err;

			err << "Pole Structure (" << inst.additional_rails << ", " << inst.pole_structure_index
			    << ") isn't mapped. Ignoring call. Use Structure.Pole to "
			       "declare it.";
			add_error(_errors, issuer_filename, inst.line, err);
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

		auto& state = current_rail_state[inst.rail_index];

		if (!state.active) {
			std::ostringstream err;

			err << "Rail number " << inst.rail_index << " isn't active. Use Track.RailStart to start the track.";
			add_error(_errors, issuer_filename, inst.line, err);
		}

		add_poll_objects_up_to_position(inst.rail_index, state, inst.absolute_position);

		state.pole_active = false;
	}

	void pass3_executor::operator()(const instructions::track::Crack& inst) {
		(void) inst;
		// TODO(sirflankalot): crack
	}

	void pass3_executor::add_ground_objects_up_to_position(rail_state& state, float position) {
		for (auto pos = std::size_t(state.position_ground_updated); pos < std::size_t(position); pos += 25) {
			auto track_location = track_position_at(float(pos));
			auto ground_height = ground_height_at(float(pos));

			auto filename_iter_optional =
			    get_cycle_filename_index(cycle_ground_mapping, object_ground_mapping, state.ground_index, pos);

			if (!filename_iter_optional) {
				return;
			}

			rail_object_info roi;
			roi.filename = *filename_iter_optional.get_ptr();
			roi.position = openbve2::math::position_from_offsets(track_location.position, track_location.tangent, 0,
			                                                     -ground_height);
			roi.rotation = glm::vec3(0);
			_route_data.objects.emplace_back(roi);
		}

		state.position_ground_updated = position;
	}

	void pass3_executor::operator()(const instructions::track::Ground& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		auto& state = current_rail_state[0];

		add_ground_objects_up_to_position(state, inst.absolute_position);

		if (object_ground_mapping.count(inst.ground_structure_index) == 0) {
			std::ostringstream err;

			err << "Ground Structure #" << inst.ground_structure_index
			    << " isn't mapped. Ignoring call. Use Structure.Ground to declare it.";
			add_error(_errors, issuer_filename, inst.line, err);
			return;
		}

		state.ground_index = inst.ground_structure_index;
	}
} // namespace csv_rw_route
} // namespace parsers
