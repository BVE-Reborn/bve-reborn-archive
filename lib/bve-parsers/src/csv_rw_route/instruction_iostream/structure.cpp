#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::structure::Command& inst) {
		start(inst, "Structure.Command");

		PRINT_ENUM18(command_type, structure::Command::Type, ground, rail, wall_l, wall_r, dike_l,
		             dike_r, form_l, form_r, form_cl, form_cr, roof_l, form_r, form_cl, form_cr,
		             crack_l, crack_r, free_obj, beacon);

		PRINT_VALUE(structure_index);
		PRINT_VALUE(filename);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::structure::Pole& inst) {
		start(inst, "Structure.Pole");

		PRINT_VALUE(additional_rails);
		PRINT_VALUE(pole_structure_index);
		PRINT_VALUE(filename);

		end();
	};
} // namespace bve::parsers::csv_rw_route