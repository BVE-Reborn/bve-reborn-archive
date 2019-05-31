#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::naked::Position& inst) {
		start(inst, "Position");

		PRINT_VECTOR(distances);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::naked::None& inst) {
		start(inst, "None");

		end();
	}
} // namespace bve::parsers::csv_rw_route
