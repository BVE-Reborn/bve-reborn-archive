#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace bve::parsers::csv_rw_route::instruction_generation {
	instruction create_instruction_route_comment(const line_splitting::instruction_info& inst) {
		return create_single_string_instruction<instructions::route::comment>(inst,
		                                                                      "Route.Comment");
	}

	instruction create_instruction_route_image(const line_splitting::instruction_info& inst) {
		return create_single_string_instruction<instructions::route::image>(inst, "Route.Image");
	}

	instruction create_instruction_route_timetable(const line_splitting::instruction_info& inst) {
		return create_single_string_instruction<instructions::route::timetable>(inst,
		                                                                        "Route."
		                                                                        "Timetable");
	}

	instruction create_instruction_route_change(const line_splitting::instruction_info& inst) {
		instructions::route::change change;

		if (!inst.args.empty()) {
			auto const value = util::parse_loose_integer(inst.args[0], -1);
			switch (value) {
				default:
				case -1:
					change.mode = instructions::route::change::safty_activiated_service_brakes;
					break;
				case 0:
					change.mode = instructions::route::change::safty_activiated_emergency_brakes;
					break;
				case 1:
					change.mode = instructions::route::change::safty_deactivated_emergency_brakes;
					break;
			}
		}
		else {
			change.mode = instructions::route::change::safty_activiated_service_brakes;
		}

		return change;
	}

	instruction create_instruction_route_gauge(const line_splitting::instruction_info& inst) {
		return create_single_float_instruction<instructions::route::guage>(inst, "Route.Guage",
		                                                                   1435);
	}

	instruction create_instruction_route_signal(const line_splitting::instruction_info& inst) {
		indices_at_least(inst, 1, "Route.Signal");
		args_at_least(inst, 1, "Route.Signal");

		instructions::route::signal s;

		s.aspect_index = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.indices[0]));
		s.speed = util::parse_loose_float(inst.args[0]);

		return s;
	}

	instruction create_instruction_route_runinterval(const line_splitting::instruction_info& inst) {
		args_at_least(inst, 1, "Route.RunInterval");

		instructions::route::run_interval ri;
		ri.time_interval.reserve(inst.args.size());
		std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(ri.time_interval),
		               [](const std::string& s) { return util::parse_loose_float(s); });

		return ri;
	}

	instruction create_instruction_route_accelerationduetogravity(
	    const line_splitting::instruction_info& inst) {
		return create_single_float_instruction<
		    instructions::route::acceleration_due_to_gravity>(inst,
		                                                      "Route."
		                                                      "AccelerationDue"
		                                                      "ToGravity",
		                                                      9.80665f);
	}

	instruction create_instruction_route_elevation(const line_splitting::instruction_info& inst) {
		return create_single_float_instruction<instructions::route::elevation>(inst,
		                                                                       "Route."
		                                                                       "Elevation",
		                                                                       0);
	}

	instruction create_instruction_route_temperature(const line_splitting::instruction_info& inst) {
		return create_single_float_instruction<instructions::route::temperature>(inst,
		                                                                         "Route."
		                                                                         "Temperature",
		                                                                         20);
	}

	instruction create_instruction_route_pressure(const line_splitting::instruction_info& inst) {
		return create_single_float_instruction<instructions::route::pressure>(inst,
		                                                                      "Route.Pressure",
		                                                                      101.325f);
	}

	instruction create_instruction_route_displayspeed(
	    const line_splitting::instruction_info& inst) {
		args_at_least(inst, 2, "Route.DisplaySpeed");

		instructions::route::display_speed ds;

		ds.unit_string = inst.args[0];
		ds.conversion_factor = util::parse_loose_float(inst.args[1]);

		return ds;
	}

	instruction create_instruction_route_loadingscreen(
	    const line_splitting::instruction_info& inst) {
		return create_single_string_instruction<instructions::route::loading_screen>(inst,
		                                                                             "Route."
		                                                                             "LoadingSc"
		                                                                             "r"
		                                                                             "een");
	}

	instruction create_instruction_route_starttime(const line_splitting::instruction_info& inst) {
		return create_single_time_instruction<instructions::route::start_time>(inst,
		                                                                       "Route."
		                                                                       "StartTime");
	}

	instruction create_instruction_route_dynamiclight(
	    const line_splitting::instruction_info& inst) {
		return create_single_string_instruction<instructions::route::dynamic_light>(inst,
		                                                                            "Route."
		                                                                            "DynamicLig"
		                                                                            "h"
		                                                                            "t");
	}

	instruction create_instruction_route_ambientlight(
	    const line_splitting::instruction_info& inst) {
		instructions::route::ambiant_light al;

		switch (inst.args.size()) {
			default:
			case 3:
				al.color.b =
				    gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[2], 160));
				// fall through
			case 2:
				al.color.g =
				    gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[1], 160));
				// fall through
			case 1:
				al.color.r =
				    gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[0], 160));
				// fall through
			case 0:
				break;
		}

		return al;
	}

	instruction create_instruction_route_directionallight(
	    const line_splitting::instruction_info& inst) {
		instructions::route::directional_light dl;

		switch (inst.args.size()) {
			default:
			case 3:
				dl.color.b =
				    gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[2], 160));
				// fall through
			case 2:
				dl.color.g =
				    gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[1], 160));
				// fall through
			case 1:
				dl.color.r =
				    gsl::narrow<std::uint8_t>(util::parse_loose_integer(inst.args[0], 160));
				// fall through
			case 0:
				break;
		}

		return dl;
	}

	instruction create_instruction_route_lightdirection(
	    const line_splitting::instruction_info& inst) {
		instructions::route::light_direction ld;

		switch (inst.args.size()) {
			default:
			case 2:
				ld.theta = util::parse_loose_float(inst.args[0], 60);
				// fall through
			case 1:
				ld.phi = util::parse_loose_float(inst.args[1], -26.57f);
				// fall through
			case 0:
				break;
		}

		return ld;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
