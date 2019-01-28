#pragma once

#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <set>
#include <string>
#include <unordered_map>

namespace bve::parsers::config::sound_cfg {
	/**
	 * \brief Iterator into a sound.cfg's filename set.
	 */
	using FilenameIterator = std::set<std::string>::const_iterator;

	struct Run {
		FilenameIterator filename;
	};

	struct Flange {
		FilenameIterator filename;
	};

	struct Motor {
		FilenameIterator filename;
	};

	struct Switch {
		FilenameIterator filename;
	};

	struct Brake {
		FilenameIterator bc_release_high;
		FilenameIterator bc_release;
		FilenameIterator bc_release_full;
		FilenameIterator emergency;
		FilenameIterator bp_decompression;
	};

	struct Compressor {
		FilenameIterator attack;
		FilenameIterator loop;
		FilenameIterator release;
	};

	struct Suspension {
		FilenameIterator left;
		FilenameIterator right;
	};

	struct LoopedHorn {
		FilenameIterator primary_start;
		FilenameIterator primary_loop;
		FilenameIterator primary_end;

		FilenameIterator secondary_start;
		FilenameIterator secondary_loop;
		FilenameIterator secondary_end;

		FilenameIterator music_start;
		FilenameIterator music_loop;
		FilenameIterator music_end;
	};

	struct LegacyHorn {
		FilenameIterator primary;
		FilenameIterator secondary;
		FilenameIterator music;
	};

	/**
	 * \brief A horn_t is a variant with either a LoopedHorn or a LegacyHorn in it. There are
	 * mutually exclusive legacy and modern (looped) horn configurations with the modern one
	 * takes priority.
	 */
	using Horn = mapbox::util::variant<LoopedHorn, LegacyHorn>;

	struct Doors {
		FilenameIterator open_left;
		FilenameIterator close_left;

		FilenameIterator open_right;
		FilenameIterator close_right;
	};

	struct Ats {
		FilenameIterator filename;
	};

	struct Buzzer {
		FilenameIterator correct;
	};

	struct PilotLamp {
		FilenameIterator on;
		FilenameIterator off;
	};

	struct BrakeHandle {
		FilenameIterator apply;
		FilenameIterator release;
		FilenameIterator min;
		FilenameIterator max;
	};

	struct MasterController {
		FilenameIterator up;
		FilenameIterator down;
		FilenameIterator min;
		FilenameIterator max;
	};

	struct Reverser {
		FilenameIterator on;
		FilenameIterator off;
	};

	struct Breaker {
		FilenameIterator on;
		FilenameIterator off;
	};

	struct Misc {
		FilenameIterator noise;
		FilenameIterator shoe;
	};

	struct ParsedSoundCFG {
		ParsedSoundCFG() = default;
		ParsedSoundCFG(ParsedSoundCFG const& other) = delete;
		ParsedSoundCFG(ParsedSoundCFG&& other) = default;
		ParsedSoundCFG& operator=(ParsedSoundCFG const& other) = delete;
		ParsedSoundCFG& operator=(ParsedSoundCFG&& other) = default;

		/**
		 * \brief Master set of all filenames in this file. Unique. Every single file is used by at least one setting.
		 */
		std::set<std::string> filenames;

		std::unordered_map<std::size_t, Run> run_sounds;
		std::unordered_map<std::size_t, Flange> flange_sounds;
		std::unordered_map<std::size_t, Motor> motor_sounds;
		std::unordered_map<std::size_t, Switch> switch_sounds;
		Brake brake_sounds;
		Compressor compressor_sounds;
		Suspension suspension_sounds;
		Horn horn_sounds;
		Doors door_sounds;
		std::unordered_map<std::size_t, Ats> ats_sounds;
		Buzzer buzzer_sounds;
		PilotLamp pilot_lamp_sounds;
		BrakeHandle brake_handle_sounds;
		MasterController master_controller_sounds;
		Reverser reverser_sounds;
		Breaker breaker_sounds;
		Misc misc_sounds;
	};

	/**
	 * \brief Parses a sound.cfg configuration file into a structure.
	 * \param filename Source filename of the file. The file will not be accessed. Only used to get relative file names.
	 * \param input_string Contents of the file to be parsed.
	 * \param errors All errors encountered while parsing will be put into errors[filename].
	 * \param get_relative_file Function that takes this file's filepath and a relative path and returns the path relative to this filepath's directory
	 * \return Parsed structure containing contents of the file.
	 */
	ParsedSoundCFG parse(std::string const& filename,
	                     std::string const& input_string,
	                     errors::MultiError& errors,
	                     RelativeFileFunc const& get_relative_file);
} // namespace bve::parsers::config::sound_cfg
