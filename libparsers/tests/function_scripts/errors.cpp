#include "variant_compare.hpp"
#include <doctest.h>
#include <iostream>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("errors - invalid function") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("lookatme[2, 3]");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("errors - invalid variable") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("lookatme");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("errors - missing unary operand") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("-");
	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
}

TEST_CASE("errors - missing unary operand inside function") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin[-]");
	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_sin{});
}

TEST_CASE("errors - single dot") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse(".");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("errors - single dot inside function") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin[.]");
	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("errors - malformed variable") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("absjs211");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("errors - malformed number") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("12131asj");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{12131}, value);
}

TEST_CASE("errors - missing left bracket") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin odometer]");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("errors - missing right bracket") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin [odometer");
	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES(result.instructions[0], fs_inst::op_variable_lookup{});
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("errors - missing left parenthesis") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("5)");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{5}, value);
}

TEST_CASE("errors - missing right parenthesis") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("(5");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{5}, value);
}

TEST_CASE("errors - missing unary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin[]");
	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("errors - too many unary function arguments") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin[1, 2, 3]");
	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{1}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("errors - 2 missing binary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("randomInt[]");
	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_randomInt{});
}

TEST_CASE("errors - 1 missing binary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("randomInt[1]");
	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{1}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_randomInt{});
}

TEST_CASE("errors - too many binary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("randomInt[1, 2, 3]");
	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{1}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_randomInt{});
}

TEST_CASE("errors - 3 missing ternary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("if[]");
	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("errors - 2 missing ternary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("if[2]");
	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("errors - 1 missing ternary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("if[2, 3]");
	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("errors - too many ternary function argument") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("if[2, 3, 4, 5]");
	REQUIRE_EQ(result.instructions.size(), 4);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_CASE("errors - 1 missing variatic function argument") {
	parsers::function_scripts::instruction_list result;

	auto function = fs_inst::func_max{};
	function.count = 1;

	result = parsers::function_scripts::parse("max[]");
	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_GE(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], function, count);
}

TEST_SUITE_END();
