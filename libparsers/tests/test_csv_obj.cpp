#include "csv_object.hpp"
#include <fstream>
#include <iostream>

void test_csv_obj() {
	std::fstream f("tests/ind4.csv");

	if (f)
		std::cout << "yay!\n";

	std::string content((std::istreambuf_iterator<char>(f)), (std::istreambuf_iterator<char>()));

	auto instr = parsers::csv_object::create_instructions(content);

	for (auto& i : instr) {
		std::cout << i << '\n';
	}
}