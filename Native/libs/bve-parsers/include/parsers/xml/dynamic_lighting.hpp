#pragma once

#include "parsers/errors.hpp"
#include "util/datatypes.hpp"
#include <vector>

namespace bve::parsers::xml::dynamic_lighting {
	struct LightingInfo {
		util::datatypes::Time time = 0;
		util::datatypes::Color8RGB ambient = {160, 160, 160};
		util::datatypes::Color8RGB directional_lighting = {160, 160, 160};
		glm::vec3 light_direction = {-0.223645424F, -0.866025404F, 0.44719428F};
		uint8_t cab_lighting = 255;
	};

	std::vector<LightingInfo> parse(const std::string& filename, std::string input_string, errors::MultiError& errors);
} // namespace bve::parsers::xml::dynamic_lighting
