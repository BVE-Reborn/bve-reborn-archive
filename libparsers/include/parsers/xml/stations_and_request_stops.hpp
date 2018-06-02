#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace parsers {
namespace xml {
	namespace stations_and_stops {
		struct station_marker {
			enum class door : std::int8_t { left = -1, right = 1, none = 0 };
			std::string station_name;
			openbve2::datatypes::time arrival_time = 0;
			openbve2::datatypes::time departure_time = 0;
			door doors = door::none;
			bool force_red_signal = false;
			int passenger_ratio = 0;
			std::string arrival_sound_file;
			std::string departure_sound_file;
			openbve2::datatypes::time stop_duration = 15;
			int time_table_index = 0;
		};
		struct request_stop_marker {
			enum class behaviour : std::uint8_t { fullspeed, normalbrake };
			openbve2::datatypes::time early_time = 0;
			openbve2::datatypes::time late_time = 0;
			float distance = 0;
			std::string stop_message;
			std::string pass_messaage;
			int probability = 0;
			int max_cars = 0;
			behaviour AI_behaviour = behaviour::fullspeed;
		};
		using parsed_station_marker = mapbox::util::variant<station_marker, request_stop_marker>;

		parsed_station_marker parse(const std::string& filename,
		                            std::string& input_string,
		                            errors::multi_error_t& errors,
		                            const find_relative_file_func& get_relative_file);
	} // namespace stations_and_stops
} // namespace xml
} // namespace parsers