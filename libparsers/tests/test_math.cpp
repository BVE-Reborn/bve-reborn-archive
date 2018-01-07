#include "core/math.hpp"
#include <glm/vec3.hpp>

void test_math() {
	float distance = 12.56637f * (5.0f / 4.0f) * (1 / 0.707f) * 4;
	auto vala = openbve2::math::evaluate_curve(glm::vec3(100, 0, 0), glm::vec3(0, -1, 1), distance, 10);
	// auto valb = openbve2::math::evaluate_curve(vala.position, vala.tangent, distance, 10);
	vala.tangent.y = 1;
	// auto valc = openbve2::math::evaluate_curve(valb.position, valb.tangent, distance, 10);
	auto vald = openbve2::math::evaluate_curve(vala.position, vala.tangent, distance, 10);
	(void) vald;
}
