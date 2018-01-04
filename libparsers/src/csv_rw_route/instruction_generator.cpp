#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <algorithm>
#include <map>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	namespace {
		static void args_at_least(const line_splitting::instruction_info& instr, std::size_t num, const char* name) {
			if (instr.args.size() < num) {
				throw std::invalid_argument(name + " must have at least one argument"s);
			}
		}

		static void indices_at_least(const line_splitting::instruction_info& instr, std::size_t num, const char* name) {
			if (instr.indices.size() < num) {
				throw std::invalid_argument(name + " must have at least one index"s);
			}
		}

		template <class T>
		static instruction create_single_string_instruction(const line_splitting::instruction_info& instr,
		                                                    const char* name) {
			args_at_least(instr, 1, name);

			return T{instr.args[0]};
		}

		template <class T>
		static instruction create_single_float_instruction(const line_splitting::instruction_info& instr,
		                                                   const char* name) {
			args_at_least(instr, 1, name);

			return T{util::parse_loose_float(instr.args[0])};
		}

		template <class T>
		static instruction create_single_float_instruction(const line_splitting::instruction_info& instr,
		                                                   const char* name, float def) {
			args_at_least(instr, 1, name);

			return T{util::parse_loose_float(instr.args[0], def)};
		}

		template <class T>
		static instruction create_single_integer_instruction(const line_splitting::instruction_info& instr,
		                                                     const char* name) {
			args_at_least(instr, 1, name);

			return T{util::parse_loose_integer(instr.args[0])};
		}

		template <class T>
		static instruction create_single_integer_instruction(const line_splitting::instruction_info& instr,
		                                                     const char* name, std::intmax_t def) {
			args_at_least(instr, 1, name);

			return T{util::parse_loose_integer(instr.args[0], def)};
		}

		template <class T>
		static instruction create_single_time_instruction(const line_splitting::instruction_info& instr,
		                                                  const char* name) {
			args_at_least(instr, 1, name);

			return T{util::parse_time(instr.args[0])};
		}

		static instruction create_instruction_location_statement(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Location Statement");

			instructions::naked::position pos;
			pos.distances.reserve(instr.args.size());
			std::transform(instr.args.begin(), instr.args.end(), std::back_inserter(pos.distances),
			               [](const std::string& s) { return util::parse_loose_float(s, 0); });

			return pos;
		}

		static instruction create_instruction_options_unitoflength(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.UnitOfLength");

			instructions::options::UnitOfLength uol;
			uol.factors_in_meters.reserve(instr.args.size());
			uol.factors_in_meters.emplace_back(util::parse_loose_float(instr.args[0], 1));
			std::transform(instr.args.begin() + 1, instr.args.end(), std::back_inserter(uol.factors_in_meters),
			               [](const std::string& s) { return util::parse_loose_float(s, 0); });

			return uol;
		}

		static instruction create_instruction_options_unitofspeed(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::options::UnitOfSpeed>(instr, "Options.UnitOfSpeed", 1);
		}

		static instruction create_instruction_options_blocklength(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::options::BlockLength>(instr, "Options.BlockLength",
			                                                                           25);
		}

		static instruction create_instruction_options_objectvisibility(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.ObjectVisibility");

			auto value = util::parse_loose_integer(instr.args[0], 0);

			auto enumeration = value == 0 ? instructions::options::ObjectVisibility::Legacy
			                              : instructions::options::ObjectVisibility::TrackBased;

			return instructions::options::ObjectVisibility{enumeration};
		}

		static instruction create_instruction_options_sectionbehavior(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.SectionBehavior");

			auto value = util::parse_loose_integer(instr.args[0], 0);

			auto enumeration = value == 0 ? instructions::options::SectionBehavior::Default
			                              : instructions::options::SectionBehavior::Simplified;

			return instructions::options::SectionBehavior{enumeration};
		}

		static instruction create_instruction_options_cantbehavior(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.CantBehavior");

			auto value = util::parse_loose_integer(instr.args[0], 0);

			auto enumeration = value == 0 ? instructions::options::CantBehavior::Unsigned
			                              : instructions::options::CantBehavior::Signed;

			return instructions::options::CantBehavior{enumeration};
		}

		static instruction create_instruction_options_fogbehavior(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.FogBehavior");

			auto value = util::parse_loose_integer(instr.args[0], 0);

			auto enumeration = value == 0 ? instructions::options::FogBehavior::BlockBased
			                              : instructions::options::FogBehavior::Interpolated;

			return instructions::options::FogBehavior{enumeration};
		}

		static instruction
		create_instruction_options_compatibletransparencymode(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.CompatibleTransparencyMode");

			auto value = util::parse_loose_integer(instr.args[0], 0);

			auto enumeration = value == 0 ? instructions::options::CompatibleTransparencyMode::Off
			                              : instructions::options::CompatibleTransparencyMode::On;

			return instructions::options::CompatibleTransparencyMode{enumeration};
		}

		static instruction create_instruction_options_enablebvetshacks(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.EnableBveTsHacks");

			auto value = util::parse_loose_integer(instr.args[0], 0);

			auto enumeration =
			    value == 0 ? instructions::options::EnableBveTsHacks::Off : instructions::options::EnableBveTsHacks::On;

			return instructions::options::EnableBveTsHacks{enumeration};
		}

		static instruction create_instruction_route_comment(const line_splitting::instruction_info& instr) {
			return create_single_string_instruction<instructions::route::Comment>(instr, "Route.Comment");
		}

		static instruction create_instruction_route_image(const line_splitting::instruction_info& instr) {
			return create_single_string_instruction<instructions::route::Image>(instr, "Route.Image");
		}

		static instruction create_instruction_route_timetable(const line_splitting::instruction_info& instr) {
			return create_single_string_instruction<instructions::route::Timetable>(instr, "Route.Timetable");
		}

		static instruction create_instruction_route_change(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Route.Change");

			auto value = util::parse_loose_integer(instr.args[0], -1);

			instructions::route::Change change;
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

			return change;
		}

		static instruction create_instruction_route_gauge(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::route::Guage>(instr, "Route.Guage", 1435);
		}

		static instruction create_instruction_route_signal(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Route.Signal");
			args_at_least(instr, 1, "Route.Signal");

			auto aspect_index = util::parse_loose_integer(instr.indices[0]);
			auto speed = util::parse_loose_float(instr.args[0]);

			return instructions::route::Signal{std::size_t(aspect_index), speed};
		}

		static instruction create_instruction_route_runinterval(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Route.RunInterval");

			instructions::route::RunInterval ri;
			ri.time_interval.reserve(instr.args.size());
			std::transform(instr.args.begin(), instr.args.end(), std::back_inserter(ri.time_interval),
			               [](const std::string& s) { return util::parse_loose_float(s); });

			return ri;
		}

		static instruction
		create_instruction_route_accelerationduetogravity(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::route::AccelerationDueToGravity>(
			    instr, "Route.AccelerationDueToGravity", 9.80665f);
		}

		static instruction create_instruction_route_elevation(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::route::Elevation>(instr, "Route.Elevation", 0);
		}

		static instruction create_instruction_route_temperature(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::route::Temperature>(instr, "Route.Temperature", 20);
		}

		static instruction create_instruction_route_pressure(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::route::Pressure>(instr, "Route.Pressure", 101.325f);
		}

		static instruction create_instruction_route_displayspeed(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Route.DisplaySpeed");

			auto conversion_fac = util::parse_loose_float(instr.args[1]);

			return instructions::route::DisplaySpeed{instr.args[0], conversion_fac};
		}

		static instruction create_instruction_route_loadingscreen(const line_splitting::instruction_info& instr) {
			return create_single_string_instruction<instructions::route::LoadingScreen>(instr, "Route.LoadingScreen");
		}

		static instruction create_instruction_route_starttime(const line_splitting::instruction_info& instr) {
			return create_single_time_instruction<instructions::route::StartTime>(instr, "Route.StartTime");
		}

		static instruction create_instruction_route_dynamiclight(const line_splitting::instruction_info& instr) {
			return create_single_string_instruction<instructions::route::DynamicLight>(instr, "Route.DynamicLight");
		}

		static instruction create_instruction_route_ambientlight(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Route.AmbientLight");

			auto r = util::parse_loose_integer(instr.args[0], 160);
			auto g = util::parse_loose_integer(instr.args[1], 160);
			auto b = util::parse_loose_integer(instr.args[2], 160);

			auto color = openbve2::datatypes::color8_rgb{r, g, b};

			return instructions::route::AmbiantLight{color};
		}

		static instruction create_instruction_route_directionallight(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Route.DirectionalLight");

			auto r = util::parse_loose_integer(instr.args[0], 160);
			auto g = util::parse_loose_integer(instr.args[1], 160);
			auto b = util::parse_loose_integer(instr.args[2], 160);

			auto color = openbve2::datatypes::color8_rgb{r, g, b};

			return instructions::route::DirectionalLight{color};
		}

		static instruction create_instruction_route_lightdirection(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Route.LightDirection");

			auto theta = util::parse_loose_float(instr.args[0], 60);
			auto phi = util::parse_loose_float(instr.args[1], -26.57f);

			return instructions::route::LightDirection{theta, phi};
		}

		static instruction create_instruction_train_folder(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_train_run(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_train_flange(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_train_timetable(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_train_gauge(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_train_velocity(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_structure_command(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_structure_pole(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_texture_background(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_cycle_ground(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_signal(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_railstart(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_rail(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_railtype(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_railend(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_accuracy(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_adhesion(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_pitch(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_curve(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_turn(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_height(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_freeobj(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_wall(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_wallend(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_dike(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_dikeend(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_pole(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_poleend(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_crack(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_ground(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_sta(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_station(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_stop(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_form(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_limit(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_section(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_sigf(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_signal(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_relay(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_beacon(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_transponder(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_atssn(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_atsp(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_pattern(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_plimit(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_back(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_fog(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_brightness(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_marker(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_pointofinterest(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_pretrain(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_announce(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_doppler(const line_splitting::instruction_info& instr) {}
		static instruction create_instruction_track_buffer(const line_splitting::instruction_info& instr) {}

		static const std::map<std::string, instruction (*)(const line_splitting::instruction_info& instr)>
		    function_mapping = {
		        // Options namespace
		        {"options.unitoflength"s, &create_instruction_options_unitoflength},
		        {"options.unitofspeed"s, &create_instruction_options_unitofspeed},
		        {"options.blocklength"s, &create_instruction_options_blocklength},
		        {"options.objectvisibility"s, &create_instruction_options_objectvisibility},
		        {"options.sectionbehavior"s, &create_instruction_options_sectionbehavior},
		        {"options.cantbehavior"s, &create_instruction_options_cantbehavior},
		        {"options.fogbehavior"s, &create_instruction_options_fogbehavior},
		        {"options.compatibletransparencymode"s, &create_instruction_options_compatibletransparencymode},
		        {"options.enablebvetshacks"s, &create_instruction_options_enablebvetshacks},

		        // Route namespace
		        {"route.comment"s, &create_instruction_route_comment},
		        {"route.image"s, &create_instruction_route_image},
		        {"route.timetable"s, &create_instruction_route_timetable},
		        {"route.change"s, &create_instruction_route_change},
		        {"route.gauge"s, &create_instruction_route_gauge},
		        {"route.signal"s, &create_instruction_route_signal},
		        {"route.runinterval"s, &create_instruction_route_runinterval},
		        {"route.accelerationduetogravity"s, &create_instruction_route_accelerationduetogravity},
		        {"route.elevation"s, &create_instruction_route_elevation},
		        {"route.temperature"s, &create_instruction_route_temperature},
		        {"route.pressure"s, &create_instruction_route_pressure},
		        {"route.displayspeed"s, &create_instruction_route_displayspeed},
		        {"route.loadingscreen"s, &create_instruction_route_loadingscreen},
		        {"route.starttime "s, &create_instruction_route_starttime},
		        {"route.dynamiclight"s, &create_instruction_route_dynamiclight},
		        {"route.ambientlight"s, &create_instruction_route_ambientlight},
		        {"route.directionallight"s, &create_instruction_route_directionallight},
		        {"route.lightdirection"s, &create_instruction_route_lightdirection},

		        // Train namespace
		        {"train.folder"s, &create_instruction_train_folder},
		        {"train.run"s, &create_instruction_train_run},
		        {"train.flange"s, &create_instruction_train_flange},
		        {"train.timetable"s, &create_instruction_train_timetable},
		        {"train.timetable"s, &create_instruction_train_timetable},
		        {"train.gauge"s, &create_instruction_route_gauge},
		        {"train.interval"s, &create_instruction_route_runinterval},
		        {"train.velocity"s, &create_instruction_train_velocity},

		        // Structure namespace
		        {"structure.rail"s, &create_instruction_structure_command},
		        {"structure.beacon"s, &create_instruction_structure_command},
		        {"structure.pole"s, &create_instruction_structure_command},
		        {"structure.ground"s, &create_instruction_structure_command},
		        {"structure.walll"s, &create_instruction_structure_command},
		        {"structure.wallr"s, &create_instruction_structure_command},
		        {"structure.dikel"s, &create_instruction_structure_command},
		        {"structure.diker"s, &create_instruction_structure_command},
		        {"structure.forml"s, &create_instruction_structure_command},
		        {"structure.formr"s, &create_instruction_structure_command},
		        {"structure.formcl"s, &create_instruction_structure_command},
		        {"structure.formcr"s, &create_instruction_structure_command},
		        {"structure.roofl"s, &create_instruction_structure_command},
		        {"structure.roofr"s, &create_instruction_structure_command},
		        {"structure.roofcl"s, &create_instruction_structure_command},
		        {"structure.roofcr"s, &create_instruction_structure_command},
		        {"structure.crackl"s, &create_instruction_structure_command},
		        {"structure.crackr"s, &create_instruction_structure_command},
		        {"structure.freeobj"s, &create_instruction_structure_command},
		        {"structure.pole"s, &create_instruction_structure_pole},

		        // Texture namespace
		        {"texture.background"s, &create_instruction_texture_background},

		        // Cycle namespace
		        {"cycle.ground"s, &create_instruction_cycle_ground},

		        // Signal namespace
		        {"signal"s, &create_instruction_signal},

		        // Track namespace
		        // Rails:
		        {"track.railstart"s, &create_instruction_track_railstart},
		        {"track.rail"s, &create_instruction_track_rail},
		        {"track.railtype"s, &create_instruction_track_railtype},
		        {"track.railend"s, &create_instruction_track_railend},
		        {"track.accuracy"s, &create_instruction_track_accuracy},
		        {"track.adhesion"s, &create_instruction_track_adhesion},

		        // Geometry:
		        {"track.pitch"s, &create_instruction_track_pitch},
		        {"track.curve"s, &create_instruction_track_curve},
		        {"track.turn"s, &create_instruction_track_turn},
		        {"track.height"s, &create_instruction_track_height},

		        // Objects:
		        {"track.freeobj"s, &create_instruction_track_freeobj},
		        {"track.wall"s, &create_instruction_track_wall},
		        {"track.wallend"s, &create_instruction_track_wallend},
		        {"track.dike"s, &create_instruction_track_dike},
		        {"track.dikeend"s, &create_instruction_track_dikeend},
		        {"track.pole"s, &create_instruction_track_pole},
		        {"track.poleend"s, &create_instruction_track_poleend},
		        {"track.crack"s, &create_instruction_track_crack},
		        {"track.ground"s, &create_instruction_track_ground},

		        // Stations:
		        {"track.sta"s, &create_instruction_track_sta},
		        {"track.station"s, &create_instruction_track_station},
		        {"track.stop"s, &create_instruction_track_stop},
		        {"track.form"s, &create_instruction_track_form},

		        // Signalling and speed limits:
		        {"track.limit"s, &create_instruction_track_limit},
		        {"track.section"s, &create_instruction_track_section},
		        {"track.sigf"s, &create_instruction_track_sigf},
		        {"track.signal"s, &create_instruction_track_signal},
		        {"track.relay"s, &create_instruction_track_relay},

		        // Safety systems:
		        {"track.beacon"s, &create_instruction_track_beacon},
		        {"track.transponder"s, &create_instruction_track_transponder},
		        {"track.atssn"s, &create_instruction_track_atssn},
		        {"track.atsp"s, &create_instruction_track_atsp},
		        {"track.pattern"s, &create_instruction_track_pattern},
		        {"track.plimit"s, &create_instruction_track_plimit},

		        // Miscellaneous:
		        {"track.back"s, &create_instruction_track_back},
		        {"track.fog"s, &create_instruction_track_fog},
		        {"track.brightness"s, &create_instruction_track_brightness},
		        {"track.marker"s, &create_instruction_track_marker},
		        {"track.pointofinterest"s, &create_instruction_track_pointofinterest},
		        {"track.pretrain"s, &create_instruction_track_pretrain},
		        {"track.announce"s, &create_instruction_track_announce},
		        {"track.doppler"s, &create_instruction_track_doppler},
		        {"track.buffer"s, &create_instruction_track_buffer},
		};
	} // namespace

	instruction_list generate_instructions(preprocessed_lines& lines) {
		instruction_list i;

		std::string with_value = "";
		for (auto& line : lines.lines) {
			auto parsed = line_splitting::csv(line);
			util::lower(parsed.name);

			auto dot_iter = std::find(parsed.name.begin(), parsed.name.end(), '.');
			bool has_dot = dot_iter != parsed.name.end();

			// get fully qualified name
			std::string qualified_name;
			if (has_dot && dot_iter == parsed.name.begin()) {
				qualified_name = with_value + parsed.name;
			}
			else {
				qualified_name = parsed.name;
			}
		}

		return i;
	}
} // namespace csv_rw_route
} // namespace parsers
