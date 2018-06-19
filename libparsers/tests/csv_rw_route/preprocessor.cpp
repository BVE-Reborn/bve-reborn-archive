#include "csv_rw_route.hpp"
#include <doctest.h>
#include <fstream>

using namespace std::string_literals;
namespace cs = parsers::csv_rw_route;

std::string file_func(std::string const& base, std::string const& rel) {
	return base + "/" + rel;
}

void write_to_file(std::string const& directive) {
	std::fstream file("directive.csv");
	file.write(directive.c_str(), sizeof(char)*directive.size());
}

void print_file(){
	std::fstream file("directive.csv");
	std::string direct{};
	std::string x{};
	while(file >> direct){
		x += direct;
	}

}

TEST_SUITE_BEGIN("libparser - csv_rw_route - preprocessor");

TEST_CASE("libparsers - csv_rw_route - preprocessor - $char") {
	std::string test_command =
	    "\n$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78),\n$Chr(75),$Chr(69),$Chr(86),$Chr(73),$Chr(78)"s;
	write_to_file(test_command);
	print_file();
	parsers::errors::multi_error_t output_errors;
	int const seed = 1;
	auto rng = openbve2::datatypes::rng{seed};
	auto processed = cs::process_include_directives("directive.csv"s, rng, output_errors,
	                                                cs::file_type::csv, file_func);
	cs::preprocess_file(processed, rng, output_errors, cs::file_type::csv);

}