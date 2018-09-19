#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace bve::parsers::xml::dynamic_background {
	struct texture_background_info {
		bve::core::datatypes::time time = 0;
		enum { fade_in, fade_out, none } transition_mode = none;
		std::size_t repetitions = 6;
		std::string filename;
		std::size_t transition_time = 10;
		bool preserve_aspect = false;
		bool from_xml = true;
	};

	struct object_background_info {
		std::string filename;
	};

	using parsed_dynamic_background =
	    mapbox::util::variant<std::vector<texture_background_info>, object_background_info>;

	parsed_dynamic_background parse(const std::string& filename,
	                                std::string input_string,
	                                errors::MultiError& errors,
	                                const find_relative_file_func& get_relative_file);
} // namespace bve::parsers::xml::dynamic_background
