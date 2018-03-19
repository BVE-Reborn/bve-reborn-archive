#include "variant_compare.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

#define str(a) #a
#define CREATE_OOO_DIFFERENT_TEST_CASE(name1, name2, instr_name1, instr_name2, symbol1, symbol2)   \
	TEST_CASE("order-of-operations - " str(name1) " > " str(name2)) {                              \
		auto function1 = fs_inst::instr_name1{};                                                   \
		auto function2 = fs_inst::instr_name2{};                                                   \
		auto result = parsers::function_scripts::parse("2 " str(symbol1) " 3 " str(symbol2) " 4"); \
                                                                                                   \
		REQUIRE_EQ(result.instructions.size(), 5);                                                 \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);       \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);       \
		COMPARE_VARIANT_NODES(result.instructions[2], function1);                                  \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], fs_inst::stack_push{4}, value);       \
		COMPARE_VARIANT_NODES(result.instructions[4], function2);                                  \
	}

#define CREATE_OOO_SAME_TEST_CASE(name1, name2, instr_name1, instr_name2, symbol1, symbol2)        \
	TEST_CASE("order-of-operations - " str(name1) " = " str(name2)) {                              \
		auto front_function1 = fs_inst::instr_name1{};                                             \
		auto front_function2 = fs_inst::instr_name2{};                                             \
		auto front_result =                                                                        \
		    parsers::function_scripts::parse("2 " str(symbol1) " 3 " str(symbol2) " 4");           \
                                                                                                   \
		REQUIRE_EQ(front_result.instructions.size(), 5);                                           \
		COMPARE_VARIANT_NODES_MEMBER(front_result.instructions[0], fs_inst::stack_push{2}, value); \
		COMPARE_VARIANT_NODES_MEMBER(front_result.instructions[1], fs_inst::stack_push{3}, value); \
		COMPARE_VARIANT_NODES_MEMBER(front_result.instructions[2], fs_inst::stack_push{4}, value); \
		COMPARE_VARIANT_NODES(front_result.instructions[3], front_function2);                      \
		COMPARE_VARIANT_NODES(front_result.instructions[4], front_function1);                      \
                                                                                                   \
		auto back_function1 = fs_inst::instr_name2{};                                              \
		auto back_function2 = fs_inst::instr_name1{};                                              \
		auto back_result =                                                                         \
		    parsers::function_scripts::parse("2 " str(symbol2) " 3 " str(symbol1) " 4");           \
                                                                                                   \
		REQUIRE_EQ(back_result.instructions.size(), 5);                                            \
		COMPARE_VARIANT_NODES_MEMBER(back_result.instructions[0], fs_inst::stack_push{2}, value);  \
		COMPARE_VARIANT_NODES_MEMBER(back_result.instructions[1], fs_inst::stack_push{3}, value);  \
		COMPARE_VARIANT_NODES_MEMBER(back_result.instructions[2], fs_inst::stack_push{4}, value);  \
		COMPARE_VARIANT_NODES(back_result.instructions[3], back_function2);                        \
		COMPARE_VARIANT_NODES(back_result.instructions[4], back_function1);                        \
	}

#define UNARY_NOT_TEST(name, instr_name, symbol)                                                   \
	TEST_CASE("order-of-operations - " str(name) " > unary not") {                                 \
		auto function1 = fs_inst::op_unary_not{};                                                  \
		auto function2 = fs_inst::instr_name{};                                                    \
		auto result = parsers::function_scripts::parse("!2 " str(symbol) " 3");                    \
                                                                                                   \
		REQUIRE_EQ(result.instructions.size(), 4);                                                 \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);       \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);       \
		COMPARE_VARIANT_NODES(result.instructions[2], function2);                                  \
		COMPARE_VARIANT_NODES(result.instructions[3], function1);                                  \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

// ^
CREATE_OOO_DIFFERENT_TEST_CASE(binary xor, binary or, op_xor, op_or, ^, |)
// &
CREATE_OOO_DIFFERENT_TEST_CASE(binary and, binary xor, op_and, op_xor, &, ^)

// !
TEST_CASE("order-of-operations - unary not > binary and") {
	auto function1 = fs_inst::op_unary_not{};
	auto function2 = fs_inst::op_and{};
	auto result = parsers::function_scripts::parse("!2 & 3");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], function1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], function2);
}
UNARY_NOT_TEST(equals, op_equal, ==)
UNARY_NOT_TEST(not equals, op_unequal, !=)
UNARY_NOT_TEST(less, op_less, <)
UNARY_NOT_TEST(greater, op_greater, >)
UNARY_NOT_TEST(lesser or equal, op_less_equal, <=)
UNARY_NOT_TEST(greater or equal, op_greater_equal, >=)

