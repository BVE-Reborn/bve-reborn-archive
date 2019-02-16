#include "executor_pass3.hpp"
#include <sstream>

namespace bve::parsers::csv_rw_route {
	void Pass3Executor::operator()(const instructions::track::Sta& inst) const {
		RailStation rsi;
		rsi.arrival = inst.arrival;
		rsi.arrival_sound = addSoundFilename(inst.arrival_sound);
		rsi.arrival_tag = inst.arrival_tag;
		rsi.departure = inst.departure;
		rsi.departure_sound = addSoundFilename(inst.departure_sound);
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

	void Pass3Executor::operator()(const instructions::track::StationXml& inst) const {
		RailStation rsi;

		auto const xml_file_loc = get_relative_file_(getFilename(inst.file_index), inst.filename);

		auto const file_contents = util::parsers::load_from_file_utf8_bom(xml_file_loc);

		auto const parsed_xml = xml::stations::parse(xml_file_loc, file_contents, errors_, get_relative_file_);

		rsi.arrival = parsed_xml.arrival_time;
		rsi.arrival_sound = addSoundFilename(parsed_xml.arrival_sound_file);
		rsi.arrival_tag = [&]() -> RailStation::ArrivalTime {
			if (parsed_xml.using_arrival) {
				return RailStation::ArrivalTime::time;
			}
			return RailStation::ArrivalTime::any_time;
		}();
		rsi.departure = parsed_xml.departure_time;
		rsi.departure_sound = addSoundFilename(parsed_xml.departure_sound_file);
		rsi.departure_tag = [&]() -> RailStation::DepartureTime {
			if (parsed_xml.using_departure) {
				return RailStation::DepartureTime::time;
			}
			return RailStation::DepartureTime ::any_time;
		}();
		rsi.doors = [&]() -> RailStation::Doors {
			switch (parsed_xml.door) {
				default:
				case xml::stations::ParsedStationMarker::Doors::none:
					return RailStation::Doors::none;
				case xml::stations::ParsedStationMarker::Doors::left:
					return RailStation::Doors::left;
				case xml::stations::ParsedStationMarker::Doors::right:
					return RailStation::Doors::right;
			}
		}();
		rsi.name = std::move(parsed_xml.station_name);
		rsi.timetable_index = parsed_xml.time_table_index;
		rsi.stop_duration = static_cast<float>(parsed_xml.stop_duration);
		rsi.force_red = parsed_xml.force_red_signal;
		rsi.request_stop_info = parsed_xml.request_stop;

		route_data_.stations.emplace_back(std::move(rsi));
	}

	void Pass3Executor::operator()(const instructions::track::Stop& inst) const {
		if (route_data_.stations.empty()) {
			std::ostringstream err;

			err << "Track.Stop: no station to bind to. Ignoring.";
			add_error(errors_, getFilename(inst.file_index), inst.line, err);
			return;
		}

		RailStationStop stop{};
		stop.position = inst.absolute_position;
		stop.backward_tolerance = inst.backwards_tolerance;
		stop.forward_tolerance = inst.forwards_tolerance;
		stop.car_count = inst.cars;
		stop.direction = inst.stop_post;

		route_data_.stations.back().stop_points.emplace_back(std::move(stop));
	}

	void Pass3Executor::operator()(const instructions::track::Form& inst) const {
		(void) inst;
		// Todo(cwfitzgerald): form?
	}
} // namespace bve::parsers::csv_rw_route
