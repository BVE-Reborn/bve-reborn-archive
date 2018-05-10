#include "executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::sta& inst) const {
		rail_station_info rsi;
		rsi.arrival = inst.arrival;
		rsi.arrival_sound = add_sound_filename(inst.arrival_sound);
		rsi.arrival_tag = inst.arrival_tag;
		rsi.departure = inst.departure;
		rsi.departure_sound = add_sound_filename(inst.departure_sound);
		rsi.departure_tag = inst.departure_tag;
		rsi.doors = inst.doors;
		rsi.force_red = inst.force_red;
		rsi.name = inst.name;
		rsi.passenger_ratio = inst.passenger_ratio;
		rsi.pass_alarm = inst.pass_alarm;
		rsi.stop_duration = inst.stop_duration;
		rsi.system = inst.system;
		rsi.timetable_index = inst.timetable_index;

		route_data_.stations.emplace_back(std::move(rsi));
	}

	void pass3_executor::operator()(const instructions::track::stop& inst) const {
		if (route_data_.stations.empty()) {
			std::ostringstream err;

			err << "Track.Stop: no station to bind to. Ignoring.";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
			return;
		}

		rail_station_stop_info rssi;
		rssi.position = inst.absolute_position;
		rssi.backward_tolerance = inst.backwards_tolerance;
		rssi.forward_tolerance = inst.forwards_tolerance;
		rssi.car_count = inst.cars;
		rssi.direction = inst.stop_post;

		route_data_.stations.back().stop_points.emplace_back(std::move(rssi));
	}

	void pass3_executor::operator()(const instructions::track::form& inst) const {
		(void) inst;
		// Todo(sirflankalot): form?
	}
} // namespace csv_rw_route
} // namespace parsers
