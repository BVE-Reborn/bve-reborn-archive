#include <doctest/doctest.h>
#include <parsers/function_scripts.hpp>
#include <util/testing/variant_macros.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - lexer - integer") {
	auto result = bve::parsers::function_scripts::parse("2");

	REQUIRE_EQ(result.instructions.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
}

TEST_CASE("libparsers - function scripts - lexer - negative integer") {
	auto result = bve::parsers::function_scripts::parse("-2");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::OPUnaryMinus{});
}

TEST_CASE("libparsers - function scripts - lexer - float") {
	auto result = bve::parsers::function_scripts::parse("2.2");

	REQUIRE_EQ(result.instructions.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2.2F}, value);
}

TEST_CASE("libparsers - function scripts - lexer - negative float") {
	auto result = bve::parsers::function_scripts::parse("-2.2");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2.2F}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::OPUnaryMinus{});
}

TEST_CASE("libparsers - function scripts - lexer - float no decimal") {
	auto result = bve::parsers::function_scripts::parse("2.");

	REQUIRE_EQ(result.instructions.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
}

TEST_CASE("libparsers - function scripts - lexer - negative float no decimal") {
	auto result = bve::parsers::function_scripts::parse("-2.");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::OPUnaryMinus{});
}

TEST_CASE("libparsers - function scripts - lexer - subtraction over unary minus") {
	auto result = bve::parsers::function_scripts::parse("2-2");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPSubtract{});
}

TEST_SUITE_END();
