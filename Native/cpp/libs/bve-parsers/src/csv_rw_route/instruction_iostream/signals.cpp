#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::naked::SignalAnimated& inst) {
		start(inst, "SignalAnimated");

		PRINT_VALUE(signal_index);
		PRINT_VALUE(filename);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::naked::Signal& inst) {
		start(inst, "Signal");

		PRINT_VALUE(signal_index);
		PRINT_VALUE(signal_filename);
		PRINT_VALUE(glow_filename);

		end();
	}
} // namespace bve::parsers::csv_rw_route
