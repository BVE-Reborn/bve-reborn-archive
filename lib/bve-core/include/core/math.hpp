#pragma once

#include <glm/vec3.hpp>
#include <type_traits>

namespace bve::core::math {
	template <class T, class A, class B>
	T clamp(T value, A min, B max) {
		return value > max ? T(max) : (value < min ? T(min) : value);
	}

	template <class R = void, class A, class B>
	auto max(A a, B b) -> typename std::
	    conditional<std::is_same<R, void>::value, typename std::common_type<A, B>::type, R>::type {
		using t = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return t(a) > t(b) ? t(a) : t(b);
	}

	template <class R = void, class A, class B>
	auto min(A a, B b) -> typename std::
	    conditional<std::is_same<R, void>::value, typename std::common_type<A, B>::type, R>::type {
		using t = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return t(a) < t(b) ? t(a) : t(b);
	}

	template <class A, class B, class C>
	auto lerp(A v0, B v1, C t) {
		using common = typename std::common_type<A, B, C>::type;
		return common(v0) + common(t) * (common(v1) - common(v0));
	}

	float radius_from_distances(float deltax, float deltay);

	struct evaulate_curve_t {
		glm::vec3 position;
		glm::vec3 tangent;
	};

	evaulate_curve_t evaluate_curve(glm::vec3 input_position,
	                                glm::vec3 input_direction,
	                                float distance,
	                                float radius);

	glm::vec3 position_from_offsets(glm::vec3 input_position,
	                                glm::vec3 input_tangent,
	                                float x_offset,
	                                float y_offset);
} // namespace bve::core::math