// ==
CREATE_OOO_SAME_TEST_CASE(equals, not equals, op_equal, op_unequal, ==, !=)
CREATE_OOO_SAME_TEST_CASE(equals, less, op_equal, op_less, ==, <)
CREATE_OOO_SAME_TEST_CASE(equals, greater, op_equal, op_greater, ==, >)
CREATE_OOO_SAME_TEST_CASE(equals, lesser or equals, op_equal, op_less_equal, ==, <=)
CREATE_OOO_SAME_TEST_CASE(equals, greater or equals, op_equal, op_greater_equal, ==, >=)

// !=
CREATE_OOO_SAME_TEST_CASE(not equals, less, op_unequal, op_less, !=, <)
CREATE_OOO_SAME_TEST_CASE(not equals, greater, op_unequal, op_greater, !=, >)
CREATE_OOO_SAME_TEST_CASE(not equals, lesser or equals, op_unequal, op_less_equal, !=, <=)
CREATE_OOO_SAME_TEST_CASE(not equals, greater or equals, op_unequal, op_greater_equal, !=, >=)

// <
CREATE_OOO_SAME_TEST_CASE(less, greater, op_less, op_greater, <, >)
CREATE_OOO_SAME_TEST_CASE(less, lesser or equals, op_less, op_less_equal, <, <=)
CREATE_OOO_SAME_TEST_CASE(less, greater or equals, op_less, op_greater_equal, <, >=)

// >
CREATE_OOO_SAME_TEST_CASE(greater, lesser or equals, op_greater, op_less_equal, >, <=)
CREATE_OOO_SAME_TEST_CASE(greater, greater or equals, op_greater, op_greater_equal, >, >=)

// <=
CREATE_OOO_SAME_TEST_CASE(lesser or equals,
                          greater or equals,
                          op_less_equal,
                          op_greater_equal,
                          <=,
                          >=)

// +
CREATE_OOO_DIFFERENT_TEST_CASE(add, equals, op_add, op_equal, +, ==)
CREATE_OOO_DIFFERENT_TEST_CASE(add, not equals, op_add, op_unequal, +, !=)
CREATE_OOO_DIFFERENT_TEST_CASE(add, less, op_add, op_less, +, <)
CREATE_OOO_DIFFERENT_TEST_CASE(add, greater, op_add, op_greater, +, >)
CREATE_OOO_DIFFERENT_TEST_CASE(add, lesser or equals, op_add, op_less_equal, +, <=)
CREATE_OOO_DIFFERENT_TEST_CASE(add, greater or equals, op_add, op_greater_equal, +, >=)
// -
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, equals, op_subtract, op_equal, -, ==)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, not equals, op_subtract, op_unequal, -, !=)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, less, op_subtract, op_less, -, <)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, greater, op_subtract, op_greater, -, >)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, lesser or equals, op_subtract, op_less_equal, -, <=)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, greater or equals, op_subtract, op_greater_equal, -, >=)

CREATE_OOO_SAME_TEST_CASE(add, subtract, op_add, op_subtract, +, -)

// *
CREATE_OOO_DIFFERENT_TEST_CASE(multiply, add, op_multiply, op_add, *, +)
CREATE_OOO_DIFFERENT_TEST_CASE(multiply, subtract, op_multiply, op_subtract, *, -)

// /
CREATE_OOO_DIFFERENT_TEST_CASE(divide, multiply, op_divide, op_multiply, /, *)

// - (unary)
TEST_CASE("order-of-operations - unary minus > divide") {
	auto function1 = fs_inst::op_unary_minus{};
	auto function2 = fs_inst::op_divide{};
	auto result = parsers::function_scripts::parse("- 2 / 3");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], function1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], function2);
}

// a[]
TEST_CASE("order-of-operations - function > unary minus") {
	auto function1 = fs_inst::op_unary_minus{};
	auto function2 = fs_inst::func_sin{};
	auto result = parsers::function_scripts::parse("sin[- 2]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], function1);
	COMPARE_VARIANT_NODES(result.instructions[2], function2);
}

TEST_SUITE_END();
