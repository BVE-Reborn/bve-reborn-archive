#pragma once

#include <cinttypes>
#include <glm/integer.hpp>
#include <glm/vec3.hpp>
#include <random>

namespace openbve2 {
namespace datatypes {
	using color8_r = glm::u8vec1;
	using color8_rg = glm::u8vec2;
	using color8_rgb = glm::u8vec3;
	using color8_rgba = glm::u8vec4;

	using color16_r = glm::u16vec1;
	using color16_rg = glm::u16vec2;
	using color16_rgb = glm::u16vec3;
	using color16_rgba = glm::u16vec4;

	using colorF_r = glm::highp_vec1;
	using colorF_rg = glm::vec2;
	using colorF_rgb = glm::vec3;
	using colorF_rgba = glm::vec4;

	using rng = std::mt19937;

	using time = std::intmax_t;
} // namespace datatypes
} // namespace openbve2
