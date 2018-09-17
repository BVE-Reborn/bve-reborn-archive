#include "core/math.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void print_evaluate_curve(const core::math::evaulate_curve_t& val) {
	std::cout << "Position: " << to_string(val.position) << ", Tangent: " << to_string(val.tangent)
	          << '\n';
}

void test_math() {
	auto const distance = 12.56637f * (5.0f / 4.0f) * (1.0f / 0.7071067811f);
	auto const vala = core::math::evaluate_curve(glm::vec3(0), glm::vec3(0, -1, 1), distance, -10);
	auto valb = core::math::evaluate_curve(vala.position, vala.tangent, distance, -10);
	valb.tangent.y = 1;
	auto const valc = core::math::evaluate_curve(valb.position, valb.tangent, distance, -10);
	auto const vald = core::math::evaluate_curve(valc.position, valc.tangent, distance, -10);

	print_evaluate_curve(vala);
	print_evaluate_curve(valb);
	print_evaluate_curve(valc);
	print_evaluate_curve(vald);
}
