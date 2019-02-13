#pragma once

#include "core/inlining_util.hpp"
#include <string>

FORCE_INLINE std::string rel_file_func(const std::string& base, const std::string& rel) {
	return base + "/" + rel;
}
