#include "tests.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "First argument must be test mode\n";
		return 1;
	}

	auto val = std::stoll(argv[1]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

	switch (val) {
		default:
		case 1:
			test_csv();
			break;
		case 2:
			test_csv_obj();
			break;
		case 3:
			test_animated();
			break;
		case 4:
			test_fs();
			break;
		case 5:
			test_csv_route();
			break;
		case 6:
			test_math();
			break;
	}
}
