#include "b3d_csv_object.hpp"
#include "test_macro_util.hpp"
#include <doctest.h>

using namespace std::string_literals;

// ReSharper disable once CppInconsistentNaming
namespace b3d = bve::parsers::b3d_csv_object;

TEST_SUITE_BEGIN("libparsers - b3d_csv_object");

TEST_CASE("libparsers - b3d_csv_object - command execution - face creation") {
	b3d::instruction_list const instrs{b3d::instructions::add_vertex{0, 0, 0, 1, 1, 1},
	                                   b3d::instructions::add_vertex{1, 0, 0, 1, 1, 1},
	                                   b3d::instructions::add_vertex{1, 1, 0, 1, 1, 1},
	                                   b3d::instructions::add_vertex{0, 1, 0, 1, 1, 1},
	                                   b3d::instructions::add_face{{0, 1, 2, 3},
	                                                               b3d::instructions::sides::one}};

	auto result = b3d::run_csv_instructions(instrs);
}

TEST_SUITE_END();
