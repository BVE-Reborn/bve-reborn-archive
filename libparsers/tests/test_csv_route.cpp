#include "core/variant_util.hpp"
#include "csv_rw_route.hpp"
#include "tests.hpp"
#include <algorithm>
#include <array>
#include <experimental/filesystem>
#include <iostream>

using namespace std::string_literals;

template <std::size_t num, class... Args>
void print_sizes(const std::tuple<Args...>& val) {
	std::cout << typeid(std::get<num>(val)).name() << " " << sizeof(std::get<num>(val)) << '\n';
	if constexpr (num != 0) {
		print_sizes<num - 1>(val);
	}
}

void test_csv_route() {
	std::array<int, std::mt19937::state_size> seed_data;
	std::random_device r;
	std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

	std::mt19937 eng(seq);

	parsers::errors::multi_error me;

	auto file_location = "libparsers/tests/plymouth/1980s/HST (Non-Stopping) - Liskeard [c].csv";
	auto used_file = std::experimental::filesystem::path(file_location);

	auto vals = parsers::csv_rw_route::process_include_files(
	    std::experimental::filesystem::canonical(file_location).string(), eng, me,
	    [&](const std::string& base_file, const std::string& file) {
		    std::string new_file = file;
		    std::replace(new_file.begin(), new_file.end(), '\\', '/');
		    auto file_path = std::experimental::filesystem::path(base_file); //
		    auto new_file_path = std::experimental::filesystem::absolute(new_file, file_path.parent_path());
		    new_file_path =
		        std::experimental::filesystem::canonical(new_file_path.parent_path()) / new_file_path.filename();
		    return new_file_path.string();
	    });

	std::cout << vals.filenames.size() << '\n';
	std::cout << vals.lines.size() << '\n';

	parsers::csv_rw_route::preprocess_file(vals, eng, me);

	parsers::csv_rw_route::instruction val;

	openbve2::variant_util::print_sizes(std::cout, val);
}
