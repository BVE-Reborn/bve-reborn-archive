#include "variant_compare.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("lexer - integer") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("2");

	REQUIRE_EQ(result.instructions.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
}

TEST_CASE("lexer - negative integer") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("-2");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
}

TEST_CASE("lexer - float") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("2.2");

	REQUIRE_EQ(result.instructions.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2.2f}, value);
}

TEST_CASE("lexer - negative float") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("-2.2");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2.2f}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
}

TEST_CASE("lexer - float no decimal") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("2.");

	REQUIRE_EQ(result.instructions.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
}

TEST_CASE("lexer - negative float no decimal") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("-2.");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
}

TEST_CASE("lexer - subtraction over unary minus") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("2-2");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_subtract{});
}

TEST_SUITE_END();
