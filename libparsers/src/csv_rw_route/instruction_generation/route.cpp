#include "instruction_generator.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_route_comment(const line_splitting::instruction_info& inst) {
			return create_single_string_instruction<instructions::route::Comment>(inst,
			                                                                      "Route.Comment");
		}

		instruction create_instruction_route_image(const line_splitting::instruction_info& inst) {
			return create_single_string_instruction<instructions::route::Image>(inst,
			                                                                    "Route.Image");
		}

		instruction create_instruction_route_timetable(
		    const line_splitting::instruction_info& inst) {
			return create_single_string_instruction<instructions::route::Timetable>(inst,
			                                                                        "Route."
			                                                                        "Timetable");
		}

		instruction create_instruction_route_change(const line_splitting::instruction_info& inst) {
			instructions::route::Change change;

			if (!inst.args.empty()) {
				auto value = util::parse_loose_integer(inst.args[0], -1);
				switch (value) {
					default:
					case -1:
						change.mode = instructions::route::Change::SaftyActiviatedServiceBrakes;
						break;
					case 0:
						change.mode = instructions::route::Change::SaftyActiviatedEmergencyBrakes;
						break;
					case 1:
						change.mode = instructions::route::Change::SaftyDeactivatedEmergencyBrakes;
						break;
				}
			}
			else {
				change.mode = instructions::route::Change::SaftyActiviatedServiceBrakes;
			}

			return change;
		}

		instruction create_instruction_route_gauge(const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<instructions::route::Guage>(inst, "Route.Guage",
			                                                                   1435);
		}

		instruction create_instruction_route_signal(const line_splitting::instruction_info& inst) {
			indices_at_least(inst, 1, "Route.Signal");
			args_at_least(inst, 1, "Route.Signal");

			instructions::route::Signal s;

			s.aspect_index = std::size_t(util::parse_loose_integer(inst.indices[0]));
			s.speed = util::parse_loose_float(inst.args[0]);

			return s;
		}

		instruction create_instruction_route_runinterval(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Route.RunInterval");

			instructions::route::RunInterval ri;
			ri.time_interval.reserve(inst.args.size());
			std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(ri.time_interval),
			               [](const std::string& s) { return util::parse_loose_float(s); });

			return ri;
		}

		instruction create_instruction_route_accelerationduetogravity(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<
			    instructions::route::AccelerationDueToGravity>(inst,
			                                                   "Route."
			                                                   "AccelerationDue"
			                                                   "ToGravity",
			                                                   9.80665f);
		}

		instruction create_instruction_route_elevation(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<instructions::route::Elevation>(inst,
			                                                                       "Route."
			                                                                       "Elevation",
			                                                                       0);
		}

		instruction create_instruction_route_temperature(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<instructions::route::Temperature>(inst,
			                                                                         "Route."
			                                                                         "Temperature",
			                                                                         20);
		}

		instruction create_instruction_route_pressure(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<instructions::route::Pressure>(inst,
			                                                                      "Route.Pressure",
			                                                                      101.325f);
		}

		instruction create_instruction_route_displayspeed(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 2, "Route.DisplaySpeed");

			instructions::route::DisplaySpeed ds;

			ds.unit_string = inst.args[0];
			ds.conversion_factor = util::parse_loose_float(inst.args[1]);

			return ds;
		}

		instruction create_instruction_route_loadingscreen(
		    const line_splitting::instruction_info& inst) {
			return create_single_string_instruction<instructions::route::LoadingScreen>(inst,
			                                                                            "Route."
			                                                                            "LoadingScr"
			                                                                            "een");
		}

		instruction create_instruction_route_starttime(
		    const line_splitting::instruction_info& inst) {
			return create_single_time_instruction<instructions::route::StartTime>(inst,
			                                                                      "Route."
			                                                                      "StartTime");
		}

		instruction create_instruction_route_dynamiclight(
		    const line_splitting::instruction_info& inst) {
			return create_single_string_instruction<instructions::route::DynamicLight>(inst,
			                                                                           "Route."
			                                                                           "DynamicLigh"
			                                                                           "t");
		}

		instruction create_instruction_route_ambientlight(
		    const line_splitting::instruction_info& inst) {
			instructions::route::AmbiantLight al;

			switch (inst.args.size()) {
				default:
				case 3:
					al.color.b = std::uint8_t(util::parse_loose_integer(inst.args[2], 160));
					// fall through
				case 2:
					al.color.g = std::uint8_t(util::parse_loose_integer(inst.args[1], 160));
					// fall through
				case 1:
					al.color.r = std::uint8_t(util::parse_loose_integer(inst.args[0], 160));
					// fall through
				case 0:
					break;
			}

			return al;
		}

		instruction create_instruction_route_directionallight(
		    const line_splitting::instruction_info& inst) {
			instructions::route::DirectionalLight dl;

			switch (inst.args.size()) {
				default:
				case 3:
					dl.color.b = std::uint8_t(util::parse_loose_integer(inst.args[2], 160));
					// fall through
				case 2:
					dl.color.g = std::uint8_t(util::parse_loose_integer(inst.args[1], 160));
					// fall through
				case 1:
					dl.color.r = std::uint8_t(util::parse_loose_integer(inst.args[0], 160));
					// fall through
				case 0:
					break;
			}

			return dl;
		}

		instruction create_instruction_route_lightdirection(
		    const line_splitting::instruction_info& inst) {
			instructions::route::LightDirection ld;

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
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
