#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Curve& inst) {
		start(inst, "Track.Curve");

		PRINT_VALUE(radius);
		PRINT_VALUE(cant);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Turn& inst) {
		start(inst, "Track.Turn");

		PRINT_VALUE(ratio);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Height& inst) {
		start(inst, "Track.Height");

		PRINT_VALUE(y);

		end();
	};
} // namespace bve::parsers::csv_rw_route