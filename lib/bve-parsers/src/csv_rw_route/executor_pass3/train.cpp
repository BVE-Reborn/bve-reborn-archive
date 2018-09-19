#include "executor_pass3.hpp"

namespace bve::parsers::csv_rw_route {
	void pass3_executor::operator()(const instructions::train::Folder& inst) const {
		route_data_.default_train = inst.filepath;
	}

	void pass3_executor::operator()(const instructions::train::Rail& inst) {
		route_data_.rail_runsound_mapping[inst.rail_type_index] = inst.run_sound_index;
		rail_runsound_blame_[inst.rail_type_index] =
		    file_index_line_pair{inst.file_index, inst.line};
	}

	void pass3_executor::operator()(const instructions::train::Flange& inst) {
		route_data_.rail_flangesound_mapping[inst.rail_type_index] = inst.flange_sound_index;
		rail_flangesound_blame_[inst.rail_type_index] =
		    file_index_line_pair{inst.file_index, inst.line};
	}

	void pass3_executor::operator()(const instructions::train::Timetable& inst) const {
		(void) inst;
		// TODO(sirflankalot): Timetables?
	}

	void pass3_executor::operator()(const instructions::train::Velocity& inst) const {
		route_data_.ai_max_speed = inst.speed * unit_of_speed_;
	}
} // namespace bve::parsers::csv_rw_route
