#pragma once

#include "core/datatypes.hpp"
#include "csv_rw_route/instructions.hpp"
#include "csv_rw_route/route_structure.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <functional>
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace bve::parsers::csv_rw_route {
	enum class file_type { csv, rw };

	struct preprocessed_line {
		std::string contents;
		std::size_t filename_index;
		std::size_t line;
		float offset;
	};

	struct preprocessed_lines {
		std::vector<preprocessed_line> lines;
		std::vector<std::string> filenames;
	};

	preprocessed_lines process_include_directives(const std::string& filename,
	                                              bve::core::datatypes::rng& rng,
	                                              errors::multi_error_t& errors,
	                                              file_type ft,
	                                              const find_relative_file_func& get_abs_path);

	void preprocess_file(preprocessed_lines& lines,
	                     bve::core::datatypes::rng& rng,
	                     errors::multi_error_t& errors,
	                     file_type ft);

	using instruction = mapbox::util::variant<instructions::naked::none,
	                                          instructions::naked::position,
	                                          // options commands
	                                          instructions::options::unit_of_length,
	                                          instructions::options::unit_of_speed,
	                                          instructions::options::block_length,
	                                          instructions::options::object_visibility,
	                                          instructions::options::section_behavior,
	                                          instructions::options::cant_behavior,
	                                          instructions::options::fog_behavior,
	                                          instructions::options::compatible_transparency_mode,
	                                          instructions::options::enable_bve_ts_hacks,
	                                          // route commands
	                                          instructions::route::comment,
	                                          instructions::route::image,
	                                          instructions::route::timetable,
	                                          instructions::route::change,
	                                          instructions::route::guage,
	                                          instructions::route::signal,
	                                          instructions::route::run_interval,
	                                          instructions::route::acceleration_due_to_gravity,
	                                          instructions::route::elevation,
	                                          instructions::route::temperature,
	                                          instructions::route::pressure,
	                                          instructions::route::display_speed,
	                                          instructions::route::loading_screen,
	                                          instructions::route::start_time,
	                                          instructions::route::dynamic_light,
	                                          instructions::route::ambiant_light,
	                                          instructions::route::directional_light,
	                                          instructions::route::light_direction,
	                                          // train commands
	                                          instructions::train::folder,
	                                          instructions::train::rail,
	                                          instructions::train::flange,
	                                          instructions::train::timetable,
	                                          instructions::train::velocity,
	                                          // structure commands
	                                          instructions::structure::command,
	                                          instructions::structure::pole,
	                                          // texture commands
	                                          instructions::texture::background_load,
	                                          instructions::texture::background_x,
	                                          instructions::texture::background_aspect,
	                                          // cycle commands
	                                          instructions::cycle::ground,
	                                          instructions::cycle::rail,
	                                          // signal commands
	                                          instructions::naked::signal_animated,
	                                          instructions::naked::signal,
	                                          // track,
	                                          instructions::track::rail_start,
	                                          instructions::track::rail,
	                                          instructions::track::rail_type,
	                                          instructions::track::rail_end,
	                                          instructions::track::adhesion,
	                                          instructions::track::pitch,
	                                          instructions::track::curve,
	                                          instructions::track::turn,
	                                          instructions::track::height,
	                                          instructions::track::free_obj,
	                                          instructions::track::wall,
	                                          instructions::track::wall_end,
	                                          instructions::track::dike,
	                                          instructions::track::dike_end,
	                                          instructions::track::pole,
	                                          instructions::track::pole_end,
	                                          instructions::track::crack,
	                                          instructions::track::ground,
	                                          instructions::track::sta,
	                                          instructions::track::station_xml,
	                                          instructions::track::stop,
	                                          instructions::track::form,
	                                          instructions::track::limit,
	                                          instructions::track::section,
	                                          instructions::track::sig_f,
	                                          instructions::track::signal,
	                                          instructions::track::relay,
	                                          instructions::track::beacon,
	                                          instructions::track::transponder,
	                                          instructions::track::pattern,
	                                          instructions::track::back,
	                                          instructions::track::fog,
	                                          instructions::track::brightness,
	                                          instructions::track::marker,
	                                          instructions::track::marker_xml,
	                                          instructions::track::text_marker,
	                                          instructions::track::point_of_interest,
	                                          instructions::track::pre_train,
	                                          instructions::track::announce,
	                                          instructions::track::doppler,
	                                          instructions::track::buffer>;

	struct instruction_list {
		std::vector<instruction> instructions;
		std::vector<std::string> filenames;
	};

	namespace line_splitting {
		struct instruction_info {
			std::string name;
			std::vector<std::string> indices;
			std::vector<std::string> args;
			std::string suffix;
			float offset;
			bool track_position = false;
		};

		instruction_info csv(const preprocessed_line& l);
		instruction_info rw(const preprocessed_line& l);
	} // namespace line_splitting

	instruction_list generate_instructions(const preprocessed_lines& lines,
	                                       errors::multi_error_t& errors,
	                                       file_type ft);

	void execute_instructions_pass1(instruction_list& list, errors::multi_error_t& errors);
	parsed_route_data execute_instructions_pass2(instruction_list& list,
	                                             errors::multi_error_t& errors);
	void execute_instructions_pass3(parsed_route_data& rd,
	                                instruction_list& list,
	                                errors::multi_error_t& errors,
	                                const find_relative_file_func& get_abs_path);

} // namespace bve::parsers::csv_rw_route

std::ostream& operator<<(std::ostream& os, const bve::parsers::csv_rw_route::instruction& i);
std::ostream& operator<<(std::ostream& os,
                         const bve::parsers::csv_rw_route::instruction_list& list);
