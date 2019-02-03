#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::train::Folder& inst) {
		start(inst, "Train.Folder");

		PRINT_VALUE(filepath);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::train::Rail& inst) {
		start(inst, "Train.Rail");

		PRINT_VALUE(rail_type_index);
		PRINT_VALUE(run_sound_index);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::train::Flange& inst) {
		start(inst, "Train.Flange");

		PRINT_VALUE(rail_type_index);
		PRINT_VALUE(flange_sound_index);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::train::Timetable& inst) {
		start(inst, "Train.Timetable");

		PRINT_VALUE(timetable_index);
		PRINT_VALUE(filename);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::train::Velocity& inst) {
		start(inst, "Train.Velocity");

		PRINT_VALUE(speed);

		end();
	};
} // namespace bve::parsers::csv_rw_route