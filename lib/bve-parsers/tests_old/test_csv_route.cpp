#include "core/variant_util.hpp"
#include "old_tests_config.hpp"
#include "csv_rw_route/csv_rw_route.hpp"
#include "tests.hpp"
#include <algorithm>
#include <array>
#include <cppfs/FilePath.h>
#include <iostream>
#include <string>

using namespace std::string_literals;

void test_csv_route() {
	std::array<int, std::mt19937::state_size> seed_data{};
	std::random_device r;
	std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

	std::mt19937 eng(seq);

	bve::parsers::errors::MultiError me;

	auto const file_location = TESTLOCATION "/Kaligung Mas.csv"s;
	auto used_file = cppfs::FilePath(file_location);

	auto const get_abs_path = [&](const std::string& base_file, const std::string& file) {
		cppfs::FilePath base_fp(base_file);
		cppfs::FilePath fp(file);

		return base_fp.resolve("..").resolve(file).path();
	};

	auto vals = process_include_directives(cppfs::FilePath(file_location).resolved(), eng, me,
	                                       bve::parsers::csv_rw_route::FileType::csv, get_abs_path);

	std::cout << vals.filenames.size() << '\n';
	std::cout << vals.lines.size() << '\n';

	preprocess_file(vals, eng, me, bve::parsers::csv_rw_route::FileType::csv);

	auto instructions = generate_instructions(vals, me, bve::parsers::csv_rw_route::FileType::csv);

	std::cout << instructions.instructions.size() << '\n';

	execute_instructions_pass1(instructions, me);
	auto route_data = execute_instructions_pass2(instructions, me);
	execute_instructions_pass3(route_data, instructions, me, get_abs_path);

	std::cout << route_data.objects.size() << '\n';

	// std::cout << instructions;
	// std::cout << me;
}
