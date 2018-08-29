#include "parsers/config/sound_cfg.hpp"
#include "gsl/gsl_util"
#include "ini.hpp"
#include "utils.hpp"

using namespace std::string_literals;

namespace parsers {
namespace config {
	namespace sound_cfg {
		namespace {
			template <class F>
			void function_check(F& f) {
				static_assert(std::is_same<decltype(f(std::declval<std::string>())),
				                           filename_iterator>::value,
				              "get_filename must take a std::string and return a "
				              "filename_iterator");
			}

			static void check_if_set(bool& flag,
			                         errors::errors_t& errors,
			                         char const* const section,
			                         char const* const name,
			                         std::size_t line) {
				if (flag) {
					std::ostringstream err;
					err << name << " already set within [" << section
					    << "] section. Overriding with new value."s;
					errors::add_error(errors, line, err);
				}
				flag = true;
			}

			template <class Ret, class F>
			auto parse_number_filename_pairs(F& get_filename,
			                                 errors::errors_t& errors,
			                                 ini::ini_section_t const& section) {
				function_check(get_filename);

				std::unordered_map<std::size_t, bool> used;
				std::unordered_map<std::size_t, Ret> ret;

				for (auto const& kvp : section.key_value_pairs) {
					std::size_t index;
					try {
						index = gsl::narrow<std::size_t>(util::parse_loose_integer(kvp.key));
					}
					catch (std::exception const& e) {
						errors::add_error(errors, kvp.line, e.what());
						continue;
					}

					check_if_set(used[index], errors, util::lower_copy(section.name).c_str(),
					             kvp.key.c_str(), kvp.line);
					ret[index].filename = get_filename(kvp.value);
				}

				return ret;
			}

