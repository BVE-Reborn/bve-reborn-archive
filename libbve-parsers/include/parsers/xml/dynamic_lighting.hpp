#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include <vector>

namespace parsers::xml::dynamic_lighting {
	struct lighting_info {
		core::datatypes::time time = 0;
		core::datatypes::color8_rgb ambient = {160, 160, 160};
		core::datatypes::color8_rgb directional_lighting = {160, 160, 160};
		glm::vec3 light_direction = {-0.223645424f, -0.866025404f, 0.44719428f};
		uint8_t cablighting = 255;
	};

	std::vector<lighting_info> parse(const std::string& filename,
	                                 std::string input_string,
	                                 errors::multi_error_t& errors);
} // namespace parsers::xml::dynamic_lighting
