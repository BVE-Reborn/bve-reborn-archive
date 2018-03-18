#include "b3d_csv_object.hpp"
#include "tests.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <mapbox/variant_io.hpp>

void test_csv_obj() {
	auto content = parsers::util::load_from_file_utf8_bom(DIRECTORY "tests/test_files/pole_2.csv");

	auto inst =
	    parsers::b3d_csv_object::create_instructions(content,
	                                                 parsers::b3d_csv_object::file_type::csv);

	for (auto& i : inst) {
		std::cout << i << '\n';
	}

	auto obj = parsers::b3d_csv_object::run_csv_instructions(inst);

	for (auto& error : obj.errors) {
		std::cout << "Line: " << error.line << ", Reason: " << error.error << '\n';
	}
}