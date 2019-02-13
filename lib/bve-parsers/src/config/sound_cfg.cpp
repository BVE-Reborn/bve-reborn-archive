#include "parsers/config/sound_cfg.hpp"
#include "core/inlining_util.hpp"
#include "gsl/gsl_util"
#include "parsers/ini.hpp"
#include "parsers/utils.hpp"
#include <regex>
#include <sstream>

using namespace std::string_literals;

namespace bve::parsers::config::sound_cfg {
	namespace {
		/**
		 * \brief Checks if the version number in the section is 1.0 exactly. If it isn't complains.
		 * \param errors File error list. Puts complaints inside.
		 * \param section Section holding version number.
		 */
		void check_version_number(errors::Errors& errors, ini::INISection const& section) {
			static std::regex const r(R"(version\s*(\d+(?:\.\d*)?))",
			                          std::regex_constants::optimize | std::regex_constants::icase
			                              | std::regex_constants::ECMAScript);

			for (auto const& value : section.values) {
				std::smatch res;
				bool const found = std::regex_match(value.value, res, r);

				if (found) {
					auto const version = util::parse_loose_float(res[1]);
					if (version != 1.0F) {
						std::ostringstream err;
						err << version << " is not a recognized version number";
						add_error(errors, value.line, err);
					}
				}
				else {
					std::ostringstream err;
					err << "Unrecognized value \"" << value.value << "\"";
					add_error(errors, value.line, err);
				}
			}
			for (auto const& kvp : section.key_value_pairs) {
				std::ostringstream err;
				err << "Unrecognized key \"" << kvp.key << "\"";
				add_error(errors, kvp.line, err);
			}
		}

		/**
		 * \brief Cheeky little function that checks to see if the function provided is of the right signature: filename_iterator(std::string). Exists at compile time only.
		 * \tparam F Function type.
		 */
		template <class F>
		FORCE_INLINE void function_check(F& /*unused*/) {
			static_assert(std::is_same<decltype(std::declval<F>()(std::declval<std::string>())),
			                           FilenameIterator>::value,
			              "get_filename must take a std::string and return a "
			              "filename_iterator");
		}

		/**
		 * \brief Checks if boolean flag is set already. If it is, complains about overriding data using the file position information provided.
		 * \param flag Flag to check
		 * \param errors Error list for the file. All complaining goes here.
		 * \param section C String with ini section name
		 * \param name C String with the key of the key value pair
		 * \param line Line within the file that the error occured.
		 */
		void check_if_set(bool& flag,
		                  errors::Errors& errors,
		                  char const* const section,
		                  char const* const name,
		                  std::size_t const line) {
			if (flag) {
				std::ostringstream err;
				err << name << " already set within [" << section
				    << "] section. Overriding with new value."s;
				add_error(errors, line, err);
			}
			flag = true;
		}

		/**
		 * \brief Generic function for parsing blocks with `number = file` pairs into a unordered_map of number -> struct w/ iterator
		 * \tparam Ret Type of return structure.
		 * \tparam F
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \return unordered_map of std::size_t -> Ret
		 */
		template <class Ret, class F>
		std::unordered_map<std::size_t, Ret> parse_number_filename_pairs(
		    F& get_filename,
		    errors::Errors& errors,
		    ini::INISection const& section) {
			function_check(get_filename);

			std::unordered_map<std::size_t, bool> used;
			std::unordered_map<std::size_t, Ret> ret;

			for (auto const& kvp : section.key_value_pairs) {
				std::size_t index;
				try {
					index = gsl::narrow<std::size_t>(util::parse_loose_integer(kvp.key));
				}
				catch (std::exception const& e) {
					std::ostringstream err;
					err << "Key \"" << kvp.key << "\" had number parsing error: " << e.what();
					add_error(errors, kvp.line, err);
					continue;
				}

				check_if_set(used[index], errors, util::lower_copy(section.name).c_str(),
				             kvp.key.c_str(), kvp.line);
				ret[index].filename = get_filename(kvp.value);
			}

			return ret;
		}

