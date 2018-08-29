#include "parsers/config/sound_cfg.hpp"
#include "gsl/gsl_util"
#include "ini.hpp"
#include "utils.hpp"
#include <boost/regex.hpp>

using namespace std::string_literals;

namespace parsers {
namespace config {
	namespace sound_cfg {
		namespace {
			void check_version_number(errors::errors_t& errors, ini::ini_section_t const& section) {
				static boost::regex const r(R"(version\w*(\d+(?:\.\d*)?))",
				                            boost::regex_constants::optimize
				                                | boost::regex_constants::icase
				                                | boost::regex_constants::perl);

				for (auto const& value : section.values) {
					boost::smatch res;
					bool found = boost::regex_match(value.value, res, r);

					if (found) {
						auto version = util::parse_loose_float(res[1]);
						if (version != 1.0f) {
							std::ostringstream err;
							err << version << " is not a recognized version number";
							errors::add_error(errors, value.line, err);
						}
					}
					else {
						std::ostringstream err;
						err << "Unrecognized value \"" << value.value << "\"";
						errors::add_error(errors, value.line, err);
					}
				}
				for (auto const& kvp : section.key_value_pairs) {
					std::ostringstream err;
					err << "Unrecognized key \"" << kvp.key << "\"";
					errors::add_error(errors, kvp.line, err);
				}
			}

			template <class F>
			void function_check(F& f) {
				static_assert(std::is_same<decltype(f(std::declval<std::string>())),
				                           filename_iterator>::value,
				              "get_filename must take a std::string and return a "
				              "filename_iterator");
			}

			void check_if_set(bool& flag,
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
						std::ostringstream err;
						err << "Key \"" << kvp.key << "\" had number parsing error: " << e.what();
						errors::add_error(errors, kvp.line, err);
						continue;
					}

					check_if_set(used[index], errors, util::lower_copy(section.name).c_str(),
					             kvp.key.c_str(), kvp.line);
					ret[index].filename = get_filename(kvp.value);
				}

				return ret;
			}

			break_t create_empty_break(filename_iterator end) {
				break_t ret;

				ret.bc_release_high = end;
				ret.bc_release = end;
				ret.bc_release_full = end;
				ret.emergency = end;
				ret.bp_decompression = end;

				return ret;
			}

