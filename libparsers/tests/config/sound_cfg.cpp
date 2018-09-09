#include "parsers/config/sound_cfg.hpp"
#include "parsers/errors.hpp"
#include "sample_relative_file_func.hpp"
#include <doctest.h>
#include <string>

using namespace std::string_literals;

namespace sound_cfg = parsers::config::sound_cfg;
namespace errors = parsers::errors;

TEST_SUITE_BEGIN("libparsers - config - sound");

#define CHECK_ITER_EQ(container, itr, value)                                                       \
	CHECK((((itr) != (container).end()) ? (*(itr) == (value)) : false))

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

NUMBERED_PAIR_TEST(run)
NUMBERED_PAIR_TEST(flange)
NUMBERED_PAIR_TEST(motor)
NUMBERED_PAIR_TEST(switch)
NUMBERED_PAIR_TEST(ats)

TEST_CASE("libparsers - config - sound - version") {
	std::string const input = "Version 1.0\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	CHECK_EQ(me["sound.cfg"s].size(), 0);
}

TEST_CASE("libparsers - config - sound - version - invalid") {
	std::string const input = "Version 2.2\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	CHECK_EQ(me["sound.cfg"s].size(), 1);
}

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

	CHECK_ITER_EQ(result.filenames, result.brake_sounds.bc_release_high, "sound.cfg/bc/rel/high"s);
	CHECK_ITER_EQ(result.filenames, result.brake_sounds.bc_release, "sound.cfg/bc/rel/norm"s);
	CHECK_ITER_EQ(result.filenames, result.brake_sounds.bc_release_full, "sound.cfg/bc/rel/full"s);
	CHECK_ITER_EQ(result.filenames, result.brake_sounds.emergency, "sound.cfg/emer"s);
	CHECK_ITER_EQ(result.filenames, result.brake_sounds.bp_decompression, "sound.cfg/bp/decomp"s);
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

	CHECK_ITER_EQ(result.filenames, result.compressor_sounds.attack, "sound.cfg/comp/att"s);
	CHECK_ITER_EQ(result.filenames, result.compressor_sounds.loop, "sound.cfg/comp/loop"s);
	CHECK_ITER_EQ(result.filenames, result.compressor_sounds.release, "sound.cfg/comp/release"s);
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

	CHECK_ITER_EQ(result.filenames, result.suspension_sounds.left, "sound.cfg/sus/left"s);
	CHECK_ITER_EQ(result.filenames, result.suspension_sounds.right, "sound.cfg/sus/right"s);
}

TEST_CASE("libparsers - config - sound - [horn] - looped horn") {
	std::string const input =
	    "[horn]\n"
	    "PrimaryStart = primary/start\n"
	    "SecondaryStart = secondary/start\n"
	    "MusicStart = music/start\n"

	    "PrimaryLoop = primary/loop\n"
	    "SecondaryLoop = secondary/loop\n"
	    "MusicLoop = music/loop\n"

	    "PrimaryEnd = primary/end\n"
	    "SecondaryEnd = secondary/end\n"
	    "MusicEnd = music/end\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 9);
	CHECK_EQ(result.filenames.count("sound.cfg/primary/start"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/secondary/start"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/music/start"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/primary/loop"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/secondary/loop"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/music/loop"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/primary/end"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/secondary/end"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/music/end"s), 1);

	REQUIRE(result.horn_sounds.is<sound_cfg::looped_horn_t>());
	auto& looped_horn = result.horn_sounds.get_unchecked<sound_cfg::looped_horn_t>();

	CHECK_ITER_EQ(result.filenames, looped_horn.primary_start, "sound.cfg/primary/start"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.secondary_start, "sound.cfg/secondary/start"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.music_start, "sound.cfg/music/start"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.primary_loop, "sound.cfg/primary/loop"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.secondary_loop, "sound.cfg/secondary/loop"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.music_loop, "sound.cfg/music/loop"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.primary_end, "sound.cfg/primary/end"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.secondary_end, "sound.cfg/secondary/end"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.music_end, "sound.cfg/music/end"s);
}

