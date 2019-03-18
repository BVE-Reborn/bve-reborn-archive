#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::RailStart& inst) {
		start(inst, "Track.RailStart");

		PRINT_VALUE(rail_index);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(rail_type);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Rail& inst) {
		start(inst, "Track.Rail");

		PRINT_VALUE(rail_index);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(rail_type);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::RailType& inst) {
		start(inst, "Track.RailType");

		PRINT_VALUE(rail_index);
		PRINT_VALUE(rail_type_number);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::RailEnd& inst) {
		start(inst, "Track.RailEnd");

		PRINT_VALUE(rail_index);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Adhesion& inst) {
		start(inst, "Track.Adhesion");

		PRINT_VALUE(value);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Pitch& inst) {
		start(inst, "Track.Pitch");

		PRINT_VALUE(rate);

		end();
	}
} // namespace bve::parsers::csv_rw_route
