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

	parsers::errors::multi_error me;

	// auto file_location = DIRECTORY
	//     "../libparsers/tests/test_files/plymouth/1980s/HST (Non-Stopping) - "
	//     "Liskeard [c].csv";
	auto file_location =
	    DIRECTORY "../libparsers/tests/test_files/NYCT- R [Astoria to 95 St - 4th Av].csv"s;
	//	auto file_location = DIRECTORY "../libparsers/tests/Xmasmonorail.csv";
	// Bridge NTT [v1.0].csv";
	auto used_file = boost::filesystem::path(file_location);

	auto get_abs_path = [&](const std::string& base_file, const std::string& file) {
		std::string new_file = file;
		std::replace(new_file.begin(), new_file.end(), '\\', '/');
		auto file_path = boost::filesystem::path(base_file); //
		auto new_file_path = boost::filesystem::absolute(new_file, file_path.parent_path());
		new_file_path =
		    boost::filesystem::canonical(new_file_path.parent_path()) / new_file_path.filename();
		return new_file_path.string();
	};

	auto vals =
	    parsers::csv_rw_route::process_include_directives(boost::filesystem::canonical(
	                                                          file_location)
	                                                          .string(),
	                                                      eng, me,
	                                                      parsers::csv_rw_route::file_type::csv,
	                                                      get_abs_path);

	std::cout << vals.filenames.size() << '\n';
	std::cout << vals.lines.size() << '\n';

	parsers::csv_rw_route::preprocess_file(vals, eng, me, parsers::csv_rw_route::file_type::csv);

	auto instructions =
	    parsers::csv_rw_route::generate_instructions(vals, me,
	                                                 parsers::csv_rw_route::file_type::csv);

	parsers::csv_rw_route::execute_instructions_pass1(instructions, me);
	auto route_data = parsers::csv_rw_route::execute_instructions_pass2(instructions, me);
	parsers::csv_rw_route::execute_instructions_pass3(route_data, instructions, me, get_abs_path);

	std::cout << instructions;
	std::cout << me;
}