TEST_CASE("libparsers - config - sound - [horn] - legacy horn") {
	std::string const input =
	    "[horn]\n"
	    "Primary = legacy/primary\n"
	    "Secondary = legacy/secondary\n"
	    "Music = legacy/music\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 3);
	CHECK_EQ(result.filenames.count("sound.cfg/legacy/primary"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/legacy/secondary"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/legacy/music"s), 1);

	REQUIRE(result.horn_sounds.is<sound_cfg::legacy_horn_t>());
	auto& legacy_horn = result.horn_sounds.get_unchecked<sound_cfg::legacy_horn_t>();

	CHECK_ITER_EQ(result.filenames, legacy_horn.primary, "sound.cfg/legacy/primary"s);
	CHECK_ITER_EQ(result.filenames, legacy_horn.secondary, "sound.cfg/legacy/secondary"s);
	CHECK_ITER_EQ(result.filenames, legacy_horn.music, "sound.cfg/legacy/music"s);
}

TEST_CASE("libparsers - config - sound - [horn] - looped horn priority") {
	std::string const input =
	    "[horn]\n"
	    "PrimaryStart = primary/start\n"
	    "SecondaryStart = secondary/start\n"
	    "MusicStart = music/start\n"

	    "PrimaryLoop = primary/loop\n"
	    "SecondaryLoop = secondary/loop\n"
	    "MusicLoop = music/loop\n"

	    "PrimaryEnd = primary/end\n"
	    "SecondaryEnd = secondary/end\n"
	    "MusicEnd = music/end\n"

	    "Primary = legacy/primary\n"
	    "Secondary = legacy/secondary\n"
	    "Music = legacy/music\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	CHECK_EQ(me["sound.cfg"s].size(), 3);

	REQUIRE_EQ(result.filenames.size(), 9);
	CHECK_EQ(result.filenames.count("sound.cfg/primary/start"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/secondary/start"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/music/start"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/primary/loop"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/secondary/loop"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/music/loop"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/primary/end"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/secondary/end"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/music/end"s), 1);

	REQUIRE(result.horn_sounds.is<sound_cfg::looped_horn_t>());
	auto& looped_horn = result.horn_sounds.get_unchecked<sound_cfg::looped_horn_t>();

	CHECK_ITER_EQ(result.filenames, looped_horn.primary_start, "sound.cfg/primary/start"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.secondary_start, "sound.cfg/secondary/start"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.music_start, "sound.cfg/music/start"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.primary_loop, "sound.cfg/primary/loop"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.secondary_loop, "sound.cfg/secondary/loop"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.music_loop, "sound.cfg/music/loop"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.primary_end, "sound.cfg/primary/end"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.secondary_end, "sound.cfg/secondary/end"s);
	CHECK_ITER_EQ(result.filenames, looped_horn.music_end, "sound.cfg/music/end"s);
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

	CHECK_ITER_EQ(result.filenames, result.suspension_sounds.left, "sound.cfg/sus/left"s);
	CHECK_ITER_EQ(result.filenames, result.suspension_sounds.right, "sound.cfg/sus/right"s);
}

TEST_CASE("libparsers - config - sound - [door]") {
	std::string const input =
	    "[door]\n"
	    "Open Left = open/left\n"
	    "Open Right = open/right\n"
	    "Close Left = close/left\n"
	    "Close Right = close/right\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 4);
	CHECK_EQ(result.filenames.count("sound.cfg/open/left"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/open/right"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/close/left"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/close/right"s), 1);

	CHECK_ITER_EQ(result.filenames, result.door_sounds.open_left, "sound.cfg/open/left"s);
	CHECK_ITER_EQ(result.filenames, result.door_sounds.open_right, "sound.cfg/open/right"s);
	CHECK_ITER_EQ(result.filenames, result.door_sounds.close_left, "sound.cfg/close/left"s);
	CHECK_ITER_EQ(result.filenames, result.door_sounds.close_right, "sound.cfg/close/right"s);
}

TEST_CASE("libparsers - config - sound - [buzzer]") {
	std::string const input =
	    "[buzzer]\n"
	    "correct = cor/rect\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/cor/rect"s), 1);

	CHECK_ITER_EQ(result.filenames, result.buzzer_sounds.correct, "sound.cfg/cor/rect"s);
}

