#include "executor_pass3.hpp"

namespace bve::parsers::csv_rw_route {
	void Pass3Executor::operator()(const instructions::train::Folder& inst) const {
		route_data_.default_train = inst.filepath;
	}

	void Pass3Executor::operator()(const instructions::train::Rail& inst) {
		route_data_.rail_runsound_mapping[inst.rail_type_index] = inst.run_sound_index;
		rail_runsound_blame_[inst.rail_type_index] = FileIndexLinePair{inst.file_index, inst.line};
	}

	void Pass3Executor::operator()(const instructions::train::Flange& inst) {
		route_data_.rail_flangesound_mapping[inst.rail_type_index] = inst.flange_sound_index;
		rail_flangesound_blame_[inst.rail_type_index] =
		    FileIndexLinePair{inst.file_index, inst.line};
	}

	void Pass3Executor::operator()(const instructions::train::Timetable& inst) const {
		(void) inst;
		// TODO(cwfitzgerald): Timetables?
	}

	void Pass3Executor::operator()(const instructions::train::Velocity& inst) const {
		route_data_.ai_max_speed = inst.speed * unit_of_speed_;
	}
} // namespace bve::parsers::csv_rw_route
