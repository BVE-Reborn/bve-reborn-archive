#pragma once

#include "core/datatypes.hpp"
#include "route_structure.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include <tl/optional.hpp>
#include <cstddef>
#include <string>
#include <vector>

namespace parsers {
namespace csv_rw_route {
	namespace instructions {

		struct instruction_base {
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		namespace naked {
			struct position : instruction_base {
				// UnitOfLength
				std::vector<float> distances;
			};

			// for ignored instructions
			struct none : instruction_base {};
		} // namespace naked

		namespace options {
			struct unit_of_length : instruction_base {
				std::vector<float> factors_in_meters;
			};

			struct unit_of_speed {
				float factor_in_kph;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct block_length {
				// UnitOfLength
				float length;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct object_visibility : instruction_base {
				enum { legacy, track_based } mode = legacy;
			};

			struct section_behavior : instruction_base {
				enum { normal, simplified } mode = normal;
			};

			struct cant_behavior : instruction_base {
				enum { unsigned_cant, signed_cant } mode = unsigned_cant;
			};

			struct fog_behavior : instruction_base {
				enum { block_based, interpolated } mode = block_based;
			};

			struct compatible_transparency_mode : instruction_base {
				enum { off, on } mode = off;
			};

			struct enable_bve_ts_hacks : instruction_base {
				enum { off, on } mode = off;
			};
		} // namespace options

