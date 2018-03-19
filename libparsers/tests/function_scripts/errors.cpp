#include "variant_compare.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>
#include <iostream>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("errors - invalid function") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("lookatme[2, 3]");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_EQ(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}

TEST_CASE("errors - invalid variable") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("lookatme");
	REQUIRE_EQ(result.instructions.size(), 1);
	REQUIRE_EQ(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
}



TEST_CASE("errors - missing unary operand") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("-");
	REQUIRE_EQ(result.instructions.size(), 2);
	REQUIRE_EQ(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
}



TEST_CASE("errors - missing unary operand inside function") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin[-]");
	REQUIRE_EQ(result.instructions.size(), 3);
	REQUIRE_EQ(result.errors.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{0}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_sin{});
}

TEST_SUITE_END();
