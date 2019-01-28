#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include <vector>

namespace bve::parsers::xml::dynamic_lighting {
	struct LightingInfo {
		core::datatypes::Time time = 0;
		core::datatypes::Color8RGB ambient = {160, 160, 160};
		core::datatypes::Color8RGB directional_lighting = {160, 160, 160};
		glm::vec3 light_direction = {-0.223645424f, -0.866025404f, 0.44719428f};
		uint8_t cab_lighting = 255;
	};

	std::vector<LightingInfo> parse(const std::string& filename,
	                                std::string input_string,
	                                errors::MultiError& errors);
} // namespace bve::parsers::xml::dynamic_lighting