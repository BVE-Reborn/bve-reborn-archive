#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::cycle::Ground& inst) {
		start(inst, "Cycle.Ground");

		PRINT_VALUE(cycle_structure_index);
		PRINT_VECTOR(input_indices);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::cycle::Rail& inst) {
		start(inst, "Cycle.Rail");

		PRINT_VALUE(cycle_structure_index);
		PRINT_VECTOR(input_indices);

		end();
	};
} // namespace bve::parsers::csv_rw_route