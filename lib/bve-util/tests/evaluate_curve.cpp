#include "util/math.hpp"
#include <doctest.h>

TEST_SUITE_BEGIN("libutil - math");

TEST_CASE("libutil - math - evaluate curve") {
	namespace m = bve::util::math;

	auto const distance = 12.56637F * (5.0F / 4.0F) * (1.0F / 0.7071067811F);
	auto const val_a = m::evaluate_curve(glm::vec3(0), glm::vec3(0, -1, 1), distance, -10);
	auto val_b = m::evaluate_curve(val_a.position, val_a.tangent, distance, -10);
	val_b.tangent.y = 1;
	auto const val_c = m::evaluate_curve(val_b.position, val_b.tangent, distance, -10);
	auto const val_d = m::evaluate_curve(val_c.position, val_c.tangent, distance, -10);

	CHECK_EQ(val_d.position.x, doctest::Approx(0).epsilon(0.0001F));
	CHECK_EQ(val_d.position.y, doctest::Approx(0).epsilon(0.0001F));
	CHECK_EQ(val_d.position.z, doctest::Approx(0).epsilon(0.0001F));
	CHECK_EQ(val_d.tangent.x, doctest::Approx(0).epsilon(0.0001F));
	CHECK_EQ(val_d.tangent.y, doctest::Approx(1).epsilon(0.0001F));
	CHECK_EQ(val_d.tangent.z, doctest::Approx(1).epsilon(0.0001F));
}

TEST_SUITE_END();
