#include "variant_compare.hpp"
#include <algorithm>
#include <doctest.h>
#include <parsers/function_scripts.hpp>
#include <sstream>

using namespace std::string_literals;
namespace fs_instruction = parsers::function_scripts::instructions;

// This function helps when dealing with pretty printed output. Because the amount of spaces may
// vary, collapsing lengths > 1 down to only one help with making the code easy to test generically.
static std::string canonicalize(std::string str) {
	bool last_char_space = false;
	str.erase(std::remove_if(str.begin(), str.end(),
	                         [&last_char_space](const char c) {
		                         bool remove = last_char_space && c == ' ';
		                         last_char_space = c == ' ';
		                         return remove;
	                         }),
	          str.end());

	return str;
}

#define stringify(a) #a
#define UNARY_OP_TEST(rep, name, type_name)                                                        \
	TEST_CASE("instruction iostream - " stringify(name)) {                                         \
		parsers::function_scripts::instruction_list test_list{{},                                  \
		                                                      {},                                  \
		                                                      {fs_instruction::stack_push{2},      \
		                                                       fs_instruction::type_name{}},       \
		                                                      {}};                                 \
                                                                                                   \
		std::ostringstream output;                                                                 \
                                                                                                   \
		output << test_list;                                                                       \
                                                                                                   \
		CHECK_EQ(canonicalize(output.str()),                                                       \
		         "0\tSTACK_PUSH: 2 #2 -> 0\n"                                                      \
		         "1\t" rep " -> 0\n");                                                             \
	}

#define BINARY_OP_TEST(rep, name, type_name)                                                       \
	TEST_CASE("instruction iostream - " stringify(name)) {                                         \
		parsers::function_scripts::instruction_list test_list{{},                                  \
		                                                      {},                                  \
		                                                      {fs_instruction::stack_push{2},      \
		                                                       fs_instruction::stack_push{3},      \
		                                                       fs_instruction::type_name{}},       \
		                                                      {}};                                 \
                                                                                                   \
		std::ostringstream output;                                                                 \
                                                                                                   \
		output << test_list;                                                                       \
                                                                                                   \
		CHECK_EQ(canonicalize(output.str()),                                                       \
		         "0\tSTACK_PUSH: 2 #2 -> 0\n"                                                      \
		         "1\tSTACK_PUSH: 3 #3 -> 1\n"                                                      \
		         "2\t" rep " -> 0\n");                                                             \
	}

#define REGULAR_VARIABLE_TEST(name)                                                                    \
	TEST_CASE("instruction_iostream - unindexed " stringify(name)) {                                   \
		parsers::function_scripts::instruction_list                                                    \
		    test_list{{parsers::function_scripts::instructions::variable::name},                       \
		              {},                                                                              \
		              {fs_instruction::op_variable_lookup{                                             \
		                  parsers::function_scripts::instructions::variable::name}},                   \
		              {}};                                                                             \
                                                                                                       \
		std::ostringstream output;                                                                     \
                                                                                                       \
		output << test_list;                                                                           \
                                                                                                       \
		CHECK_EQ(canonicalize(output.str()),                                                         \
		         "Variables Used: " stringify(name) "\n"                                             \
                 "0\tOP_VARIABLE_LOOKUP: " stringify(name) " " stringify(name) " -> 0\n"); \
	}

