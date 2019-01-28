#include "executor_pass3.hpp"

namespace bve::parsers::csv_rw_route {
	void Pass3Executor::finalize(float const max_position) {
		for (auto& state : current_rail_state_) {
			auto& rail_num = state.first;
			auto& state_val = state.second;

			addRailObjectsToPosition(state_val, max_position);
			addWallObjectsToPosition(state_val, max_position, 0);
			addWallObjectsToPosition(state_val, max_position, 1);
			addWallObjectsToPosition(state_val, max_position, 2);
			addWallObjectsToPosition(state_val, max_position, 3);
			addPollObjectsToPosition(rail_num, state_val, max_position);
			if (rail_num == 0) {
				addGroundObjectsToPosition(state_val, max_position);
			}
		}
	}
} // namespace bve::parsers::csv_rw_route
