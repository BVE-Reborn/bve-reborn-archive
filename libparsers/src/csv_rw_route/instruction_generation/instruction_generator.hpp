#pragma once

#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <map>
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		/////////////////////////////////////////////
		// Argument/Indices Count Helper Functions //
		/////////////////////////////////////////////

		inline void args_at_least(const line_splitting::instruction_info& inst,
		                          std::size_t num,
		                          const char* name) {
			if (inst.args.size() < num) {
				std::ostringstream oss;
				oss << name << " must have at least " << num << " argument"
				    << (num == 1 ? "" : "s");
				throw std::invalid_argument(oss.str());
			}
		}

		inline void indices_at_least(const line_splitting::instruction_info& inst,
		                             std::size_t num,
		                             const char* name) {
			if (inst.indices.size() < num) {
				std::ostringstream oss;
				oss << name << " must have at least " << num << " ind"
				    << (num == 1 ? "ex" : "ices");
				throw std::invalid_argument(oss.str());
			}
		}

		//////////////////////////////
		// Common Instruction Forms //
		//////////////////////////////

		template <class T>
		static instruction create_single_string_instruction(
		    const line_splitting::instruction_info& inst,
		    const char* name) {
			args_at_least(inst, 1, name);

			return T{inst.args[0]};
		}

		template <class T>
		static instruction create_single_float_instruction(
		    const line_splitting::instruction_info& inst,
		    const char* name) {
			args_at_least(inst, 1, name);

			return T{util::parse_loose_float(inst.args[0])};
		}

		template <class T>
		static instruction create_single_float_instruction(
		    const line_splitting::instruction_info& inst,
		    const char* name,
		    float def) {
			(void) name;

			if (!inst.args.empty()) {
				return T{util::parse_loose_float(inst.args[0], def)};
			}

			return T{def};
		}

		template <class T>
		static instruction create_single_sizet_instruction(
		    const line_splitting::instruction_info& inst,
		    const char* name) {
			args_at_least(inst, 1, name);

			return T{std::size_t(util::parse_loose_integer(inst.args[0]))};
		}

		template <class T>
		static instruction create_single_sizet_instruction(
		    const line_splitting::instruction_info& inst,
		    const char* name,
		    std::intmax_t def) {
			(void) name;

			if (!inst.args.empty()) {
				return T{std::size_t(util::parse_loose_integer(inst.args[0], def))};
			}

			return T{def};
		}

		template <class T>
		static instruction create_single_time_instruction(
		    const line_splitting::instruction_info& inst,
		    const char* name) {
			args_at_least(inst, 1, name);

			return T{util::parse_time(inst.args[0])};
		}

		template <std::size_t offset, class T>
		static void set_positions(T& inst, const line_splitting::instruction_info& instr_info) {
			if (instr_info.args.size() > offset) {
				switch (instr_info.args.size()) {
					default:
					case offset + 5:
						inst.roll = util::parse_loose_float(instr_info.args[offset + 4], 0);
						// fall through
					case offset + 4:
						inst.pitch = util::parse_loose_float(instr_info.args[offset + 3], 0);
						// fall through
					case offset + 3:
						inst.yaw = util::parse_loose_float(instr_info.args[offset + 2], 0);
						// fall through
					case offset + 2:
						inst.y_offset = util::parse_loose_float(instr_info.args[offset + 1], 0);
						// fall through
					case offset + 1:
						inst.x_offset = util::parse_loose_float(instr_info.args[offset + 0], 0);
						// fall through
					case offset:
						break;
				}
			}
		}

		using inst_info = line_splitting::instruction_info;

		// string_instruction_mapping.cpp
		extern const std::map<std::string, instruction (*)(const inst_info& inst)> function_mapping;
		extern const std::map<std::string, decltype(instructions::structure::Command::command)>
		    command_type_mapping;

		// location_statement.cpp
		instruction create_instruction_location_statement(const inst_info&);

		// optoins.cpp
		instruction create_instruction_options_unitoflength(const inst_info&);
		instruction create_instruction_options_unitofspeed(const inst_info&);
		instruction create_instruction_options_blocklength(const inst_info&);
		instruction create_instruction_options_objectvisibility(const inst_info&);
		instruction create_instruction_options_sectionbehavior(const inst_info&);
		instruction create_instruction_options_cantbehavior(const inst_info&);
		instruction create_instruction_options_fogbehavior(const inst_info&);
		instruction create_instruction_options_compatibletransparencymode(const inst_info&);
		instruction create_instruction_options_enablebvetshacks(const inst_info&);

		// route.cpp
		instruction create_instruction_route_comment(const inst_info&);
		instruction create_instruction_route_image(const inst_info&);
		instruction create_instruction_route_timetable(const inst_info&);
		instruction create_instruction_route_change(const inst_info&);
		instruction create_instruction_route_gauge(const inst_info&);
		instruction create_instruction_route_signal(const inst_info&);
		instruction create_instruction_route_runinterval(const inst_info&);
		instruction create_instruction_route_accelerationduetogravity(const inst_info&);
		instruction create_instruction_route_elevation(const inst_info&);
		instruction create_instruction_route_temperature(const inst_info&);
		instruction create_instruction_route_pressure(const inst_info&);
		instruction create_instruction_route_displayspeed(const inst_info&);
		instruction create_instruction_route_loadingscreen(const inst_info&);
		instruction create_instruction_route_starttime(const inst_info&);
		instruction create_instruction_route_dynamiclight(const inst_info&);
		instruction create_instruction_route_ambientlight(const inst_info&);
		instruction create_instruction_route_directionallight(const inst_info&);
		instruction create_instruction_route_lightdirection(const inst_info&);

		// train.cpp
		instruction create_instruction_train_folder(const inst_info&);
		instruction create_instruction_train_run(const inst_info&);
		instruction create_instruction_train_flange(const inst_info&);
		instruction create_instruction_train_timetable(const inst_info&);
		instruction create_instruction_train_velocity(const inst_info&);

		// structure.cpp
		instruction create_instruction_structure_command(const inst_info&);
		instruction create_instruction_structure_pole(const inst_info&);

		// texture.cpp
		instruction create_instruction_texture_background(const inst_info&);

		// cycle.cpp
		instruction create_instruction_cycle_ground(const inst_info&);
		instruction create_instruction_cycle_rail(const inst_info&);

		// signal.cpp
		instruction create_instruction_signal(const inst_info&);

		// track_rails.cpp
		instruction create_instruction_track_railstart(const inst_info&);
		instruction create_instruction_track_rail(const inst_info&);
		instruction create_instruction_track_railtype(const inst_info&);
		instruction create_instruction_track_railend(const inst_info&);
		instruction create_instruction_track_accuracy(const inst_info&);
		instruction create_instruction_track_adhesion(const inst_info&);
		instruction create_instruction_track_pitch(const inst_info&);

		// track_geometry.cpp
		instruction create_instruction_track_curve(const inst_info&);
		instruction create_instruction_track_turn(const inst_info&);
		instruction create_instruction_track_height(const inst_info&);

		// track_objects.cpp
		instruction create_instruction_track_freeobj(const inst_info&);
		instruction create_instruction_track_wall(const inst_info&);
		instruction create_instruction_track_wallend(const inst_info&);
		instruction create_instruction_track_dike(const inst_info&);
		instruction create_instruction_track_dikeend(const inst_info&);
		instruction create_instruction_track_pole(const inst_info&);
		instruction create_instruction_track_poleend(const inst_info&);
		instruction create_instruction_track_crack(const inst_info&);
		instruction create_instruction_track_ground(const inst_info&);

		// track_stations.cpp
		instruction create_instruction_track_sta(const inst_info&);
		instruction create_instruction_track_station(const inst_info&);
		instruction create_instruction_track_stop(const inst_info&);
		instruction create_instruction_track_form(const inst_info&);

		// track_singals_limits.cpp
		instruction create_instruction_track_limit(const inst_info&);
		instruction create_instruction_track_section(const inst_info&);
		instruction create_instruction_track_sigf(const inst_info&);
		instruction create_instruction_track_signal(const inst_info&);
		instruction create_instruction_track_relay(const inst_info&);

		// track_safety.cpp
		instruction create_instruction_track_beacon(const inst_info&);
		instruction create_instruction_track_transponder(const inst_info&);
		instruction create_instruction_track_atssn(const inst_info&);
		instruction create_instruction_track_atsp(const inst_info&);
		instruction create_instruction_track_pattern(const inst_info&);
		instruction create_instruction_track_plimit(const inst_info&);

		// track_misc.cpp
		instruction create_instruction_track_back(const inst_info&);
		instruction create_instruction_track_fog(const inst_info&);
		instruction create_instruction_track_brightness(const inst_info&);
		instruction create_instruction_track_marker(const inst_info&);
		instruction create_instruction_track_text_marker(const inst_info&);
		instruction create_instruction_track_pointofinterest(const inst_info&);
		instruction create_instruction_track_pretrain(const inst_info&);
		instruction create_instruction_track_announce(const inst_info&);
		instruction create_instruction_track_doppler(const inst_info&);
		instruction create_instruction_track_buffer(const inst_info&);
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
