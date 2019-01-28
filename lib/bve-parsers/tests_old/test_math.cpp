#include "core/math.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void print_evaluate_curve(const bve::core::math::EvaluateCurveState& val) {
	std::cout << "Position: " << to_string(val.position) << ", Tangent: " << to_string(val.tangent)
	          << '\n';
}

void test_math() {
	auto const distance = 12.56637f * (5.0f / 4.0f) * (1.0f / 0.7071067811f);
	auto const val_a =
	    bve::core::math::evaluate_curve(glm::vec3(0), glm::vec3(0, -1, 1), distance, -10);
	auto val_b = bve::core::math::evaluate_curve(val_a.position, val_a.tangent, distance, -10);
	val_b.tangent.y = 1;
	auto const val_c =
	    bve::core::math::evaluate_curve(val_b.position, val_b.tangent, distance, -10);
	auto const val_d =
	    bve::core::math::evaluate_curve(val_c.position, val_c.tangent, distance, -10);

	print_evaluate_curve(val_a);
	print_evaluate_curve(val_b);
	print_evaluate_curve(val_c);
	print_evaluate_curve(val_d);
}
