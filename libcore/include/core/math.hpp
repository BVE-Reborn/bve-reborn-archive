#pragma once

#include <type_traits>
#include <glm/vec3.hpp>

namespace openbve2 {
namespace math {
	template <class T, class A, class B>
	inline T clamp(T value, A min, B max) {
		return value > max ? T(max) : (value < min ? T(min) : value);
	}

	template <class R = void, class A, class B>
	inline auto max(A a, B b) ->
		typename std::conditional<std::is_same<R, void>::value,
		typename std::common_type<A, B>::type, R>::type {
		using t = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return t(a) > t(b) ? t(a) : t(b);
	}

	template <class R = void, class A, class B>
	inline auto min(A a, B b) ->
	    typename std::conditional<std::is_same<R, void>::value,
	                              typename std::common_type<A, B>::type, R>::type {
		using t = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return t(a) < t(b) ? t(a) : t(b);
	}

	float radius_from_distances(float deltax, float deltay);

	struct evaulate_curve_t {
		glm::vec3 position;
		glm::vec3 tangent;
	};

	evaulate_curve_t evaluate_curve(glm::vec3 input_position, glm::vec3 input_direction, float distance, float radius);
} // namespace math
} // namespace openbve2
