#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::train::Folder& inst) {
		_route_data.default_train = inst.filepath;
	}

	void pass3_executor::operator()(const instructions::train::Rail& inst) {
		_route_data.rail_runsound_mapping[inst.rail_type_index] = inst.run_sound_index;
		rail_runsound_blame[inst.rail_type_index] =
		    file_index_line_pair{inst.file_index, inst.line};
	}

	void pass3_executor::operator()(const instructions::train::Flange& inst) {
		_route_data.rail_flangesound_mapping[inst.rail_type_index] = inst.flange_sound_index;
		rail_flangesound_blame[inst.rail_type_index] =
		    file_index_line_pair{inst.file_index, inst.line};
	}

	void pass3_executor::operator()(const instructions::train::Timetable& inst) {
		(void) inst;
		// TODO(sirflankalot): Timetables?
	}

	void pass3_executor::operator()(const instructions::train::Velocity& inst) {
		_route_data.ai_max_speed = inst.speed * unit_of_speed;
	}
} // namespace csv_rw_route
} // namespace parsers
