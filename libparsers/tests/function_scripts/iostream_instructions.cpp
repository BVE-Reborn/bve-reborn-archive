#include "variant_compare.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>
#include <sstream>

using namespace std::string_literals;
namespace fs_instruction = parsers::function_scripts::instructions;

#define stringify(a) #a
#define UNARY_OP_TEST(rep, padding, name, type_name)                                               \
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
		CHECK_EQ(output.str(), "0\tSTACK_PUSH: 2     " padding                                     \
		                       "#2 -> 0\n"                                                         \
		                       "1\t" rep "0\n");                                                   \
	}

#define BINARY_OP_TEST(rep, padding, name, type_name)                                              \
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
		CHECK_EQ(output.str(), "0\tSTACK_PUSH: 2        " padding                                  \
		                       "#2 -> 0\n"                                                         \
		                       "1\tSTACK_PUSH: 3        " padding                                  \
		                       "#3 -> 1\n"                                                         \
		                       "2\t" rep "0\n");                                                   \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

// clang-format off
UNARY_OP_TEST("OP_UNARY_NOT    (!0) -> ", "", unary not, op_unary_not);
UNARY_OP_TEST("OP_UNARY_MINUS   (-0) -> ", " ", unary minus, op_unary_minus);
BINARY_OP_TEST("OP_ADD2         (0 + 1) -> ", "", binary add, op_add);
BINARY_OP_TEST("OP_SUBTRACT     (0 - 1) -> ", "", binary subtract, op_subtract);
BINARY_OP_TEST("OP_MULTIPLY2    (0 * 1) -> ", "", binary multiply, op_multiply);
BINARY_OP_TEST("OP_DIVIDE       (0 / 1) -> ", "", binary divide, op_divide);
BINARY_OP_TEST("OP_EQUAL        (0 == 1) -> ", " ", binary equal, op_equal);
BINARY_OP_TEST("OP_UNEQUAL      (0 != 1) -> ", " ", binary unequal, op_unequal);
BINARY_OP_TEST("OP_LESS         (0 < 1) -> ", "", binary less, op_less);
BINARY_OP_TEST("OP_GREATER      (0 > 1) -> ", "", binary greater, op_greater);
BINARY_OP_TEST("OP_LESS_EQUAL   (0 <= 1) -> ", " ", binary lesser or equal, op_less_equal);
BINARY_OP_TEST("OP_GREATER_EQUAL   (0 >= 1) -> ", "    ", binary greater or equals, op_greater_equal);
BINARY_OP_TEST("OP_AND          (0 & 1) -> ", "", binary and, op_and);
BINARY_OP_TEST("OP_OR           (0 | 1) -> ", "", binary or, op_or);
BINARY_OP_TEST("OP_XOR          (0 ^ 1) -> ", "", binary xor, op_xor);
//clang-format on

TEST_SUITE_END();

