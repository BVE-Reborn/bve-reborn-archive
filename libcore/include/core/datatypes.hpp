#pragma once

#include <cinttypes>
#include <cmath>
#include <glm/integer.hpp>
#include <glm/vec3.hpp>
#include <random>
#include <type_traits>

namespace bvereborn {
namespace datatypes {
	using color8_r = glm::u8vec1;
	using color8_rg = glm::u8vec2;
	using color8_rgb = glm::u8vec3;
	using color8_rgba = glm::u8vec4;

	using color16_r = glm::u16vec1;
	using color16_rg = glm::u16vec2;
	using color16_rgb = glm::u16vec3;
	using color16_rgba = glm::u16vec4;

	using colorf_r = glm::highp_vec1;
	using colorf_rg = glm::vec2;
	using colorf_rgb = glm::vec3;
	using colorf_rgba = glm::vec4;

	using rng = std::mt19937;

	using time = std::intmax_t;

	/*class position_value {
	  private:
	    constexpr static std::intmax_t wraparound = 32782;
	    std::intmax_t offset;
	    float value;

	  public:
	    position_value() : offset(0), value(0.0f){};

	    template <class T, typename std::enable_if<std::is_integral<T>::value, void>::type** =
	nullptr> position_value(T input) : offset(input / wraparound), value(input % wraparound){};

	    template <class T, typename std::enable_if<std::is_floating_point<T>::value, void>::type** =
	nullptr> position_value(T input) : offset(std::floor(input / static_cast<float>(wraparound))),
	value(std::remainder(input, static_cast<float>(wraparound))){};

	    template <class T, typename std::enable_if<std::is_integral<T>::value, void>::type** =
	nullptr> explicit operator T() { return T(offset * wraparound + std::intmax_t(value));
	    };

	    template <class T, typename std::enable_if<std::is_floating_point<T>::value, void>::type** =
	nullptr> explicit operator T() { return T(offset) * wraparound + value;
	    };

	    friend position_value& operator+(position_value& lhs, const position_value& rhs);
	    friend position_value& operator-(position_value& lhs, const position_value& rhs);
	    friend bool operator<(position_value& lhs, const position_value& rhs);
	    friend bool operator>(position_value& lhs, const position_value& rhs);
	    friend bool operator<=(position_value& lhs, const position_value& rhs);
	    friend bool operator>=(position_value& lhs, const position_value& rhs);
	    friend bool operator==(position_value& lhs, const position_value& rhs);
	    friend bool operator!=(position_value& lhs, const position_value& rhs);
	};

	inline position_value& operator+(position_value& lhs, const position_value& rhs) {
	    auto sum = 2;
	    float val = std::remainder(, static_cast<float>(position_value::wraparound));


	    return lhs;
	}
	inline position_value& operator-(position_value& lhs, const position_value& rhs);
	inline bool operator<(position_value& lhs, const position_value& rhs);
	inline bool operator>(position_value& lhs, const position_value& rhs);
	inline bool operator<=(position_value& lhs, const position_value& rhs);
	inline bool operator>=(position_value& lhs, const position_value& rhs);
	inline bool operator==(position_value& lhs, const position_value& rhs);
	inline bool operator!=(position_value& lhs, const position_value& rhs);*/
} // namespace datatypes
} // namespace bvereborn
