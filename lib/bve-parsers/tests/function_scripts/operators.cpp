#include <doctest/doctest.h>
#include <parsers/function_scripts.hpp>
#include <util/testing/variant_macros.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - operators - logical or") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 | 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("or[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPOr{});
}

TEST_CASE("libparsers - function scripts - operators - logical xor") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 ^ 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("xor[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPXor{});
}

TEST_CASE("libparsers - function scripts - operators - logical and") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 & 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("and[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPAnd{});
}

TEST_CASE("libparsers - function scripts - operators - logical not") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("!3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("not[3]");
	}

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::OPUnaryNot{});
}

TEST_CASE("libparsers - function scripts - operators - equals") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 == 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("equal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPEqual{});
}

TEST_CASE("libparsers - function scripts - operators - not equals") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 != 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("unequal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPUnequal{});
}

TEST_CASE("libparsers - function scripts - operators - less") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 < 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("less[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPLess{});
}

TEST_CASE("libparsers - function scripts - operators - greater") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 > 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("greater[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPGreater{});
}

TEST_CASE("libparsers - function scripts - operators - less equal") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 <= 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("lessequal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPLessEqual{});
}

TEST_CASE("libparsers - function scripts - operators - greater equal") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 >= 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("greaterequal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPGreaterEqual{});
}

TEST_CASE("libparsers - function scripts - operators - plus") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 + 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("plus[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPAdd{});
}

TEST_CASE("libparsers - function scripts - operators - subtract") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 - 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("subtract[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPSubtract{});
}

TEST_CASE("libparsers - function scripts - operators - times") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 * 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("times[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPMultiply{});
}

TEST_CASE("libparsers - function scripts - operators - divide") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 / 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("divide[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::StackPush{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::OPDivide{});
}

TEST_CASE("libparsers - function scripts - operators - unary minus") {
	bve::parsers::function_scripts::InstructionList result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("-(2)");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("minus[2]");
	}

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::StackPush{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::OPUnaryMinus{});
}

TEST_SUITE_END();
