#include "core/macro_helpers.hpp"
#include "test_macro_util.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

#define CREATE_VARIABLE_TEST_CASE(script_name, variable_name)                                      \
	TEST_CASE("libparsers - function scripts - variables - " STR(variable_name)) {                 \
		auto variable = fs_inst::op_variable_lookup{};                                             \
		variable.name = fs_inst::variable::variable_name;                                          \
		auto result = bve::parsers::function_scripts::parse(script_name);                          \
                                                                                                   \
		REQUIRE_EQ(result.instructions.size(), 1);                                                 \
		REQUIRE_EQ(result.used_variables.size(), 1);                                               \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], variable, name);                      \
		CHECK(result.used_variables.count(variable.name));                                         \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

CREATE_VARIABLE_TEST_CASE("value", value)
CREATE_VARIABLE_TEST_CASE("delta", delta)
CREATE_VARIABLE_TEST_CASE("currentState", current_state)
CREATE_VARIABLE_TEST_CASE("time", time)
CREATE_VARIABLE_TEST_CASE("cameraDistance", camera_distance)
CREATE_VARIABLE_TEST_CASE("cameraMode", camera_mode)
CREATE_VARIABLE_TEST_CASE("cars", cars)
CREATE_VARIABLE_TEST_CASE("speed", speed)
CREATE_VARIABLE_TEST_CASE("speedometer", speedometer)
CREATE_VARIABLE_TEST_CASE("acceleration", acceleration)
CREATE_VARIABLE_TEST_CASE("accelerationMotor", acceleration_motor)
CREATE_VARIABLE_TEST_CASE("distance", distance)
CREATE_VARIABLE_TEST_CASE("trackDistance", track_distance)
CREATE_VARIABLE_TEST_CASE("mainReservoir", main_reservoir)
CREATE_VARIABLE_TEST_CASE("emergencyReservoir", emergency_reservoir)
CREATE_VARIABLE_TEST_CASE("brakeCylinder", brake_cylinder)
CREATE_VARIABLE_TEST_CASE("straightAirPipe", straight_air_pipe)
CREATE_VARIABLE_TEST_CASE("doors", doors)
CREATE_VARIABLE_TEST_CASE("leftDoors", left_doors)
CREATE_VARIABLE_TEST_CASE("rightDoors", right_doors)
CREATE_VARIABLE_TEST_CASE("leftDoorsTarget", left_doors_target)
CREATE_VARIABLE_TEST_CASE("rightDoorsTarget", right_doors_target)
CREATE_VARIABLE_TEST_CASE("leftDoorsButton", left_doors_button)
CREATE_VARIABLE_TEST_CASE("rightDoorsButton", right_doors_button)
CREATE_VARIABLE_TEST_CASE("reverserNotch", reverser_notch)
CREATE_VARIABLE_TEST_CASE("powerNotch", power_notch)
CREATE_VARIABLE_TEST_CASE("powerNotches", power_notches)
CREATE_VARIABLE_TEST_CASE("brakeNotch", brake_notch)
CREATE_VARIABLE_TEST_CASE("brakeNotches", brake_notches)
CREATE_VARIABLE_TEST_CASE("brakeNotchLinear", brake_notch_linear)
CREATE_VARIABLE_TEST_CASE("brakeNotchesLinear", brake_notches_linear)
CREATE_VARIABLE_TEST_CASE("emergencyBrake", emergency_brake)
CREATE_VARIABLE_TEST_CASE("hasAirBrake", has_air_brake)
CREATE_VARIABLE_TEST_CASE("holdBrake", hold_brake)
CREATE_VARIABLE_TEST_CASE("hasHoldBrake", has_hold_brake)
CREATE_VARIABLE_TEST_CASE("constSpeed", const_speed)
CREATE_VARIABLE_TEST_CASE("hasConstSpeed", has_const_speed)
CREATE_VARIABLE_TEST_CASE("hasPlugin", has_plugin)
CREATE_VARIABLE_TEST_CASE("odometer", odometer)
CREATE_VARIABLE_TEST_CASE("klaxon", klaxon)
CREATE_VARIABLE_TEST_CASE("primaryKlaxon", primary_klaxon)
CREATE_VARIABLE_TEST_CASE("secondaryKlaxon", secondary_klaxon)
CREATE_VARIABLE_TEST_CASE("musicKlaxon", music_klaxon)
CREATE_VARIABLE_TEST_CASE("section", section)

TEST_SUITE_END();