			template <class F>
			break_t parse_break(F& get_filename,
			                    errors::errors_t& errors,
			                    ini::ini_section_t const& section) {
				function_check(get_filename);

				break_t ret;

				bool bc_release_high_flag = false, //
				    bc_release_flag = false,       //
				    bc_release_full_flag = false,  //
				    emergency_flag = false,        //
				    bp_decompression_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "bc release high")) {
						check_if_set(bc_release_high_flag, errors, "break", "BC Release High",
						             kvp.line);
						ret.bc_release_high = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "bc release")) {
						check_if_set(bc_release_flag, errors, "break", "BC Release", kvp.line);
						ret.bc_release = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "bc release full")) {
						check_if_set(bc_release_full_flag, errors, "break", "BC Release Full",
						             kvp.line);
						ret.bc_release_full = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "emergency")) {
						check_if_set(emergency_flag, errors, "break", "Emergency", kvp.line);
						ret.emergency = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "bp decomp")) {
						check_if_set(bp_decompression_flag, errors, "break", "BP Decomp", kvp.line);
						ret.bp_decompression = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			compressor_t parse_compressor(F& get_filename,
			                              errors::errors_t& errors,
			                              ini::ini_section_t const& section) {
				function_check(get_filename);

				compressor_t ret;

				bool attack_flag = false, //
				    loop_flag = false,    //
				    release_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "attack")) {
						check_if_set(attack_flag, errors, "compressor", "Attack", kvp.line);
						ret.attack = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "loop")) {
						check_if_set(loop_flag, errors, "compressor", "Loop", kvp.line);
						ret.loop = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "release")) {
						check_if_set(release_flag, errors, "compressor", "Release", kvp.line);
						ret.release = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			suspension_t parse_suspension(F& get_filename,
			                              errors::errors_t& errors,
			                              ini::ini_section_t const& section) {
				function_check(get_filename);

				suspension_t ret;

				bool left_flag = false, //
				    right_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "left")) {
						check_if_set(left_flag, errors, "suspension", "Left", kvp.line);
						ret.left = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "right")) {
						check_if_set(right_flag, errors, "suspension", "Right", kvp.line);
						ret.right = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			horn_t parse_horn(F& get_filename,
			                  errors::errors_t& errors,
			                  ini::ini_section_t const& section) {
				function_check(get_filename);

				looped_horn_t looped_ret;
				legacy_horn_t legacy_ret;

				bool new_stuff = false;

				bool primary_start_flag = false,  //
				    primary_loop_flag = false,    //
				    primary_end_flag = false,     //
				    secondary_start_flag = false, //
				    secondary_loop_flag = false,  //
				    secondary_end_flag = false,   //
				    music_start_flag = false,     //
				    music_loop_flag = false,      //
				    music_end_flag = false;

				bool primary_flag = false,  //
				    secondary_flag = false, //
				    music_flag = false;

				auto legacy_test = [&](char const* const name, std::size_t const line) {
					if (new_stuff) {
						std::ostringstream err;
						err << name
						    << " is a legacy horn setting. Looping horn already in use, ignoring.";
						errors::add_error(errors, line, err);
					}
				};

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "primarystart")) {
						check_if_set(primary_start_flag, errors, "horn", "Primary Start", kvp.line);
						looped_ret.primary_start = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "primaryloop")) {
						check_if_set(primary_loop_flag, errors, "horn", "Primary Loop", kvp.line);
						looped_ret.primary_loop = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "primaryend")) {
						check_if_set(primary_end_flag, errors, "horn", "Primary End", kvp.line);
						looped_ret.primary_end = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "secondarystart")) {
						check_if_set(secondary_start_flag, errors, "horn", "Secondary Start",
						             kvp.line);
						looped_ret.secondary_start = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "secondaryloop")) {
						check_if_set(secondary_loop_flag, errors, "horn", "Secondary Loop",
						             kvp.line);
						looped_ret.secondary_loop = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "secondaryend")) {
						check_if_set(secondary_end_flag, errors, "horn", "Secondary End", kvp.line);
						looped_ret.secondary_end = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "musicstart")) {
						check_if_set(music_start_flag, errors, "horn", "Music Start", kvp.line);
						looped_ret.music_start = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "musicloop")) {
						check_if_set(music_loop_flag, errors, "horn", "Music Loop", kvp.line);
						looped_ret.music_loop = get_filename(kvp.value);
						new_stuff = true;
					}
					else if (util::match_against_lower(kvp.value, "musicend")) {
						check_if_set(music_end_flag, errors, "horn", "Music End", kvp.line);
						looped_ret.music_end = get_filename(kvp.value);
						new_stuff = true;
					}

					else if (util::match_against_lower(kvp.value, "primary")) {
						check_if_set(primary_flag, errors, "horn", "Primary", kvp.line);
						legacy_ret.primary = get_filename(kvp.value);
						legacy_test("Primary", kvp.line);
					}
					else if (util::match_against_lower(kvp.value, "secondary")) {
						check_if_set(secondary_flag, errors, "horn", "Secondary", kvp.line);
						legacy_ret.secondary = get_filename(kvp.value);
						legacy_test("Secondary", kvp.line);
					}
					else if (util::match_against_lower(kvp.value, "music")) {
						check_if_set(music_flag, errors, "horn", "Music", kvp.line);
						legacy_ret.music = get_filename(kvp.value);
						legacy_test("Music", kvp.line);
					}
				}

				if (new_stuff) {
					return looped_ret;
				}
				else {
					return legacy_ret;
				}
			}

			template <class F>
			doors_t parse_doors(F& get_filename,
			                    errors::errors_t& errors,
			                    ini::ini_section_t const& section) {
				function_check(get_filename);

				doors_t ret;

				bool open_left_flag = false, //
				    open_right_flag = false, //
				    close_left_flag = false, //
				    close_right_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "open left")) {
						check_if_set(open_left_flag, errors, "doors", "Open Left", kvp.line);
						ret.open_left = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "open right")) {
						check_if_set(open_right_flag, errors, "doors", "Open Right", kvp.line);
						ret.open_right = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "close left")) {
						check_if_set(close_left_flag, errors, "doors", "Close Left", kvp.line);
						ret.close_left = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "open right")) {
						check_if_set(close_right_flag, errors, "doors", "Close Right", kvp.line);
						ret.close_right = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			buzzer_t parse_buzzer(F& get_filename,
			                      errors::errors_t& errors,
			                      ini::ini_section_t const& section) {
				function_check(get_filename);

				buzzer_t ret;

				bool correct_flag = false; //

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "correct")) {
						check_if_set(correct_flag, errors, "buzzer", "Correct", kvp.line);
						ret.filename = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			pilot_lamp_t parse_pilot_lamp(F& get_filename,
			                              errors::errors_t& errors,
			                              ini::ini_section_t const& section) {
				function_check(get_filename);

				pilot_lamp_t ret;

				bool on_flag = false, //
				    off_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "on")) {
						check_if_set(on_flag, errors, "Pilot Lamp", "On", kvp.line);
						ret.on = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "off")) {
						check_if_set(off_flag, errors, "Pilot Lamp", "Off", kvp.line);
						ret.off = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			break_handle_t parse_break_handle(F& get_filename,
			                                  errors::errors_t& errors,
			                                  ini::ini_section_t const& section) {
				function_check(get_filename);

				break_handle_t ret;

				bool apply_flag = false,  //
				    release_flag = false, //
				    min_flag = false,     //
				    max_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "apply")) {
						check_if_set(apply_flag, errors, "Break Handle", "Apply", kvp.line);
						ret.apply = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "release")) {
						check_if_set(release_flag, errors, "Break Handle", "Release", kvp.line);
						ret.release = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "min")) {
						check_if_set(min_flag, errors, "Break Handle", "Min", kvp.line);
						ret.min = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "max")) {
						check_if_set(max_flag, errors, "Break Handle", "Max", kvp.line);
						ret.max = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			master_controller_t parse_master_controller(F& get_filename,
			                                            errors::errors_t& errors,
			                                            ini::ini_section_t const& section) {
				function_check(get_filename);

				master_controller_t ret;

				bool up_flag = false,  //
				    down_flag = false, //
				    min_flag = false,  //
				    max_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "up")) {
						check_if_set(up_flag, errors, "Master Controller", "Up", kvp.line);
						ret.up = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "down")) {
						check_if_set(down_flag, errors, "Master Controller", "Down", kvp.line);
						ret.down = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "min")) {
						check_if_set(min_flag, errors, "Master Controller", "Min", kvp.line);
						ret.min = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "max")) {
						check_if_set(max_flag, errors, "Master Controller", "Max", kvp.line);
						ret.max = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			reverser_t parse_reverser(F& get_filename,
			                          errors::errors_t& errors,
			                          ini::ini_section_t const& section) {
				function_check(get_filename);

				reverser_t ret;

				bool on_flag = false, //
				    off_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "on")) {
						check_if_set(on_flag, errors, "Reverser", "On", kvp.line);
						ret.on = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "off")) {
						check_if_set(off_flag, errors, "Reverser", "Off", kvp.line);
						ret.off = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			breaker_t parse_breaker(F& get_filename,
			                        errors::errors_t& errors,
			                        ini::ini_section_t const& section) {
				function_check(get_filename);

				breaker_t ret;

				bool on_flag = false, //
				    off_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "on")) {
						check_if_set(on_flag, errors, "Breaker", "On", kvp.line);
						ret.on = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "off")) {
						check_if_set(off_flag, errors, "Breaker", "Off", kvp.line);
						ret.off = get_filename(kvp.value);
					}
				}

				return ret;
			}

			template <class F>
			misc_t parse_misc(F& get_filename,
			                  errors::errors_t& errors,
			                  ini::ini_section_t const& section) {
				function_check(get_filename);

				misc_t ret;

				bool noise_flag = false, //
				    shoe_flag = false;

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "noise")) {
						check_if_set(noise_flag, errors, "Others", "Noise", kvp.line);
						ret.noise = get_filename(kvp.value);
					}
					else if (util::match_against_lower(kvp.value, "shoe")) {
						check_if_set(shoe_flag, errors, "Others", "Shoe", kvp.line);
						ret.shoe = get_filename(kvp.value);
					}
				}

				return ret;
			}
		} // namespace
	} // namespace sound_cfg
} // namespace config
} // namespace parsers