		namespace route {
			struct comment {
				std::string text;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct image {
				std::string filename;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct timetable {
				std::string text;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct change : instruction_base {
				enum {
					safty_activiated_service_brakes,
					safty_activiated_emergency_brakes,
					safty_deactivated_emergency_brakes
				} mode = safty_activiated_service_brakes;
			};

			struct guage {
				// millimeters
				float width = 1435;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct signal : instruction_base {
				std::size_t aspect_index;
				// UnitOfSpeed
				float speed;
				float absolute_position = -1;
			};

			struct run_interval : instruction_base {
				// seconds
				std::vector<float> time_interval;
				float absolute_position = -1;
			};

			struct acceleration_due_to_gravity {
				// m / s^2
				float value;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct elevation {
				// UnitOfLength
				float height = 0;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct temperature {
				// celcius
				float celcius;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct pressure {
				// kpa
				float kpa = 101.325f;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct display_speed : instruction_base {
				std::string unit_string;
				// relative to kph
				float conversion_factor;
			};

			struct loading_screen {
				std::string filename;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct start_time {
				bvereborn::datatypes::time time;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct dynamic_light {
				std::string filename;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct ambiant_light : instruction_base {
				bvereborn::datatypes::color8_rgb color = {160, 160, 160};
			};

			struct directional_light : instruction_base {
				bvereborn::datatypes::color8_rgb color = {160, 160, 160};
			};

			struct light_direction : instruction_base {
				// degrees
				float theta = 60.0f;
				// degrees
				float phi = -26.57f;
			};
		} // namespace route

		namespace train {
			// Train.Folder
			// Train.File
			struct folder {
				std::string filepath;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			// Train.Run
			// Train.Rail
			struct rail : instruction_base {
				std::size_t rail_type_index;
				std::size_t run_sound_index;
			};

			struct flange : instruction_base {
				std::size_t rail_type_index;
				std::size_t flange_sound_index;
			};

			struct timetable : instruction_base {
				bool day;
				std::size_t timetable_index;
				std::string filename;
			};

			using guage = route::guage;

			using interval = route::run_interval;

			struct velocity {
				// UnitOfSpeed
				float speed;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};
		} // namespace train

		namespace structure {
			struct command : instruction_base {
				enum : uint8_t {
					ground,
					rail,
					wall_l,
					wall_r,
					dike_l,
					dike_r,
					form_l,
					form_r,
					form_cl,
					form_cr,
					roof_l,
					roof_r,
					roof_cl,
					roof_cr,
					crack_l,
					crack_r,
					free_obj,
					beacon
				} command_type;

				std::size_t structure_index;

				std::string filename;
			};

			struct pole : instruction_base {
				std::size_t additional_rails;
				std::size_t pole_structure_index;
				std::string filename;
			};
		} // namespace structure

		namespace texture {
			struct background_load : instruction_base {
				std::size_t background_texture_index;
				std::string filename;
			};

			struct background_x : instruction_base {
				std::size_t background_texture_index;
				std::size_t repetition_count;
			};

			struct background_aspect : instruction_base {
				std::size_t background_texture_index;
				enum : uint8_t { fixed, aspect } mode = fixed;
			};
		} // namespace texture

		namespace cycle {
			struct ground : instruction_base {
				std::size_t cycle_structure_index;
				std::vector<std::size_t> input_indices;
			};

			struct rail : instruction_base {
				std::size_t cycle_structure_index;
				std::vector<std::size_t> input_indices;
			};
		} // namespace cycle

		namespace naked {
			struct signal_animated : instruction_base {
				std::size_t signal_index;
				std::string filename;
			};

			struct signal : instruction_base {
				std::size_t signal_index;
				// Without file extant
				std::string signal_filename;
				// Without file extant
				std::string glow_filename;
			};
		} // namespace naked

		namespace track {
			struct rail_start : instruction_base {
				std::size_t rail_index;
				// UnitOfLength
				tl::optional<float> x_offset;
				// UnitOfLength
				tl::optional<float> y_offset;
				tl::optional<std::size_t> rail_type;
			};

			struct rail : instruction_base {
				std::size_t rail_index;
				// UnitOfLength
				tl::optional<float> x_offset;
				// UnitOfLength
				tl::optional<float> y_offset;
				tl::optional<std::size_t> rail_type;
			};

			struct rail_type : instruction_base {
				std::size_t rail_index = 0;
				std::size_t rail_type_number = 0;
			};

			struct rail_end : instruction_base {
				std::size_t rail_index;
				// UnitOfLength
				tl::optional<float> x_offset;
				// UnitOfLength
				tl::optional<float> y_offset;
			};

			struct adhesion {
				float value = 100;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct pitch {
				// Per thousands
				float rate = 0;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct curve : instruction_base {
				// UnitOfLength
				float radius = 0;
				// Millimeters
				float cant = 0;
			};

			struct turn {
				float ratio = 0;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct height {
				// UnitOfLength
				float y;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct free_obj : instruction_base {
				std::size_t rail_index = 0;
				std::size_t free_obj_structure_index = 0;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
				// degrees
				float yaw = 0;
				// degrees
				float pitch = 0;
				// degrees
				float roll = 0;
			};

			struct wall : instruction_base {
				std::size_t rail_index;
				enum : uint8_t { left, both, right } direction = both;
				std::size_t wall_structure_index = 0;
			};

			struct wall_end {
				std::size_t rail_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct dike : instruction_base {
				std::size_t rail_index;
				enum : uint8_t { left, both, right } direction = both;
				std::size_t dike_structure_index = 0;
			};

			struct dike_end {
				std::size_t rail_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct pole : instruction_base {
				std::size_t rail_index = 0;
				std::size_t additional_rails = 0;
				std::intmax_t location = 0;
				// UnitOfLength
				std::intmax_t interval = 1;
				std::size_t pole_structure_index = 0;
			};

			struct pole_end {
				std::size_t rail_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct crack : instruction_base {
				std::size_t rail_index_1;
				std::size_t rail_index_2;
				std::size_t crack_structure_index = 0;
			};

			struct ground {
				std::size_t ground_structure_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct sta : instruction_base {
				std::string name;
				std::string arrival_sound;
				std::string departure_sound;
				std::size_t timetable_index = 0;
				bvereborn::datatypes::time arrival = 0;
				bvereborn::datatypes::time departure = 0;
				float stop_duration = 15;
				float passenger_ratio = 100;
				bool pass_alarm = false;
				bool force_red = false;
				bool system = false;
				using arrival_time_t = rail_station_info::arrival_time_t;
				arrival_time_t arrival_tag = arrival_time_t::any_time;
				using departure_time_t = rail_station_info::departure_time_t;
				departure_time_t departure_tag = departure_time_t::any_time;
				using doors_t = rail_station_info::doors_t;
				doors_t doors = doors_t::none;
			};

			struct station_xml : instruction_base {
				std::string filename;
			};

			struct stop : instruction_base {
				using stop_post_t = direction_t;
				stop_post_t stop_post = stop_post_t::none;

				float forwards_tolerance = 5;
				float backwards_tolerance = 5;
				std::size_t cars = 0;
			};

			struct form : instruction_base {
				std::size_t rail_index_1;
				std::size_t rail_index_2 = 0;
				std::size_t roof_structure_index = 0;
				std::size_t form_structure_index = 0;
				enum { left, right, rail_index } placement = rail_index;
			};

			struct limit : instruction_base {
				// Unit of speed
				float speed = 0;
				enum class post_t { left, none, right } post = post_t::none;
				enum class cource_t { left, none, right } cource = cource_t::none;
			};

			struct section : instruction_base {
				std::vector<std::size_t> a_term;
			};

			struct sig_f : instruction_base {
				std::size_t signal_index;
				std::size_t section;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
				// Degrees
				float yaw = 0;
				// Degrees
				float pitch = 0;
				// Degrees
				float roll = 0;
			};

			// Track.Signal
			// Track.Sig
			struct signal : instruction_base {
				enum : uint8_t {
					r_y,
					r_g,
					r_y_g,
					r_yy_y_g,
					r_y_yg_g,
					r_yy_y_yg_g,
					r_y_yg_g_gg,
					r_yy_y_yg_g_gg
				} type = r_g;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
				// Degrees
				float yaw = 0;
				// Degrees
				float pitch = 0;
				// Degrees
				float roll = 0;
			};

			struct relay : instruction_base {
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
				// Degrees
				float yaw = 0;
				// Degrees
				float pitch = 0;
				// Degrees
				float roll = 0;
			};

			struct beacon : instruction_base {
				std::size_t type;
				std::size_t beacon_structure_index;
				std::intmax_t section;
				std::intmax_t data;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
				// Degrees
				float yaw = 0;
				// Degrees
				float pitch = 0;
				// Degrees
				float roll = 0;
			};

			// Track.Transponder
			// Track.Tr
			// Track.AtsSn
			// Track.AtsP
			struct transponder : instruction_base {
				enum : uint8_t {
					s_type = 0,
					sn_type = 1,
					departure = 2,
					ats_p_renewal = 3,
					ats_p_stop = 4
				} type = s_type;
				std::size_t signal = 0;
				bool switch_system = true;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
				// Degrees
				float yaw = 0;
				// Degrees
				float pitch = 0;
				// Degrees
				float roll = 0;
			};

			struct pattern : instruction_base {
				enum : bool { temporary, permanent } type = temporary;
				// UnitOfSpeed
				float speed;
			};

			struct back {
				std::size_t background_texture_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct fog : instruction_base {
				// UnitOfLength
				float starting_distance = 0;
				// UnitOfLength
				float ending_distance = 0;
				bvereborn::datatypes::color8_rgb color = {128, 128, 128};
			};

			struct brightness : instruction_base {
				std::uint8_t value = 255;
			};

			struct marker : instruction_base {
				std::string filename;
				// UnitOfLength
				float distance;
			};

			struct marker_xml : instruction_base {
				std::string filename;
			};

			struct text_marker : instruction_base {
				std::string text;
				// UnitOfLength
				float distance;
				enum : uint8_t { black, gray, white, red, orange, green, blue, magenta } font_color;
			};

			struct point_of_interest : instruction_base {
				std::size_t rail_index;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
				// Degrees
				float yaw = 0;
				// Degrees
				float pitch = 0;
				// Degrees
				float roll = 0;
				std::string text;
			};

			struct pre_train : instruction_base {
				bvereborn::datatypes::time time;
			};

			struct announce : instruction_base {
				std::string filename;
				// UnitOfSpeed
				float speed = 0;
			};

			struct doppler : instruction_base {
				std::string filename;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
			};

			struct buffer : instruction_base {};
		} // namespace track
	}     // namespace instructions
} // namespace csv_rw_route
} // namespace parsers
