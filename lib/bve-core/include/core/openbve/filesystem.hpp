#pragma once

#include <cppfs/FilePath.h>

namespace bve::openbve {
	cppfs::FilePath home_directory();
	cppfs::FilePath appdata_directory();
	cppfs::FilePath data_directory();
} // namespace bve::openbve