			template <class F>
			break_t parse_break(F& get_filename,
			                    errors::errors_t& errors,
			                    ini::ini_section_t const& section,
			                    filename_iterator end) {
				function_check(get_filename);

				break_t ret = create_empty_break(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			compressor_t create_empty_compressor(filename_iterator end) {
				compressor_t ret;

				ret.attack = end;
				ret.release = end;
				ret.loop = end;

				return ret;
			}

			template <class F>
			compressor_t parse_compressor(F& get_filename,
			                              errors::errors_t& errors,
			                              ini::ini_section_t const& section,
			                              filename_iterator end) {
				function_check(get_filename);

				compressor_t ret = create_empty_compressor(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			suspension_t create_empty_suspension(filename_iterator end) {
				suspension_t ret;

				ret.left = end;
				ret.right = end;

				return ret;
			}

			template <class F>
			suspension_t parse_suspension(F& get_filename,
			                              errors::errors_t& errors,
			                              ini::ini_section_t const& section,
			                              filename_iterator end) {
				function_check(get_filename);

				suspension_t ret = create_empty_suspension(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			legacy_horn_t create_empty_legacy_horn(filename_iterator end) {
				legacy_horn_t ret;

				ret.primary = end;
				ret.secondary = end;
				ret.music = end;

				return ret;
			}

			looped_horn_t create_empty_looped_horn(filename_iterator end) {
				looped_horn_t ret;

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

			template <class F>
			horn_t parse_horn(F& get_filename,
			                  errors::errors_t& errors,
			                  ini::ini_section_t const& section,
			                  filename_iterator end) {
				function_check(get_filename);

				looped_horn_t looped_ret = create_empty_looped_horn(end);
				legacy_horn_t legacy_ret = create_empty_legacy_horn(end);

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

					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				if (new_stuff) {
					return looped_ret;
				}

				return legacy_ret;
			}

			doors_t create_empty_doors(filename_iterator end) {
				doors_t ret;

				ret.open_left = end;
				ret.open_right = end;
				ret.close_left = end;
				ret.close_right = end;

				return ret;
			}

			template <class F>
			doors_t parse_doors(F& get_filename,
			                    errors::errors_t& errors,
			                    ini::ini_section_t const& section,
			                    filename_iterator end) {
				function_check(get_filename);

				doors_t ret = create_empty_doors(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			buzzer_t create_empty_buzzer(filename_iterator end) {
				buzzer_t ret;

				ret.correct = end;

				return ret;
			}

			template <class F>
			buzzer_t parse_buzzer(F& get_filename,
			                      errors::errors_t& errors,
			                      ini::ini_section_t const& section,
			                      filename_iterator end) {
				function_check(get_filename);

				buzzer_t ret = create_empty_buzzer(end);

				bool correct_flag = false; //

				for (auto const& kvp : section.key_value_pairs) {
					if (util::match_against_lower(kvp.value, "correct")) {
						check_if_set(correct_flag, errors, "buzzer", "Correct", kvp.line);
						ret.correct = get_filename(kvp.value);
					}
				}

				return ret;
			}

			pilot_lamp_t create_empty_pilot_lamp(filename_iterator end) {
				pilot_lamp_t ret;

				ret.on = end;
				ret.off = end;

				return ret;
			}

			template <class F>
			pilot_lamp_t parse_pilot_lamp(F& get_filename,
			                              errors::errors_t& errors,
			                              ini::ini_section_t const& section,
			                              filename_iterator end) {
				function_check(get_filename);

				pilot_lamp_t ret = create_empty_pilot_lamp(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			break_handle_t create_empty_break_handle(filename_iterator end) {
				break_handle_t ret;

				ret.apply = end;
				ret.release = end;
				ret.min = end;
				ret.max = end;

				return ret;
			}

			template <class F>
			break_handle_t parse_break_handle(F& get_filename,
			                                  errors::errors_t& errors,
			                                  ini::ini_section_t const& section,
			                                  filename_iterator end) {
				function_check(get_filename);

				break_handle_t ret = create_empty_break_handle(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			master_controller_t create_empty_master_controller(filename_iterator end) {
				master_controller_t ret;

				ret.up = end;
				ret.down = end;
				ret.min = end;
				ret.max = end;

				return ret;
			}

			template <class F>
			master_controller_t parse_master_controller(F& get_filename,
			                                            errors::errors_t& errors,
			                                            ini::ini_section_t const& section,
			                                            filename_iterator end) {
				function_check(get_filename);

				master_controller_t ret = create_empty_master_controller(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			reverser_t create_empty_reverser(filename_iterator end) {
				reverser_t ret;

				ret.on = end;
				ret.off = end;

				return ret;
			}

			template <class F>
			reverser_t parse_reverser(F& get_filename,
			                          errors::errors_t& errors,
			                          ini::ini_section_t const& section,
			                          filename_iterator end) {
				function_check(get_filename);

				reverser_t ret = create_empty_reverser(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			breaker_t create_empty_breaker(filename_iterator end) {
				breaker_t ret;

				ret.on = end;
				ret.off = end;

				return ret;
			}

			template <class F>
			breaker_t parse_breaker(F& get_filename,
			                        errors::errors_t& errors,
			                        ini::ini_section_t const& section,
			                        filename_iterator end) {
				function_check(get_filename);

				breaker_t ret = create_empty_breaker(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}

			misc_t create_empty_misc(filename_iterator end) {
				misc_t ret;

				ret.noise = end;
				ret.shoe = end;

				return ret;
			}

			template <class F>
			misc_t parse_misc(F& get_filename,
			                  errors::errors_t& errors,
			                  ini::ini_section_t const& section,
			                  filename_iterator end) {
				function_check(get_filename);

				misc_t ret = create_empty_misc(end);

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
					else {
						std::ostringstream err;
						err << "Unrecognized key \"" << kvp.key << "\"";
						errors::add_error(errors, kvp.line, err);
					}
				}

				return ret;
			}
		} // namespace

		parsed_sound_cfg_t parse(std::string const& filename,
		                         std::string const& input_string,
		                         errors::multi_error_t& errors,
		                         find_relative_file_func const& get_relative_file) {
			parsed_sound_cfg_t psc;

			auto get_filename = [&](std::string const& relfile) {
				return psc.filenames.insert(get_relative_file(filename, relfile)).first;
			};

			auto& file_error = errors[filename];

			auto const parsed_ini = ini::parse(input_string);

			bool run_flag = false,              //
			    flange_flag = false,            //
			    motor_flag = false,             //
			    switch_flag = false,            //
			    break_flag = false,             //
			    compressor_flag = false,        //
			    suspension_flag = false,        //
			    horn_flag = false,              //
			    door_flag = false,              //
			    ats_flag = false,               //
			    buzzer_flag = false,            //
			    pilot_lamp_flag = false,        //
			    break_handle_flag = false,      //
			    master_controller_flag = false, //
			    reverser_flag = false,          //
			    breaker_flag = false,           //
			    misc_flag = false;

			auto check_if_section_used = [&](bool& flag, char const* const name, std::size_t line) {
				if (flag) {
					std::ostringstream err;
					err << "[" << name << "] has already been defined. Overriding with new value."s;
					errors::add_error(file_error, line, err);
				}
				flag = true;
			};

			auto end_iterator = psc.filenames.end();

			for (auto const& section : parsed_ini) {
				if (util::match_against_lower(section.name, "run")) {
					check_if_section_used(run_flag, "run", section.line);
					psc.run_sounds =
					    parse_number_filename_pairs<run_t>(get_filename, file_error, section);
				}
				else if (util::match_against_lower(section.name, "flange")) {
					check_if_section_used(flange_flag, "flange", section.line);
					psc.flange_sounds =
					    parse_number_filename_pairs<flange_t>(get_filename, file_error, section);
				}
				else if (util::match_against_lower(section.name, "motor")) {
					check_if_section_used(motor_flag, "motor", section.line);
					psc.motor_sounds =
					    parse_number_filename_pairs<motor_t>(get_filename, file_error, section);
				}
				else if (util::match_against_lower(section.name, "switch")) {
					check_if_section_used(switch_flag, "switch", section.line);
					psc.switch_sounds =
					    parse_number_filename_pairs<switch_t>(get_filename, file_error, section);
				}
				else if (util::match_against_lower(section.name, "break")) {
					check_if_section_used(break_flag, "break", section.line);
					psc.break_sounds = parse_break(get_filename, file_error, section, end_iterator);
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
					    parse_number_filename_pairs<ats_t>(get_filename, file_error, section);
				}
				else if (util::match_against_lower(section.name, "buzzer")) {
					check_if_section_used(buzzer_flag, "buzzer", section.line);
					psc.buzzer_sounds =
					    parse_buzzer(get_filename, file_error, section, end_iterator);
				}
				else if (util::match_against_lower(section.name, "pilot lamp")) {
					check_if_section_used(pilot_lamp_flag, "pilot lamp", section.line);
					psc.pilot_lamp_sounds =
					    parse_pilot_lamp(get_filename, file_error, section, end_iterator);
				}
				else if (util::match_against_lower(section.name, "break handle")) {
					check_if_section_used(break_handle_flag, "break handle", section.line);
					psc.break_handle_sounds =
					    parse_break_handle(get_filename, file_error, section, end_iterator);
				}
				else if (util::match_against_lower(section.name, "master controller")) {
					check_if_section_used(master_controller_flag, "master controller",
					                      section.line);
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
					psc.breaker_sounds =
					    parse_breaker(get_filename, file_error, section, end_iterator);
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
					errors::add_error(file_error, section.line, err);
				}

				// check_version_number does the checking for us.
				if (!section.name.empty()) {
					for (auto const& value : section.values) {
						std::ostringstream err;
						err << "Unrecongnized value \"" << value.value << "\".";
						errors::add_error(file_error, value.line, err);
					}
				}
			}

			// Only functions that are raw structs. Maps don't have to be filled, they would just be
			// empty.
			if (!break_flag) {
				psc.break_sounds = create_empty_break(end_iterator);
			}
			if (!compressor_flag) {
				psc.compressor_sounds = create_empty_compressor(end_iterator);
			}
			if (!suspension_flag) {
				psc.suspension_sounds = create_empty_suspension(end_iterator);
			}
			if (!buzzer_flag) {
				psc.buzzer_sounds = create_empty_buzzer(end_iterator);
			}
			if (!pilot_lamp_flag) {
				psc.pilot_lamp_sounds = create_empty_pilot_lamp(end_iterator);
			}
			if (!break_handle_flag) {
				psc.break_handle_sounds = create_empty_break_handle(end_iterator);
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

			return psc;
		}
	} // namespace sound_cfg
} // namespace config
} // namespace parsers
