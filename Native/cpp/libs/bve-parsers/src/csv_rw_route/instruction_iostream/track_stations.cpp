#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Sta& inst) {
		start(inst, "Track.Sta");

		PRINT_VALUE(name);
		PRINT_TIME(arrival);
		PRINT_ENUM6(arrival_tag, track::Sta::ArrivalTime, time, any_time, all_pass, player_pass, player_stop, all_stop);
		PRINT_TIME(departure);
		PRINT_ENUM6(departure_tag, track::Sta::DepartureTime, time, any_time, terminal, terminal_time, change_ends, change_ends_time);
		PRINT_VALUE(pass_alarm);
		PRINT_ENUM4(doors, track::Sta::Doors, left, none, right, both);
		PRINT_VALUE(force_red);
		PRINT_VALUE(system);
		PRINT_VALUE(arrival_sound);
		PRINT_VALUE(stop_duration);
		PRINT_VALUE(passenger_ratio);
		PRINT_VALUE(departure_sound);
		PRINT_VALUE(timetable_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::StationXml& inst) {
		start(inst, "Track.StationXML");

		PRINT_VALUE(filename);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Stop& inst) {
		start(inst, "Track.Stop");

		PRINT_ENUM3(stop_post, track::Stop::StopPost, left, none, right);
		PRINT_VALUE(forwards_tolerance);
		PRINT_VALUE(backwards_tolerance);
		PRINT_VALUE(cars);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Form& inst) {
		start(inst, "Track.Form");

		PRINT_VALUE(rail_index_1);
		PRINT_VALUE(rail_index_2);
		PRINT_VALUE(roof_structure_index);
		PRINT_VALUE(form_structure_index);
		PRINT_ENUM3(placement, track::Form::Placement, left, right, rail_index);

		end();
	}
} // namespace bve::parsers::csv_rw_route
