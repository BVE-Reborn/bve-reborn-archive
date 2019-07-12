#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::FreeObj& inst) {
		start(inst, "Track.FreeObj");

		PRINT_VALUE(rail_index);
		PRINT_VALUE(free_obj_structure_index);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(yaw);
		PRINT_VALUE(pitch);
		PRINT_VALUE(roll);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Wall& inst) {
		start(inst, "Track.Wall");

		PRINT_VALUE(rail_index);
		PRINT_ENUM3(direction, track::Wall::Direction, left, both, right);
		PRINT_VALUE(wall_structure_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::WallEnd& inst) {
		start(inst, "Track.WallEnd");

		PRINT_VALUE(rail_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Dike& inst) {
		start(inst, "Track.Dike");

		PRINT_VALUE(rail_index);
		PRINT_ENUM3(direction, track::Dike::Direction, left, both, right);
		PRINT_VALUE(dike_structure_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::DikeEnd& inst) {
		start(inst, "Track.DikeEnd");

		PRINT_VALUE(rail_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Pole& inst) {
		start(inst, "Track.Pole");

		PRINT_VALUE(rail_index);
		PRINT_VALUE(additional_rails);
		PRINT_VALUE(location);
		PRINT_VALUE(interval);
		PRINT_VALUE(pole_structure_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::PoleEnd& inst) {
		start(inst, "Track.PoleEnd");

		PRINT_VALUE(rail_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Crack& inst) {
		start(inst, "Track.Crack");

		PRINT_VALUE(rail_index_1);
		PRINT_VALUE(rail_index_2);
		PRINT_VALUE(crack_structure_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Ground& inst) {
		start(inst, "Track.Ground");

		PRINT_VALUE(ground_structure_index);

		end();
	}
} // namespace bve::parsers::csv_rw_route
