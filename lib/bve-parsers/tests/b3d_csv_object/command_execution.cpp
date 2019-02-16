#include "b3d_csv_object/b3d_csv_object.hpp"
#include <doctest.h>
#include <util/testing/variant_macros.hpp>

using namespace std::string_literals;

// ReSharper disable once CppInconsistentNaming
namespace b3d = bve::parsers::b3d_csv_object;

TEST_SUITE_BEGIN("libparsers - b3d_csv_object");

TEST_CASE("libparsers - b3d_csv_object - command execution - face creation") {
	b3d::InstructionList const instructions{b3d::instructions::AddVertex{0, 0, 0, 1, 1, 1}, b3d::instructions::AddVertex{1, 0, 0, 1, 1, 1},
	                                        b3d::instructions::AddVertex{1, 1, 0, 1, 1, 1}, b3d::instructions::AddVertex{0, 1, 0, 1, 1, 1},
	                                        b3d::instructions::AddFace{{0, 1, 2, 3}, b3d::instructions::Sides::one}};

	auto result = b3d::run_csv_instructions(instructions);
}

TEST_SUITE_END();
