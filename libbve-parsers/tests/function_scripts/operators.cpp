#include "test_macro_util.hpp"
#include <doctest.h>
#include <parsers/function_scripts.hpp>

using namespace std::string_literals;
namespace fs_inst = bve::parsers::function_scripts::instructions;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - operators - logical or") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 | 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("or[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_or{});
}

TEST_CASE("libparsers - function scripts - operators - logical xor") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 ^ 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("xor[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_xor{});
}

TEST_CASE("libparsers - function scripts - operators - logical and") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 & 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("and[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_and{});
}

TEST_CASE("libparsers - function scripts - operators - logical not") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("!3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("not[3]");
	}

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_not{});
}

TEST_CASE("libparsers - function scripts - operators - equals") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 == 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("equal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_equal{});
}

TEST_CASE("libparsers - function scripts - operators - not equals") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 != 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("unequal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_unequal{});
}

TEST_CASE("libparsers - function scripts - operators - less") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 < 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("less[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_less{});
}

TEST_CASE("libparsers - function scripts - operators - greater") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 > 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("greater[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_greater{});
}

TEST_CASE("libparsers - function scripts - operators - less equal") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 <= 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("lessequal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_less_equal{});
}

TEST_CASE("libparsers - function scripts - operators - greater equal") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 >= 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("greaterequal[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_greater_equal{});
}

TEST_CASE("libparsers - function scripts - operators - plus") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 + 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("plus[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_add{});
}

TEST_CASE("libparsers - function scripts - operators - subtract") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 - 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("subtract[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_subtract{});
}

TEST_CASE("libparsers - function scripts - operators - times") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 * 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("times[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_multiply{});
}

TEST_CASE("libparsers - function scripts - operators - divide") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("2 / 3");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("divide[2, 3]");
	}

	REQUIRE_EQ(result.instructions.size(), 3);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[1], fs_inst::stack_push{3}, value);
	COMPARE_VARIANT_NODES(result.instructions[2], fs_inst::op_divide{});
}

TEST_CASE("libparsers - function scripts - operators - unary minus") {
	bve::parsers::function_scripts::instruction_list result;

	SUBCASE("infix") {
		result = bve::parsers::function_scripts::parse("-(2)");
	}
	SUBCASE("function") {
		result = bve::parsers::function_scripts::parse("minus[2]");
	}

	REQUIRE_EQ(result.instructions.size(), 2);
	COMPARE_VARIANT_NODES_MEMBER(result.instructions[0], fs_inst::stack_push{2}, value);
	COMPARE_VARIANT_NODES(result.instructions[1], fs_inst::op_unary_minus{});
}

TEST_SUITE_END();
