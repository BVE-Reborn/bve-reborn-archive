//
// Created by kevin Cybura on 9/1/18.
//
#pragma once

#include "ini.hpp"
#include <include/parsers/errors.hpp>
#include <core/datatypes.hpp>

namespace parsers {
namespace panel2 {
	namespace sections {
		using Point = std::pair<float, float>;
		using color = bvereborn::datatypes::color8_rgba;

		struct linear_gauge_s {
			//Subject
			Point location;
			float min;
			float max;
			Point direction;
			uint32_t width;
			uint32_t layer;
		};

		struct digital_gauge_s{
			//Subject
			Point location;
			float radius;
			color color_;
			float initial_angle;
			float last_angle;
			float min;
			float max;
			float step;
			u_int32_t layer;
		};

		struct digital_number_s {
		    // Subject
		    Point location;
		    std::string day_time_image;
		    std::string night_time_image;
		    color transparent_color;
		    u_int32_t interval;
		    u_int32_t layer;
		};

		struct needle_s {
			// Subject
			Point location;
			float radius;
			std::string day_time_image;
			std::string night_time_image;
			color color_;
			color transparent_color;
			Point origin;
			float initial_angle;
			float last_angle;
			float min;
			float max;
			float natural_freq;
			float damping_ratio;
			u_int32_t layer;
		};

		struct pilot_lamp_s {
			// Subject.
			Point location;
			std::string day_time_image;
			std::string night_time_image;
			color transparent_color;
			u_int32_t layer = 0;
		};
		struct this_s {
			float resolution = 0;
			// Not required.
			float left = 0;
			float right = 0;
			float top = 0;
			float bottom = 0;
			//
			// Must be specified together.
			std::string day_time_image;
			std::string night_time_image;
			//
			color transparent_color;
			Point center;
			Point origin;
		};
	} // namespace sections
	struct parsed_panel2_cfg_t {
		sections::this_s section_this;
		sections::pilot_lamp_s pilot_lamp;
		sections::needle_s needle;
		sections::digital_number_s number;
		sections::digital_gauge_s gauge;
		sections::linear_gauge_s linear;
	};
	parsed_panel2_cfg_t parse_panel2_cfg(std::string const& file_contents,
	                                     std::string const& file_name,
	                                     errors::multi_error_t& errors);
	parsed_panel2_cfg_t create_sections(ini::parsed_ini_object const& obj,
	                                    std::string const& file_name,
	                                    errors::multi_error_t& errors);
} // namespace panel2
} // namespace parsers
