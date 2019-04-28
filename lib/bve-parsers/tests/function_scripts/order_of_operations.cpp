#include "util/macro_helpers.hpp"
#include <doctest/doctest.h>
#include <ostream>
#include <parsers/function_scripts.hpp>
#include <util/testing/variant_macros.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

#define CREATE_OOO_DIFFERENT_TEST_CASE(name1, name2, instr_name1, instr_name2, symbol1, symbol2)        \
	TEST_CASE("libparsers - function scripts - order-of-operations - " STR(name1) " > " STR(name2)) {   \
		auto function1 = fs_inst::instr_name1{};                                                        \
		auto function2 = fs_inst::instr_name2{};                                                        \
		auto result = bve::parsers::function_scripts::parse("2 " STR(symbol1) " 3 " STR(symbol2) " 4"); \
                                                                                                        \
		REQUIRE_EQ(result.instructions.size(), 5);                                                      \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);             \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);             \
		COMPARE_VARIANT_NODES(result.instructions[2], function1);                                       \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], fs_inst::StackPush{4}, value);             \
		COMPARE_VARIANT_NODES(result.instructions[4], function2);                                       \
	}

#define CREATE_OOO_SAME_TEST_CASE(name1, name2, instr_name1, instr_name2, symbol1, symbol2)                   \
	TEST_CASE("libparsers - function scripts - order-of-operations - " STR(name1) " = " STR(name2)) {         \
		auto front_function1 = fs_inst::instr_name1{};                                                        \
		auto front_function2 = fs_inst::instr_name2{};                                                        \
		auto front_result = bve::parsers::function_scripts::parse("2 " STR(symbol1) " 3 " STR(symbol2) " 4"); \
                                                                                                              \
		REQUIRE_EQ(front_result.instructions.size(), 5);                                                      \
		COMPARE_VARIANT_NODES_MEMBER(front_result.instructions[0], fs_inst::StackPush{2}, value);             \
		COMPARE_VARIANT_NODES_MEMBER(front_result.instructions[1], fs_inst::StackPush{3}, value);             \
		COMPARE_VARIANT_NODES_MEMBER(front_result.instructions[2], fs_inst::StackPush{4}, value);             \
		COMPARE_VARIANT_NODES(front_result.instructions[3], front_function2);                                 \
		COMPARE_VARIANT_NODES(front_result.instructions[4], front_function1);                                 \
                                                                                                              \
		auto back_function1 = fs_inst::instr_name2{};                                                         \
		auto back_function2 = fs_inst::instr_name1{};                                                         \
		auto back_result = bve::parsers::function_scripts::parse("2 " STR(symbol2) " 3 " STR(symbol1) " 4");  \
                                                                                                              \
		REQUIRE_EQ(back_result.instructions.size(), 5);                                                       \
		COMPARE_VARIANT_NODES_MEMBER(back_result.instructions[0], fs_inst::StackPush{2}, value);              \
		COMPARE_VARIANT_NODES_MEMBER(back_result.instructions[1], fs_inst::StackPush{3}, value);              \
		COMPARE_VARIANT_NODES_MEMBER(back_result.instructions[2], fs_inst::StackPush{4}, value);              \
		COMPARE_VARIANT_NODES(back_result.instructions[3], back_function2);                                   \
		COMPARE_VARIANT_NODES(back_result.instructions[4], back_function1);                                   \
	}

#define UNARY_NOT_TEST(name, instr_name, symbol)                                                   \
	TEST_CASE("libparsers - function scripts - order-of-operations - " STR(name) " > unary not") { \
		auto function1 = fs_inst::OPUnaryNot{};                                                    \
		auto function2 = fs_inst::instr_name{};                                                    \
		auto result = bve::parsers::function_scripts::parse("!2 " STR(symbol) " 3");               \
                                                                                                   \
		REQUIRE_EQ(result.instructions.size(), 4);                                                 \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);        \
		COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);        \
		COMPARE_VARIANT_NODES(result.instructions[2], function2);                                  \
		COMPARE_VARIANT_NODES(result.instructions[3], function1);                                  \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

// ^
CREATE_OOO_DIFFERENT_TEST_CASE(binary xor, binary or, OPXor, OPOr, ^, |)
// &
CREATE_OOO_DIFFERENT_TEST_CASE(binary and, binary xor, OPAnd, OPXor, &, ^)

// !
TEST_CASE("libparsers - function scripts - order-of-operations - unary not > binary and") {
	auto function1 = fs_inst::OPUnaryNot{};
	auto function2 = fs_inst::OPAnd{};
	auto result = bve::parsers::function_scripts::parse("!2 & 3");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], function1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], function2);
}
UNARY_NOT_TEST(equals, OPEqual, ==)
UNARY_NOT_TEST(not equals, OPUnequal, !=)
UNARY_NOT_TEST(less, OPLess, <)
UNARY_NOT_TEST(greater, OPGreater, >)
UNARY_NOT_TEST(lesser or equal, OPLessEqual, <=)
UNARY_NOT_TEST(greater or equal, OPGreaterEqual, >=)

