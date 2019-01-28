#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace bve::parsers::xml::dynamic_background {
	struct TextureBackgroundInfo {
		core::datatypes::Time time = 0;
		enum { fade_in, fade_out, none } transition_mode = none;
		std::size_t repetitions = 6;
		std::string filename;
		std::size_t transition_time = 10;
		bool preserve_aspect = false;
		bool from_xml = true;
	};

	struct ObjectBackgroundInfo {
		std::string filename;
	};

	using ParsedDynamicBackground =
	    mapbox::util::variant<std::vector<TextureBackgroundInfo>, ObjectBackgroundInfo>;

	ParsedDynamicBackground parse(const std::string& filename,
	                              std::string input_string,
	                              errors::MultiError& errors,
	                              const RelativeFileFunc& get_relative_file);
} // namespace bve::parsers::xml::dynamic_background
