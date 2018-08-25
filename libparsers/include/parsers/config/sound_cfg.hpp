#pragma one

#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <set>
#include <string>
#include <unordered_map>

namespace parsers {
namespace config {
	namespace sound_cfg {
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

		struct break_t {
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
			filename_iterator start;
			filename_iterator loop;
			filename_iterator end;
		};

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
			filename_iterator filename;
		};

		struct pilot_lamp_t {
			filename_iterator on;
			filename_iterator off;
		};

		struct break_handle_t {
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
			std::set<std::string> filenames;

			std::unordered_map<std::size_t, run_t> run_sounds;
			std::unordered_map<std::size_t, flange_t> flange_sounds;
			std::unordered_map<std::size_t, motor_t> motor_sounds;
			std::unordered_map<std::size_t, switch_t> switch_sounds;
			break_t break_sounds;
			compressor_t compressor_sounds;
			suspension_t suspension_sounds;
			horn_t horn_sounds;
			doors_t door_sounds;
			ats_t ats_sounds;
			buzzer_t buzzer_sounds;
			pilot_lamp_t pilot_lamp_sounds;
			break_handle_t break_handle_sounds;
			master_controller_t master_controller_sounds;
			reverser_t reverser_sounds;
			breaker_t breaker_sounds;
			misc_t misc_sounds;
		};

		parsed_sound_cfg_t parse(std::string const& filename,
		                         std::string input_string,
		                         errors::multi_error_t& errors,
		                         find_relative_file_func const& get_relative_file);
	} // namespace sound_cfg
} // namespace config
} // namespace parsers
