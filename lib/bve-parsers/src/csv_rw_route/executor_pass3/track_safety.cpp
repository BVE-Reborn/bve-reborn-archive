#include "executor_pass3.hpp"
#include <limits>
#include <sstream>

using namespace std::string_literals;

namespace bve::parsers::csv_rw_route {
	void pass3_executor::operator()(const instructions::track::beacon& inst) {
		beacon_info bi;

		bi.position = inst.absolute_position;
		bi.beacon_type = inst.type;
		bi.beacon_data = inst.data;
		bi.section_offset = inst.section;

		route_data_.beacons.emplace_back(std::move(bi));

		if (inst.beacon_structure_index != std::numeric_limits<std::size_t>::max()) {
			return;
		}

		rail_object_info roi;

		auto const file_iter = object_beacon_mapping_.find(inst.beacon_structure_index);

		if (file_iter == object_beacon_mapping_.end()) {
			std::ostringstream oss;

			oss << "Beacon Structure #" << inst.beacon_structure_index
			    << " isn't mapped. Use Structure.Beacon to declare it.";

			add_error(errors_, get_filename(inst.file_index), inst.line, oss);
		}

		roi.filename = file_iter->second;
		roi.position =
		    position_relative_to_rail(0, inst.absolute_position, inst.x_offset, inst.y_offset);
		// TODO(sirflankalot): convert PYR to angle vector
		/* roi.rotation = */

		route_data_.objects.emplace_back(std::move(roi));
	}

	void pass3_executor::operator()(const instructions::track::transponder& inst) {
		beacon_info bi;

		bi.position = inst.absolute_position;
		bi.beacon_type = static_cast<std::underlying_type<decltype(inst.type)>::type>(inst.type);
		if (inst.type != 2) {
			bi.beacon_data = inst.switch_system ? 0 : -1;
		}
		else {
			bi.beacon_data = 0;
		}
		bi.section_offset = inst.signal;

		route_data_.beacons.emplace_back(std::move(bi));

		rail_object_info roi;

		switch (inst.type) {
			case 0:
				roi.filename = add_object_filename("\034compat\034/transponder/S"s);
				break;
			case 1:
				roi.filename = add_object_filename("\034compat\034/transponder/SN"s);
				break;
			case 2:
				roi.filename = add_object_filename("\034compat\034/transponder/AccidentalDep"s);
				break;
			case 3:
				roi.filename = add_object_filename("\034compat\034/transponder/ATSP-Pattern"s);
				break;
			case 4:
				roi.filename = add_object_filename("\034compat\034/transponder/ATSP-Immediate"s);
				break;
			default:
				assert(false);
				break;
		}

		roi.position =
		    position_relative_to_rail(0, inst.absolute_position, inst.x_offset, inst.y_offset);
		// TODO(sirflankalot): convert PYR to angle vector
		/* roi.rotation = */

		route_data_.objects.emplace_back(std::move(roi));
	}

	void pass3_executor::operator()(const instructions::track::pattern& inst) const {
		atsp_section_info asi;

		asi.position = inst.absolute_position;
		asi.permanent = bool(inst.type) ? bool(inst.permanent) : bool(inst.temporary);
		asi.speed = inst.speed;

		route_data_.patterns.emplace_back(std::move(asi));
	}
} // namespace bve::parsers::csv_rw_route
