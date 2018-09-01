//
// Created by kevin Cybura on 9/1/18.
//
#pragma once

#include "ini.hpp"

namespace parsers {
namespace panel2 {
	namespace sections {
		using Point = std::pair<double, double>;
		struct this_s {
			double resolution = 0;
			// Not required.
			double left = 0;
			double right = 0;
			double top = 0;
			double bottom = 0;
			//
			// Must be specified together.
			std::string day_time_image;
			std::string night_time_image;
			//
			u_int32_t teansparent_color = 0; // THIS IS HEX.
			Point center;
			Point origin;
		};
	} // namespace sections
	struct parsed_panel2_cfg_t {
		sections::this_s section_this;
	};
	parsed_panel2_cfg_t parse_panel2_cfg(std::string const& file_contents);
	parsed_panel2_cfg_t create_sections(ini::parsed_ini_object const& obj);
} // namespace panel2
} // namespace parsers
