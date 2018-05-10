#include "function_scripts.hpp"
#include "test_macro_util.hpp"
#include <doctest.h>
#include <sstream>

using namespace std::string_literals;
namespace fs_tree_node = parsers::function_scripts::tree_types;

#define UNARY_TREE_TEST(full_name, type_name, output_name)                                         \
	TEST_CASE("libparsers - function scripts - tree iostream - " STRINGIFY(full_name)) {           \
		fs_tree_node::type_name test_node{fs_tree_node::integer{2}};                               \
                                                                                                   \
		std::ostringstream output;                                                                 \
                                                                                                   \
		output << test_node;                                                                       \
                                                                                                   \
		CHECK_EQ(output.str(), STRINGIFY(output_name) "\n"                         \
                                                  "| 2\n");               \
	}

#define BINARY_TREE_TEST(full_name, type_name, output_name)                                        \
	TEST_CASE("libparsers - function scripts - tree iostream - " STRINGIFY(full_name)) {           \
		fs_tree_node::type_name test_node{fs_tree_node::integer{2}, fs_tree_node::floating{3}};    \
                                                                                                   \
		std::ostringstream output;                                                                 \
                                                                                                   \
		output << test_node;                                                                       \
                                                                                                   \
		CHECK_EQ(output.str(), STRINGIFY(output_name) "\n"                         \
                                                  "| 2\n"                      \
                                                  "| 3\n");               \
	}

TEST_SUITE_BEGIN("libparsers - function scripts");

UNARY_TREE_TEST(unary not, unary_not, NOT)
UNARY_TREE_TEST(unary minus, unary_minus, MINUS)
BINARY_TREE_TEST(binary and, binary_and, AND)
BINARY_TREE_TEST(binary xor, binary_xor, XOR)
BINARY_TREE_TEST(binary or, binary_or, OR)
BINARY_TREE_TEST(binary equal, binary_eq, EQ)
BINARY_TREE_TEST(binary not equal, binary_not_eq, NEQ)
BINARY_TREE_TEST(binary less, binary_less, LESS)
BINARY_TREE_TEST(binary greater, binary_greater, GREATER)
BINARY_TREE_TEST(binary less or equal, binary_less_eq, LESS_EQ)
BINARY_TREE_TEST(binary greater or equal, binary_greater_eq, GREATER_EQ)
BINARY_TREE_TEST(binary add, binary_add, ADD)
BINARY_TREE_TEST(binary subtract, binary_subtract, SUB)
BINARY_TREE_TEST(binary multiply, binary_multiply, MULTIPLY)
BINARY_TREE_TEST(binary divide, binary_divide, DIVIDE)

TEST_CASE("libparsers - function scripts - tree iostream - function call 1 arg") {
	fs_tree_node::function_call test_node{{"max"s}, {fs_tree_node::integer{2}}};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(),
	         "FUNC_CALL: max\n"
	         "| 2\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - function call 2 args") {
	fs_tree_node::function_call test_node{{"max"s},
	                                      {fs_tree_node::integer{2}, fs_tree_node::integer{3}}};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(),
	         "FUNC_CALL: max\n"
	         "| 2\n"
	         "| 3\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - function call 3 args") {
	fs_tree_node::function_call test_node{{"max"s},
	                                      {fs_tree_node::integer{2}, fs_tree_node::integer{3},
	                                       fs_tree_node::integer{4}}};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(),
	         "FUNC_CALL: max\n"
	         "| 2\n"
	         "| 3\n"
	         "| 4\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - variable") {
	fs_tree_node::identifier test_node{"odometer"s};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(), "VARIABLE: odometer\n");
}

TEST_CASE("libparsers - function scripts - tree iostream - none") {
	fs_tree_node::none test_node{};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(), "NONE\n");
}

TEST_SUITE_END();
