#include "core/macro_helpers.hpp"
#include "function_scripts/parse_tree.hpp"
#include <doctest.h>
#include <sstream>

using namespace std::string_literals;
namespace fs_tree_node = bve::parsers::function_scripts::tree_types;

#define UNARY_TREE_TEST(full_name, type_name, output_name)                                         \
	TEST_CASE("libparsers - function scripts - tree iostream - " STR(full_name)) {                 \
		fs_tree_node::type_name test_node{fs_tree_node::Integer{2}};                               \
                                                                                                   \
		std::ostringstream output;                                                                 \
                                                                                                   \
		output << test_node;                                                                       \
                                                                                                   \
		CHECK_EQ(output.str(), STR(output_name) "\n"                         \
                                                  "| 2\n");                    \
	}

#define BINARY_TREE_TEST(full_name, type_name, output_name)                                        \
	TEST_CASE("libparsers - function scripts - tree iostream - " STR(full_name)) {                 \
		fs_tree_node::type_name test_node{fs_tree_node::Integer{2}, fs_tree_node::Floating{3}};    \
                                                                                                   \
		std::ostringstream output;                                                                 \
                                                                                                   \
		output << test_node;                                                                       \
                                                                                                   \
		CHECK_EQ(output.str(), STR(output_name) "\n"                         \
                                                  "| 2\n"                      \
                                                  "| 3\n");                    \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

UNARY_TREE_TEST(unary not, UnaryNot, NOT)
UNARY_TREE_TEST(unary minus, UnaryMinus, MINUS)
BINARY_TREE_TEST(binary and, BinaryAnd, AND)
BINARY_TREE_TEST(binary xor, BinaryXor, XOR)
BINARY_TREE_TEST(binary or, BinaryOr, OR)
BINARY_TREE_TEST(binary equal, BinaryEq, EQ)
BINARY_TREE_TEST(binary not equal, BinaryNotEq, NEQ)
BINARY_TREE_TEST(binary less, BinaryLess, LESS)
BINARY_TREE_TEST(binary greater, BinaryGreater, GREATER)
BINARY_TREE_TEST(binary less or equal, BinaryLessEq, LESS_EQ)
BINARY_TREE_TEST(binary greater or equal, BinaryGreaterEq, GREATER_EQ)
BINARY_TREE_TEST(binary add, BinaryAdd, ADD)
BINARY_TREE_TEST(binary subtract, BinarySubtract, SUB)
BINARY_TREE_TEST(binary multiply, BinaryMultiply, MULTIPLY)
BINARY_TREE_TEST(binary divide, BinaryDivide, DIVIDE)

TEST_CASE("libparsers - function scripts - tree iostream - function call 1 arg") {
	fs_tree_node::FunctionCall test_node{{"max"s}, {fs_tree_node::Integer{2}}};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(),
	         "FUNC_CALL: max\n"
	         "| 2\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - function call 2 args") {
	fs_tree_node::FunctionCall test_node{{"max"s},
	                                     {fs_tree_node::Integer{2}, fs_tree_node::Integer{3}}};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(),
	         "FUNC_CALL: max\n"
	         "| 2\n"
	         "| 3\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - function call 3 args") {
	fs_tree_node::FunctionCall test_node{{"max"s},
	                                     {fs_tree_node::Integer{2}, fs_tree_node::Integer{3},
	                                      fs_tree_node::Integer{4}}};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(),
	         "FUNC_CALL: max\n"
	         "| 2\n"
	         "| 3\n"
	         "| 4\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - variable") {
	fs_tree_node::Identifier test_node{"odometer"s};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(), "VARIABLE: odometer\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - none") {
	fs_tree_node::None test_node{};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(), "NONE\n");
}

TEST_SUITE_END();
