#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace parsers {
namespace xml {
	namespace route_marker {
		struct image_marker {
			std::vector<std::string> allowed_trains;
			std::string early_filename;
			std::string on_time_filename;
			std::string late_filename;
			openbve2::datatypes::time early_time;
			openbve2::datatypes::time late_time;
			std::intmax_t timeout = 0;
			float distance;
			bool using_early = false;
			bool using_late = false;
		};

		struct text_marker {
			enum class color : std::uint8_t { Black, Gray, White, Red, Orange, Green, Blue, Magenta };

			std::vector<std::string> allowed_trains;
			std::string early_text;
			std::string on_time_text;
			std::string late_text;
			openbve2::datatypes::time early_time;
			openbve2::datatypes::time late_time;
			std::intmax_t timeout = 0;
			float distance;
			color early_color = color::White;
			color on_time_color = color::White;
			color late_color = color::White;
			bool using_early = false;
			bool using_late = false;
		};

		using parsed_route_marker = mapbox::util::variant<image_marker, text_marker>;

		parsed_route_marker parse(const std::string& filename,
		                          std::string input_string,
		                          errors::errors_t& file_errors,
		                          const find_relative_file_func& get_relative_file);
	} // namespace route_marker
} // namespace xml
} // namespace parsers
