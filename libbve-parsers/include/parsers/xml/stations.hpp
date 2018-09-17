#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <cinttypes>
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace parsers::xml::stations {
	struct message {
		std::string early;
		std::string ontime;
		std::string late;
	};

	struct probabilities {
		std::uint8_t early = 0;
		std::uint8_t ontime = 0;
		std::uint8_t late = 0;
	};

	struct request_stop_marker {
		enum class behaviour : std::uint8_t { fullspeed, normalbrake };
		core::datatypes::time early_time = 0;
		bool using_early = false;
		core::datatypes::time late_time = 0;
		bool using_late = false;
		float distance = 0;
		message stop_message;
		message pass_message;
		probabilities probability;
		std::uintmax_t max_cars = 0;
		behaviour ai_behaviour = behaviour::fullspeed;
	};

	struct parsed_station_marker {
		enum class doors : std::int8_t { left = -1, right = 1, none = 0 };
		std::string station_name;
		core::datatypes::time arrival_time = 0;
		bool using_arrival = false;
		core::datatypes::time departure_time = 0;
		bool using_departure = false;
		doors door = doors::none;
		bool force_red_signal = false;
		std::uint8_t passenger_ratio = 100;
		std::string arrival_sound_file;
		std::string departure_sound_file;
		std::uintmax_t stop_duration = 15;
		std::size_t time_table_index = 0;
		request_stop_marker request_stop;
	};

	parsed_station_marker parse(const std::string& filename,
	                            std::string input_string,
	                            errors::multi_error_t& errors,
	                            const find_relative_file_func& get_relative_file);
} // namespace parsers::xml::stations
