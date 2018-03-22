#include "function_scripts.hpp"
#include "variant_compare.hpp"
#include <doctest.h>
#include <sstream>

using namespace std::string_literals;
namespace fs_lex_token = parsers::function_scripts::lexer_types;
namespace fs_tree_node = parsers::function_scripts::tree_types;

#define stringify(n) #n

#define UNARY_TREE_TEST(full_name, type_name, output_name)                                             \
	TEST_CASE("tree iostream - " stringify(full_name)) {                                               \
		fs_tree_node::type_name test_node{fs_tree_node::integer{2}};                                   \
                                                                                                       \
		std::ostringstream output;                                                                     \
                                                                                                       \
		output << test_node;                                                                           \
                                                                                                       \
		CHECK_EQ(output.str(),                                                                       \
		         stringify(output_name) "\n"                                                         \
		         "| 2\n"); \
	}

#define BINARY_TREE_TEST(full_name, type_name, output_name)                                            \
	TEST_CASE("tree iostream - " stringify(full_name)) {                                               \
		fs_tree_node::type_name test_node{fs_tree_node::integer{2}, fs_tree_node::floating{2}};        \
                                                                                                       \
		std::ostringstream output;                                                                     \
                                                                                                       \
		output << test_node;                                                                           \
                                                                                                       \
		CHECK_EQ(output.str(),                                                                       \
		         stringify(output_name) "\n"                                                         \
		         "| 2\n"                                                                             \
		         "| 2\n"); \
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
BINARY_TREE_TEST(binary greater, binary_add, ADD)
BINARY_TREE_TEST(binary less or equal, binary_add, ADD)
BINARY_TREE_TEST(binary greater or equal, binary_add, ADD)
BINARY_TREE_TEST(binary add, binary_add, ADD)
BINARY_TREE_TEST(binary subtract, binary_subtract, SUB)
BINARY_TREE_TEST(binary multiply, binary_multiply, MULTIPLY)
BINARY_TREE_TEST(binary divide, binary_divide, DIVIDE)

TEST_CASE("tree iostream - function call single arg") {
	fs_tree_node::function_call test_node{{"sin"s}, {fs_tree_node::integer{2}}};

	std::ostringstream output;

	output << test_node;

	CHECK_EQ(output.str(),
	         "FUNC_CALL: sin\n"
	         "| 2\n");
}

TEST_SUITE_END();
