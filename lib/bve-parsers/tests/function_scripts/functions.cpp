#include <doctest/doctest.h>
#include <parsers/function_scripts.hpp>
#include <util/testing/variant_macros.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - functions - reciprocal") {
	auto result = bve::parsers::function_scripts::parse("reciprocal[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncReciprocal{});
}

TEST_CASE("libparsers - function scripts - functions - power2") {
	auto result = bve::parsers::function_scripts::parse("power[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);

	auto function = fs_inst::FuncPower{2};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("libparsers - function scripts - functions - power3") {
	auto result = bve::parsers::function_scripts::parse("power[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::StackPush{4}, value);

	auto function = fs_inst::FuncPower{3};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("libparsers - function scripts - functions - quotient") {
	auto result = bve::parsers::function_scripts::parse("quotient[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::FuncQuotient{});
}

TEST_CASE("libparsers - function scripts - functions - mod") {
	auto result = bve::parsers::function_scripts::parse("mod[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::FuncMod{});
}

TEST_CASE("libparsers - function scripts - functions - min2") {
	auto result = bve::parsers::function_scripts::parse("min[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);

	auto function = fs_inst::FuncMin{2};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("libparsers - function scripts - functions - min3") {
	auto result = bve::parsers::function_scripts::parse("min[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::StackPush{4}, value);

	auto function = fs_inst::FuncMin{3};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("libparsers - function scripts - functions - max2") {
	auto result = bve::parsers::function_scripts::parse("max[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);

	auto function = fs_inst::FuncMax{2};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], function, count);
}

TEST_CASE("libparsers - function scripts - functions - max3") {
	auto result = bve::parsers::function_scripts::parse("max[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::StackPush{4}, value);

	auto function = fs_inst::FuncMax{3};
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[3], function, count);
}

TEST_CASE("libparsers - function scripts - functions - abs") {
	auto result = bve::parsers::function_scripts::parse("abs[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncAbs{});
}

TEST_CASE("libparsers - function scripts - functions - sign") {
	auto result = bve::parsers::function_scripts::parse("sign[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncSign{});
}

TEST_CASE("libparsers - function scripts - functions - floor") {
	auto result = bve::parsers::function_scripts::parse("floor[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncFloor{});
}

TEST_CASE("libparsers - function scripts - functions - ceiling") {
	auto result = bve::parsers::function_scripts::parse("ceiling[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncCeiling{});
}

TEST_CASE("libparsers - function scripts - functions - round") {
	auto result = bve::parsers::function_scripts::parse("round[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncRound{});
}

TEST_CASE("libparsers - function scripts - functions - random") {
	auto result = bve::parsers::function_scripts::parse("random[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::FuncRandom{});
}

TEST_CASE("libparsers - function scripts - functions - randomInt") {
	auto result = bve::parsers::function_scripts::parse("randomint[2, 3]");

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::FuncRandomInt{});
}

TEST_CASE("libparsers - function scripts - functions - exp") {
	auto result = bve::parsers::function_scripts::parse("exp[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncExp{});
}

TEST_CASE("libparsers - function scripts - functions - log") {
	auto result = bve::parsers::function_scripts::parse("log[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncLog{});
}

TEST_CASE("libparsers - function scripts - functions - sqrt") {
	auto result = bve::parsers::function_scripts::parse("sqrt[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncSqrt{});
}

TEST_CASE("libparsers - function scripts - functions - sin") {
	auto result = bve::parsers::function_scripts::parse("sin[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncSin{});
}

TEST_CASE("libparsers - function scripts - functions - cos") {
	auto result = bve::parsers::function_scripts::parse("cos[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncCos{});
}

TEST_CASE("libparsers - function scripts - functions - tan") {
	auto result = bve::parsers::function_scripts::parse("tan[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncTan{});
}

TEST_CASE("libparsers - function scripts - functions - arctan") {
	auto result = bve::parsers::function_scripts::parse("arctan[2]");

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::FuncArctan{});
}

TEST_CASE("libparsers - function scripts - functions - if") {
	auto result = bve::parsers::function_scripts::parse("if[2, 3, 4]");

	REQUIRE_EQ(result.instructions.size(), 4);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[2], fs_inst::StackPush{4}, value);
	COMPARE_VARIANT_NODES(result.instructions[3], fs_inst::FuncIf{});
}

TEST_SUITE_END();
