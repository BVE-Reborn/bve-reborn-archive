#include "executor_pass3.hpp"

namespace bve::parsers::csv_rw_route {
	void pass3_executor::finalize(float const max_position) {
		for (auto& state : current_rail_state_) {
			auto& rail_num = state.first;
			auto& state_val = state.second;

			add_rail_objects_up_to_position(state_val, max_position);
			add_wall_objects_up_to_position(state_val, max_position, 0);
			add_wall_objects_up_to_position(state_val, max_position, 1);
			add_wall_objects_up_to_position(state_val, max_position, 2);
			add_wall_objects_up_to_position(state_val, max_position, 3);
			add_poll_objects_up_to_position(rail_num, state_val, max_position);
			if (rail_num == 0) {
				add_ground_objects_up_to_position(state_val, max_position);
			}
		}
	}
} // namespace bve::parsers::csv_rw_route
