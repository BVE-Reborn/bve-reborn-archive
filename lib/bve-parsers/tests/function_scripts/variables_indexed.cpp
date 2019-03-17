#include "util/macro_helpers.hpp"
#include <doctest/doctest.h>
#include <parsers/function_scripts.hpp>
#include <util/testing/variant_macros.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

#define CREATE_INDEXED_VARIABLE_TEST_CASE(script_name, variable_name)                       \
	TEST_CASE("libparsers - function scripts - indexed variables - " STR(variable_name)) {  \
		auto variable = fs_inst::OPVariableIndexed{};                                       \
		variable.name = fs_inst::IndexedVariable::variable_name;                            \
		auto result = bve::parsers::function_scripts::parse(script_name "[2]");             \
                                                                                            \
		REQUIRE_EQ(result.instructions.size(), 2);                                          \
		REQUIRE_EQ(result.used_indexed_variables.size(), 1);                                \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value); \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], variable, name);               \
		CHECK(result.used_indexed_variables.count(variable.name));                          \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

CREATE_INDEXED_VARIABLE_TEST_CASE("speed", speed)
CREATE_INDEXED_VARIABLE_TEST_CASE("speedometer", speedometer)
CREATE_INDEXED_VARIABLE_TEST_CASE("acceleration", acceleration)
CREATE_INDEXED_VARIABLE_TEST_CASE("accelerationMotor", acceleration_motor)
CREATE_INDEXED_VARIABLE_TEST_CASE("distance", distance)
CREATE_INDEXED_VARIABLE_TEST_CASE("trackDistance", track_distance)
CREATE_INDEXED_VARIABLE_TEST_CASE("mainReservoir", main_reservoir)
CREATE_INDEXED_VARIABLE_TEST_CASE("emergencyReservoir", emergency_reservoir)
CREATE_INDEXED_VARIABLE_TEST_CASE("brakePipe", brake_pipe)
CREATE_INDEXED_VARIABLE_TEST_CASE("brakeCylinder", brake_cylinder)
CREATE_INDEXED_VARIABLE_TEST_CASE("straightAirPipe", straight_air_pipe)
CREATE_INDEXED_VARIABLE_TEST_CASE("doors", doors)
CREATE_INDEXED_VARIABLE_TEST_CASE("leftDoors", left_doors)
CREATE_INDEXED_VARIABLE_TEST_CASE("rightDoors", right_doors)
CREATE_INDEXED_VARIABLE_TEST_CASE("leftDoorsTarget", left_doors_target)
CREATE_INDEXED_VARIABLE_TEST_CASE("rightDoorsTarget", right_doors_target)
CREATE_INDEXED_VARIABLE_TEST_CASE("pluginState", plugin_state)
CREATE_INDEXED_VARIABLE_TEST_CASE("frontAxleCurveRadius", front_axle_curve_radius)
CREATE_INDEXED_VARIABLE_TEST_CASE("rearAxleCurveRadius", rear_axle_curve_radius)
CREATE_INDEXED_VARIABLE_TEST_CASE("curveCant", curve_cant)
CREATE_INDEXED_VARIABLE_TEST_CASE("odometer", odometer)

TEST_SUITE_END();
