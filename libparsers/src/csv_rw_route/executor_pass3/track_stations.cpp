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

	void pass3_executor::operator()(const instructions::track::station_xml& inst) const {
		rail_station_info rsi;

		auto const xml_file_loc = get_relative_file_(get_filename(inst.file_index), inst.filename);

		auto const file_contents = util::load_from_file_utf8_bom(xml_file_loc);

		auto const parsed_xml =
		    xml::stations::parse(xml_file_loc, file_contents, errors_, get_relative_file_);

		rsi.arrival = parsed_xml.arrival_time;
		rsi.arrival_sound = add_sound_filename(parsed_xml.arrival_sound_file);
		rsi.arrival_tag = [&]() -> rail_station_info::arrival_time_t {
			if (parsed_xml.using_arrival) {
				return rail_station_info::arrival_time_t::time;
			}
			else {
				return rail_station_info::arrival_time_t::any_time;
			}
		}();
		rsi.departure = parsed_xml.departure_time;
		rsi.departure_sound = add_sound_filename(parsed_xml.departure_sound_file);
		rsi.departure_tag = [&]() -> rail_station_info::departure_time_t {
			if (parsed_xml.using_departure) {
				return rail_station_info::departure_time_t::time;
			}
			else {
				return rail_station_info::departure_time_t ::any_time;
			}
		}();
		rsi.doors = [&]() -> rail_station_info::doors_t {
			switch (parsed_xml.door) {
				default:
				case xml::stations::parsed_station_marker::doors::none:
					return rail_station_info::doors_t::none;
				case xml::stations::parsed_station_marker::doors::left:
					return rail_station_info::doors_t::left;
				case xml::stations::parsed_station_marker::doors::right:
					return rail_station_info::doors_t::right;
			}
		}();
		rsi.name = std::move(parsed_xml.station_name);
		rsi.timetable_index = parsed_xml.time_table_index;
		rsi.stop_duration = static_cast<float>(parsed_xml.stop_duration);
		rsi.force_red = parsed_xml.force_red_signal;
		rsi.request_stop_info = parsed_xml.request_stop;

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
