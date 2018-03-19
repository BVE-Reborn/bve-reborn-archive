#include "variant_compare.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("functions - reciprocal") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("reciprocal[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_reciprocal{});
}

TEST_CASE("functions - power2") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("power[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);

	auto function = fs_inst::func_power{};
	function.count = 2;
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("functions - power3") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("power[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);

	auto function = fs_inst::func_power{};
	function.count = 3;
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("functions - quotient") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("quotient[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_quotient{});
}

TEST_CASE("functions - mod") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("mod[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_mod{});
}

TEST_CASE("functions - min2") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("min[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);

	auto function = fs_inst::func_min{};
	function.count = 2;
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("functions - min3") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("min[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);

	auto function = fs_inst::func_min{};
	function.count = 3;
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("functions - max2") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("max[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);

	auto function = fs_inst::func_max{};
	function.count = 2;
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("functions - max3") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("max[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);

	auto function = fs_inst::func_max{};
	function.count = 3;
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("functions - abs") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("abs[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_abs{});
}

TEST_CASE("functions - sign") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sign[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sign{});
}

TEST_CASE("functions - floor") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("floor[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_floor{});
}

TEST_CASE("functions - ceiling") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("ceiling[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_ceiling{});
}

TEST_CASE("functions - round") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("round[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_round{});
}

TEST_CASE("functions - random") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("random[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_random{});
}

TEST_CASE("functions - randomInt") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("randomint[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::func_randomInt{});
}

TEST_CASE("functions - exp") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("exp[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_exp{});
}

TEST_CASE("functions - log") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("log[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_log{});
}

TEST_CASE("functions - sqrt") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sqrt[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sqrt{});
}

TEST_CASE("functions - sin") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("sin[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_sin{});
}

TEST_CASE("functions - cos") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("cos[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_cos{});
}

TEST_CASE("functions - tan") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("tan[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_tan{});
}

TEST_CASE("functions - arctan") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("arctan[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::func_arctan{});
}

TEST_CASE("functions - if") {
	parsers::function_scripts::instruction_list result;

	result = parsers::function_scripts::parse("if[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::stack_push{4}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::func_if{});
}

TEST_SUITE_END();