TEST_CASE("libparsers - config - sound - [pilot lamp]") {
	std::string const input =
	    "[pilot lamp]\n"
	    "on = pl/on\n"
	    "off = pl/off\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 2);
	CHECK_EQ(result.filenames.count("sound.cfg/pl/on"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/pl/off"s), 1);

	CHECK_ITER_EQ(result.filenames, result.pilot_lamp_sounds.on, "sound.cfg/pl/on"s);
	CHECK_ITER_EQ(result.filenames, result.pilot_lamp_sounds.off, "sound.cfg/pl/off"s);
}

TEST_CASE("libparsers - config - sound - [brake handle]") {
	std::string const input =
	    "[brake handle]\n"
	    "apply = bh/apply\n"
	    "release = bh/release\n"
	    "min = bh/min\n"
	    "max = bh/max\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 4);
	CHECK_EQ(result.filenames.count("sound.cfg/bh/apply"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/bh/release"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/bh/min"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/bh/max"s), 1);

	CHECK_ITER_EQ(result.filenames, result.brake_handle_sounds.apply, "sound.cfg/bh/apply"s);
	CHECK_ITER_EQ(result.filenames, result.brake_handle_sounds.release, "sound.cfg/bh/release"s);
	CHECK_ITER_EQ(result.filenames, result.brake_handle_sounds.min, "sound.cfg/bh/min"s);
	CHECK_ITER_EQ(result.filenames, result.brake_handle_sounds.max, "sound.cfg/bh/max"s);
}

TEST_CASE("libparsers - config - sound - [master controller]") {
	std::string const input =
	    "[master controller]\n"
	    "up = mc/up\n"
	    "down = mc/down\n"
	    "min = mc/min\n"
	    "max = mc/max\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 4);
	CHECK_EQ(result.filenames.count("sound.cfg/mc/up"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/mc/down"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/mc/min"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/mc/max"s), 1);

	CHECK_ITER_EQ(result.filenames, result.master_controller_sounds.up, "sound.cfg/mc/up"s);
	CHECK_ITER_EQ(result.filenames, result.master_controller_sounds.down, "sound.cfg/mc/down"s);
	CHECK_ITER_EQ(result.filenames, result.master_controller_sounds.min, "sound.cfg/mc/min"s);
	CHECK_ITER_EQ(result.filenames, result.master_controller_sounds.max, "sound.cfg/mc/max"s);
}

TEST_CASE("libparsers - config - sound - [reverser]") {
	std::string const input =
	    "[reverser]\n"
	    "on = rv/on\n"
	    "off = rv/off\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 2);
	CHECK_EQ(result.filenames.count("sound.cfg/rv/on"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/rv/off"s), 1);

	CHECK_ITER_EQ(result.filenames, result.reverser_sounds.on, "sound.cfg/rv/on"s);
	CHECK_ITER_EQ(result.filenames, result.reverser_sounds.off, "sound.cfg/rv/off"s);
}

TEST_CASE("libparsers - config - sound - [breaker]") {
	std::string const input =
	    "[breaker]\n"
	    "on = bk/on\n"
	    "off = bk/off\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 2);
	CHECK_EQ(result.filenames.count("sound.cfg/bk/on"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/bk/off"s), 1);

	CHECK_ITER_EQ(result.filenames, result.breaker_sounds.on, "sound.cfg/bk/on"s);
	CHECK_ITER_EQ(result.filenames, result.breaker_sounds.off, "sound.cfg/bk/off"s);
}

TEST_CASE("libparsers - config - sound - [others]") {
	std::string const input =
	    "[others]\n"
	    "noise = misc/noise\n"
	    "shoe = misc/shoe\n"s;

	errors::multi_error_t me;

	auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);

	REQUIRE_EQ(result.filenames.size(), 2);
	CHECK_EQ(result.filenames.count("sound.cfg/misc/noise"s), 1);
	CHECK_EQ(result.filenames.count("sound.cfg/misc/shoe"s), 1);

	CHECK_ITER_EQ(result.filenames, result.misc_sounds.noise, "sound.cfg/misc/noise"s);
	CHECK_ITER_EQ(result.filenames, result.misc_sounds.shoe, "sound.cfg/misc/shoe"s);
}

#define INVALID_KEY_TEST(name)                                                                       \
	TEST_CASE("libparsers - config - sound - [" STR(name) "] - invalid key") {                       \
		std::string const input =                                                                  \
	    "[" STR(name) "]\n"                                                                        \
	    "thisdoesntmakesence = misc/noise\n"s; \
                                                                                                     \
		errors::multi_error_t me;                                                                    \
                                                                                                     \
		auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);                \
                                                                                                     \
		CHECK_EQ(result.filenames.size(), 0);                                                        \
		CHECK_EQ(me["sound.cfg"s].size(), 1);                                                        \
	}

INVALID_KEY_TEST(run)
INVALID_KEY_TEST(flange)
INVALID_KEY_TEST(motor)
INVALID_KEY_TEST(switch)
INVALID_KEY_TEST(brake)
INVALID_KEY_TEST(compressor)
INVALID_KEY_TEST(suspension)
INVALID_KEY_TEST(horn)
INVALID_KEY_TEST(door)
INVALID_KEY_TEST(ats)
INVALID_KEY_TEST(buzzer)
INVALID_KEY_TEST(pilot lamp)
INVALID_KEY_TEST(brake handle)
INVALID_KEY_TEST(master controller)
INVALID_KEY_TEST(reverser)
INVALID_KEY_TEST(breaker)
INVALID_KEY_TEST(others)

#define DUPLICATE_KEY_TEST(name, key, geteq)                                                       \
	TEST_CASE("libparsers - config - sound - [" STR(name) "] - duplicate key removal") {           \
		std::string const input = /**/                                                             \
		    "[" STR(name) "]\n"   /**/                                                             \
		    STR(key) " = orig\n"  /**/                                                             \
		    STR(key) " = repl\n"s;                                                                 \
                                                                                                   \
		errors::multi_error_t me;                                                                  \
                                                                                                   \
		auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);              \
                                                                                                   \
		REQUIRE_EQ(result.filenames.size(), 1);                                                    \
		CHECK_EQ(result.filenames.count("sound.cfg/repl"s), 1);                                    \
		CHECK_EQ(me["sound.cfg"].size(), 1);                                                       \
                                                                                                   \
		CHECK_ITER_EQ(result.filenames, (geteq), "sound.cfg/repl"s);                               \
	}

#define DUPLICATE_KEY_TEST_INDEXED(name)                                                           \
	DUPLICATE_KEY_TEST(name, 0,                                                                    \
	                   ((result.CONCAT(name, _sounds).find(0)                                      \
	                     != result.CONCAT(name, _sounds).end())                                    \
	                        ? result.CONCAT(name, _sounds).find(0)->second.filename                \
	                        : result.filenames.end()))

DUPLICATE_KEY_TEST_INDEXED(run)
DUPLICATE_KEY_TEST_INDEXED(flange)
DUPLICATE_KEY_TEST_INDEXED(motor)
DUPLICATE_KEY_TEST_INDEXED(switch)
DUPLICATE_KEY_TEST_INDEXED(ats)

DUPLICATE_KEY_TEST(brake, emergency, result.brake_sounds.emergency);
DUPLICATE_KEY_TEST(compressor, attack, result.compressor_sounds.attack)
DUPLICATE_KEY_TEST(suspension, left, result.suspension_sounds.left)
DUPLICATE_KEY_TEST(horn,
                   primarystart,
                   result.horn_sounds.get<sound_cfg::looped_horn_t>().primary_start)
DUPLICATE_KEY_TEST(door, open left, result.door_sounds.open_left)
DUPLICATE_KEY_TEST(buzzer, correct, result.buzzer_sounds.correct)
DUPLICATE_KEY_TEST(pilot lamp, on, result.pilot_lamp_sounds.on)
DUPLICATE_KEY_TEST(brake handle, apply, result.brake_handle_sounds.apply)
DUPLICATE_KEY_TEST(master controller, up, result.master_controller_sounds.up)
DUPLICATE_KEY_TEST(reverser, on, result.reverser_sounds.on)
DUPLICATE_KEY_TEST(breaker, on, result.breaker_sounds.on)
DUPLICATE_KEY_TEST(others, noise, result.misc_sounds.noise)

#define DUPLICATE_SECTION_TEST(name, key, geteq)                                                     \
	TEST_CASE("libparsers - config - sound - [" STR(name) "] - duplicate key removal") {             \
		std::string const input = /**/                                                             \
		    "[" STR(name) "]\n"   /**/                                                             \
		    STR(key) " = orig\n"  /**/                                                             \
		    "[" STR(name) "]\n"   /**/                                                             \
		    STR(key) " = repl\n"s; \
                                                                                                     \
		errors::multi_error_t me;                                                                    \
                                                                                                     \
		auto const result = sound_cfg::parse("sound.cfg"s, input, me, rel_file_func);                \
                                                                                                     \
		REQUIRE_EQ(result.filenames.size(), 1);                                                      \
		CHECK_EQ(result.filenames.count("sound.cfg/repl"s), 1);                                      \
		CHECK_EQ(me["sound.cfg"].size(), 1);                                                         \
                                                                                                     \
		CHECK_ITER_EQ(result.filenames, (geteq), "sound.cfg/repl"s);                                 \
	}

#define DUPLICATE_SECTION_TEST_INDEXED(name)                                                       \
	DUPLICATE_KEY_TEST(name, 0,                                                                    \
	                   ((result.CONCAT(name, _sounds).find(0)                                      \
	                     != result.CONCAT(name, _sounds).end())                                    \
	                        ? result.CONCAT(name, _sounds).find(0)->second.filename                \
	                        : result.filenames.end()))

DUPLICATE_SECTION_TEST_INDEXED(run)
DUPLICATE_SECTION_TEST_INDEXED(flange)
DUPLICATE_SECTION_TEST_INDEXED(motor)
DUPLICATE_SECTION_TEST_INDEXED(switch)
DUPLICATE_SECTION_TEST_INDEXED(ats)

DUPLICATE_SECTION_TEST(brake, emergency, result.brake_sounds.emergency)
DUPLICATE_SECTION_TEST(compressor, attack, result.compressor_sounds.attack)
DUPLICATE_SECTION_TEST(suspension, left, result.suspension_sounds.left)
DUPLICATE_SECTION_TEST(horn,
                       primarystart,
                       result.horn_sounds.get<sound_cfg::looped_horn_t>().primary_start)
DUPLICATE_SECTION_TEST(door, open left, result.door_sounds.open_left)
DUPLICATE_SECTION_TEST(buzzer, correct, result.buzzer_sounds.correct)
DUPLICATE_SECTION_TEST(pilot lamp, on, result.pilot_lamp_sounds.on)
DUPLICATE_SECTION_TEST(brake handle, apply, result.brake_handle_sounds.apply)
DUPLICATE_SECTION_TEST(master controller, up, result.master_controller_sounds.up)
DUPLICATE_SECTION_TEST(reverser, on, result.reverser_sounds.on)
DUPLICATE_SECTION_TEST(breaker, on, result.breaker_sounds.on)
DUPLICATE_SECTION_TEST(others, noise, result.misc_sounds.noise)

TEST_SUITE_END();
