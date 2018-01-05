#include "csv_rw_route.hpp"
#include "utils.hpp"
#include <algorithm>
#include <map>
#include <sstream>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	namespace {
		/////////////////////////////////////////////
		// Argument/Indices Count Helper Functions //
		/////////////////////////////////////////////

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

		//////////////////////////////
		// Common Instruction Forms //
		//////////////////////////////

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
		static instruction create_single_sizet_instruction(const line_splitting::instruction_info& instr,
		                                                   const char* name) {
			args_at_least(instr, 1, name);

			return T{std::size_t(util::parse_loose_integer(instr.args[0]))};
		}

		template <class T>
		static instruction create_single_sizet_instruction(const line_splitting::instruction_info& instr,
		                                                   const char* name, std::intmax_t def) {
			args_at_least(instr, 1, name);

			return T{std::size_t(util::parse_loose_integer(instr.args[0], def))};
		}

		template <class T>
		static instruction create_single_time_instruction(const line_splitting::instruction_info& instr,
		                                                  const char* name) {
			args_at_least(instr, 1, name);

			return T{util::parse_time(instr.args[0])};
		}

		template <std::size_t offset, class T>
		static void set_positions(T& instr, const line_splitting::instruction_info& instr_info) {
			instr.x_offset = util::parse_loose_float(instr_info.args[offset + 0], 0);
			instr.y_offset = util::parse_loose_float(instr_info.args[offset + 1], 0);
			instr.yaw = util::parse_loose_float(instr_info.args[offset + 2], 0);
			instr.pitch = util::parse_loose_float(instr_info.args[offset + 3], 0);
			instr.roll = util::parse_loose_float(instr_info.args[offset + 4], 0);
		}

		//////////////////////////////////////
		// Instruction Generation Functions //
		//////////////////////////////////////

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

			instructions::options::ObjectVisibility ov;

			auto value = util::parse_loose_integer(instr.args[0], 0);

			ov.mode = value == 0 ? instructions::options::ObjectVisibility::Legacy
			                     : instructions::options::ObjectVisibility::TrackBased;

			return ov;
		}

		static instruction create_instruction_options_sectionbehavior(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.SectionBehavior");

			instructions::options::SectionBehavior sb;

			auto value = util::parse_loose_integer(instr.args[0], 0);

			sb.mode = value == 0 ? instructions::options::SectionBehavior::Default
			                     : instructions::options::SectionBehavior::Simplified;

			return sb;
		}

		static instruction create_instruction_options_cantbehavior(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.CantBehavior");

			instructions::options::CantBehavior cb;

			auto value = util::parse_loose_integer(instr.args[0], 0);

			cb.mode = value == 0 ? instructions::options::CantBehavior::Unsigned
			                     : instructions::options::CantBehavior::Signed;

			return cb;
		}

		static instruction create_instruction_options_fogbehavior(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.FogBehavior");

			instructions::options::FogBehavior fb;

			auto value = util::parse_loose_integer(instr.args[0], 0);

			fb.mode = value == 0 ? instructions::options::FogBehavior::BlockBased
			                     : instructions::options::FogBehavior::Interpolated;

			return fb;
		}

		static instruction
		create_instruction_options_compatibletransparencymode(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.CompatibleTransparencyMode");

			instructions::options::CompatibleTransparencyMode ctm;

			auto value = util::parse_loose_integer(instr.args[0], 0);

			ctm.mode = value == 0 ? instructions::options::CompatibleTransparencyMode::Off
			                      : instructions::options::CompatibleTransparencyMode::On;

			return ctm;
		}

		static instruction create_instruction_options_enablebvetshacks(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Options.EnableBveTsHacks");

			instructions::options::EnableBveTsHacks ebth;

			auto value = util::parse_loose_integer(instr.args[0], 0);

			ebth.mode =
			    value == 0 ? instructions::options::EnableBveTsHacks::Off : instructions::options::EnableBveTsHacks::On;

			return ebth;
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

			instructions::route::Signal s;

			s.aspect_index = util::parse_loose_integer(instr.indices[0]);
			s.speed = util::parse_loose_float(instr.args[0]);

			return s;
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

			instructions::route::DisplaySpeed ds;

			ds.unit_string = instr.args[0];
			ds.conversion_factor = util::parse_loose_float(instr.args[1]);

			return ds;
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

			instructions::route::AmbiantLight al;

			al.color.r = std::uint8_t(util::parse_loose_integer(instr.args[0], 160));
			al.color.g = std::uint8_t(util::parse_loose_integer(instr.args[1], 160));
			al.color.b = std::uint8_t(util::parse_loose_integer(instr.args[2], 160));

			return al;
		}

		static instruction create_instruction_route_directionallight(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Route.DirectionalLight");

			instructions::route::DirectionalLight dl;

			dl.color.r = std::uint8_t(util::parse_loose_integer(instr.args[0], 160));
			dl.color.g = std::uint8_t(util::parse_loose_integer(instr.args[1], 160));
			dl.color.b = std::uint8_t(util::parse_loose_integer(instr.args[2], 160));

			return dl;
		}

		static instruction create_instruction_route_lightdirection(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Route.LightDirection");

			instructions::route::LightDirection ld;

			ld.theta = util::parse_loose_float(instr.args[0], 60);
			ld.phi = util::parse_loose_float(instr.args[1], -26.57f);

			return ld;
		}

		static instruction create_instruction_train_folder(const line_splitting::instruction_info& instr) {
			return create_single_string_instruction<instructions::train::Folder>(instr, "Train.Folder");
		}

		static instruction create_instruction_train_run(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Train.Run");
			args_at_least(instr, 1, "Train.Run");

			instructions::train::Rail r;

			r.rail_type_index = util::parse_loose_integer(instr.indices[0]);
			r.run_sound_index = util::parse_loose_integer(instr.args[0]);

			return r;
		}

		static instruction create_instruction_train_flange(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Train.Flange");
			args_at_least(instr, 1, "Train.Flange");

			instructions::train::Flange f;

			f.rail_type_index = util::parse_loose_integer(instr.indices[0]);
			f.flange_sound_index = util::parse_loose_integer(instr.args[0]);

			return f;
		}

		static instruction create_instruction_train_timetable(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Train.Timetable");
			args_at_least(instr, 1, "Train.Timetable");

			instructions::train::Timetable tt;

			tt.timetable_index = util::parse_loose_integer(instr.indices[0]);
			tt.filename = instr.args[0];

			auto suffixes = util::split_text(instr.suffix, '.');

			if (suffixes.size() == 0 || (suffixes[0] != "day" && suffixes[0] != "night")) {
				throw std::invalid_argument("A suffix of .Day or .Night is required for Train.Timetable");
			}

			tt.day = suffixes[0] == "day";

			return tt;
		}

		static instruction create_instruction_train_velocity(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::train::Velocity>(instr, "Train.Velocity");
		}

		static instruction create_instruction_structure_command(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Structure.Command");
			args_at_least(instr, 1, "Structure.Command");

			static std::map<std::string, decltype(instructions::structure::Command::command)> command_mapping{
			    {"structure.ground"s, instructions::structure::Command::Ground},
			    {"structure.rail"s, instructions::structure::Command::Rail},
			    {"structure.walll"s, instructions::structure::Command::WallL},
			    {"structure.wallr"s, instructions::structure::Command::WallR},
			    {"structure.dikel"s, instructions::structure::Command::DikeL},
			    {"structure.diker"s, instructions::structure::Command::DikeR},
			    {"structure.forml"s, instructions::structure::Command::FormL},
			    {"structure.formr"s, instructions::structure::Command::FormR},
			    {"structure.formcl"s, instructions::structure::Command::FormCL},
			    {"structure.formcr"s, instructions::structure::Command::FormCR},
			    {"structure.roofl"s, instructions::structure::Command::RoofL},
			    {"structure.roofr"s, instructions::structure::Command::RoofR},
			    {"structure.roofcl"s, instructions::structure::Command::RoofCL},
			    {"structure.roofcr"s, instructions::structure::Command::RoofCR},
			    {"structure.crackl"s, instructions::structure::Command::CrackL},
			    {"structure.crackr"s, instructions::structure::Command::CrackR},
			    {"structure.freeobj"s, instructions::structure::Command::FreeObj},
			    {"structure.beacon"s, instructions::structure::Command::Beacon},
			};

			instructions::structure::Command c;

			c.command = command_mapping.find(instr.name)->second;

			c.structure_index = std::size_t(util::parse_loose_integer(instr.indices[0]));
			c.filename = instr.args[0];

			return c;
		}

		static instruction create_instruction_structure_pole(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 2, "Structure.Pole");
			args_at_least(instr, 1, "Structure.Pole");

			instructions::structure::Pole p;

			p.additional_rails = std::size_t(util::parse_loose_integer(instr.indices[0]));
			p.pole_structure_index = std::size_t(util::parse_loose_integer(instr.indices[1]));
			p.filename = instr.args[0];

			return p;
		}

		static instruction create_instruction_texture_background(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Texture.Background");
			args_at_least(instr, 1, "Texture.Background");

			auto background_texture_index = std::size_t(util::parse_loose_integer(instr.indices[0]));

			if (instr.suffix.size() == 0 || instr.suffix == "load") {
				instructions::texture::Background_Load bl;
				bl.background_texture_index = background_texture_index;
				bl.filename = instr.args[0];
				return bl;
			}

			auto number = std::size_t(util::parse_loose_integer(instr.args[0]));

			if (instr.suffix == "x") {
				instructions::texture::Background_X x;
				x.background_texture_index = background_texture_index;
				x.repetition_count = number;
				return x;
			}

			if (instr.suffix == "aspect") {
				instructions::texture::Background_Aspect ba;

				ba.background_texture_index = background_texture_index;
				ba.mode = number != 0 ? instructions::texture::Background_Aspect::Aspect
				                      : instructions::texture::Background_Aspect::Fixed;

				return ba;
			}

			throw std::invalid_argument("Invalid suffix to Texture.Background");
		}

		static instruction create_instruction_cycle_ground(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Cycle.Ground");
			args_at_least(instr, 1, "Cycle.Ground");

			instructions::cycle::Ground g;

			g.ground_structure_index = std::size_t(util::parse_loose_integer(instr.indices[0]));

			g.input_indices.reserve(instr.args.size());
			std::transform(instr.args.begin(), instr.args.end(), std::back_inserter(g.input_indices),
			               [](const std::string& arg) { return std::size_t(util::parse_loose_integer(arg)); });

			return g;
		}

		static instruction create_instruction_cycle_rail(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Cycle.Rail");
			args_at_least(instr, 1, "Cycle.Rail");

			instructions::cycle::Rail r;

			r.rail_structure_index = std::size_t(util::parse_loose_integer(instr.indices[0]));

			r.input_indices.reserve(instr.args.size());
			std::transform(instr.args.begin(), instr.args.end(), std::back_inserter(r.input_indices),
			               [](const std::string& arg) { return std::size_t(util::parse_loose_integer(arg)); });

			return r;
		}

		static instruction create_instruction_signal(const line_splitting::instruction_info& instr) {
			indices_at_least(instr, 1, "Signal");
			args_at_least(instr, 1, "Signal");

			auto signal_index = std::size_t(util::parse_loose_integer(instr.indices[0]));

			// Signal File version
			if (instr.args.size() >= 2) {
				instructions::naked::Signal s;
				s.signal_index = signal_index;
				s.signal_filename = instr.args[0];
				s.glow_filename = instr.args[1];
				return s;
			}

			// Animated File version
			instructions::naked::SignalAnimated sa;
			sa.signal_index = signal_index;
			sa.filename = instr.args[0];
			return sa;
		}

		static instruction create_instruction_track_railstart(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 4, "RailStart");

			instructions::track::RailStart rs;

			rs.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			rs.x_offset = util::parse_loose_float(instr.args[1]);
			rs.y_offset = util::parse_loose_float(instr.args[2]);
			rs.rail_type = std::size_t(util::parse_loose_integer(instr.args[3]));

			return rs;
		}

		static instruction create_instruction_track_rail(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 4, "Rail");

			instructions::track::Rail r;

			r.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			r.x_offset = util::parse_loose_float(instr.args[1]);
			r.y_offset = util::parse_loose_float(instr.args[2]);
			r.rail_type = std::size_t(util::parse_loose_integer(instr.args[3]));

			return r;
		}

		static instruction create_instruction_track_railtype(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "RailType");

			instructions::track::RailType rt;

			rt.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			rt.rail_type = std::size_t(util::parse_loose_integer(instr.args[1]));

			return rt;
		}

		static instruction create_instruction_track_railend(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "RailEnd");

			instructions::track::RailEnd re;

			re.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			re.x_offset = util::parse_loose_float(instr.args[1]);
			re.y_offset = util::parse_loose_float(instr.args[2]);

			return re;
		}

		static instruction create_instruction_track_accuracy(const line_splitting::instruction_info& instr) {
			// Ignored instruction
			(void) instr;
			return instructions::naked::None{};
		}

		static instruction create_instruction_track_adhesion(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::track::Adhesion>(instr, "Track.Adhesion");
		}

		static instruction create_instruction_track_pitch(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::track::Pitch>(instr, "Track.Pitch");
		}

		static instruction create_instruction_track_curve(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Track.Curve");

			instructions::track::Curve c;

			c.radius = util::parse_loose_float(instr.args[0]);
			c.cant = util::parse_loose_float(instr.args[1]);

			return c;
		}

		static instruction create_instruction_track_turn(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::track::Turn>(instr, "Track.Turn");
		}

		static instruction create_instruction_track_height(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::track::Height>(instr, "Track.Height");
		}

		static instruction create_instruction_track_freeobj(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 7, "Track.FreeObj");

			instructions::track::FreeObj fobj;

			fobj.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			fobj.free_obj_structure_index = std::size_t(util::parse_loose_integer(instr.args[1]));
			fobj.x_offset = util::parse_loose_float(instr.args[2]);
			fobj.y_offset = util::parse_loose_float(instr.args[3]);
			fobj.yaw = util::parse_loose_float(instr.args[4]);
			fobj.pitch = util::parse_loose_float(instr.args[5]);
			fobj.roll = util::parse_loose_float(instr.args[6]);

			return fobj;
		}

		static instruction create_instruction_track_wall(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Track.Wall");

			instructions::track::Wall w;

			w.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			auto direction_num = util::parse_loose_integer(instr.args[1]);
			w.wall_structure_index = std::size_t(util::parse_loose_integer(instr.args[2]));

			switch (direction_num) {
				case -1:
					w.direction = instructions::track::Wall::Left;
					break;
				default:
				case 0:
					w.direction = instructions::track::Wall::Both;
					break;
				case 1:
					w.direction = instructions::track::Wall::Right;
					break;
			}

			return w;
		}

		static instruction create_instruction_track_wallend(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::WallEnd>(instr, "Track.WallEnd");
		}

		static instruction create_instruction_track_dike(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Track.Dike");

			instructions::track::Dike d;

			d.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			auto direction_num = util::parse_loose_integer(instr.args[1]);
			d.dike_structure_index = std::size_t(util::parse_loose_integer(instr.args[2]));

			switch (direction_num) {
				case -1:
					d.direction = instructions::track::Dike::Left;
					break;
				default:
				case 0:
					d.direction = instructions::track::Dike::Both;
					break;
				case 1:
					d.direction = instructions::track::Dike::Right;
					break;
			}

			return d;
		}

		static instruction create_instruction_track_dikeend(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::DikeEnd>(instr, "Track.DikeEnd");
		}

		static instruction create_instruction_track_pole(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 5, "Track.Pole");

			instructions::track::Pole p;

			p.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			p.additional_rails = std::size_t(util::parse_loose_integer(instr.args[1]));
			p.location = util::parse_loose_integer(instr.args[2]);
			p.interval = util::parse_loose_integer(instr.args[3]);
			p.pole_structure_index = std::size_t(util::parse_loose_integer(instr.args[4]));

			return p;
		}

		static instruction create_instruction_track_poleend(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::PoleEnd>(instr, "Track.PoleEnd");
		}

		static instruction create_instruction_track_crack(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Track.Crack");

			instructions::track::Crack c;

			c.rail_index_1 = std::size_t(util::parse_loose_integer(instr.args[0]));
			c.rail_index_2 = std::size_t(util::parse_loose_integer(instr.args[1]));
			c.crack_structure_index = std::size_t(util::parse_loose_integer(instr.args[2]));

			return c;
		}

		static instruction create_instruction_track_ground(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::Ground>(instr, "Track.Ground");
		}

		static instruction create_instruction_track_sta(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 12, "Track.Sta");

			instructions::track::Sta s;

			s.name = instr.args[0];

			// Arrival time
			{
				auto& arr_arg = instr.args[1];
				if (arr_arg.size() == 0) {
					s.arrival_tag = instructions::track::Sta::ArrivalTime_t::AnyTime;
				}
				else if (arr_arg[0] == 'p' || arr_arg[0] == 'l') {
					s.arrival_tag = instructions::track::Sta::ArrivalTime_t::AllPass;
				}
				else if (arr_arg[0] == 'b') {
					s.arrival_tag = instructions::track::Sta::ArrivalTime_t::PlayerPass;
				}
				else if (arr_arg[0] == 's') {
					s.arrival_tag = instructions::track::Sta::ArrivalTime_t::PlayerStop;

					if (arr_arg.size() >= 3 && arr_arg[1] == ':') {
						s.arrival = util::parse_time(arr_arg.substr(2));
					}
				}
				else {
					s.arrival_tag = instructions::track::Sta::ArrivalTime_t::Time;
					s.arrival = util::parse_time(arr_arg);
				}
			}

			// Departure Time
			{
				auto& arr_arg = instr.args[2];
				if (arr_arg.size() == 0) {
					s.departure_tag = instructions::track::Sta::DepartureTime_t::AnyTime;
				}
				else if (arr_arg[0] == 't') {
					if (arr_arg.size() >= 3 && arr_arg[1] == ':') {
						s.departure = util::parse_time(arr_arg.substr(2));
						s.departure_tag = instructions::track::Sta::DepartureTime_t::TerminalTime;
					}
					else {
						s.departure_tag = instructions::track::Sta::DepartureTime_t::Terminal;
					}
				}
				else if (arr_arg[0] == 'c') {
					if (arr_arg.size() >= 3 && arr_arg[1] == ':') {
						s.departure = util::parse_time(arr_arg.substr(2));
						s.departure_tag = instructions::track::Sta::DepartureTime_t::ChangeEnds;
					}
					else {
						s.departure_tag = instructions::track::Sta::DepartureTime_t::ChangeEndsTime;
					}
				}
				else {
					s.departure = util::parse_time(arr_arg);
				}
			}

			// Pass Alarm
			{
				auto val = util::parse_loose_integer(instr.args[3], 0);
				s.pass_alarm = val == 1;
			}

			// Doors
			{
				auto& arg_val = instr.args[4];
				if (arg_val.size() == 0) {
					s.doors = instructions::track::Sta::Doors_t::None;
				}
				else {
					switch (arg_val[0]) {
						case 'l':
							s.doors = instructions::track::Sta::Doors_t::Left;
							break;
						case 'n':
							s.doors = instructions::track::Sta::Doors_t::None;
							break;
						case 'r':
							s.doors = instructions::track::Sta::Doors_t::Right;
							break;
						case 'b':
							s.doors = instructions::track::Sta::Doors_t::Both;
							break;
						default: {
							auto val = util::parse_loose_integer(arg_val, 0);
							switch (val) {
								case -1:
									s.doors = instructions::track::Sta::Doors_t::Left;
									break;
								default:
								case 0:
									s.doors = instructions::track::Sta::Doors_t::None;
									break;
								case 1:
									s.doors = instructions::track::Sta::Doors_t::Right;
									break;
							}
							break;
						}
					}
				}
			}

			// Forced Red Signal
			{
				auto val = util::parse_loose_integer(instr.args[5], 0);
				s.force_red = val == 1;
			}

			// System
			{
				auto& arg_val = instr.args[6];
				s.system = arg_val == "atc" || arg_val == "1";
			}

			s.arrival_sound = instr.args[7];
			s.stop_duration = util::parse_loose_float(instr.args[8], 15);
			s.passenger_ratio = util::parse_loose_float(instr.args[9], 100);
			s.departure_sound = instr.args[10];
			s.timetable_index = std::size_t(util::parse_loose_integer(instr.args[11], 0));

			return s;
		}

		static instruction create_instruction_track_station(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 6, "Track.Station");

			line_splitting::instruction_info ii;
			ii.name = instr.name;
			ii.args = {instr.args[0], // Name
			           instr.args[1], // ArivalTime
			           instr.args[2], // DepartureTime
			           "0"s,          // Pass Alarm
			           "b"s,          // Doors
			           instr.args[3], // ForcedRedSignal
			           instr.args[4], // System
			           ""s,           // Arival Sound
			           "15"s,         // Stop Duration
			           "100"s,        // PassengerRatio
			           instr.args[5], // DepartureSound
			           "0"s};         // Timetable Index

			return create_instruction_track_sta(ii);
		}

		static instruction create_instruction_track_stop(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 4, "Track.Stop");

			instructions::track::Stop s;

			auto direction_num = util::parse_loose_integer(instr.args[0], 0);
			s.backwards_tolerance = util::parse_loose_float(instr.args[1], 5);
			s.forwards_tolerance = util::parse_loose_float(instr.args[2], 5);
			s.cars = std::size_t(util::parse_loose_integer(instr.args[3], 0));

			switch (direction_num) {
				case -1:
					s.stop_post = instructions::track::Stop::Left;
					break;
				default:
				case 0:
					s.stop_post = instructions::track::Stop::None;
					break;
				case 1:
					s.stop_post = instructions::track::Stop::Right;
					break;
			}

			return s;
		}

		static instruction create_instruction_track_form(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 4, "Tract.Form");

			instructions::track::Form f;

			f.rail_index_1 = std::size_t(util::parse_loose_integer(instr.args[0]));
			f.rail_index_2 = std::size_t(util::parse_loose_integer(instr.args[1]));
			f.roof_structure_index = std::size_t(util::parse_loose_integer(instr.args[2]));
			f.form_structure_index = std::size_t(util::parse_loose_integer(instr.args[3]));

			return f;
		}

		static instruction create_instruction_track_limit(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Tract.Limit");

			instructions::track::Limit l;

			l.speed = util::parse_loose_float(instr.args[0], 0);
			auto post_num = util::parse_loose_integer(instr.args[1], 0);
			auto course_num = util::parse_loose_integer(instr.args[1], 0);

			switch (post_num) {
				case -1:
					l.post = instructions::track::Limit::Post_t::Left;
					break;
				default:
				case 0:
					l.post = instructions::track::Limit::Post_t::None;
					break;
				case 1:
					l.post = instructions::track::Limit::Post_t::Right;
					break;
			}

			switch (course_num) {
				case -1:
					l.cource = instructions::track::Limit::Cource_t::Left;
					break;
				default:
				case 0:
					l.cource = instructions::track::Limit::Cource_t::None;
					break;
				case 1:
					l.cource = instructions::track::Limit::Cource_t::Right;
					break;
			}

			return l;
		}

		static instruction create_instruction_track_section(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.Section");

			instructions::track::Section s;
			s.a_term.reserve(instr.args.size());

			std::transform(instr.args.begin(), instr.args.end(), std::back_inserter(s.a_term),
			               [](const std::string& s) { return util::parse_loose_integer(s, 0); });

			return s;
		}

		static instruction create_instruction_track_sigf(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 7, "Track.SigF");

			instructions::track::SigF sf;

			sf.signal_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			sf.section = std::size_t(util::parse_loose_integer(instr.args[1]));
			set_positions<2>(sf, instr);

			return sf;
		}
		static instruction create_instruction_track_signal(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 7, "Track.Signal");

			instructions::track::Signal s;

			auto type_num = util::parse_loose_integer(instr.args[0]);

			switch (type_num) {
				case 2:
					s.type = instructions::track::Signal::R_Y;
					break;
				default:
				case -2:
					s.type = instructions::track::Signal::R_G;
					break;
				case 3:
					s.type = instructions::track::Signal::R_Y_G;
					break;
				case 4:
					s.type = instructions::track::Signal::R_YY_Y_G;
					break;
				case -4:
					s.type = instructions::track::Signal::R_Y_YG_G;
					break;
				case 5:
					s.type = instructions::track::Signal::R_YY_Y_YG_G;
					break;
				case -5:
					s.type = instructions::track::Signal::R_Y_YG_G_GG;
					break;
				case 6:
					s.type = instructions::track::Signal::R_YY_Y_YG_G_GG;
					break;
			}

			set_positions<2>(s, instr);

			return s;
		}

		static instruction create_instruction_track_relay(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 5, "Track.Relay");

			instructions::track::Relay r;

			set_positions<0>(r, instr);

			return r;
		}

		static instruction create_instruction_track_beacon(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 9, "Track.Beacon");

			instructions::track::Beacon b;

			b.type = std::size_t(util::parse_loose_integer(instr.args[0]));
			b.beacon_structure_index = std::size_t(util::parse_loose_integer(instr.args[1]));
			b.section = std::size_t(util::parse_loose_integer(instr.args[2]));
			b.data = std::size_t(util::parse_loose_integer(instr.args[3]));

			set_positions<4>(b, instr);

			return b;
		}

		static instruction create_instruction_track_transponder(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 8, "Track.Transponder");

			instructions::track::Transponder t;

			auto type_num = util::parse_loose_integer(instr.args[0], 0);
			t.signal = std::size_t(util::parse_loose_integer(instr.args[1], 0));
			t.switch_system = util::parse_loose_integer(instr.args[2], 0) == 0;

			switch (type_num) {
				default:
				case 0:
					t.type = instructions::track::Transponder::S_type;
					break;
				case 1:
					t.type = instructions::track::Transponder::SN_type;
					break;
				case 2:
					t.type = instructions::track::Transponder::Departure;
					break;
				case 3:
					t.type = instructions::track::Transponder::ATS_P_RENEWAL;
					break;
				case 4:
					t.type = instructions::track::Transponder::ATS_P_STOP;
					break;
			}

			set_positions<3>(t, instr);

			return t;
		}

		static instruction create_instruction_track_atssn(const line_splitting::instruction_info& instr) {
			(void) instr;

			instructions::track::Transponder t;

			t.type = instructions::track::Transponder::S_type;
			t.signal = 0;
			t.switch_system = true;

			return t;
		}

		static instruction create_instruction_track_atsp(const line_splitting::instruction_info& instr) {
			(void) instr;

			instructions::track::Transponder t;

			t.type = instructions::track::Transponder::ATS_P_RENEWAL;
			t.signal = 0;
			t.switch_system = true;

			return t;
		}

		static instruction create_instruction_track_pattern(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Track.Pattern");

			instructions::track::Pattern p;

			auto type_num = util::parse_loose_integer(instr.args[0]);
			p.type = type_num == 0 ? instructions::track::Pattern::Permanent : instructions::track::Pattern::Temporary;
			p.speed = util::parse_loose_float(instr.args[1]);

			return p;
		}

		static instruction create_instruction_track_plimit(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.PLimit");

			instructions::track::Pattern p;

			p.type = instructions::track::Pattern::Temporary;
			p.speed = util::parse_loose_float(instr.args[0]);

			return p;
		}

		static instruction create_instruction_track_back(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::Back>(instr, "Track.Back");
		}

		static instruction create_instruction_track_fog(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 5, "Track.Fog");

			instructions::track::Fog f;

			f.starting_distance = util::parse_loose_float(instr.args[0], 0);
			f.ending_distance = util::parse_loose_float(instr.args[1], 0);
			f.color.r = std::uint8_t(util::parse_loose_integer(instr.args[2], 128));
			f.color.g = std::uint8_t(util::parse_loose_integer(instr.args[3], 128));
			f.color.b = std::uint8_t(util::parse_loose_integer(instr.args[4], 128));

			return f;
		}

		static instruction create_instruction_track_brightness(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.Brightness");

			instructions::track::Brightness b;
			b.value = std::uint8_t(util::parse_loose_integer(instr.args[0], 255));

			return b;
		}

		static instruction create_instruction_track_marker(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.Marker");

			if (instr.args.size() >= 2) {
				instructions::track::Marker m;

				m.filename = instr.args[0];
				m.distance = util::parse_loose_float(instr.args[1]);

				return m;
			}

			instructions::track::MarkerXML mxml;

			mxml.filename = instr.args[0];

			return mxml;
		}

		static instruction create_instruction_track_pointofinterest(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.PointOfInterest");

			instructions::track::PointOfInterest poi;

			poi.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			set_positions<1>(poi, instr);
			poi.text = instr.args[6];

			return poi;
		}

		static instruction create_instruction_track_pretrain(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.PreTrain");

			instructions::track::PreTrain pt;

			pt.time = util::parse_time(instr.args[0]);

			return pt;
		}

		static instruction create_instruction_track_announce(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Track.Announce");

			instructions::track::Announce a;

			a.filename = instr.args[0];
			a.speed = util::parse_loose_float(instr.args[1], 0);

			return a;
		}

		static instruction create_instruction_track_doppler(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 3, "Track.Doppler");

			instructions::track::Doppler d;

			d.filename = instr.args[0];
			d.x_offset = util::parse_loose_float(instr.args[1]);
			d.y_offset = util::parse_loose_float(instr.args[2]);

			return d;
		}

		static instruction create_instruction_track_buffer(const line_splitting::instruction_info& instr) {
			(void) instr;
			return instructions::track::Buffer{};
		}

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
		        {"structure.beacon"s, &create_instruction_structure_command},
		        {"structure.pole"s, &create_instruction_structure_pole},

		        // Texture namespace
		        {"texture.background"s, &create_instruction_texture_background},

		        // Cycle namespace
		        {"cycle.ground"s, &create_instruction_cycle_ground},
		        {"cycle.rail"s, &create_instruction_cycle_rail},

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

	static instruction generate_instruction(const preprocessed_lines& lines, preprocessed_line& line,
	                                 errors::multi_error& errors, std::string& with_value) {
		instruction i;

		auto parsed = line_splitting::csv(line);

		if (parsed.track_position) {
			return create_instruction_location_statement(parsed);
		}

		util::lower(parsed.name);

		auto dot_iter = std::find(parsed.name.begin(), parsed.name.end(), '.');
		bool has_dot = dot_iter != parsed.name.end();

		// get fully qualified name
		if (has_dot && dot_iter == parsed.name.begin()) {
			parsed.name = with_value + parsed.name;
		}

		// lookup function
		auto func_iter = function_mapping.find(parsed.name);
		if (func_iter == function_mapping.end()) {
			if (parsed.name == "with") {
				with_value = util::lower_copy(parsed.args[0]);
			}
			else {
				std::ostringstream oss;
				oss << "\"" << parsed.name << "\" is not a known function in a csv file";
				errors[lines.filenames[line.filename_index]].emplace_back(errors::error_t{line.line, oss.str()});
			}
			return instructions::naked::None{};
		}

		try {
			i = func_iter->second(parsed);
		}
		catch (const std::invalid_argument& e) {
			errors[lines.filenames[line.filename_index]].emplace_back(errors::error_t{line.line, e.what()});
			return instructions::naked::None{};
		}

		return i;
	}

	instruction_list generate_instructions(preprocessed_lines& lines, errors::multi_error& errors) {
		instruction_list i_list;
		i_list.reserve(lines.lines.size());

		std::string with_value = "";
		for (auto& line : lines.lines) {
			auto i = generate_instruction(lines, line, errors, with_value);

			mapbox::util::apply_visitor(
			    [&line](auto& i) {
				    i.file_index = line.filename_index;
				    i.line = line.line;
			    },
			    i);

			i_list.emplace_back(i);
		}

		return i_list;
	}
} // namespace csv_rw_route
} // namespace parsers
