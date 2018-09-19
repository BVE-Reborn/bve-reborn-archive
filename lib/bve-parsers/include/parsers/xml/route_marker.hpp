#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace bve::parsers::xml::route_marker {
	struct image_marker {
		std::vector<std::string> allowed_trains;
		std::string early_filename;
		std::string on_time_filename;
		std::string late_filename;
		bve::core::datatypes::time early_time = 0;
		bve::core::datatypes::time late_time = 0;
		std::intmax_t timeout = 0;
		float distance = 0;
		bool using_early = false;
		bool using_ontime = false;
		bool using_late = false;
	};

	struct text_marker {
		enum class color : std::uint8_t { black, gray, white, red, orange, green, blue, magenta };

		std::vector<std::string> allowed_trains;
		std::string early_text;
		std::string on_time_text;
		std::string late_text;
		bve::core::datatypes::time early_time = 0;
		bve::core::datatypes::time late_time = 0;
		std::intmax_t timeout = 0;
		float distance = 0;
		color early_color = color::white;
		color on_time_color = color::white;
		color late_color = color::white;
		bool using_early = false;
		bool using_ontime = false;
		bool using_late = false;
	};

	using parsed_route_marker = mapbox::util::variant<image_marker, text_marker>;

	parsed_route_marker parse(const std::string& filename,
	                          std::string input_string,
	                          errors::MultiError& errors,
	                          const find_relative_file_func& get_relative_file);
} // namespace bve::parsers::xml::route_marker
