#pragma once

#include "parsers/csv_rw_route.hpp"
#include "parsers/utils.hpp"
#include <gsl/gsl_util>
#include <map>
#include <sstream>

namespace bve::parsers::csv_rw_route::instruction_generation {
	/////////////////////////////////////////////
	// Argument/Indices Count Helper Functions //
	/////////////////////////////////////////////

	inline void args_at_least(const line_splitting::InstructionInfo& inst,
	                          std::size_t const num,
	                          const char* const name) {
		if (inst.args.size() < num) {
			std::ostringstream oss;
			oss << name << " must have at least " << num << " argument" << (num == 1 ? "" : "s");
			throw std::invalid_argument(oss.str());
		}
	}

	inline void indices_at_least(const line_splitting::InstructionInfo& inst,
	                             std::size_t const num,
	                             const char* const name) {
		if (inst.indices.size() < num) {
			std::ostringstream oss;
			oss << name << " must have at least " << num << " ind" << (num == 1 ? "ex" : "ices");
			throw std::invalid_argument(oss.str());
		}
	}

	//////////////////////////////
	// Common Instruction Forms //
	//////////////////////////////

	template <class T>
	static Instruction create_single_string_instruction(const line_splitting::InstructionInfo& inst,
	                                                    const char* name) {
		args_at_least(inst, 1, name);

		return T{inst.args[0]};
	}

	template <class T>
	static Instruction create_single_float_instruction(const line_splitting::InstructionInfo& inst,
	                                                   const char* name) {
		args_at_least(inst, 1, name);

		return T{util::parse_loose_float(inst.args[0])};
	}

	template <class T>
	static Instruction create_single_float_instruction(const line_splitting::InstructionInfo& inst,
	                                                   const char* name,
	                                                   float def) {
		(void) name;

		if (!inst.args.empty()) {
			return T{util::parse_loose_float(inst.args[0], def)};
		}

		return T{def};
	}

