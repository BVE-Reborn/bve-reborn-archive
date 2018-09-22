#include "old_tests_config.hpp"
#include "parsers/utils.hpp"
#include "b3d_csv_object/b3d_csv_object.hpp"
#include "tests.hpp"
#include <iostream>
#include <mapbox/variant_io.hpp>

void test_csv_obj() {
	auto const content = bve::parsers::util::load_from_file_utf8_bom(TESTLOCATION "/pole_2.csv");

	auto inst = create_instructions(content, bve::parsers::b3d_csv_object::FileType::csv);

	for (auto& i : inst) {
		std::cout << i << '\n';
	}

	auto obj = bve::parsers::b3d_csv_object::run_csv_instructions(inst);

	for (auto& error : obj.errors) {
		std::cout << "Line: " << error.line << ", Reason: " << error.error << '\n';
	}
}