// ==
CREATE_OOO_SAME_TEST_CASE(equals, not equals, OPEqual, OPUnequal, ==, !=)
CREATE_OOO_SAME_TEST_CASE(equals, less, OPEqual, OPLess, ==, <)
CREATE_OOO_SAME_TEST_CASE(equals, greater, OPEqual, OPGreater, ==, >)
CREATE_OOO_SAME_TEST_CASE(equals, lesser or equals, OPEqual, OPLessEqual, ==, <=)
CREATE_OOO_SAME_TEST_CASE(equals, greater or equals, OPEqual, OPGreaterEqual, ==, >=)

// !=
CREATE_OOO_SAME_TEST_CASE(not equals, less, OPUnequal, OPLess, !=, <)
CREATE_OOO_SAME_TEST_CASE(not equals, greater, OPUnequal, OPGreater, !=, >)
CREATE_OOO_SAME_TEST_CASE(not equals, lesser or equals, OPUnequal, OPLessEqual, !=, <=)
CREATE_OOO_SAME_TEST_CASE(not equals, greater or equals, OPUnequal, OPGreaterEqual, !=, >=)

// <
CREATE_OOO_SAME_TEST_CASE(less, greater, OPLess, OPGreater, <, >)
CREATE_OOO_SAME_TEST_CASE(less, lesser or equals, OPLess, OPLessEqual, <, <=)
CREATE_OOO_SAME_TEST_CASE(less, greater or equals, OPLess, OPGreaterEqual, <, >=)

// >
CREATE_OOO_SAME_TEST_CASE(greater, lesser or equals, OPGreater, OPLessEqual, >, <=)
CREATE_OOO_SAME_TEST_CASE(greater, greater or equals, OPGreater, OPGreaterEqual, >, >=)

// <=
CREATE_OOO_SAME_TEST_CASE(lesser or equals, greater or equals, OPLessEqual, OPGreaterEqual, <=, >=)

// +
CREATE_OOO_DIFFERENT_TEST_CASE(add, equals, OPAdd, OPEqual, +, ==)
CREATE_OOO_DIFFERENT_TEST_CASE(add, not equals, OPAdd, OPUnequal, +, !=)
CREATE_OOO_DIFFERENT_TEST_CASE(add, less, OPAdd, OPLess, +, <)
CREATE_OOO_DIFFERENT_TEST_CASE(add, greater, OPAdd, OPGreater, +, >)
CREATE_OOO_DIFFERENT_TEST_CASE(add, lesser or equals, OPAdd, OPLessEqual, +, <=)
CREATE_OOO_DIFFERENT_TEST_CASE(add, greater or equals, OPAdd, OPGreaterEqual, +, >=)
// -
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, equals, OPSubtract, OPEqual, -, ==)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, not equals, OPSubtract, OPUnequal, -, !=)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, less, OPSubtract, OPLess, -, <)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, greater, OPSubtract, OPGreater, -, >)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, lesser or equals, OPSubtract, OPLessEqual, -, <=)
CREATE_OOO_DIFFERENT_TEST_CASE(subtract, greater or equals, OPSubtract, OPGreaterEqual, -, >=)

CREATE_OOO_SAME_TEST_CASE(add, subtract, OPAdd, OPSubtract, +, -)

// *
CREATE_OOO_DIFFERENT_TEST_CASE(multiply, add, OPMultiply, OPAdd, *, +)
CREATE_OOO_DIFFERENT_TEST_CASE(multiply, subtract, OPMultiply, OPSubtract, *, -)

// /
CREATE_OOO_DIFFERENT_TEST_CASE(divide, multiply, OPDivide, OPMultiply, /, *)

// - (unary)
TEST_CASE("libparsers - function scripts - order-of-operations - unary minus > divide") {
	auto function1 = fs_inst::OPUnaryMinus{};
	auto function2 = fs_inst::OPDivide{};
	auto result = bve::parsers::function_scripts::parse("- 2 / 3");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], function1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], function2);
}

// a[]
TEST_CASE("libparsers - function scripts - order-of-operations - function > unary minus") {
	auto function1 = fs_inst::OPUnaryMinus{};
	auto function2 = fs_inst::FuncSin{};
	auto result = bve::parsers::function_scripts::parse("sin[- 2]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], function1);
	COMPARE_VARIANT_NODES(result.instructions[2], function2);
}

TEST_SUITE_END();
