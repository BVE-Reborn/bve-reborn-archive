#pragma once

#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include "util/datatypes.hpp"
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace bve::parsers::xml::route_marker {
	struct ImageMarker {
		std::vector<std::string> allowed_trains;
		std::string early_filename;
		std::string on_time_filename;
		std::string late_filename;
		util::datatypes::Time early_time = 0;
		util::datatypes::Time late_time = 0;
		std::intmax_t timeout = 0;
		float distance = 0;
		bool using_early = false;
		bool using_ontime = false;
		bool using_late = false;
	};

	struct TextMarker {
		enum class Color : std::uint8_t { black, gray, white, red, orange, green, blue, magenta };

		std::vector<std::string> allowed_trains;
		std::string early_text;
		std::string on_time_text;
		std::string late_text;
		util::datatypes::Time early_time = 0;
		util::datatypes::Time late_time = 0;
		std::intmax_t timeout = 0;
		float distance = 0;
		Color early_color = Color::white;
		Color on_time_color = Color::white;
		Color late_color = Color::white;
		bool using_early = false;
		bool using_ontime = false;
		bool using_late = false;
	};

	using ParsedRouteMarker = mapbox::util::variant<ImageMarker, TextMarker>;

	ParsedRouteMarker parse(const std::string& filename,
	                        std::string input_string,
	                        errors::MultiError& errors,
	                        const RelativeFileFunc& get_relative_file);
} // namespace bve::parsers::xml::route_marker
