#pragma once

#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <set>
#include <string>
#include <unordered_map>

namespace parsers::config::sound_cfg {
	/**
	 * \brief Iterator into a sound.cfg's filename set.
	 */
	using filename_iterator = std::set<std::string>::const_iterator;

	struct run_t {
		filename_iterator filename;
	};

	struct flange_t {
		filename_iterator filename;
	};

	struct motor_t {
		filename_iterator filename;
	};

	struct switch_t {
		filename_iterator filename;
	};

	struct brake_t {
		filename_iterator bc_release_high;
		filename_iterator bc_release;
		filename_iterator bc_release_full;
		filename_iterator emergency;
		filename_iterator bp_decompression;
	};

	struct compressor_t {
		filename_iterator attack;
		filename_iterator loop;
		filename_iterator release;
	};

	struct suspension_t {
		filename_iterator left;
		filename_iterator right;
	};

	struct looped_horn_t {
		filename_iterator primary_start;
		filename_iterator primary_loop;
		filename_iterator primary_end;

		filename_iterator secondary_start;
		filename_iterator secondary_loop;
		filename_iterator secondary_end;

		filename_iterator music_start;
		filename_iterator music_loop;
		filename_iterator music_end;
	};

	struct legacy_horn_t {
		filename_iterator primary;
		filename_iterator secondary;
		filename_iterator music;
	};

	/**
	 * \brief A horn_t is a variant with either a looped_horn_t or a legacy_horn_t in it. There are
	 * mutually exclusive legacy and modern (looped) horn configurations with the modern one
	 * takes priority.
	 */
	using horn_t = mapbox::util::variant<looped_horn_t, legacy_horn_t>;

	struct doors_t {
		filename_iterator open_left;
		filename_iterator close_left;

		filename_iterator open_right;
		filename_iterator close_right;
	};

	struct ats_t {
		filename_iterator filename;
	};

	struct buzzer_t {
		filename_iterator correct;
	};

	struct pilot_lamp_t {
		filename_iterator on;
		filename_iterator off;
	};

	struct brake_handle_t {
		filename_iterator apply;
		filename_iterator release;
		filename_iterator min;
		filename_iterator max;
	};

	struct master_controller_t {
		filename_iterator up;
		filename_iterator down;
		filename_iterator min;
		filename_iterator max;
	};

	struct reverser_t {
		filename_iterator on;
		filename_iterator off;
	};

	struct breaker_t {
		filename_iterator on;
		filename_iterator off;
	};

	struct misc_t {
		filename_iterator noise;
		filename_iterator shoe;
	};

	struct parsed_sound_cfg_t {
		parsed_sound_cfg_t() = default;
		parsed_sound_cfg_t(parsed_sound_cfg_t const& other) = delete;
		parsed_sound_cfg_t(parsed_sound_cfg_t&& other) = default;
		parsed_sound_cfg_t& operator=(parsed_sound_cfg_t const& other) = delete;
		parsed_sound_cfg_t& operator=(parsed_sound_cfg_t&& other) = default;

		/**
		 * \brief Master set of all filenames in this file. Unique. Every single file is used by at least one setting.
		 */
		std::set<std::string> filenames;

		std::unordered_map<std::size_t, run_t> run_sounds;
		std::unordered_map<std::size_t, flange_t> flange_sounds;
		std::unordered_map<std::size_t, motor_t> motor_sounds;
		std::unordered_map<std::size_t, switch_t> switch_sounds;
		brake_t brake_sounds;
		compressor_t compressor_sounds;
		suspension_t suspension_sounds;
		horn_t horn_sounds;
		doors_t door_sounds;
		std::unordered_map<std::size_t, ats_t> ats_sounds;
		buzzer_t buzzer_sounds;
		pilot_lamp_t pilot_lamp_sounds;
		brake_handle_t brake_handle_sounds;
		master_controller_t master_controller_sounds;
		reverser_t reverser_sounds;
		breaker_t breaker_sounds;
		misc_t misc_sounds;
	};

	/**
	 * \brief Parses a sound.cfg configuration file into a structure.
	 * \param filename Source filename of the file. The file will not be accessed. Only used to get relative file names.
	 * \param input_string Contents of the file to be parsed.
	 * \param errors All errors encountered while parsing will be put into errors[filename].
	 * \param get_relative_file Function that takes this file's filepath and a relative path and returns the path relative to this filepath's directory
	 * \return Parsed structure containing contents of the file.
	 */
	parsed_sound_cfg_t parse(std::string const& filename,
	                         std::string const& input_string,
	                         errors::multi_error_t& errors,
	                         find_relative_file_func const& get_relative_file);
} // namespace parsers::config::sound_cfg