	template <class T>
	static Instruction create_single_uint_instruction(const line_splitting::InstructionInfo& inst,
	                                                  const char* name) {
		args_at_least(inst, 1, name);

		return T{gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[0]))};
	}

	template <class T>
	static Instruction create_single_uint_instruction(const line_splitting::InstructionInfo& inst,
	                                                  const char* name,
	                                                  std::intmax_t def) {
		(void) name;

		if (!inst.args.empty()) {
			return T{gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[0], def))};
		}

		return T{def};
	}

	template <class T>
	static Instruction create_single_time_instruction(const line_splitting::InstructionInfo& inst,
	                                                  const char* name) {
		args_at_least(inst, 1, name);

		return T{util::parse_time(inst.args[0])};
	}

	template <std::size_t Offset, class T>
	static void set_positions(T& inst, const line_splitting::InstructionInfo& instr_info) {
		if (instr_info.args.size() > Offset) {
			switch (instr_info.args.size()) {
				default:
				case Offset + 5:
					inst.roll = util::parse_loose_float(instr_info.args[Offset + 4], 0);
					// fall through
				case Offset + 4:
					inst.pitch = util::parse_loose_float(instr_info.args[Offset + 3], 0);
					// fall through
				case Offset + 3:
					inst.yaw = util::parse_loose_float(instr_info.args[Offset + 2], 0);
					// fall through
				case Offset + 2:
					inst.y_offset = util::parse_loose_float(instr_info.args[Offset + 1], 0);
					// fall through
				case Offset + 1:
					inst.x_offset = util::parse_loose_float(instr_info.args[Offset + 0], 0);
					// fall through
				case Offset:
					break;
			}
		}
	}

	using InstInfo = line_splitting::InstructionInfo;

	// string_instruction_mapping.cpp
	extern const std::map<std::string, Instruction (*)(const InstInfo& inst)> function_mapping;
	extern const std::map<std::string, instructions::structure::Command::Type> command_type_mapping;

	// location_statement.cpp
	Instruction create_instruction_location_statement(const InstInfo& /*inst*/);

	// options.cpp
	Instruction create_instruction_options_unitoflength(const InstInfo& /*inst*/);
	Instruction create_instruction_options_unitofspeed(const InstInfo& /*inst*/);
	Instruction create_instruction_options_blocklength(const InstInfo& /*inst*/);
	Instruction create_instruction_options_objectvisibility(const InstInfo& /*inst*/);
	Instruction create_instruction_options_sectionbehavior(const InstInfo& /*inst*/);
	Instruction create_instruction_options_cantbehavior(const InstInfo& /*inst*/);
	Instruction create_instruction_options_fogbehavior(const InstInfo& /*inst*/);
	Instruction create_instruction_options_compatibletransparencymode(const InstInfo& /*inst*/);
	Instruction create_instruction_options_enablebvetshacks(const InstInfo& /*inst*/);

	// route.cpp
	Instruction create_instruction_route_comment(const InstInfo& /*inst*/);
	Instruction create_instruction_route_image(const InstInfo& /*inst*/);
	Instruction create_instruction_route_timetable(const InstInfo& /*inst*/);
	Instruction create_instruction_route_change(const InstInfo& /*inst*/);
	Instruction create_instruction_route_gauge(const InstInfo& /*inst*/);
	Instruction create_instruction_route_signal(const InstInfo& /*inst*/);
	Instruction create_instruction_route_runinterval(const InstInfo& /*inst*/);
	Instruction create_instruction_route_accelerationduetogravity(const InstInfo& /*inst*/);
	Instruction create_instruction_route_elevation(const InstInfo& /*inst*/);
	Instruction create_instruction_route_temperature(const InstInfo& /*inst*/);
	Instruction create_instruction_route_pressure(const InstInfo& /*inst*/);
	Instruction create_instruction_route_displayspeed(const InstInfo& /*inst*/);
	Instruction create_instruction_route_loadingscreen(const InstInfo& /*inst*/);
	Instruction create_instruction_route_starttime(const InstInfo& /*inst*/);
	Instruction create_instruction_route_dynamiclight(const InstInfo& /*inst*/);
	Instruction create_instruction_route_ambientlight(const InstInfo& /*inst*/);
	Instruction create_instruction_route_directionallight(const InstInfo& /*inst*/);
	Instruction create_instruction_route_lightdirection(const InstInfo& /*inst*/);

	// train.cpp
	Instruction create_instruction_train_folder(const InstInfo& /*inst*/);
	Instruction create_instruction_train_run(const InstInfo& /*inst*/);
	Instruction create_instruction_train_flange(const InstInfo& /*inst*/);
	Instruction create_instruction_train_timetable(const InstInfo& /*inst*/);
	Instruction create_instruction_train_velocity(const InstInfo& /*inst*/);

	// structure.cpp
	Instruction create_instruction_structure_command(const InstInfo& /*inst*/);
	Instruction create_instruction_structure_pole(const InstInfo& /*inst*/);

	// texture.cpp
	Instruction create_instruction_texture_background(const InstInfo& /*inst*/);

	// cycle.cpp
	Instruction create_instruction_cycle_ground(const InstInfo& /*inst*/);
	Instruction create_instruction_cycle_rail(const InstInfo& /*inst*/);

	// signal.cpp
	Instruction create_instruction_signal(const InstInfo& /*inst*/);

	// track_rails.cpp
	Instruction create_instruction_track_railstart(const InstInfo& /*inst*/);
	Instruction create_instruction_track_rail(const InstInfo& /*inst*/);
	Instruction create_instruction_track_railtype(const InstInfo& /*inst*/);
	Instruction create_instruction_track_railend(const InstInfo& /*inst*/);
	Instruction create_instruction_track_accuracy(const InstInfo& /*inst*/);
	Instruction create_instruction_track_adhesion(const InstInfo& /*inst*/);
	Instruction create_instruction_track_pitch(const InstInfo& /*inst*/);

	// track_geometry.cpp
	Instruction create_instruction_track_curve(const InstInfo& /*inst*/);
	Instruction create_instruction_track_turn(const InstInfo& /*inst*/);
	Instruction create_instruction_track_height(const InstInfo& /*inst*/);

	// track_objects.cpp
	Instruction create_instruction_track_freeobj(const InstInfo& /*inst*/);
	Instruction create_instruction_track_wall(const InstInfo& /*inst*/);
	Instruction create_instruction_track_wallend(const InstInfo& /*inst*/);
	Instruction create_instruction_track_dike(const InstInfo& /*inst*/);
	Instruction create_instruction_track_dikeend(const InstInfo& /*inst*/);
	Instruction create_instruction_track_pole(const InstInfo& /*inst*/);
	Instruction create_instruction_track_poleend(const InstInfo& /*inst*/);
	Instruction create_instruction_track_crack(const InstInfo& /*inst*/);
	Instruction create_instruction_track_ground(const InstInfo& /*inst*/);

	// track_stations.cpp
	Instruction create_instruction_track_sta(const InstInfo& /*inst*/);
	Instruction create_instruction_track_station_xml(const InstInfo& /*inst*/);
	Instruction create_instruction_track_station(const InstInfo& /*inst*/);
	Instruction create_instruction_track_stop(const InstInfo& /*inst*/);
	Instruction create_instruction_track_form(const InstInfo& /*inst*/);

	// track_signals_limits.cpp
	Instruction create_instruction_track_limit(const InstInfo& /*inst*/);
	Instruction create_instruction_track_section(const InstInfo& /*inst*/);
	Instruction create_instruction_track_sigf(const InstInfo& /*inst*/);
	Instruction create_instruction_track_signal(const InstInfo& /*inst*/);
	Instruction create_instruction_track_relay(const InstInfo& /*inst*/);

	// track_safety.cpp
	Instruction create_instruction_track_beacon(const InstInfo& /*inst*/);
	Instruction create_instruction_track_transponder(const InstInfo& /*inst*/);
	Instruction create_instruction_track_atssn(const InstInfo& /*inst*/);
	Instruction create_instruction_track_atsp(const InstInfo& /*inst*/);
	Instruction create_instruction_track_pattern(const InstInfo& /*inst*/);
	Instruction create_instruction_track_plimit(const InstInfo& /*inst*/);

	// track_misc.cpp
	Instruction create_instruction_track_back(const InstInfo& /*inst*/);
	Instruction create_instruction_track_fog(const InstInfo& /*inst*/);
	Instruction create_instruction_track_brightness(const InstInfo& /*inst*/);
	Instruction create_instruction_track_marker(const InstInfo& /*inst*/);
	Instruction create_instruction_track_text_marker(const InstInfo& /*inst*/);
	Instruction create_instruction_track_pointofinterest(const InstInfo& /*inst*/);
	Instruction create_instruction_track_pretrain(const InstInfo& /*inst*/);
	Instruction create_instruction_track_announce(const InstInfo& /*inst*/);
	Instruction create_instruction_track_doppler(const InstInfo& /*inst*/);
	Instruction create_instruction_track_buffer(const InstInfo& /*inst*/);
} // namespace bve::parsers::csv_rw_route::instruction_generation
