#include "core/math.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <iostream>

void print_evaluate_curve(const openbve2::math::evaulate_curve_t& val) {
	std::cout << "Position: " << to_string(val.position) << ", Tangent: " << to_string(val.tangent)
	          << '\n';
}

void test_math() {
	float distance = 12.56637f * (5.0f / 4.0f) * (1 / 0.7071067811f);
	auto vala = openbve2::math::evaluate_curve(glm::vec3(0), glm::vec3(0, -1, 1), distance, -10);
	auto valb = openbve2::math::evaluate_curve(vala.position, vala.tangent, distance, -10);
	valb.tangent.y = 1;
	auto valc = openbve2::math::evaluate_curve(valb.position, valb.tangent, distance, -10);
	auto vald = openbve2::math::evaluate_curve(valc.position, valc.tangent, distance, -10);

	print_evaluate_curve(vala);
	print_evaluate_curve(valb);
	print_evaluate_curve(valc);
	print_evaluate_curve(vald);
}
