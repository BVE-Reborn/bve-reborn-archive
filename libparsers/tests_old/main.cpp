#include "old_tests_config.hpp"
#include "tests.hpp"
#include <iostream>
#include <string>

int main(int const argc, char** argv) {
	if (argc != 2) {
		std::cout << "First argument must be test mode\n";
		return 1;
	}

	auto const val = std::stoll(argv[1]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

	switch (val) {
		case 1:
			test_csv();
			break;
#ifdef BVEREBORN_WITH_OLD_TESTS
		case 2:
			test_csv_obj();
			break;
		case 3:
			test_animated();
			break;
#endif
		case 4:
			test_fs();
			break;
#ifdef BVEREBORN_WITH_OLD_TESTS
		case 5:
			test_csv_route();
			break;
#endif
		case 6:
			test_math();
			break;
		default:
			std::cout << "Not a valid test number. Try: ";
#ifdef BVEREBORN_WITH_OLD_TESTS
			std::cout << "1, 2, 3, 4, 5, 6\n";
#else
			std::cout << "1, 4, 6\n";
#endif
	}
}
