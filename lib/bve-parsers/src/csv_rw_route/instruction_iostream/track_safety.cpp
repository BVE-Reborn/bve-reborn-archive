#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Beacon& inst) {
		start(inst, "Track.Beacon");

		PRINT_VALUE(type);
		PRINT_VALUE(beacon_structure_index);
		PRINT_VALUE(section);
		PRINT_VALUE(data);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(yaw);
		PRINT_VALUE(pitch);
		PRINT_VALUE(roll);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Transponder& inst) {
		start(inst, "Track.Transponder");

		PRINT_ENUM5(type, track::Transponder::Type, s_type, sn_type, departure, ats_p_renewal, ats_p_stop);
		PRINT_VALUE(signal);
		PRINT_VALUE(switch_system);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(yaw);
		PRINT_VALUE(pitch);
		PRINT_VALUE(roll);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Pattern& inst) {
		start(inst, "Track.Pattern");

		PRINT_ENUM2(type, track::Pattern::Type, temporary, permanent);
		PRINT_VALUE(speed);

		end();
	};
} // namespace bve::parsers::csv_rw_route