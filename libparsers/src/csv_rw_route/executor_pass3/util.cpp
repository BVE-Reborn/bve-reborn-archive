#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	float pass3_executor::ground_height_at(float position) {
		if (position <= _route_data.ground_height.front().position) {
			return _route_data.ground_height.front().value;
		}
		if (position >= _route_data.ground_height.back().position) {
			return _route_data.ground_height.back().value;
		}

		decltype(_route_data.ground_height)::const_iterator start_iter;
		decltype(_route_data.ground_height)::const_iterator end_iter;

		// WHY DO LOWER BOUND AND UPPER BOUND HAVE DIFFERENT F***ING INTERFACES!
		// I CAN'T USE std::equal_range UNLESS I PROVIDE IT A FUNCTION WITH *OVERLOADS*
		// GAAH!
		start_iter = std::lower_bound(_route_data.ground_height.begin(), _route_data.ground_height.end(), position,
		                              [](const ground_height_info& a, float b) { return a.position < b; });
		end_iter = std::upper_bound(_route_data.ground_height.begin(), _route_data.ground_height.end(), position,
		                            [](float a, const ground_height_info& b) { return a < b.position; });
		if (start_iter->position != position) {
			start_iter -= 1;
		}

		return openbve2::math::lerp(start_iter->value, end_iter->value,
		                            (position - start_iter->position) / (end_iter->position - start_iter->position));
	}

	openbve2::math::evaulate_curve_t pass3_executor::track_position_at(float position) {
		decltype(_route_data.blocks)::const_iterator starting_it;

		if (position < _route_data.blocks.front().position) {
			starting_it = _route_data.blocks.cbegin();
		}
		else {
			starting_it = std::upper_bound(_route_data.blocks.begin(), _route_data.blocks.end(), position,
			                               [](float a, const rail_block_info& b) { return a < b.position; });
			starting_it -= 1;
		}

		return openbve2::math::evaluate_curve(starting_it->cache.location, starting_it->cache.direction,
		                                      position - starting_it->position, starting_it->radius);
	}

	glm::vec3 pass3_executor::position_relative_to_rail(std::size_t rail_num,
	                                                    float position,
	                                                    float x_offset,
	                                                    float y_offset) {
		auto track_position = track_position_at(position);

		auto track_state_iter = current_rail_state.find(rail_num);

		if (track_state_iter == current_rail_state.end()) {
			throw std::invalid_argument("Rail Num Invalid");
		}

		return openbve2::math::position_from_offsets(track_position.position, track_position.tangent,
		                                             track_state_iter->second.x_offset + x_offset,
		                                             track_state_iter->second.y_offset + y_offset);
	}
} // namespace csv_rw_route
} // namespace parsers
