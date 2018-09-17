#include "core/macro_helpers.hpp"
#include "test_macro_util.hpp"
#include <algorithm>
#include <doctest.h>
#include <parsers/function_scripts.hpp>
#include <sstream>

using namespace std::string_literals;
namespace fs_instruction = parsers::function_scripts::instructions;

// This function helps when dealing with pretty printed output. Because the amount of spaces may
// vary, collapsing lengths > 1 down to only one help with making the code easy to test generically.
static std::string canonicalize(std::string str) {
	auto last_char_space = false;
	str.erase(std::remove_if(str.begin(), str.end(),
	                         [&last_char_space](const char c) {
		                         auto const remove = last_char_space && c == ' ';
		                         last_char_space = c == ' ';
		                         return remove;
	                         }),
	          str.end());

	return str;
}

#define UNARY_OP_TEST(rep, name, type_name)                                                        \
	TEST_CASE("libparsers - function scripts - instruction_iostream - " STR(name)) {               \
		parsers::function_scripts::instruction_list const                                          \
		    test_list{{}, {}, {fs_instruction::stack_push{2}, fs_instruction::type_name{}}, {}};   \
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
	TEST_CASE("libparsers - function scripts - instruction_iostream - " STR(name)) {               \
		parsers::function_scripts::instruction_list const                                          \
		    test_list{{},                                                                          \
		              {},                                                                          \
		              {fs_instruction::stack_push{2}, fs_instruction::stack_push{3},               \
		               fs_instruction::type_name{}},                                               \
		              {}};                                                                         \
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

#define REGULAR_VARIABLE_TEST(name)                                                                     \
	TEST_CASE("libparsers - function scripts - instruction_iostream - unindexed " STR(name)) {          \
		parsers::function_scripts::instruction_list const                                               \
		    test_list{{parsers::function_scripts::instructions::variable::name},                        \
		              {},                                                                               \
		              {fs_instruction::op_variable_lookup{                                              \
		                  parsers::function_scripts::instructions::variable::name}},                    \
		              {}};                                                                              \
                                                                                                        \
		std::ostringstream output;                                                                      \
                                                                                                        \
		output << test_list;                                                                            \
                                                                                                        \
		CHECK_EQ(canonicalize(output.str()),                                                         \
		         "Variables Used: " STR(name) "\n"                                             \
                 "0\tOP_VARIABLE_LOOKUP: " STR(name) " " STR(name) " -> 0\n"); \
	}

#define INDEXED_VARIABLE_TEST(name)                                                                     \
	TEST_CASE("libparsers - function scripts - instruction_iostream - index " STR(name)) {              \
		parsers::function_scripts::instruction_list const                                               \
		    test_list{{},                                                                               \
		              {parsers::function_scripts::instructions::indexed_variable::name},                \
		              {fs_instruction::stack_push{2},                                                   \
		               fs_instruction::op_variable_indexed{                                             \
		                   parsers::function_scripts::instructions::indexed_variable::name}},           \
		              {}};                                                                              \
                                                                                                        \
		std::ostringstream output;                                                                      \
                                                                                                        \
		output << test_list;                                                                            \
                                                                                                        \
		CHECK_EQ(canonicalize(output.str()),                                                         \
		         "Index Variables Used: " STR(name) "\n"                                       \
		         "0\tSTACK_PUSH: 2 #2 -> 0\n"                                                        \
		         "1\tOP_VARIABLE_INDEXED: " STR(name) " " STR(name) "[0] -> 0\n"); \
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
BINARY_OP_TEST("FUNC_RANDOMINT RandomInt[0, 1]", random, func_random_int)
UNARY_OP_TEST("FUNC_EXP Exp[0]", exp, func_exp)
UNARY_OP_TEST("FUNC_LOG Log[0]", log, func_log)
UNARY_OP_TEST("FUNC_SQRT Sqrt[0]", sqrt, func_sqrt)
UNARY_OP_TEST("FUNC_SIN Sin[0]", sin, func_sin)
UNARY_OP_TEST("FUNC_COS Cos[0]", cos, func_cos)
UNARY_OP_TEST("FUNC_TAN Tan[0]", tan, func_tan)
UNARY_OP_TEST("FUNC_ARCTAN Arctan[0]", arctan, func_arctan)

TEST_CASE("libparsers - function scripts - instruction_iostream - if") {
	
	parsers::function_scripts::instruction_list const test_list{
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
REGULAR_VARIABLE_TEST(current_state)
REGULAR_VARIABLE_TEST(time)
REGULAR_VARIABLE_TEST(camera_distance)
REGULAR_VARIABLE_TEST(camera_mode)
REGULAR_VARIABLE_TEST(cars)
REGULAR_VARIABLE_TEST(speed)
REGULAR_VARIABLE_TEST(speedometer)
REGULAR_VARIABLE_TEST(acceleration)
REGULAR_VARIABLE_TEST(acceleration_motor)
REGULAR_VARIABLE_TEST(distance)
REGULAR_VARIABLE_TEST(track_distance)
REGULAR_VARIABLE_TEST(main_reservoir)
REGULAR_VARIABLE_TEST(emergency_reservoir)
REGULAR_VARIABLE_TEST(brake_pipe)
REGULAR_VARIABLE_TEST(brake_cylinder)
REGULAR_VARIABLE_TEST(straight_air_pipe)
REGULAR_VARIABLE_TEST(doors)
REGULAR_VARIABLE_TEST(left_doors)
REGULAR_VARIABLE_TEST(right_doors)
REGULAR_VARIABLE_TEST(left_doors_target)
REGULAR_VARIABLE_TEST(right_doors_target)
REGULAR_VARIABLE_TEST(left_doors_button)
REGULAR_VARIABLE_TEST(right_doors_button)
REGULAR_VARIABLE_TEST(reverser_notch)
REGULAR_VARIABLE_TEST(power_notch)
REGULAR_VARIABLE_TEST(power_notches)
REGULAR_VARIABLE_TEST(brake_notch)
REGULAR_VARIABLE_TEST(brake_notches)
REGULAR_VARIABLE_TEST(brake_notch_linear)
REGULAR_VARIABLE_TEST(brake_notches_linear)
REGULAR_VARIABLE_TEST(emergency_brake)
REGULAR_VARIABLE_TEST(has_air_brake)
REGULAR_VARIABLE_TEST(hold_brake)
REGULAR_VARIABLE_TEST(has_hold_brake)
REGULAR_VARIABLE_TEST(const_speed)
REGULAR_VARIABLE_TEST(has_const_speed)
REGULAR_VARIABLE_TEST(has_plugin)
REGULAR_VARIABLE_TEST(odometer)
REGULAR_VARIABLE_TEST(klaxon)
REGULAR_VARIABLE_TEST(primary_klaxon)
REGULAR_VARIABLE_TEST(secondary_klaxon)
REGULAR_VARIABLE_TEST(music_klaxon)
REGULAR_VARIABLE_TEST(section)

INDEXED_VARIABLE_TEST(none)
INDEXED_VARIABLE_TEST(speed)
INDEXED_VARIABLE_TEST(speedometer)
INDEXED_VARIABLE_TEST(acceleration)
INDEXED_VARIABLE_TEST(acceleration_motor)
INDEXED_VARIABLE_TEST(distance)
INDEXED_VARIABLE_TEST(track_distance)
INDEXED_VARIABLE_TEST(main_reservoir)
INDEXED_VARIABLE_TEST(emergency_reservoir)
INDEXED_VARIABLE_TEST(brake_pipe)
INDEXED_VARIABLE_TEST(brake_cylinder)
INDEXED_VARIABLE_TEST(straight_air_pipe)
INDEXED_VARIABLE_TEST(doors)
INDEXED_VARIABLE_TEST(left_doors)
INDEXED_VARIABLE_TEST(right_doors)
INDEXED_VARIABLE_TEST(left_doors_target)
INDEXED_VARIABLE_TEST(right_doors_target)
INDEXED_VARIABLE_TEST(plugin_state)
INDEXED_VARIABLE_TEST(front_axle_curve_radius)
INDEXED_VARIABLE_TEST(rear_axle_curve_radius)
INDEXED_VARIABLE_TEST(curve_cant)
INDEXED_VARIABLE_TEST(odometer)

//clang-format on

TEST_SUITE_END();




