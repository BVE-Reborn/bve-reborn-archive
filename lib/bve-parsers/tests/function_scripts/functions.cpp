#include "test_macro_util.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - functions - reciprocal") {
	auto result = bve::parsers::function_scripts::parse("reciprocal[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_reciprocal{});
}

TEST_CASE("libparsers - function scripts - functions - power2") {
	auto result = bve::parsers::function_scripts::parse("power[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);

	auto function = fs_inst::func_power{2};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("libparsers - function scripts - functions - power3") {
	auto result = bve::parsers::function_scripts::parse("power[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);

	auto function = fs_inst::func_power{3};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("libparsers - function scripts - functions - quotient") {
	auto result = bve::parsers::function_scripts::parse("quotient[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_quotient{});
}

TEST_CASE("libparsers - function scripts - functions - mod") {
	auto result = bve::parsers::function_scripts::parse("mod[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_mod{});
}

TEST_CASE("libparsers - function scripts - functions - min2") {
	auto result = bve::parsers::function_scripts::parse("min[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);

	auto function = fs_inst::func_min{2};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("libparsers - function scripts - functions - min3") {
	auto result = bve::parsers::function_scripts::parse("min[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);

	auto function = fs_inst::func_min{3};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("libparsers - function scripts - functions - max2") {
	auto result = bve::parsers::function_scripts::parse("max[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);

	auto function = fs_inst::func_max{2};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("libparsers - function scripts - functions - max3") {
	auto result = bve::parsers::function_scripts::parse("max[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);

	auto function = fs_inst::func_max{3};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("libparsers - function scripts - functions - abs") {
	auto result = bve::parsers::function_scripts::parse("abs[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_abs{});
}

TEST_CASE("libparsers - function scripts - functions - sign") {
	auto result = bve::parsers::function_scripts::parse("sign[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sign{});
}

TEST_CASE("libparsers - function scripts - functions - floor") {
	auto result = bve::parsers::function_scripts::parse("floor[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_floor{});
}

TEST_CASE("libparsers - function scripts - functions - ceiling") {
	auto result = bve::parsers::function_scripts::parse("ceiling[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_ceiling{});
}

TEST_CASE("libparsers - function scripts - functions - round") {
	auto result = bve::parsers::function_scripts::parse("round[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_round{});
}

TEST_CASE("libparsers - function scripts - functions - random") {
	auto result = bve::parsers::function_scripts::parse("random[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_random{});
}

TEST_CASE("libparsers - function scripts - functions - randomInt") {
	auto result = bve::parsers::function_scripts::parse("randomint[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_random_int{});
}

TEST_CASE("libparsers - function scripts - functions - exp") {
	auto result = bve::parsers::function_scripts::parse("exp[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_exp{});
}

TEST_CASE("libparsers - function scripts - functions - log") {
	auto result = bve::parsers::function_scripts::parse("log[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_log{});
}

TEST_CASE("libparsers - function scripts - functions - sqrt") {
	auto result = bve::parsers::function_scripts::parse("sqrt[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sqrt{});
}

TEST_CASE("libparsers - function scripts - functions - sin") {
	auto result = bve::parsers::function_scripts::parse("sin[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("libparsers - function scripts - functions - cos") {
	auto result = bve::parsers::function_scripts::parse("cos[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_cos{});
}

TEST_CASE("libparsers - function scripts - functions - tan") {
	auto result = bve::parsers::function_scripts::parse("tan[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_tan{});
}

TEST_CASE("libparsers - function scripts - functions - arctan") {
	auto result = bve::parsers::function_scripts::parse("arctan[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_arctan{});
}

TEST_CASE("libparsers - function scripts - functions - if") {
	auto result = bve::parsers::function_scripts::parse("if[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_SUITE_END();
