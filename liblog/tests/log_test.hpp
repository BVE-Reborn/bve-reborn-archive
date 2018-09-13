#pragma once

#include "core/inlining_util.hpp"
#include <regex>

// Log regex
using namespace std::string_literals;

static std::regex log_regex(
    "[0-9]{4}.[01][0-9].[0-3][0-9] [0-2][0-9]:[0-5][0-9]:[0-6][0-9].[0-9]{3}: "
    "(debug|info|note|warning|error|fatal_error): (?:([^:\\s]+):(\\d+): )?(.*)"s);

#ifdef _MSC_VER
#include <stdio.h>
#include <string>
FORCE_INLINE std::string create_temp_file() {
	std::string s;
	s.resize(100);
	tmpnam_s(s.data(), 100);

	return s;
}
#else
#	include <stdlib.h>
FORCE_INLINE inline std::string create_temp_file() {
	char c[] = "/tmp/bvereborntmpXXXXXX";
	mkstemp(c);
	return c;
}
#endif
