#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_route_comment(const line_splitting::InstructionInfo& inst) {
		return create_single_string_instruction<instructions::route::Comment>(inst, "Route.Comment");
	}

	Instruction create_instruction_route_image(const line_splitting::InstructionInfo& inst) {
		return create_single_string_instruction<instructions::route::Image>(inst, "Route.Image");
	}

	Instruction create_instruction_route_timetable(const line_splitting::InstructionInfo& inst) {
		return create_single_string_instruction<instructions::route::Timetable>(inst,
		                                                                        "Route."
		                                                                        "Timetable");
	}

	Instruction create_instruction_route_change(const line_splitting::InstructionInfo& inst) {
		instructions::route::Change change;

		if (!inst.args.empty()) {
			auto const value = util::parsers::parse_loose_integer(inst.args[0], -1);
			switch (value) {
				default:
				case -1:
					change.mode = instructions::route::Change::Mode::safety_activated_service_brakes;
					break;
				case 0:
					change.mode = instructions::route::Change::Mode::safety_activated_emergency_brakes;
					break;
				case 1:
					change.mode = instructions::route::Change::Mode::safety_deactivated_emergency_brakes;
					break;
			}
		}
		else {
			change.mode = instructions::route::Change::Mode::safety_activated_service_brakes;
		}

		return change;
	}

	Instruction create_instruction_route_gauge(const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::route::Gauge>(inst, "Route.Gauge", 1435);
	}

	Instruction create_instruction_route_signal(const line_splitting::InstructionInfo& inst) {
		indices_at_least(inst, 1, "Route.Signal");
		args_at_least(inst, 1, "Route.Signal");

		instructions::route::Signal s;

		s.aspect_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(inst.indices[0]));
		s.speed = util::parsers::parse_loose_float(inst.args[0]);

		return s;
	}

	Instruction create_instruction_route_runinterval(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Route.RunInterval");

		instructions::route::RunInterval ri;
		ri.time_interval.reserve(inst.args.size());
		std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(ri.time_interval),
		               [](const std::string& s) { return util::parsers::parse_loose_float(s); });

		return ri;
	}

	Instruction create_instruction_route_accelerationduetogravity(const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::route::AccelerationDueToGravity>(inst,
		                                                                                      "Route."
		                                                                                      "AccelerationDue"
		                                                                                      "ToGravity",
		                                                                                      9.80665F);
	}

	Instruction create_instruction_route_elevation(const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::route::Elevation>(inst,
		                                                                       "Route."
		                                                                       "Elevation",
		                                                                       0);
	}

	Instruction create_instruction_route_temperature(const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::route::Temperature>(inst,
		                                                                         "Route."
		                                                                         "Temperature",
		                                                                         20);
	}

	Instruction create_instruction_route_pressure(const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::route::Pressure>(inst, "Route.Pressure", 101.325F);
	}

	Instruction create_instruction_route_displayspeed(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 2, "Route.DisplaySpeed");

		instructions::route::DisplaySpeed ds;

		ds.unit_string = inst.args[0];
		ds.conversion_factor = util::parsers::parse_loose_float(inst.args[1]);

		return ds;
	}

	Instruction create_instruction_route_loadingscreen(const line_splitting::InstructionInfo& inst) {
		return create_single_string_instruction<instructions::route::LoadingScreen>(inst,
		                                                                            "Route."
		                                                                            "LoadingSc"
		                                                                            "r"
		                                                                            "een");
	}

	Instruction create_instruction_route_starttime(const line_splitting::InstructionInfo& inst) {
		return create_single_time_instruction<instructions::route::StartTime>(inst,
		                                                                      "Route."
		                                                                      "StartTime");
	}

	Instruction create_instruction_route_dynamiclight(const line_splitting::InstructionInfo& inst) {
		return create_single_string_instruction<instructions::route::DynamicLight>(inst,
		                                                                           "Route."
		                                                                           "DynamicLig"
		                                                                           "h"
		                                                                           "t");
	}

	Instruction create_instruction_route_ambientlight(const line_splitting::InstructionInfo& inst) {
		instructions::route::AmbientLight al;

		switch (inst.args.size()) {
			default:
			case 3:
				al.color.b = gsl::narrow<std::uint8_t>(util::parsers::parse_loose_integer(inst.args[2], 160));
				// fall through
			case 2:
				al.color.g = gsl::narrow<std::uint8_t>(util::parsers::parse_loose_integer(inst.args[1], 160));
				// fall through
			case 1:
				al.color.r = gsl::narrow<std::uint8_t>(util::parsers::parse_loose_integer(inst.args[0], 160));
				// fall through
			case 0:
				break;
		}

		return al;
	}

	Instruction create_instruction_route_directionallight(const line_splitting::InstructionInfo& inst) {
		instructions::route::DirectionalLight dl;

		switch (inst.args.size()) {
			default:
			case 3:
				dl.color.b = gsl::narrow<std::uint8_t>(util::parsers::parse_loose_integer(inst.args[2], 160));
				// fall through
			case 2:
				dl.color.g = gsl::narrow<std::uint8_t>(util::parsers::parse_loose_integer(inst.args[1], 160));
				// fall through
			case 1:
				dl.color.r = gsl::narrow<std::uint8_t>(util::parsers::parse_loose_integer(inst.args[0], 160));
				// fall through
			case 0:
				break;
		}

		return dl;
	}

	Instruction create_instruction_route_lightdirection(const line_splitting::InstructionInfo& inst) {
		instructions::route::LightDirection ld;

		switch (inst.args.size()) {
			default:
			case 2:
				ld.theta = util::parsers::parse_loose_float(inst.args[0], 60);
				// fall through
			case 1:
				ld.phi = util::parsers::parse_loose_float(inst.args[1], -26.57F);
				// fall through
			case 0:
				break;
		}

		return ld;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
