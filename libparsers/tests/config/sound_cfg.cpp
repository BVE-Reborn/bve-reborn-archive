#include "parsers/config/sound_cfg.hpp"
#include "parsers/errors.hpp"
#include "sample_relative_file_func.hpp"
#include <doctest.h>
#include <string>

using namespace std::string_literals;

namespace sound_cfg = parsers::config::sound_cfg;
namespace errors = parsers::errors;

TEST_SUITE_BEGIN("libparsers - config - sound");

TEST_CASE("libparsers - config - sound - [run]") {
	std::string input =
	    "[run]\n"
	    "0 = file1\n"
	    "4 = file2\n"
	    "3 = file3\n"
	    "129 = file4\n"
	    "-1 = file5\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg", input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 4);
	CHECK_EQ(result.filenames.count("sound.cfg/file1"), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/file2"), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/file3"), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/file4"), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/file5"), 0);
}

TEST_SUITE_END();