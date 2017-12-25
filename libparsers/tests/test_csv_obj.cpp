#include "csv_object.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>

void test_csv_obj() {
	auto content = parsers::util::load_from_file_utf8_bom("libparsers/tests/westonmill_2.b3d");

	auto instr = parsers::csv_object::create_instructions(content, parsers::csv_object::file_type::b3d);

	for (auto& i : instr) {
		std::cout << i << '\n';
	}

	parsers::csv_object::run_csv_instructions(instr);
}