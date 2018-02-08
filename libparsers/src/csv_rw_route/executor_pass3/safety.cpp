#include "../executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Beacon& inst) {
		beacon_info bi;

		bi.position = inst.absolute_position;
		bi.beacon_type = inst.type;
		bi.beacon_data = inst.data;
		bi.section_offset = inst.section;

		_route_data.beacons.emplace_back(std::move(bi));

		if (inst.beacon_structure_index != std::size_t(-1)) {
			return;
		}

		rail_object_info roi;

		auto file_iter = object_beacon_mapping.find(inst.beacon_structure_index);

		if (file_iter == object_beacon_mapping.end()) {
			std::ostringstream oss;

			oss << "Beacon Structure #" << inst.beacon_structure_index
			    << " isn't mapped. Use Structure.Beacon to declare it.";

			errors::add_error(_errors, get_filename(inst.file_index), inst.line, oss);
		}

		roi.filename = file_iter->second;
		roi.position = position_relative_to_rail(0, inst.absolute_position, inst.x_offset, inst.y_offset);
		// TODO(sirflankalot): convert PYR to angle vector
		/* roi.rotation = */

		_route_data.objects.emplace_back(std::move(roi));
	}

	void pass3_executor::operator()(const instructions::track::Transponder& inst) {}
	void pass3_executor::operator()(const instructions::track::Pattern& inst) {}
} // namespace csv_rw_route
} // namespace parsers
