#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <cinttypes>
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace parsers {
namespace xml {
	namespace stations_and_stops {

		struct sub_message {
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
			openbve2::datatypes::time early_time = 0;
			openbve2::datatypes::time late_time = 0;
			float distance = 0;
			sub_message stop_message;
			sub_message pass_message;
			probabilities probability;
			std::intmax_t max_cars = 0;
			behaviour ai_behaviour = behaviour::fullspeed;
		};
		struct station_marker {
			enum class doors : std::int8_t { left = -1, right = 1, none = 0 };
			std::string station_name;
			openbve2::datatypes::time arrival_time = 0;
			openbve2::datatypes::time departure_time = 0;
			doors door = doors::none;
			bool force_red_signal = false;
			std::uint8_t passenger_ratio = 0;
			std::string arrival_sound_file;
			std::string departure_sound_file;
			openbve2::datatypes::time stop_duration = 15;
			std::uintmax_t time_table_index = 0;
			request_stop_marker request_stop;
		};
		using parsed_station_marker = mapbox::util::variant<station_marker, request_stop_marker>;

		parsed_station_marker parse(const std::string& filename,
		                            std::string& input_string,
		                            errors::multi_error_t& errors,
		                            const find_relative_file_func& get_relative_file);
	} // namespace stations_and_stops
} // namespace xml
} // namespace parsers