#define INDEXED_VARIABLE_TEST(name)                                                                    \
	TEST_CASE("instruction_iostream - index " stringify(name)) {                                       \
		parsers::function_scripts::instruction_list                                                    \
		    test_list{{},                                                                              \
		              {parsers::function_scripts::instructions::indexed_variable::name},               \
		              {fs_instruction::stack_push{2},                                                  \
		               fs_instruction::op_variable_indexed{                                            \
		                   parsers::function_scripts::instructions::indexed_variable::name}},          \
		              {}};                                                                             \
                                                                                                       \
		std::ostringstream output;                                                                     \
                                                                                                       \
		output << test_list;                                                                           \
                                                                                                       \
		CHECK_EQ(canonicalize(output.str()),                                                         \
		         "Index Variables Used: " stringify(name) "\n"                                       \
		         "0\tSTACK_PUSH: 2 #2 -> 0\n"                                                        \
		         "1\tOP_VARIABLE_INDEXED: " stringify(name) " " stringify(name) "[0] -> 0\n"); \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

// clang-format off
UNARY_OP_TEST("OP_UNARY_NOT (!0)", unary not, op_unary_not)
UNARY_OP_TEST("OP_UNARY_MINUS (-0)", unary minus, op_unary_minus)

BINARY_OP_TEST("OP_ADD2 (0 + 1)", binary add, op_add)
BINARY_OP_TEST("OP_SUBTRACT (0 - 1)",  binary subtract, op_subtract)
BINARY_OP_TEST("OP_MULTIPLY2 (0 * 1)", binary multiply, op_multiply)
BINARY_OP_TEST("OP_DIVIDE (0 / 1)",  binary divide, op_divide)
BINARY_OP_TEST("OP_EQUAL (0 == 1)",  binary equal, op_equal)
BINARY_OP_TEST("OP_UNEQUAL (0 != 1)",  binary unequal, op_unequal)
BINARY_OP_TEST("OP_LESS (0 < 1)",  binary less, op_less)
BINARY_OP_TEST("OP_GREATER (0 > 1)", binary greater, op_greater)
BINARY_OP_TEST("OP_LESS_EQUAL (0 <= 1)", binary lesser or equal, op_less_equal)
BINARY_OP_TEST("OP_GREATER_EQUAL (0 >= 1)", binary greater or equals, op_greater_equal)
BINARY_OP_TEST("OP_AND (0 & 1)",  binary and, op_and)
BINARY_OP_TEST("OP_OR (0 | 1)",  binary or, op_or)
BINARY_OP_TEST("OP_XOR (0 ^ 1)", binary xor, op_xor)

UNARY_OP_TEST("FUNC_RECIPROCAL Reciprocal[0]", reciprocal, func_reciprocal)
BINARY_OP_TEST("FUNC_POWER2 (0 ** 1)", power2, func_power)
BINARY_OP_TEST("FUNC_QUOTIENT (0 // 1)", quotient, func_quotient)
BINARY_OP_TEST("FUNC_MOD (0 % 1)", mod, func_mod)
BINARY_OP_TEST("FUNC_MIN2 Min[0, 1]", min2, func_min)
BINARY_OP_TEST("FUNC_MAX2 Max[0, 1]", max2, func_max)
UNARY_OP_TEST("FUNC_ABS Abs[0]", abs, func_abs)
UNARY_OP_TEST("FUNC_SIGN Sign[0]", sign, func_sign)
UNARY_OP_TEST("FUNC_FLOOR Floor[0]", floor, func_floor)
UNARY_OP_TEST("FUNC_CEILING Ceiling[0]", ceiling, func_ceiling)
UNARY_OP_TEST("FUNC_ROUND Round[0]", round, func_round)
BINARY_OP_TEST("FUNC_RANDOM Random[0, 1]", random, func_random)
BINARY_OP_TEST("FUNC_RANDOMINT RandomInt[0, 1]", random, func_randomInt)
UNARY_OP_TEST("FUNC_EXP Exp[0]", exp, func_exp)
UNARY_OP_TEST("FUNC_LOG Log[0]", log, func_log)
UNARY_OP_TEST("FUNC_SQRT Sqrt[0]", sqrt, func_sqrt)
UNARY_OP_TEST("FUNC_SIN Sin[0]", sin, func_sin)
UNARY_OP_TEST("FUNC_COS Cos[0]", cos, func_cos)
UNARY_OP_TEST("FUNC_TAN Tan[0]", tan, func_tan)
UNARY_OP_TEST("FUNC_ARCTAN Arctan[0]", arctan, func_arctan)

TEST_CASE("instruction iostream - if") {
	
		parsers::function_scripts::instruction_list test_list{ 
			{},
			{},
			{fs_instruction::stack_push{ 2 },
				fs_instruction::stack_push{ 3 },
				fs_instruction::stack_push{ 4 },
				fs_instruction::func_if{}},
			{}
		};
		
		std::ostringstream output;                                                                 
		
		output << test_list;                                                                       
		
		CHECK_EQ(canonicalize(output.str()), 
			"0\tSTACK_PUSH: 2 #2 -> 0\n"                                                      
			"1\tSTACK_PUSH: 3 #3 -> 1\n"                                                      
			"2\tSTACK_PUSH: 4 #4 -> 2\n"                                                      
			"3\tFUNC_IF If[0, 1, 2] -> 0\n");                                                             
}

REGULAR_VARIABLE_TEST(none)
REGULAR_VARIABLE_TEST(value)
REGULAR_VARIABLE_TEST(delta)
REGULAR_VARIABLE_TEST(currentState)
REGULAR_VARIABLE_TEST(time)
REGULAR_VARIABLE_TEST(cameraDistance)
REGULAR_VARIABLE_TEST(cameraMode)
REGULAR_VARIABLE_TEST(cars)
REGULAR_VARIABLE_TEST(speed)
REGULAR_VARIABLE_TEST(speedometer)
REGULAR_VARIABLE_TEST(acceleration)
REGULAR_VARIABLE_TEST(accelerationMotor)
REGULAR_VARIABLE_TEST(distance)
REGULAR_VARIABLE_TEST(trackDistance)
REGULAR_VARIABLE_TEST(mainReservoir)
REGULAR_VARIABLE_TEST(emergencyReservoir)
REGULAR_VARIABLE_TEST(brakePipe)
REGULAR_VARIABLE_TEST(brakeCylinder)
REGULAR_VARIABLE_TEST(straightAirPipe)
REGULAR_VARIABLE_TEST(doors)
REGULAR_VARIABLE_TEST(leftDoors)
REGULAR_VARIABLE_TEST(rightDoors)
REGULAR_VARIABLE_TEST(leftDoorsTarget)
REGULAR_VARIABLE_TEST(rightDoorsTarget)
REGULAR_VARIABLE_TEST(leftDoorsButton)
REGULAR_VARIABLE_TEST(rightDoorsButton)
REGULAR_VARIABLE_TEST(reverserNotch)
REGULAR_VARIABLE_TEST(powerNotch)
REGULAR_VARIABLE_TEST(powerNotches)
REGULAR_VARIABLE_TEST(brakeNotch)
REGULAR_VARIABLE_TEST(brakeNotches)
REGULAR_VARIABLE_TEST(brakeNotchLinear)
REGULAR_VARIABLE_TEST(brakeNotchesLinear)
REGULAR_VARIABLE_TEST(emergencyBrake)
REGULAR_VARIABLE_TEST(hasAirBrake)
REGULAR_VARIABLE_TEST(holdBrake)
REGULAR_VARIABLE_TEST(hasHoldBrake)
REGULAR_VARIABLE_TEST(constSpeed)
REGULAR_VARIABLE_TEST(hasConstSpeed)
REGULAR_VARIABLE_TEST(hasPlugin)
REGULAR_VARIABLE_TEST(Odometer)
REGULAR_VARIABLE_TEST(Klaxon)
REGULAR_VARIABLE_TEST(PrimaryKlaxon)
REGULAR_VARIABLE_TEST(SecondaryKlaxon)
REGULAR_VARIABLE_TEST(MusicKlaxon)
REGULAR_VARIABLE_TEST(section)

INDEXED_VARIABLE_TEST(none)
INDEXED_VARIABLE_TEST(speed)
INDEXED_VARIABLE_TEST(speedometer)
INDEXED_VARIABLE_TEST(acceleration)
INDEXED_VARIABLE_TEST(accelerationMotor)
INDEXED_VARIABLE_TEST(distance)
INDEXED_VARIABLE_TEST(trackDistance)
INDEXED_VARIABLE_TEST(mainReservoir)
INDEXED_VARIABLE_TEST(emergencyReservoir)
INDEXED_VARIABLE_TEST(brakePipe)
INDEXED_VARIABLE_TEST(brakeCylinder)
INDEXED_VARIABLE_TEST(straightAirPipe)
INDEXED_VARIABLE_TEST(doors)
INDEXED_VARIABLE_TEST(leftDoors)
INDEXED_VARIABLE_TEST(rightDoors)
INDEXED_VARIABLE_TEST(leftDoorsTarget)
INDEXED_VARIABLE_TEST(rightDoorsTarget)
INDEXED_VARIABLE_TEST(pluginState)
INDEXED_VARIABLE_TEST(FrontAxleCurveRadius)
INDEXED_VARIABLE_TEST(RearAxleCurveRadius)
INDEXED_VARIABLE_TEST(CurveCant)
INDEXED_VARIABLE_TEST(Odometer)

//clang-format on

TEST_SUITE_END();

