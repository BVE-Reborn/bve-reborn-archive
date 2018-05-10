#include "core/variant_util.hpp"
#include "csv_rw_route.hpp"
#include "tests.hpp"
#include <algorithm>
#include <array>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

using namespace std::string_literals;

void test_csv_route() {
	std::array<int, std::mt19937::state_size> seed_data{};
	std::random_device r;
	std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

	std::mt19937 eng(seq);

	parsers::errors::multi_error_t me;

	// auto file_location = DIRECTORY
	//     "../libparsers/tests/test_files/plymouth/1980s/HST (Non-Stopping) - "
	//     "Liskeard [c].csv";
	auto const file_location =
	    DIRECTORY "../libparsers/tests/test_files/NYCT- R [Astoria to 95 St - 4th Av].csv"s;
	//	auto file_location = DIRECTORY "../libparsers/tests/Xmasmonorail.csv";
	// Bridge NTT [v1.0].csv";
	auto used_file = boost::filesystem::path(file_location);

	auto const get_abs_path = [&](const std::string& base_file, const std::string& file) {
		auto new_file = file;
		std::replace(new_file.begin(), new_file.end(), '\\', '/');
		auto file_path = boost::filesystem::path(base_file); //
		auto new_file_path = absolute(new_file, file_path.parent_path());
		new_file_path = canonical(new_file_path.parent_path()) / new_file_path.filename();
		return new_file_path.string();
	};

	auto vals =
	    process_include_directives(boost::filesystem::canonical(file_location).string(), eng, me,
	                               parsers::csv_rw_route::file_type::csv, get_abs_path);

	std::cout << vals.filenames.size() << '\n';
	std::cout << vals.lines.size() << '\n';

	preprocess_file(vals, eng, me, parsers::csv_rw_route::file_type::csv);

	auto instructions = generate_instructions(vals, me, parsers::csv_rw_route::file_type::csv);

	execute_instructions_pass1(instructions, me);
	auto route_data = execute_instructions_pass2(instructions, me);
	execute_instructions_pass3(route_data, instructions, me, get_abs_path);

	std::cout << instructions;
	std::cout << me;
}
