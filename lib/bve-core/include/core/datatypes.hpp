#pragma once

#include <cinttypes>
#include <cmath>
#include <glm/integer.hpp>
#include <glm/vec3.hpp>
#include <random>
#include <type_traits>

namespace bve::core::datatypes {
	using Color8R = glm::u8vec1;
	using Color8RG = glm::u8vec2;
	using Color8RGB = glm::u8vec3;
	using Color8RGBA = glm::u8vec4;

	using Color16R = glm::u16vec1;
	using Color16RG = glm::u16vec2;
	using Color16RGB = glm::u16vec3;
	using Color16RGBA = glm::u16vec4;

	using ColorR32F = glm::highp_vec1;
	using ColorRG32F = glm::vec2;
	using ColorRGB32F = glm::vec3;
	using ColorRGBA32F = glm::vec4;

	using RNG = std::mt19937;

	using Time = std::intmax_t;
} // namespace bve::core::datatypes
