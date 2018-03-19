#include "variant_compare.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

#define str(a) #a
#define CREATE_VARIABLE_TEST_CASE(variable_name)                                                   \
	TEST_CASE("variables - " str(variable_name)) {                                                 \
		auto variable = fs_inst::op_variable_lookup{};                                             \
		variable.name = fs_inst::variable::variable_name;                                          \
		auto result = parsers::function_scripts::parse(str(variable_name));                        \
                                                                                                   \
		REQUIRE_EQ(result.instructions.size(), 1);                                                 \
		REQUIRE_EQ(result.used_variables.size(), 1);                                               \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], variable, name);                      \
		CHECK(result.used_variables.count(variable.name));                                         \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

CREATE_VARIABLE_TEST_CASE(value);
CREATE_VARIABLE_TEST_CASE(delta);
CREATE_VARIABLE_TEST_CASE(currentState);
CREATE_VARIABLE_TEST_CASE(time);
CREATE_VARIABLE_TEST_CASE(cameraDistance);
CREATE_VARIABLE_TEST_CASE(cameraMode);
CREATE_VARIABLE_TEST_CASE(cars);
CREATE_VARIABLE_TEST_CASE(speed);
CREATE_VARIABLE_TEST_CASE(speedometer);
CREATE_VARIABLE_TEST_CASE(acceleration);
CREATE_VARIABLE_TEST_CASE(accelerationMotor);
CREATE_VARIABLE_TEST_CASE(distance);
CREATE_VARIABLE_TEST_CASE(trackDistance);
CREATE_VARIABLE_TEST_CASE(mainReservoir);
CREATE_VARIABLE_TEST_CASE(emergencyReservoir);
CREATE_VARIABLE_TEST_CASE(brakeCylinder);
CREATE_VARIABLE_TEST_CASE(straightAirPipe);
CREATE_VARIABLE_TEST_CASE(doors);
CREATE_VARIABLE_TEST_CASE(leftDoors);
CREATE_VARIABLE_TEST_CASE(rightDoors);
CREATE_VARIABLE_TEST_CASE(leftDoorsTarget);
CREATE_VARIABLE_TEST_CASE(rightDoorsTarget);
CREATE_VARIABLE_TEST_CASE(leftDoorsButton);
CREATE_VARIABLE_TEST_CASE(rightDoorsButton);
CREATE_VARIABLE_TEST_CASE(reverserNotch);
CREATE_VARIABLE_TEST_CASE(powerNotch);
CREATE_VARIABLE_TEST_CASE(powerNotches);
CREATE_VARIABLE_TEST_CASE(brakeNotch);
CREATE_VARIABLE_TEST_CASE(brakeNotches);
CREATE_VARIABLE_TEST_CASE(brakeNotchLinear);
CREATE_VARIABLE_TEST_CASE(brakeNotchesLinear);
CREATE_VARIABLE_TEST_CASE(emergencyBrake);
CREATE_VARIABLE_TEST_CASE(hasAirBrake);
CREATE_VARIABLE_TEST_CASE(holdBrake);
CREATE_VARIABLE_TEST_CASE(hasHoldBrake);
CREATE_VARIABLE_TEST_CASE(constSpeed);
CREATE_VARIABLE_TEST_CASE(hasConstSpeed);
CREATE_VARIABLE_TEST_CASE(hasPlugin);
CREATE_VARIABLE_TEST_CASE(Odometer);
CREATE_VARIABLE_TEST_CASE(Klaxon);
CREATE_VARIABLE_TEST_CASE(PrimaryKlaxon);
CREATE_VARIABLE_TEST_CASE(SecondaryKlaxon);
CREATE_VARIABLE_TEST_CASE(MusicKlaxon);
CREATE_VARIABLE_TEST_CASE(section);

TEST_SUITE_END();
