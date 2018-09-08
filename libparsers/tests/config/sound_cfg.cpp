#include "parsers/config/sound_cfg.hpp"
#include "parsers/errors.hpp"
#include "sample_relative_file_func.hpp"
#include <doctest.h>
#include <string>

using namespace std::string_literals;

namespace sound_cfg = parsers::config::sound_cfg;
namespace errors = parsers::errors;

TEST_SUITE_BEGIN("libparsers - config - sound");

#define EXPAND(x) x
#define STR(x) #x
#define CONCAT_HELPER(a, b) a##b
#define CONCAT(a, b) EXPAND(CONCAT_HELPER(a, b))
#define NUMBERED_PAIR_TEST(name)                                                                       \
	TEST_CASE("libparsers - config - sound - [" STR(run) "]") {                                        \
		std::string const input =                                                                    \
		    "[" STR(name) "]\n"                                                                       \
		    "0 = file1\n"                                                                            \
		    "4 = file2\n"                                                                            \
		    "3 = file3\n"                                                                            \
		    "129 = file4\n"                                                                          \
		    "-1 = file5\n"s; \
                                                                                                       \
		errors::multi_error_t me;                                                                      \
                                                                                                       \
		auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);                  \
                                                                                                       \
		REQUIRE_EQ(result.filenames.size(), 4);                                                        \
		CHECK_EQ(result.filenames.count("sound.cfg/file1"s), 1);                                       \
		CHECK_EQ(result.filenames.count("sound.cfg/file2"s), 1);                                       \
		CHECK_EQ(result.filenames.count("sound.cfg/file3"s), 1);                                       \
		CHECK_EQ(result.filenames.count("sound.cfg/file4"s), 1);                                       \
		CHECK_EQ(result.filenames.count("sound.cfg/file5"s), 0);                                       \
                                                                                                       \
		REQUIRE_EQ(result.CONCAT(name, _sounds).size(), 4);                                            \
		CHECK_EQ(result.CONCAT(name, _sounds).find(0)->second.filename,                                \
		         result.filenames.find("sound.cfg/file1"s));                                           \
		CHECK_EQ(result.CONCAT(name, _sounds).find(4)->second.filename,                                \
		         result.filenames.find("sound.cfg/file2"s));                                           \
		CHECK_EQ(result.CONCAT(name, _sounds).find(3)->second.filename,                                \
		         result.filenames.find("sound.cfg/file3"s));                                           \
		CHECK_EQ(result.CONCAT(name, _sounds).find(129)->second.filename,                              \
		         result.filenames.find("sound.cfg/file4"s));                                           \
	}

NUMBERED_PAIR_TEST(run);
NUMBERED_PAIR_TEST(flange);
NUMBERED_PAIR_TEST(motor);
NUMBERED_PAIR_TEST(switch);
NUMBERED_PAIR_TEST(ats);

TEST_CASE("libparsers - config - sound - [brake]") {
	std::string const input =
	    "[brake]\n"
	    "BC Release High = bc/rel/high\n"
	    "BC Release = bc/rel/norm\n"
	    "BC Release Full = bc/rel/full\n"
	    "Emergency = emer\n"
	    "BP Decomp = bp/decomp\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 5);
	CHECK_EQ(result.filenames.count("sound.cfg/bc/rel/high"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/bc/rel/norm"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/bc/rel/full"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/emer"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/bp/decomp"s), 1);

	CHECK_EQ(*result.brake_sounds.bc_release_high, "sound.cfg/bc/rel/high"s);
	CHECK_EQ(*result.brake_sounds.bc_release, "sound.cfg/bc/rel/norm"s);
	CHECK_EQ(*result.brake_sounds.bc_release_full, "sound.cfg/bc/rel/full"s);
	CHECK_EQ(*result.brake_sounds.emergency, "sound.cfg/emer"s);
	CHECK_EQ(*result.brake_sounds.bp_decompression, "sound.cfg/bp/decomp"s);
}

TEST_CASE("libparsers - config - sound - [compressor]") {
	std::string const input =
	    "[compressor]\n"
	    "Attack = comp/att\n"
	    "Loop = comp/loop\n"
	    "Release = comp/release\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 3);
	CHECK_EQ(result.filenames.count("sound.cfg/comp/att"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/comp/loop"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/comp/release"s), 1);

	CHECK_EQ(*result.compressor_sounds.attack, "sound.cfg/comp/att"s);
	CHECK_EQ(*result.compressor_sounds.loop, "sound.cfg/comp/loop"s);
	CHECK_EQ(*result.compressor_sounds.release, "sound.cfg/comp/release"s);
}

TEST_CASE("libparsers - config - sound - [suspension]") {
	std::string const input =
	    "[suspension]\n"
	    "Left = sus/left\n"
	    "Right = sus/right\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 2);
	CHECK_EQ(result.filenames.count("sound.cfg/sus/left"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/sus/right"s), 1);

	CHECK_EQ(*result.suspension_sounds.left, "sound.cfg/sus/left"s);
	CHECK_EQ(*result.suspension_sounds.right, "sound.cfg/sus/right"s);
}

TEST_SUITE_END();
