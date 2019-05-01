#pragma once

#include <cppfs/FilePath.h>
#include <foundational/container/std_pmr/string.hpp>
#include <util/context.hpp>

namespace bve::openbve {
	cppfs::FilePath home_directory();
	cppfs::FilePath appdata_directory();
	cppfs::FilePath data_directory();
	cppfs::FilePath cwd();
	cppfs::FilePath absolute(cppfs::FilePath const& path);

} // namespace bve::openbve
