#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <cinttypes>
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace bve::parsers::xml::stations {
	struct Message {
		std::string early;
		std::string ontime;
		std::string late;
	};

	struct Probabilities {
		std::uint8_t early = 0;
		std::uint8_t ontime = 0;
		std::uint8_t late = 0;
	};

	struct RequestStopMarker {
		enum class Behaviour : std::uint8_t { full_speed, normal_brake };
		core::datatypes::Time early_time = 0;
		bool using_early = false;
		core::datatypes::Time late_time = 0;
		bool using_late = false;
		float distance = 0;
		Message stop_message;
		Message pass_message;
		Probabilities probability;
		std::uintmax_t max_cars = 0;
		Behaviour ai_behaviour = Behaviour::full_speed;
	};

	struct ParsedStationMarker {
		enum class Doors : std::int8_t { left = -1, right = 1, none = 0 };
		std::string station_name;
		core::datatypes::Time arrival_time = 0;
		bool using_arrival = false;
		core::datatypes::Time departure_time = 0;
		bool using_departure = false;
		Doors door = Doors::none;
		bool force_red_signal = false;
		std::uint8_t passenger_ratio = 100;
		std::string arrival_sound_file;
		std::string departure_sound_file;
		std::uintmax_t stop_duration = 15;
		std::size_t time_table_index = 0;
		RequestStopMarker request_stop;
	};

	ParsedStationMarker parse(const std::string& filename,
	                          std::string input_string,
	                          errors::MultiError& errors,
	                          const RelativeFileFunc& get_relative_file);
} // namespace bve::parsers::xml::stations
