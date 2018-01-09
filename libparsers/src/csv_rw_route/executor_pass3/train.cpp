#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::train::Folder& instr) {
		_route_data.default_train = instr.filepath;
	}

	void pass3_executor::operator()(const instructions::train::Rail& instr) {
		_route_data.rail_runsound_mapping[instr.rail_type_index] = instr.run_sound_index;
		rail_runsound_blame[instr.rail_type_index] = file_index_line_pair{instr.file_index, instr.line};
	}

	void pass3_executor::operator()(const instructions::train::Flange& instr) {
		_route_data.rail_flangesound_mapping[instr.rail_type_index] = instr.flange_sound_index;
		rail_flangesound_blame[instr.rail_type_index] = file_index_line_pair{instr.file_index, instr.line};
	}

	void pass3_executor::operator()(const instructions::train::Timetable& instr) {
		(void) instr;
		// TODO(sirflankalot): Timetables?
	}

	void pass3_executor::operator()(const instructions::train::Velocity& instr) {
		_route_data.ai_max_speed = instr.speed * unit_of_speed;
	}
} // namespace csv_rw_route
} // namespace parsers
