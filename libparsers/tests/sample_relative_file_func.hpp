#pragma once

#include "core/inlining_util.hpp"
#include <string>

FORCE_INLINE std::string rel_file_func(std::string base, std::string rel) {
	return base + "/" + rel;
}
