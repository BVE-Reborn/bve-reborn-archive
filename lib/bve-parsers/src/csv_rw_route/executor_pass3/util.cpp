#include "executor_pass3.hpp"

namespace bve::parsers::csv_rw_route {
	FilenameSetIterator Pass3Executor::addObjectFilename(std::string const& val) const {
		return route_data_.object_filenames.insert(util::parsers::lower_copy(val)).first;
	}

	RailState& Pass3Executor::getRailState(std::size_t const index) {
		if (index == std::numeric_limits<std::size_t>::max()) {
			return current_rail_state_[0];
		}
		return current_rail_state_[index];
	}

	float Pass3Executor::groundHeightAt(float const position) const {
		if (position <= route_data_.ground_height.front().position) {
			return route_data_.ground_height.front().value;
		}
		if (position >= route_data_.ground_height.back().position) {
			return route_data_.ground_height.back().value;
		}

		// WHY DO LOWER BOUND AND UPPER BOUND HAVE DIFFERENT F***ING INTERFACES!
		// I CAN'T USE std::equal_range UNLESS I PROVIDE IT A FUNCTION WITH
		// *OVERLOADS* GAAH!
		auto start_iter = std::lower_bound(route_data_.ground_height.begin(), route_data_.ground_height.end(), position,
		                                   [](const GroundHeight& a, float const b) { return a.position < b; });
		auto const end_iter = std::upper_bound(route_data_.ground_height.begin(), route_data_.ground_height.end(), position,
		                                       [](float const a, const GroundHeight& b) { return a < b.position; });

		if (start_iter->position != position) {
			start_iter -= 1;
		}

		return util::math::lerp(start_iter->value, end_iter->value,
		                        (position - start_iter->position) / (end_iter->position - start_iter->position));
	}

	util::math::EvaluateCurveState Pass3Executor::trackPositionAt(float const position) const {
		decltype(route_data_.blocks)::const_iterator starting_it;

		if (position < route_data_.blocks.front().position) {
			starting_it = route_data_.blocks.cbegin();
		}
		else {
			starting_it = std::upper_bound(route_data_.blocks.begin(), route_data_.blocks.end(), position,
			                               [](float const a, const RailBlockInfo& b) { return a < b.position; });
			starting_it -= 1;
		}

		return util::math::evaluate_curve(starting_it->cache.location, starting_it->cache.direction, position - starting_it->position,
		                                  starting_it->radius);
	}

	glm::vec3 Pass3Executor::positionRelativeToRail(std::size_t rail_num,
	                                                float const position,
	                                                float const x_offset,
	                                                float const y_offset) {
		auto const track_position = trackPositionAt(position);

		auto max = false;
		if (rail_num == std::numeric_limits<std::size_t>::max()) {
			max = true;
			rail_num = 0;
		}

		auto const track_state_iter = current_rail_state_.find(rail_num);

#ifndef NDEBUG
		if (track_state_iter == current_rail_state_.end()) {
			throw std::invalid_argument("Rail Num Invalid");
		}
#endif

		auto ret_val =
		    util::math::position_from_offsets(track_position.position, track_position.tangent, track_state_iter->second.x_offset + x_offset,
		                                      track_state_iter->second.y_offset + y_offset);

		if (max) {
			ret_val.y -= groundHeightAt(position);
		}

		return ret_val;
	}
} // namespace bve::parsers::csv_rw_route
