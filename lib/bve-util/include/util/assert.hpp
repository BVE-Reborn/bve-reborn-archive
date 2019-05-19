#pragma once

#include "util/macro_helpers.hpp"
#include <cstdlib>

#define BVE_ASSERT(condition, msg)                                                                                                   \
	if (!(condition)) {                                                                                                              \
		::std::fprintf(stderr, "%s %s",                                                                                              \
		               "BVE Assertion Failed in " __FILE__ ":" STR(__LINE__) ". Expression " STR(condition) " with message: ", msg); \
		::std::abort();                                                                                                              \
	}
