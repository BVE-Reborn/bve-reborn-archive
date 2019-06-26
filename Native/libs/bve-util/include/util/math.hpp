#pragma once

#include <glm/vec3.hpp>
#include <type_traits>

namespace bve::util::math {
	template <class T, class A, class B>
	T clamp(T value, A min, B max) {
		return value > max ? T(max) : (value < min ? T(min) : value);
	}

	template <class R = void, class A, class B>
	auto max(A a, B b) -> typename std::conditional<std::is_same<R, void>::value, typename std::common_type<A, B>::type, R>::type {
		using ActualRetval = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return ActualRetval(a) > ActualRetval(b) ? ActualRetval(a) : ActualRetval(b);
	}

	template <class R = void, class A, class B>
	auto min(A a, B b) -> typename std::conditional<std::is_same<R, void>::value, typename std::common_type<A, B>::type, R>::type {
		using ActualRetval = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return ActualRetval(a) < ActualRetval(b) ? ActualRetval(a) : ActualRetval(b);
	}

	template <class A, class B, class C>
	auto lerp(A v0, B v1, C t) {
		using Common = typename std::common_type<A, B, C>::type;
		return Common(v0) + Common(t) * (Common(v1) - Common(v0));
	}

	float radius_from_distances(float delta_x, float delta_y);

	struct EvaluateCurveState {
		glm::vec3 position;
		glm::vec3 tangent;
	};

	EvaluateCurveState evaluate_curve(glm::vec3 input_position, glm::vec3 input_direction, float distance, float radius);

	glm::vec3 position_from_offsets(glm::vec3 input_position, glm::vec3 input_tangent, float x_offset, float y_offset);
} // namespace bve::util::math
