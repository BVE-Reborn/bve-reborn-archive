#include "variant_compare.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

#define str(a) #a
#define CREATE_INDEXED_VARIABLE_TEST_CASE(variable_name)                                           \
	TEST_CASE("indexed variables - " str(variable_name)) {                                         \
		auto variable = fs_inst::op_variable_indexed{};                                            \
		variable.name = fs_inst::indexed_variable::variable_name;                                  \
		auto result = parsers::function_scripts::parse(str(variable_name) "[2]");                  \
                                                                                                   \
		REQUIRE_EQ(result.instructions.size(), 2);                                                 \
		REQUIRE_EQ(result.used_indexed_variables.size(), 1);                                       \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);       \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], variable, name);                      \
		CHECK(result.used_indexed_variables.count(variable.name));                                 \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

CREATE_INDEXED_VARIABLE_TEST_CASE(speed);
CREATE_INDEXED_VARIABLE_TEST_CASE(speedometer);
CREATE_INDEXED_VARIABLE_TEST_CASE(acceleration);
CREATE_INDEXED_VARIABLE_TEST_CASE(accelerationMotor);
CREATE_INDEXED_VARIABLE_TEST_CASE(distance);
CREATE_INDEXED_VARIABLE_TEST_CASE(trackDistance);
CREATE_INDEXED_VARIABLE_TEST_CASE(mainReservoir);
CREATE_INDEXED_VARIABLE_TEST_CASE(emergencyReservoir);
CREATE_INDEXED_VARIABLE_TEST_CASE(brakePipe);
CREATE_INDEXED_VARIABLE_TEST_CASE(brakeCylinder);
CREATE_INDEXED_VARIABLE_TEST_CASE(straightAirPipe);
CREATE_INDEXED_VARIABLE_TEST_CASE(doors);
CREATE_INDEXED_VARIABLE_TEST_CASE(leftDoors);
CREATE_INDEXED_VARIABLE_TEST_CASE(rightDoors);
CREATE_INDEXED_VARIABLE_TEST_CASE(leftDoorsTarget);
CREATE_INDEXED_VARIABLE_TEST_CASE(rightDoorsTarget);
CREATE_INDEXED_VARIABLE_TEST_CASE(pluginState);
CREATE_INDEXED_VARIABLE_TEST_CASE(FrontAxleCurveRadius);
CREATE_INDEXED_VARIABLE_TEST_CASE(RearAxleCurveRadius);
CREATE_INDEXED_VARIABLE_TEST_CASE(CurveCant);
CREATE_INDEXED_VARIABLE_TEST_CASE(Odometer);

TEST_SUITE_END();
