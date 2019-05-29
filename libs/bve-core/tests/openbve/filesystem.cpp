#include "core/openbve/filesystem.hpp"
#include <doctest/doctest.h>
#include <foundational/logging/logger.hpp>

TEST_SUITE_BEGIN("libcore - openbve");

TEST_CASE("libcore - openbve - data_directory") {
	// We can't really test this... but let's run it anyway

	cppfs::FilePath appdata_path = bve::openbve::appdata_directory();
	cppfs::FilePath home_path = bve::openbve::home_directory();
	cppfs::FilePath data_path = bve::openbve::data_directory();

	CHECK_FALSE(appdata_path.isEmpty());
	CHECK_FALSE(home_path.isEmpty());
	CHECK_FALSE(data_path.isEmpty());

	foundational::logging::flush_all_backends();
}

TEST_SUITE_END();