		/**
		 * \brief Creates an empty Brake block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Brake block.
		 */
		Brake create_empty_brake(FilenameIterator const end) {
			Brake ret;

			ret.bc_release_high = end;
			ret.bc_release = end;
			ret.bc_release_full = end;
			ret.emergency = end;
			ret.bp_decompression = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Brake block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Brake block.
		 */
		template <class F>
		Brake parse_brake(F& get_filename,
		                  errors::Errors& errors,
		                  ini::INISection const& section,
		                  FilenameIterator const end) {
			function_check(get_filename);

			Brake ret = create_empty_brake(end);

			bool bc_release_high_flag = false;
			bool bc_release_flag = false;
			bool bc_release_full_flag = false;
			bool emergency_flag = false;
			bool bp_decompression_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "bc release high")) {
					check_if_set(bc_release_high_flag, errors, "brake", "BC Release High",
					             kvp.line);
					ret.bc_release_high = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "bc release")) {
					check_if_set(bc_release_flag, errors, "brake", "BC Release", kvp.line);
					ret.bc_release = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "bc release full")) {
					check_if_set(bc_release_full_flag, errors, "brake", "BC Release Full",
					             kvp.line);
					ret.bc_release_full = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "emergency")) {
					check_if_set(emergency_flag, errors, "brake", "Emergency", kvp.line);
					ret.emergency = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "bp decomp")) {
					check_if_set(bp_decompression_flag, errors, "brake", "BP Decomp", kvp.line);
					ret.bp_decompression = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty Compressor block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Compressor block.
		 */
		Compressor create_empty_compressor(FilenameIterator const end) {
			Compressor ret;

			ret.attack = end;
			ret.release = end;
			ret.loop = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Compressor block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Compressor block.
		 */
		template <class F>
		Compressor parse_compressor(F& get_filename,
		                            errors::Errors& errors,
		                            ini::INISection const& section,
		                            FilenameIterator const end) {
			function_check(get_filename);

			Compressor ret = create_empty_compressor(end);

			bool attack_flag = false;
			bool loop_flag = false;
			bool release_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "attack")) {
					check_if_set(attack_flag, errors, "compressor", "Attack", kvp.line);
					ret.attack = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "loop")) {
					check_if_set(loop_flag, errors, "compressor", "Loop", kvp.line);
					ret.loop = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "release")) {
					check_if_set(release_flag, errors, "compressor", "Release", kvp.line);
					ret.release = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty Suspension block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Suspension block.
		 */
		Suspension create_empty_suspension(FilenameIterator const end) {
			Suspension ret;

			ret.left = end;
			ret.right = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Suspension block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Suspension block.
		 */
		template <class F>
		Suspension parse_suspension(F& get_filename,
		                            errors::Errors& errors,
		                            ini::INISection const& section,
		                            FilenameIterator const end) {
			function_check(get_filename);

			Suspension ret = create_empty_suspension(end);

			bool left_flag = false;
			bool right_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "left")) {
					check_if_set(left_flag, errors, "suspension", "Left", kvp.line);
					ret.left = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "right")) {
					check_if_set(right_flag, errors, "suspension", "Right", kvp.line);
					ret.right = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty LegacyHorn block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty LegacyHorn block.
		 */
		LegacyHorn create_empty_legacy_horn(FilenameIterator const end) {
			LegacyHorn ret;

			ret.primary = end;
			ret.secondary = end;
			ret.music = end;

			return ret;
		}

		/**
		 * \brief Creates an empty LoopedHorn block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty LoopedHorn block.
		 */
		LoopedHorn create_empty_looped_horn(FilenameIterator const end) {
			LoopedHorn ret;

			ret.primary_start = end;
			ret.primary_loop = end;
			ret.primary_end = end;
			ret.secondary_start = end;
			ret.secondary_loop = end;
			ret.secondary_end = end;
			ret.music_start = end;
			ret.music_loop = end;
			ret.music_end = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into horn_t block. Templating allows lambdas as function arguments.
		 * A horn_t is a variant with either a LoopedHorn or a LegacyHorn in it. There are
		 * mutually exclusive legacy and modern (looped) horn configurations with the modern one
		 * takes priority.
		 *
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Doors block.
		 */
		template <class F>
		Horn parse_horn(F& get_filename,
		                errors::Errors& errors,
		                ini::INISection const& section,
		                FilenameIterator const end) {
			function_check(get_filename);

			LoopedHorn looped_ret = create_empty_looped_horn(end);
			LegacyHorn legacy_ret = create_empty_legacy_horn(end);

			bool new_stuff = false;

			bool primary_start_flag = false;
			bool primary_loop_flag = false;
			bool primary_end_flag = false;
			bool secondary_start_flag = false;
			bool secondary_loop_flag = false;
			bool secondary_end_flag = false;
			bool music_start_flag = false;
			bool music_loop_flag = false;
			bool music_end_flag = false;

			bool primary_flag = false;
			bool secondary_flag = false;
			bool music_flag = false;

			auto legacy_test = [&](char const* const name, std::size_t const line) {
				if (new_stuff) {
					std::ostringstream err;
					err << name
					    << " is a legacy horn setting. Looping horn already in use, ignoring.";
					add_error(errors, line, err);
				}
			};

			for (auto const& kvp : section.key_value_pairs) {
				/////////////////
				// looped horn //
				/////////////////

				if (util::match_against_lower(kvp.key, "primarystart")) {
					check_if_set(primary_start_flag, errors, "horn", "Primary Start", kvp.line);
					looped_ret.primary_start = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "primaryloop")) {
					check_if_set(primary_loop_flag, errors, "horn", "Primary Loop", kvp.line);
					looped_ret.primary_loop = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "primaryend")) {
					check_if_set(primary_end_flag, errors, "horn", "Primary End", kvp.line);
					looped_ret.primary_end = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "secondarystart")) {
					check_if_set(secondary_start_flag, errors, "horn", "Secondary Start", kvp.line);
					looped_ret.secondary_start = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "secondaryloop")) {
					check_if_set(secondary_loop_flag, errors, "horn", "Secondary Loop", kvp.line);
					looped_ret.secondary_loop = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "secondaryend")) {
					check_if_set(secondary_end_flag, errors, "horn", "Secondary End", kvp.line);
					looped_ret.secondary_end = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "musicstart")) {
					check_if_set(music_start_flag, errors, "horn", "Music Start", kvp.line);
					looped_ret.music_start = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "musicloop")) {
					check_if_set(music_loop_flag, errors, "horn", "Music Loop", kvp.line);
					looped_ret.music_loop = get_filename(kvp.value);
					new_stuff = true;
				}
				else if (util::match_against_lower(kvp.key, "musicend")) {
					check_if_set(music_end_flag, errors, "horn", "Music End", kvp.line);
					looped_ret.music_end = get_filename(kvp.value);
					new_stuff = true;
				}

				/////////////////
				// legacy horn //
				/////////////////

				else if (util::match_against_lower(kvp.key, "primary")) {
					check_if_set(primary_flag, errors, "horn", "Primary", kvp.line);
					legacy_ret.primary = get_filename(kvp.value);
					legacy_test("Primary", kvp.line);
				}
				else if (util::match_against_lower(kvp.key, "secondary")) {
					check_if_set(secondary_flag, errors, "horn", "Secondary", kvp.line);
					legacy_ret.secondary = get_filename(kvp.value);
					legacy_test("Secondary", kvp.line);
				}
				else if (util::match_against_lower(kvp.key, "music")) {
					check_if_set(music_flag, errors, "horn", "Music", kvp.line);
					legacy_ret.music = get_filename(kvp.value);
					legacy_test("Music", kvp.line);
				}

				////////////
				// errors //
				////////////

				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			if (new_stuff) {
				// remove legacy filenames

				return looped_ret;
			}

			return legacy_ret;
		}

		/**
		 * \brief Creates an empty Doors block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Doors block.
		 */
		Doors create_empty_doors(FilenameIterator const end) {
			Doors ret;

			ret.open_left = end;
			ret.open_right = end;
			ret.close_left = end;
			ret.close_right = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Doors block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Doors block.
		 */
		template <class F>
		Doors parse_doors(F& get_filename,
		                  errors::Errors& errors,
		                  ini::INISection const& section,
		                  FilenameIterator const end) {
			function_check(get_filename);

			Doors ret = create_empty_doors(end);

			bool open_left_flag = false;
			bool open_right_flag = false;
			bool close_left_flag = false;
			bool close_right_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "open left")) {
					check_if_set(open_left_flag, errors, "doors", "Open Left", kvp.line);
					ret.open_left = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "open right")) {
					check_if_set(open_right_flag, errors, "doors", "Open Right", kvp.line);
					ret.open_right = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "close left")) {
					check_if_set(close_left_flag, errors, "doors", "Close Left", kvp.line);
					ret.close_left = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "close right")) {
					check_if_set(close_right_flag, errors, "doors", "Close Right", kvp.line);
					ret.close_right = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty Buzzer block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Buzzer block.
		 */
		Buzzer create_empty_buzzer(FilenameIterator const end) {
			Buzzer ret;

			ret.correct = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Buzzer block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Buzzer block.
		 */
		template <class F>
		Buzzer parse_buzzer(F& get_filename,
		                    errors::Errors& errors,
		                    ini::INISection const& section,
		                    FilenameIterator const end) {
			function_check(get_filename);

			Buzzer ret = create_empty_buzzer(end);

			bool correct_flag = false; //

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "correct")) {
					check_if_set(correct_flag, errors, "buzzer", "Correct", kvp.line);
					ret.correct = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty PilotLamp block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty PilotLamp block.
		 */
		PilotLamp create_empty_pilot_lamp(FilenameIterator const end) {
			PilotLamp ret;

			ret.on = end;
			ret.off = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a PilotLamp block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed PilotLamp block.
		 */
		template <class F>
		PilotLamp parse_pilot_lamp(F& get_filename,
		                           errors::Errors& errors,
		                           ini::INISection const& section,
		                           FilenameIterator const end) {
			function_check(get_filename);

			PilotLamp ret = create_empty_pilot_lamp(end);

			bool on_flag = false;
			bool off_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "on")) {
					check_if_set(on_flag, errors, "Pilot Lamp", "On", kvp.line);
					ret.on = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "off")) {
					check_if_set(off_flag, errors, "Pilot Lamp", "Off", kvp.line);
					ret.off = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty BrakeHandle block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty BrakeHandle block.
		 */
		BrakeHandle create_empty_brake_handle(FilenameIterator const end) {
			BrakeHandle ret;

			ret.apply = end;
			ret.release = end;
			ret.min = end;
			ret.max = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a BrakeHandle block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed BrakeHandle block.
		 */
		template <class F>
		BrakeHandle parse_brake_handle(F& get_filename,
		                               errors::Errors& errors,
		                               ini::INISection const& section,
		                               FilenameIterator const end) {
			function_check(get_filename);

			BrakeHandle ret = create_empty_brake_handle(end);

			bool apply_flag = false;
			bool release_flag = false;
			bool min_flag = false;
			bool max_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "apply")) {
					check_if_set(apply_flag, errors, "Brake Handle", "Apply", kvp.line);
					ret.apply = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "release")) {
					check_if_set(release_flag, errors, "Brake Handle", "Release", kvp.line);
					ret.release = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "min")) {
					check_if_set(min_flag, errors, "Brake Handle", "Min", kvp.line);
					ret.min = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "max")) {
					check_if_set(max_flag, errors, "Brake Handle", "Max", kvp.line);
					ret.max = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty MasterController block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty MasterController block.
		 */
		MasterController create_empty_master_controller(FilenameIterator const end) {
			MasterController ret;

			ret.up = end;
			ret.down = end;
			ret.min = end;
			ret.max = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a MasterController block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed MasterController block.
		 */
		template <class F>
		MasterController parse_master_controller(F& get_filename,
		                                         errors::Errors& errors,
		                                         ini::INISection const& section,
		                                         FilenameIterator const end) {
			function_check(get_filename);

			MasterController ret = create_empty_master_controller(end);

			bool up_flag = false;
			bool down_flag = false;
			bool min_flag = false;
			bool max_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "up")) {
					check_if_set(up_flag, errors, "Master Controller", "Up", kvp.line);
					ret.up = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "down")) {
					check_if_set(down_flag, errors, "Master Controller", "Down", kvp.line);
					ret.down = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "min")) {
					check_if_set(min_flag, errors, "Master Controller", "Min", kvp.line);
					ret.min = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "max")) {
					check_if_set(max_flag, errors, "Master Controller", "Max", kvp.line);
					ret.max = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty Reverser block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Reverser block.
		 */
		Reverser create_empty_reverser(FilenameIterator const end) {
			Reverser ret;

			ret.on = end;
			ret.off = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Reverser block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Reverser block.
		 */
		template <class F>
		Reverser parse_reverser(F& get_filename,
		                        errors::Errors& errors,
		                        ini::INISection const& section,
		                        FilenameIterator const end) {
			function_check(get_filename);

			Reverser ret = create_empty_reverser(end);

			bool on_flag = false;
			bool off_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "on")) {
					check_if_set(on_flag, errors, "Reverser", "On", kvp.line);
					ret.on = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "off")) {
					check_if_set(off_flag, errors, "Reverser", "Off", kvp.line);
					ret.off = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty Breaker block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Breaker block.
		 */
		Breaker create_empty_breaker(FilenameIterator const end) {
			Breaker ret;

			ret.on = end;
			ret.off = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Breaker block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Breaker block.
		 */
		template <class F>
		Breaker parse_breaker(F& get_filename,
		                      errors::Errors& errors,
		                      ini::INISection const& section,
		                      FilenameIterator const end) {
			function_check(get_filename);

			Breaker ret = create_empty_breaker(end);

			bool on_flag = false;
			bool off_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "on")) {
					check_if_set(on_flag, errors, "breaker", "On", kvp.line);
					ret.on = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "off")) {
					check_if_set(off_flag, errors, "breaker", "Off", kvp.line);
					ret.off = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Creates an empty Misc block with all members initialized to the end iterator of the filename set.
		 * \param end End iterator of filename set
		 * \return Empty Misc block.
		 */
		Misc create_empty_misc(FilenameIterator const end) {
			Misc ret;

			ret.noise = end;
			ret.shoe = end;

			return ret;
		}

		/**
		 * \brief Parses an ini section into a Misc block. Templating allows lambdas as function arguments.
		 * \tparam F get_filename's type.
		 * \param get_filename Function that takes a filepath and returns an iterator into the filename set for that value.
		 * \param errors This file's errors.
		 * \param section The ini section to be parsed.
		 * \param end The end iterator into the filename set.
		 * \return Parsed Misc block.
		 */
		template <class F>
		Misc parse_misc(F& get_filename,
		                errors::Errors& errors,
		                ini::INISection const& section,
		                FilenameIterator const end) {
			function_check(get_filename);

			Misc ret = create_empty_misc(end);

			bool noise_flag = false;
			bool shoe_flag = false;

			for (auto const& kvp : section.key_value_pairs) {
				if (util::match_against_lower(kvp.key, "noise")) {
					check_if_set(noise_flag, errors, "Others", "Noise", kvp.line);
					ret.noise = get_filename(kvp.value);
				}
				else if (util::match_against_lower(kvp.key, "shoe")) {
					check_if_set(shoe_flag, errors, "Others", "Shoe", kvp.line);
					ret.shoe = get_filename(kvp.value);
				}
				else {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					add_error(errors, kvp.line, err);
				}
			}

			return ret;
		}

		/**
		 * \brief Strips unused filenames by constructing a new set of filenames as it visits every member. Discards old set.
		 * \param psc Already parsed sound.cfg structure.
		 */
		void remove_unnecessary_filenames(ParsedSoundCFG& psc) {
			std::set<std::string> new_filenames;

			auto const filename_from_str_itr = [&](auto const& file_iter) {
				if (file_iter != psc.filenames.end()) {
					return new_filenames.insert(*file_iter).first;
				}
				return new_filenames.end();
			};

			for (auto&& filename : psc.run_sounds) {
				filename.second.filename = filename_from_str_itr(filename.second.filename);
			}
			for (auto&& filename : psc.flange_sounds) {
				filename.second.filename = filename_from_str_itr(filename.second.filename);
			}
			for (auto&& filename : psc.motor_sounds) {
				filename.second.filename = filename_from_str_itr(filename.second.filename);
			}
			for (auto&& filename : psc.switch_sounds) {
				filename.second.filename = filename_from_str_itr(filename.second.filename);
			}
			for (auto&& filename : psc.ats_sounds) {
				filename.second.filename = filename_from_str_itr(filename.second.filename);
			}
			psc.brake_sounds.bc_release_high =
			    filename_from_str_itr(psc.brake_sounds.bc_release_high);
			psc.brake_sounds.bc_release = filename_from_str_itr(psc.brake_sounds.bc_release);
			psc.brake_sounds.bc_release_full =
			    filename_from_str_itr(psc.brake_sounds.bc_release_full);
			psc.brake_sounds.emergency = filename_from_str_itr(psc.brake_sounds.emergency);
			psc.brake_sounds.bp_decompression =
			    filename_from_str_itr(psc.brake_sounds.bp_decompression);

			psc.compressor_sounds.attack = filename_from_str_itr(psc.compressor_sounds.attack);
			psc.compressor_sounds.loop = filename_from_str_itr(psc.compressor_sounds.loop);
			psc.compressor_sounds.release = filename_from_str_itr(psc.compressor_sounds.release);

			psc.suspension_sounds.left = filename_from_str_itr(psc.suspension_sounds.left);
			psc.suspension_sounds.right = filename_from_str_itr(psc.suspension_sounds.right);

			if (psc.horn_sounds.is<LoopedHorn>()) {
				auto&& looped = psc.horn_sounds.get_unchecked<LoopedHorn>();

				looped.primary_start = filename_from_str_itr(looped.primary_start);
				looped.primary_loop = filename_from_str_itr(looped.primary_loop);
				looped.primary_end = filename_from_str_itr(looped.primary_end);
				looped.secondary_start = filename_from_str_itr(looped.secondary_start);
				looped.secondary_loop = filename_from_str_itr(looped.secondary_loop);
				looped.secondary_end = filename_from_str_itr(looped.secondary_end);
				looped.music_start = filename_from_str_itr(looped.music_start);
				looped.music_loop = filename_from_str_itr(looped.music_loop);
				looped.music_end = filename_from_str_itr(looped.music_end);
			}
			else if (psc.horn_sounds.is<LegacyHorn>()) {
				auto&& legacy = psc.horn_sounds.get_unchecked<LegacyHorn>();

				legacy.primary = filename_from_str_itr(legacy.primary);
				legacy.secondary = filename_from_str_itr(legacy.secondary);
				legacy.music = filename_from_str_itr(legacy.music);
			}

			psc.door_sounds.open_left = filename_from_str_itr(psc.door_sounds.open_left);
			psc.door_sounds.close_left = filename_from_str_itr(psc.door_sounds.close_left);
			psc.door_sounds.open_right = filename_from_str_itr(psc.door_sounds.open_right);
			psc.door_sounds.close_right = filename_from_str_itr(psc.door_sounds.close_right);

			psc.buzzer_sounds.correct = filename_from_str_itr(psc.buzzer_sounds.correct);

			psc.pilot_lamp_sounds.on = filename_from_str_itr(psc.pilot_lamp_sounds.on);
			psc.pilot_lamp_sounds.off = filename_from_str_itr(psc.pilot_lamp_sounds.off);

			psc.brake_handle_sounds.apply = filename_from_str_itr(psc.brake_handle_sounds.apply);
			psc.brake_handle_sounds.release =
			    filename_from_str_itr(psc.brake_handle_sounds.release);
			psc.brake_handle_sounds.min = filename_from_str_itr(psc.brake_handle_sounds.min);
			psc.brake_handle_sounds.max = filename_from_str_itr(psc.brake_handle_sounds.max);

			psc.master_controller_sounds.up =
			    filename_from_str_itr(psc.master_controller_sounds.up);
			psc.master_controller_sounds.down =
			    filename_from_str_itr(psc.master_controller_sounds.down);
			psc.master_controller_sounds.min =
			    filename_from_str_itr(psc.master_controller_sounds.min);
			psc.master_controller_sounds.max =
			    filename_from_str_itr(psc.master_controller_sounds.max);

			psc.reverser_sounds.on = filename_from_str_itr(psc.reverser_sounds.on);
			psc.reverser_sounds.off = filename_from_str_itr(psc.reverser_sounds.off);

			psc.breaker_sounds.on = filename_from_str_itr(psc.breaker_sounds.on);
			psc.breaker_sounds.off = filename_from_str_itr(psc.breaker_sounds.off);

			psc.misc_sounds.noise = filename_from_str_itr(psc.misc_sounds.noise);
			psc.misc_sounds.shoe = filename_from_str_itr(psc.misc_sounds.shoe);

			// keep iterators valid
			psc.filenames = std::move(new_filenames);
		}
	} // namespace

	ParsedSoundCFG parse(std::string const& filename,
	                     std::string const& input_string,
	                     errors::MultiError& errors,
	                     RelativeFileFunc const& get_relative_file) {
		ParsedSoundCFG psc;

		auto get_filename = [&](std::string const& rel_file) {
			return psc.filenames.insert(get_relative_file(filename, rel_file)).first;
		};

		auto& file_error = errors[filename];

		auto const parsed_ini = ini::parse(input_string);

		bool run_flag = false;
		bool flange_flag = false;
		bool motor_flag = false;
		bool switch_flag = false;
		bool brake_flag = false;
		bool compressor_flag = false;
		bool suspension_flag = false;
		bool horn_flag = false;
		bool door_flag = false;
		bool ats_flag = false;
		bool buzzer_flag = false;
		bool pilot_lamp_flag = false;
		bool brake_handle_flag = false;
		bool master_controller_flag = false;
		bool reverser_flag = false;
		bool breaker_flag = false;
		bool misc_flag = false;

		auto const check_if_section_used = [&](bool& flag, char const* const name,
		                                       std::size_t const line) {
			if (flag) {
				std::ostringstream err;
				err << "[" << name << "] has already been defined. Overriding with new value."s;
				add_error(file_error, line, err);
			}
			flag = true;
		};

		auto const end_iterator = psc.filenames.end();

		for (auto const& section : parsed_ini) {
			if (util::match_against_lower(section.name, "run")) {
				check_if_section_used(run_flag, "run", section.line);
				psc.run_sounds =
				    parse_number_filename_pairs<Run>(get_filename, file_error, section);
			}
			else if (util::match_against_lower(section.name, "flange")) {
				check_if_section_used(flange_flag, "flange", section.line);
				psc.flange_sounds =
				    parse_number_filename_pairs<Flange>(get_filename, file_error, section);
			}
			else if (util::match_against_lower(section.name, "motor")) {
				check_if_section_used(motor_flag, "motor", section.line);
				psc.motor_sounds =
				    parse_number_filename_pairs<Motor>(get_filename, file_error, section);
			}
			else if (util::match_against_lower(section.name, "switch")) {
				check_if_section_used(switch_flag, "switch", section.line);
				psc.switch_sounds =
				    parse_number_filename_pairs<Switch>(get_filename, file_error, section);
			}
			else if (util::match_against_lower(section.name, "brake")) {
				check_if_section_used(brake_flag, "brake", section.line);
				psc.brake_sounds = parse_brake(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "compressor")) {
				check_if_section_used(compressor_flag, "compressor", section.line);
				psc.compressor_sounds =
				    parse_compressor(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "suspension")) {
				check_if_section_used(suspension_flag, "suspension", section.line);
				psc.suspension_sounds =
				    parse_suspension(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "horn")) {
				check_if_section_used(horn_flag, "horn", section.line);
				psc.horn_sounds = parse_horn(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "door")) {
				check_if_section_used(door_flag, "door", section.line);
				psc.door_sounds = parse_doors(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "ats")) {
				check_if_section_used(ats_flag, "ats", section.line);
				psc.ats_sounds =
				    parse_number_filename_pairs<Ats>(get_filename, file_error, section);
			}
			else if (util::match_against_lower(section.name, "buzzer")) {
				check_if_section_used(buzzer_flag, "buzzer", section.line);
				psc.buzzer_sounds = parse_buzzer(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "pilot lamp")) {
				check_if_section_used(pilot_lamp_flag, "pilot lamp", section.line);
				psc.pilot_lamp_sounds =
				    parse_pilot_lamp(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "brake handle")) {
				check_if_section_used(brake_handle_flag, "brake handle", section.line);
				psc.brake_handle_sounds =
				    parse_brake_handle(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "master controller")) {
				check_if_section_used(master_controller_flag, "master controller", section.line);
				psc.master_controller_sounds =
				    parse_master_controller(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "reverser")) {
				check_if_section_used(reverser_flag, "reverser", section.line);
				psc.reverser_sounds =
				    parse_reverser(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "breaker")) {
				check_if_section_used(breaker_flag, "breaker", section.line);
				psc.breaker_sounds = parse_breaker(get_filename, file_error, section, end_iterator);
			}
			else if (util::match_against_lower(section.name, "others")) {
				check_if_section_used(misc_flag, "others", section.line);
				psc.misc_sounds = parse_misc(get_filename, file_error, section, end_iterator);
			}
			else if (section.name.empty()) {
				check_version_number(file_error, section);
			}
			else {
				std::ostringstream err;
				err << "[" << section.name << "] is not recognized. Ignoring.";
				add_error(file_error, section.line, err);
			}

			// check_version_number does the checking for us.
			if (!section.name.empty()) {
				for (auto const& value : section.values) {
					std::ostringstream err;
					err << "Unrecognized value \"" << value.value << "\".";
					add_error(file_error, value.line, err);
				}
			}
		}

		// Only functions that are raw structs. Maps don't have to be filled, they would just be
		// empty.
		if (!brake_flag) {
			psc.brake_sounds = create_empty_brake(end_iterator);
		}
		if (!compressor_flag) {
			psc.compressor_sounds = create_empty_compressor(end_iterator);
		}
		if (!suspension_flag) {
			psc.suspension_sounds = create_empty_suspension(end_iterator);
		}
		if (!horn_flag) {
			psc.horn_sounds = create_empty_looped_horn(end_iterator);
		}
		if (!door_flag) {
			psc.door_sounds = create_empty_doors(end_iterator);
		}
		if (!buzzer_flag) {
			psc.buzzer_sounds = create_empty_buzzer(end_iterator);
		}
		if (!pilot_lamp_flag) {
			psc.pilot_lamp_sounds = create_empty_pilot_lamp(end_iterator);
		}
		if (!brake_handle_flag) {
			psc.brake_handle_sounds = create_empty_brake_handle(end_iterator);
		}
		if (!master_controller_flag) {
			psc.master_controller_sounds = create_empty_master_controller(end_iterator);
		}
		if (!reverser_flag) {
			psc.reverser_sounds = create_empty_reverser(end_iterator);
		}
		if (!breaker_flag) {
			psc.breaker_sounds = create_empty_breaker(end_iterator);
		}
		if (!misc_flag) {
			psc.misc_sounds = create_empty_misc(end_iterator);
		}

		remove_unnecessary_filenames(psc);

		return psc;
	}
} // namespace bve::parsers::config::sound_cfg
