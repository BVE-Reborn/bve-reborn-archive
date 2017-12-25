#include "b3d_csv_object.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>

void test_csv_obj() {
	auto content = parsers::util::load_from_file_utf8_bom("libparsers/tests/pole_2.csv");

	auto instr = parsers::b3d_csv_object::create_instructions(content, parsers::b3d_csv_object::file_type::csv);

	for (auto& i : instr) {
		std::cout << i << '\n';
	}

	auto obj = parsers::b3d_csv_object::run_csv_instructions(instr);

	for (auto& error : obj.errors) {
		std::cout << "Line: " << error.line << ", Reason: " << error.error << '\n';
	}
}