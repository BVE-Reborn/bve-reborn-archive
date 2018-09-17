#include "test_macro_util.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - errors - invalid function") {
	auto result = parsers::function_scripts::parse("lookatme[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("libparsers - function scripts - errors - invalid variable") {
	auto result = parsers::function_scripts::parse("lookatme");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("libparsers - function scripts - errors - missing unary operand") {
	auto result = parsers::function_scripts::parse("-");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
}

TEST_CASE("libparsers - function scripts - errors - missing unary operand inside function") {
	auto result = parsers::function_scripts::parse("sin[-]");

	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_sin{});
}

TEST_CASE("libparsers - function scripts - errors - single dot") {
	auto result = parsers::function_scripts::parse(".");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("libparsers - function scripts - errors - single dot inside function") {
	auto result = parsers::function_scripts::parse("sin[.]");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("libparsers - function scripts - errors - malformed variable") {
	auto result = parsers::function_scripts::parse("absjs211");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("libparsers - function scripts - errors - malformed number") {
	auto result = parsers::function_scripts::parse("12131asj");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{12131}, value);
}

TEST_CASE("libparsers - function scripts - errors - missing left bracket") {
	auto result = parsers::function_scripts::parse("sin odometer]");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("libparsers - function scripts - errors - missing right bracket") {
	auto result = parsers::function_scripts::parse("sin [odometer");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES(result.instructions[0], fs_inst::op_variable_lookup{});
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("libparsers - function scripts - errors - missing left parenthesis") {
	auto result = parsers::function_scripts::parse("5)");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{5}, value);
}

TEST_CASE("libparsers - function scripts - errors - missing right parenthesis") {
	auto result = parsers::function_scripts::parse("(5");

	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{5}, value);
}

TEST_CASE("libparsers - function scripts - errors - missing unary function argument") {
	auto result = parsers::function_scripts::parse("sin[]");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("libparsers - function scripts - errors - too many unary function arguments") {
	auto result = parsers::function_scripts::parse("sin[1, 2, 3]");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{1}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("libparsers - function scripts - errors - 2 missing binary function argument") {
	auto result = parsers::function_scripts::parse("randomInt[]");

	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_random_int{});
}

TEST_CASE("libparsers - function scripts - errors - 1 missing binary function argument") {
	auto result = parsers::function_scripts::parse("randomInt[1]");

	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{1}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_random_int{});
}

TEST_CASE("libparsers - function scripts - errors - too many binary function argument") {
	auto result = parsers::function_scripts::parse("randomInt[1, 2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{1}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_random_int{});
}

TEST_CASE("libparsers - function scripts - errors - 3 missing ternary function argument") {
	auto result = parsers::function_scripts::parse("if[]");

	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("libparsers - function scripts - errors - 2 missing ternary function argument") {
	auto result = parsers::function_scripts::parse("if[2]");

	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("libparsers - function scripts - errors - 1 missing ternary function argument") {
	auto result = parsers::function_scripts::parse("if[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("libparsers - function scripts - errors - too many ternary function argument") {
	auto result = parsers::function_scripts::parse("if[2, 3, 4, 5]");

	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("libparsers - function scripts - errors - 1 missing variatic function argument") {
	auto function = fs_inst::func_max{1};

	auto result = parsers::function_scripts::parse("max[]");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], function, count);
}

TEST_CASE("libparsers - function scripts - errors - missing unary variable index argument") {
	auto result = parsers::function_scripts::parse("Odometer[]");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_EQ(result.used_indexed_variables.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1],
	                             fs_inst::op_variable_indexed{fs_inst::indexed_variable::odometer},
	                             name);
	CHECK_EQ(*result.used_indexed_variables.begin(), fs_inst::indexed_variable::odometer);
}

TEST_CASE("libparsers - function scripts - errors - too many unary variable index argument") {
	auto result = parsers::function_scripts::parse("Odometer[1, 2, 3]");

	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_EQ(result.used_indexed_variables.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{1}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1],
	                             fs_inst::op_variable_indexed{fs_inst::indexed_variable::odometer},
	                             name);
	CHECK_EQ(*result.used_indexed_variables.begin(), fs_inst::indexed_variable::odometer);
}

TEST_CASE("libparsers - function scripts - errors - Single equals") {
	auto result = parsers::function_scripts::parse("2 = 3");

	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_equal{});
}

TEST_SUITE_END();
