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
				std::ostringstream oss;
				oss << name << " must have at least " << num << " argument" << (num == 1 ? "" : "s");
				throw std::invalid_argument(oss.str());
			}
		}

		static void indices_at_least(const line_splitting::instruction_info& instr, std::size_t num, const char* name) {
			if (instr.indices.size() < num) {
				std::ostringstream oss;
				oss << name << " must have at least " << num << " ind" << (num == 1 ? "ex" : "ices");
				throw std::invalid_argument(oss.str());
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
			(void) name;

			if (instr.args.size() >= 1) {
				return T{util::parse_loose_float(instr.args[0], def)};
			}
			else {
				return T{def};
			}
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
			(void) name;

			if (instr.args.size() >= 1) {
				return T{std::size_t(util::parse_loose_integer(instr.args[0], def))};
			}
			else {
				return T{def};
			}
		}

		template <class T>
		static instruction create_single_time_instruction(const line_splitting::instruction_info& instr,
		                                                  const char* name) {
			args_at_least(instr, 1, name);

			return T{util::parse_time(instr.args[0])};
		}

		template <std::size_t offset, class T>
		static void set_positions(T& instr, const line_splitting::instruction_info& instr_info) {
			if (instr_info.args.size() > offset) {
				switch (instr_info.args.size()) {
					default:
					case offset + 5:
						instr.roll = util::parse_loose_float(instr_info.args[offset + 4], 0);
						// fall through
					case offset + 4:
						instr.pitch = util::parse_loose_float(instr_info.args[offset + 3], 0);
						// fall through
					case offset + 3:
						instr.yaw = util::parse_loose_float(instr_info.args[offset + 2], 0);
						// fall through
					case offset + 2:
						instr.y_offset = util::parse_loose_float(instr_info.args[offset + 1], 0);
						// fall through
					case offset + 1:
						instr.x_offset = util::parse_loose_float(instr_info.args[offset + 0], 0);
						// fall through
					case offset:
						break;
				}
			}
		}

		//////////////////////////////////////
		// Instruction Generation Functions //
		//////////////////////////////////////

		static instruction create_instruction_location_statement(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Location Statement");

			instructions::naked::position pos;
			pos.distances.reserve(instr.args.size() + 1);
			pos.distances.emplace_back(instr.offset);
			std::transform(instr.args.begin(), instr.args.end(), std::back_inserter(pos.distances),
			               [](const std::string& s) { return util::parse_loose_float(s, 0); });

			return pos;
		}

		///////////////////////
		// Options Namespace //
		///////////////////////

		static instruction create_instruction_options_unitoflength(const line_splitting::instruction_info& instr) {
			instructions::options::UnitOfLength uol;
			uol.factors_in_meters.reserve(std::max(std::size_t(2), instr.args.size() + 1));
			uol.factors_in_meters.emplace_back(1.0f);
			if (instr.args.size() >= 1) {
				uol.factors_in_meters.emplace_back(util::parse_loose_float(instr.args[0], 1));
				std::transform(instr.args.begin() + 1, instr.args.end(), std::back_inserter(uol.factors_in_meters),
				               [](const std::string& s) { return util::parse_loose_float(s, 0); });
			}
			else {
				uol.factors_in_meters.emplace_back(1.0f);
			}

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
			instructions::options::ObjectVisibility ov;

			if (instr.args.size() >= 1) {
				auto value = util::parse_loose_integer(instr.args[0], 0);

				ov.mode = value == 0 ? instructions::options::ObjectVisibility::Legacy
				                     : instructions::options::ObjectVisibility::TrackBased;
			}
			else {
				ov.mode = instructions::options::ObjectVisibility::Legacy;
			}

			return ov;
		}

		static instruction create_instruction_options_sectionbehavior(const line_splitting::instruction_info& instr) {
			instructions::options::SectionBehavior sb;

			if (instr.args.size() >= 1) {
				auto value = util::parse_loose_integer(instr.args[0], 0);

				sb.mode = value == 0 ? instructions::options::SectionBehavior::Default
				                     : instructions::options::SectionBehavior::Simplified;
			}
			else {
				sb.mode = instructions::options::SectionBehavior::Default;
			}

			return sb;
		}

		static instruction create_instruction_options_cantbehavior(const line_splitting::instruction_info& instr) {
			instructions::options::CantBehavior cb;

			if (instr.args.size() >= 1) {
				auto value = util::parse_loose_integer(instr.args[0], 0);

				cb.mode = value == 0 ? instructions::options::CantBehavior::Unsigned
				                     : instructions::options::CantBehavior::Signed;
			}
			else {
				cb.mode = instructions::options::CantBehavior::Unsigned;
			}

			return cb;
		}

		static instruction create_instruction_options_fogbehavior(const line_splitting::instruction_info& instr) {
			instructions::options::FogBehavior fb;

			if (instr.args.size() >= 1) {
				auto value = util::parse_loose_integer(instr.args[0], 0);

				fb.mode = value == 0 ? instructions::options::FogBehavior::BlockBased
				                     : instructions::options::FogBehavior::Interpolated;
			}
			else {
				fb.mode = instructions::options::FogBehavior::BlockBased;
			}

			return fb;
		}

		static instruction
		create_instruction_options_compatibletransparencymode(const line_splitting::instruction_info& instr) {
			instructions::options::CompatibleTransparencyMode ctm;

			if (instr.args.size() >= 1) {
				auto value = util::parse_loose_integer(instr.args[0], 0);

				ctm.mode = value == 0 ? instructions::options::CompatibleTransparencyMode::Off
				                      : instructions::options::CompatibleTransparencyMode::On;
			}
			else {
				ctm.mode = instructions::options::CompatibleTransparencyMode::Off;
			}

			return ctm;
		}

		static instruction create_instruction_options_enablebvetshacks(const line_splitting::instruction_info& instr) {
			instructions::options::EnableBveTsHacks ebth;

			if (instr.args.size() >= 1) {
				auto value = util::parse_loose_integer(instr.args[0], 0);

				ebth.mode = value == 0 ? instructions::options::EnableBveTsHacks::Off
				                       : instructions::options::EnableBveTsHacks::On;
			}
			else {
				ebth.mode = instructions::options::EnableBveTsHacks::Off;
			}

			return ebth;
		}

		/////////////////////
		// Route Namespace //
		/////////////////////

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
			instructions::route::Change change;

			if (instr.args.size() >= 1) {
				auto value = util::parse_loose_integer(instr.args[0], -1);
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
			instructions::route::AmbiantLight al;

			switch (instr.args.size()) {
				default:
				case 3:
					al.color.b = std::uint8_t(util::parse_loose_integer(instr.args[2], 160));
					// fall through
				case 2:
					al.color.g = std::uint8_t(util::parse_loose_integer(instr.args[1], 160));
					// fall through
				case 1:
					al.color.r = std::uint8_t(util::parse_loose_integer(instr.args[0], 160));
					// fall through
				case 0:
					break;
			}

			return al;
		}

		static instruction create_instruction_route_directionallight(const line_splitting::instruction_info& instr) {
			instructions::route::DirectionalLight dl;

			switch (instr.args.size()) {
				default:
				case 3:
					dl.color.b = std::uint8_t(util::parse_loose_integer(instr.args[2], 160));
					// fall through
				case 2:
					dl.color.g = std::uint8_t(util::parse_loose_integer(instr.args[1], 160));
					// fall through
				case 1:
					dl.color.r = std::uint8_t(util::parse_loose_integer(instr.args[0], 160));
					// fall through
				case 0:
					break;
			}

			return dl;
		}

		static instruction create_instruction_route_lightdirection(const line_splitting::instruction_info& instr) {
			instructions::route::LightDirection ld;

			switch (instr.args.size()) {
				default:
				case 2:
					ld.theta = util::parse_loose_float(instr.args[0], 60);
					// fall through
				case 1:
					ld.phi = util::parse_loose_float(instr.args[1], -26.57f);
					// fall through
				case 0:
					break;
			}

			return ld;
		}

		/////////////////////
		// Train Namespace //
		/////////////////////

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
			return create_single_float_instruction<instructions::train::Velocity>(instr, "Train.Velocity", 0);
		}

		/////////////////////////
		// Structure Namespace //
		/////////////////////////

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
			    {"@@object@@ground"s, instructions::structure::Command::Ground},
			    {"@@object@@rail"s, instructions::structure::Command::Rail},
			    {"@@object@@walll"s, instructions::structure::Command::WallL},
			    {"@@object@@wallr"s, instructions::structure::Command::WallR},
			    {"@@object@@dikel"s, instructions::structure::Command::DikeL},
			    {"@@object@@diker"s, instructions::structure::Command::DikeR},
			    {"@@object@@forml"s, instructions::structure::Command::FormL},
			    {"@@object@@formr"s, instructions::structure::Command::FormR},
			    {"@@object@@formcl"s, instructions::structure::Command::FormCL},
			    {"@@object@@formcr"s, instructions::structure::Command::FormCR},
			    {"@@object@@roofl"s, instructions::structure::Command::RoofL},
			    {"@@object@@roofr"s, instructions::structure::Command::RoofR},
			    {"@@object@@roofcl"s, instructions::structure::Command::RoofCL},
			    {"@@object@@roofcr"s, instructions::structure::Command::RoofCR},
			    {"@@object@@crackl"s, instructions::structure::Command::CrackL},
			    {"@@object@@crackr"s, instructions::structure::Command::CrackR},
			    {"@@object@@freeobj"s, instructions::structure::Command::FreeObj},
			    {"@@object@@beacon"s, instructions::structure::Command::Beacon},
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

		///////////////////////
		// Texture Namespace //
		///////////////////////

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

		/////////////////////
		// Cycle Namespace //
		/////////////////////

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

		//////////////////////////////
		// Signal (naked) Namespace //
		//////////////////////////////

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

		/////////////////////
		// Track Namespace //
		/////////////////////

		///////////
		// Rails //
		///////////

		static instruction create_instruction_track_railstart(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "RailStart");

			instructions::track::RailStart rs;

			switch (instr.args.size()) {
				default:
				case 4:
					try {
						rs.rail_type = std::size_t(util::parse_loose_integer(instr.args[3]));
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 3:
					try {
						rs.y_offset = util::parse_loose_float(instr.args[2]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 2:
					try {
						rs.x_offset = util::parse_loose_float(instr.args[1]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 1:
					rs.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
					// fall through
				case 0:
					break;
			}

			return rs;
		}

		static instruction create_instruction_track_rail(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Rail");

			instructions::track::Rail r;

			switch (instr.args.size()) {
				default:
				case 4:
					try {
						r.rail_type = std::size_t(util::parse_loose_integer(instr.args[3]));
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 3:
					try {
						r.y_offset = util::parse_loose_float(instr.args[2]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 2:
					try {
						r.x_offset = util::parse_loose_float(instr.args[1]);
					}
					catch (const std::invalid_argument&) {
					}
					// fall through
				case 1:
					r.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
					// fall through
				case 0:
					break;
			}

			return r;
		}

		static instruction create_instruction_track_railtype(const line_splitting::instruction_info& instr) {
			instructions::track::RailType rt;

			switch (instr.args.size()) {
				default:
				case 2:
					rt.rail_type = std::size_t(util::parse_loose_integer(instr.args[1], 0));
					// fall through
				case 1:
					rt.rail_index = std::size_t(util::parse_loose_integer(instr.args[0], 0));
					// fall through
				case 0:
					break;
			}

			return rt;
		}

		static instruction create_instruction_track_railend(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "RailEnd");

			instructions::track::RailEnd re;

			switch (instr.args.size()) {
				default:
				case 3:
					try {
						re.y_offset = util::parse_loose_float(instr.args[2]);
					}
					catch (const std::invalid_argument&) {
					};
					// fall through
				case 2:
					try {
						re.x_offset = util::parse_loose_float(instr.args[1]);
					}
					catch (const std::invalid_argument&) {
					};
					// fall through
				case 1:
					re.rail_index = std::size_t(util::parse_loose_integer(instr.args[0]));
					// fall through
				case 0:
					break;
			}

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

		//////////////
		// Geometry //
		//////////////

		static instruction create_instruction_track_curve(const line_splitting::instruction_info& instr) {
			instructions::track::Curve c;

			switch (instr.args.size()) {
				default:
				case 2:
					c.cant = util::parse_loose_float(instr.args[1], 0);
					// fall through
				case 1:
					c.radius = util::parse_loose_float(instr.args[0], 0);
					// fall through
				case 0:
					break;
			}

			return c;
		}

		static instruction create_instruction_track_turn(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::track::Turn>(instr, "Track.Turn", 0);
		}

		static instruction create_instruction_track_height(const line_splitting::instruction_info& instr) {
			return create_single_float_instruction<instructions::track::Height>(instr, "Track.Height");
		}

		/////////////
		// Objects //
		/////////////

		static instruction create_instruction_track_freeobj(const line_splitting::instruction_info& instr) {
			instructions::track::FreeObj fobj;

			switch (instr.args.size()) {
				default:
				case 3:
					set_positions<2>(fobj, instr);
					// fall through
				case 2:
					fobj.free_obj_structure_index = std::size_t(util::parse_loose_integer(instr.args[1], 0));
					// fall through
				case 1:
					fobj.rail_index = std::size_t(util::parse_loose_integer(instr.args[0], 0));
					// fall through
				case 0:
					break;
			}

			return fobj;
		}

		static instruction create_instruction_track_wall(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Track.Wall");

			instructions::track::Wall w;

			w.rail_index = std::size_t(util::parse_loose_integer(instr.args[0], 0));
			auto direction_num = util::parse_loose_integer(instr.args[1]);
			if (instr.args.size() >= 3) {
				w.wall_structure_index = std::size_t(util::parse_loose_integer(instr.args[2], 0));
			}

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
			args_at_least(instr, 2, "Track.Dike");

			instructions::track::Dike d;

			d.rail_index = std::size_t(util::parse_loose_integer(instr.args[0], 0));
			auto direction_num = util::parse_loose_integer(instr.args[1]);
			if (instr.args.size() >= 3) {
				d.dike_structure_index = std::size_t(util::parse_loose_integer(instr.args[2], 0));
			}

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
			instructions::track::Pole p;

			switch (instr.args.size()) {
				default:
				case 5:
					p.pole_structure_index = std::size_t(util::parse_loose_integer(instr.args[4], 0));
					// fall through
				case 4:
					p.interval = util::parse_loose_integer(instr.args[3], 1);
					// fall through
				case 3:
					p.location = util::parse_loose_integer(instr.args[2], 0);
					// fall through
				case 2:
					p.additional_rails = std::size_t(util::parse_loose_integer(instr.args[1], 0));
					// fall through
				case 1:
					p.rail_index = std::size_t(util::parse_loose_integer(instr.args[0], 0));
					// fall through
				case 0:
					break;
			}

			return p;
		}

		static instruction create_instruction_track_poleend(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::PoleEnd>(instr, "Track.PoleEnd");
		}

		static instruction create_instruction_track_crack(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 2, "Track.Crack");

			instructions::track::Crack c;

			c.rail_index_1 = std::size_t(util::parse_loose_integer(instr.args[0]));
			c.rail_index_2 = std::size_t(util::parse_loose_integer(instr.args[1]));
			if (instr.args.size() >= 3) {
				c.crack_structure_index = std::size_t(util::parse_loose_integer(instr.args[2], 0));
			}

			return c;
		}

		static instruction create_instruction_track_ground(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::Ground>(instr, "Track.Ground");
		}

		//////////////
		// Stations //
		//////////////

		static instruction create_instruction_track_sta(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.Sta");

			instructions::track::Sta s;

			switch (instr.args.size()) {
				default:
				case 12:
					s.timetable_index = std::size_t(util::parse_loose_integer(instr.args[11], 0));
					// fall through
				case 11:
					s.departure_sound = instr.args[10];
					// fall through
				case 10:
					s.passenger_ratio = util::parse_loose_float(instr.args[9], 100);
					// fall through
				case 9:
					s.stop_duration = util::parse_loose_float(instr.args[8], 15);
					// fall through
				case 8:
					s.arrival_sound = instr.args[7];
					// fall through
				case 7:
					// System
					{
						auto arg_val = util::lower_copy(instr.args[6]);
						s.system = arg_val == "atc" || arg_val == "1";
					}
					// fall through
				case 6:
					// Forced Red Signal
					{
						auto val = util::parse_loose_integer(instr.args[5], 0);
						s.force_red = val == 1;
					}
					// fall through
				case 5:
					// Doors
					{
						auto arg_val = util::lower_copy(instr.args[4]);
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
					// fall through
				case 4:
					// Pass Alarm
					{
						auto val = util::parse_loose_integer(instr.args[3], 0);
						s.pass_alarm = val == 1;
					}
					// fall through
				case 3:
					// Departure Time
					{
						auto arr_arg = util::lower_copy(instr.args[2]);
						if (arr_arg.size() == 0) {
							s.departure_tag = instructions::track::Sta::DepartureTime_t::AnyTime;
						}
						else if (arr_arg[0] == 't' || arr_arg[0] == '=') {
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
					// fall through
				case 2:
					// Arrival time
					{
						auto arr_arg = util::lower_copy(instr.args[1]);
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
					// fall through
				case 1:
					s.name = instr.args[0];
					// fall through
				case 0:
					break;
			}

			return s;
		}

		static instruction create_instruction_track_station(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.Station");

			line_splitting::instruction_info ii;
			ii.name = instr.name;
			ii.args.resize(12);
			ii.args[0] = instr.args.size() >= 1 ? instr.args[0] : ""s;  // Name
			ii.args[1] = instr.args.size() >= 2 ? instr.args[1] : ""s;  // ArivalTime
			ii.args[2] = instr.args.size() >= 3 ? instr.args[2] : ""s;  // DepartureTime
			ii.args[3] = "0"s;                                          // Pass Alarm
			ii.args[4] = "b"s;                                          // Doors
			ii.args[5] = instr.args.size() >= 4 ? instr.args[3] : "0"s; // ForcedRedSignal
			ii.args[6] = instr.args.size() >= 5 ? instr.args[4] : "0"s; // System
			ii.args[7] = ""s;                                           // Arival Sound
			ii.args[8] = "15"s;                                         // Stop Duration
			ii.args[9] = "100"s;                                        // PassengerRatio
			ii.args[10] = instr.args.size() >= 6 ? instr.args[5] : ""s; // DepartureSound
			ii.args[11] = "0"s;                                         // Timetable Index

			return create_instruction_track_sta(ii);
		} // namespace

		static instruction create_instruction_track_stop(const line_splitting::instruction_info& instr) {
			instructions::track::Stop s;

			switch (instr.args.size()) {
				default:
				case 4:
					s.cars = std::size_t(util::parse_loose_integer(instr.args[3], 0));
					// fall through
				case 3:
					s.forwards_tolerance = util::parse_loose_float(instr.args[2], 5);
					// fall through
				case 2:
					s.backwards_tolerance = util::parse_loose_float(instr.args[1], 5);
					// fall through
				case 1: {
					auto direction_num = util::parse_loose_integer(instr.args[0], 0);

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
				}
					// fall through
				case 0:
					break;
			}

			return s;
		}

		static instruction create_instruction_track_form(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 4, "Track.Form");

			instructions::track::Form f;

			f.rail_index_1 = std::size_t(util::parse_loose_integer(instr.args[0]));
			f.rail_index_2 = std::size_t(util::parse_loose_integer(instr.args[1]));
			f.roof_structure_index = std::size_t(util::parse_loose_integer(instr.args[2]));
			f.form_structure_index = std::size_t(util::parse_loose_integer(instr.args[3]));

			return f;
		}

		/////////////////////////////////
		// Signalling and Speed Limits //
		/////////////////////////////////

		static instruction create_instruction_track_limit(const line_splitting::instruction_info& instr) {
			instructions::track::Limit l;

			switch (instr.args.size()) {
				default:
				case 3: {
					auto course_num = util::parse_loose_integer(instr.args[2], 0);

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
				}
					// fall through
				case 2: {
					auto post_num = util::parse_loose_integer(instr.args[1], 0);

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
				}
				// fall through
				case 1:
					l.speed = util::parse_loose_float(instr.args[0], 0);
					// fall through
				case 0:
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
			args_at_least(instr, 2, "Track.SigF");

			instructions::track::SigF sf;

			sf.signal_index = std::size_t(util::parse_loose_integer(instr.args[0]));
			sf.section = std::size_t(util::parse_loose_integer(instr.args[1]));
			set_positions<2>(sf, instr);

			return sf;
		}
		static instruction create_instruction_track_signal(const line_splitting::instruction_info& instr) {
			instructions::track::Signal s;

			if (instr.args.size() >= 1) {
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
			}
			else {
				s.type = instructions::track::Signal::R_G;
			}

			set_positions<2>(s, instr);

			return s;
		}

		static instruction create_instruction_track_relay(const line_splitting::instruction_info& instr) {
			instructions::track::Relay r;

			set_positions<0>(r, instr);

			return r;
		}

		////////////////////
		// Safety Systems //
		////////////////////

		static instruction create_instruction_track_beacon(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 4, "Track.Beacon");

			instructions::track::Beacon b;

			b.type = std::size_t(util::parse_loose_integer(instr.args[0]));
			b.beacon_structure_index = std::size_t(util::parse_loose_integer(instr.args[1]));
			b.section = std::size_t(util::parse_loose_integer(instr.args[2]));
			b.data = std::size_t(util::parse_loose_integer(instr.args[3]));

			set_positions<4>(b, instr);

			return b;
		}

		static instruction create_instruction_track_transponder(const line_splitting::instruction_info& instr) {
			instructions::track::Transponder t;

			switch (instr.args.size()) {
				default:
				case 3:
					t.switch_system = util::parse_loose_integer(instr.args[2], 0) == 0;
					// fall through
				case 2:
					t.signal = std::size_t(util::parse_loose_integer(instr.args[1], 0));
					// fall through
				case 1: {
					auto type_num = util::parse_loose_integer(instr.args[0], 0);

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
				}
				// fall through
				case 0:
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

		//////////
		// Misc //
		//////////

		static instruction create_instruction_track_back(const line_splitting::instruction_info& instr) {
			return create_single_sizet_instruction<instructions::track::Back>(instr, "Track.Back");
		}

		static instruction create_instruction_track_fog(const line_splitting::instruction_info& instr) {
			instructions::track::Fog f;

			switch (instr.args.size()) {
				default:
				case 5:
					f.color.b = std::uint8_t(util::parse_loose_integer(instr.args[4], 128));
					// fall through
				case 4:
					f.color.g = std::uint8_t(util::parse_loose_integer(instr.args[3], 128));
					// fall through
				case 3:
					f.color.r = std::uint8_t(util::parse_loose_integer(instr.args[2], 128));
					// fall through
				case 2:
					f.ending_distance = util::parse_loose_float(instr.args[1], 0);
					// fall through
				case 1:
					f.starting_distance = util::parse_loose_float(instr.args[0], 0);
					// fall through
				case 0:
					break;
			}

			return f;
		}

		static instruction create_instruction_track_brightness(const line_splitting::instruction_info& instr) {
			instructions::track::Brightness b;

			if (instr.args.size() >= 1) {
				b.value = std::uint8_t(util::parse_loose_integer(instr.args[0], 255));
			}

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
			if (instr.args.size() >= 7) {
				poi.text = instr.args[6];
			}

			return poi;
		}

		static instruction create_instruction_track_pretrain(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.PreTrain");

			instructions::track::PreTrain pt;

			pt.time = util::parse_time(instr.args[0]);

			return pt;
		}

		static instruction create_instruction_track_announce(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.Announce");

			instructions::track::Announce a;

			a.filename = instr.args[0];
			if (instr.args.size() >= 2) {
				a.speed = util::parse_loose_float(instr.args[1], 0);
			}

			return a;
		}

		static instruction create_instruction_track_doppler(const line_splitting::instruction_info& instr) {
			args_at_least(instr, 1, "Track.Doppler");

			instructions::track::Doppler d;

			d.filename = instr.args[0];
			switch (instr.args.size()) {
				case 3:
					d.y_offset = util::parse_loose_float(instr.args[2]);
					// fall through
				case 2:
					d.x_offset = util::parse_loose_float(instr.args[1]);
					// fall through
				case 1:
					break;
			}

			return d;
		}

		static instruction create_instruction_track_buffer(const line_splitting::instruction_info& instr) {
			(void) instr;
			return instructions::track::Buffer{};
		}

		static const std::map<std::string, instruction (*)(const line_splitting::instruction_info& instr)>
		    function_mapping = {
		        ////////////////
		        // CSV ROUTES //
		        ////////////////

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
		        {"train.file"s, &create_instruction_train_folder},
		        {"train.run"s, &create_instruction_train_run},
		        {"train.rail"s, &create_instruction_train_run},
		        {"train.flange"s, &create_instruction_train_flange},
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

		        // Rails
		        {"track.railstart"s, &create_instruction_track_railstart},
		        {"track.rail"s, &create_instruction_track_rail},
		        {"track.railtype"s, &create_instruction_track_railtype},
		        {"track.railend"s, &create_instruction_track_railend},
		        {"track.accuracy"s, &create_instruction_track_accuracy},
		        {"track.adhesion"s, &create_instruction_track_adhesion},

		        // Geometry
		        {"track.pitch"s, &create_instruction_track_pitch},
		        {"track.curve"s, &create_instruction_track_curve},
		        {"track.turn"s, &create_instruction_track_turn},
		        {"track.height"s, &create_instruction_track_height},

		        // Objects
		        {"track.freeobj"s, &create_instruction_track_freeobj},
		        {"track.wall"s, &create_instruction_track_wall},
		        {"track.wallend"s, &create_instruction_track_wallend},
		        {"track.dike"s, &create_instruction_track_dike},
		        {"track.dikeend"s, &create_instruction_track_dikeend},
		        {"track.pole"s, &create_instruction_track_pole},
		        {"track.poleend"s, &create_instruction_track_poleend},
		        {"track.crack"s, &create_instruction_track_crack},
		        {"track.ground"s, &create_instruction_track_ground},

		        // Stations
		        {"track.sta"s, &create_instruction_track_sta},
		        {"track.station"s, &create_instruction_track_station},
		        {"track.stop"s, &create_instruction_track_stop},
		        {"track.form"s, &create_instruction_track_form},

		        // Signalling and speed limits
		        {"track.limit"s, &create_instruction_track_limit},
		        {"track.section"s, &create_instruction_track_section},
		        {"track.sigf"s, &create_instruction_track_sigf},
		        {"track.signal"s, &create_instruction_track_signal},
		        {"track.sig"s, &create_instruction_track_signal},
		        {"track.relay"s, &create_instruction_track_relay},

		        // Safety systems
		        {"track.beacon"s, &create_instruction_track_beacon},
		        {"track.transponder"s, &create_instruction_track_transponder},
		        {"track.atssn"s, &create_instruction_track_atssn},
		        {"track.atsp"s, &create_instruction_track_atsp},
		        {"track.pattern"s, &create_instruction_track_pattern},
		        {"track.plimit"s, &create_instruction_track_plimit},

		        // Miscellaneous
		        {"track.back"s, &create_instruction_track_back},
		        {"track.fog"s, &create_instruction_track_fog},
		        {"track.brightness"s, &create_instruction_track_brightness},
		        {"track.marker"s, &create_instruction_track_marker},
		        {"track.pointofinterest"s, &create_instruction_track_pointofinterest},
		        {"track.pretrain"s, &create_instruction_track_pretrain},
		        {"track.announce"s, &create_instruction_track_announce},
		        {"track.doppler"s, &create_instruction_track_doppler},
		        {"track.buffer"s, &create_instruction_track_buffer},

		        ///////////////
		        // RW Routes //
		        ///////////////

		        // [Options]
		        {"@@options@@unitoflength"s, &create_instruction_options_unitoflength},
		        {"@@options@@unitofspeed"s, &create_instruction_options_unitofspeed},
		        {"@@options@@blocklength"s, &create_instruction_options_blocklength},
		        {"@@options@@objectvisibility"s, &create_instruction_options_objectvisibility},
		        {"@@options@@sectionbehavior"s, &create_instruction_options_sectionbehavior},
		        {"@@options@@cantbehavior"s, &create_instruction_options_cantbehavior},
		        {"@@options@@fogbehavior"s, &create_instruction_options_fogbehavior},

		        // [Route]
		        {"@@route@@comment"s, &create_instruction_route_comment},
		        {"@@route@@image"s, &create_instruction_route_image},
		        {"@@route@@timetable"s, &create_instruction_route_timetable},
		        {"@@route@@change"s, &create_instruction_route_change},
		        {"@@route@@gauge"s, &create_instruction_route_gauge},
		        {"@@route@@signal"s, &create_instruction_route_signal},
		        {"@@route@@runinterval"s, &create_instruction_route_runinterval},
		        {"@@route@@accelerationduetogravity"s, &create_instruction_route_accelerationduetogravity},
		        {"@@route@@elevation"s, &create_instruction_route_elevation},
		        {"@@route@@temperature"s, &create_instruction_route_temperature},
		        {"@@route@@pressure"s, &create_instruction_route_pressure},
		        {"@@route@@ambientlight"s, &create_instruction_route_ambientlight},
		        {"@@route@@directionallight"s, &create_instruction_route_directionallight},
		        {"@@route@@lightdirection"s, &create_instruction_route_lightdirection},

		        // [Train]
		        {"@@train@@folder"s, &create_instruction_train_folder},
		        {"@@train@@file"s, &create_instruction_train_folder},
		        {"@@train@@run"s, &create_instruction_train_run},
		        {"@@train@@rail"s, &create_instruction_train_run},
		        {"@@train@@flange"s, &create_instruction_train_flange},
		        {"@@train@@timetable"s, &create_instruction_train_timetable},
		        {"@@train@@gauge"s, &create_instruction_route_gauge},
		        {"@@train@@interval"s, &create_instruction_route_runinterval},
		        {"@@train@@velocity"s, &create_instruction_train_velocity},

		        // [Object]
		        {"@@object@@rail"s, &create_instruction_structure_command},
		        {"@@object@@beacon"s, &create_instruction_structure_command},
		        {"@@object@@ground"s, &create_instruction_structure_command},
		        {"@@object@@walll"s, &create_instruction_structure_command},
		        {"@@object@@wallr"s, &create_instruction_structure_command},
		        {"@@object@@dikel"s, &create_instruction_structure_command},
		        {"@@object@@diker"s, &create_instruction_structure_command},
		        {"@@object@@forml"s, &create_instruction_structure_command},
		        {"@@object@@formr"s, &create_instruction_structure_command},
		        {"@@object@@formcl"s, &create_instruction_structure_command},
		        {"@@object@@formcr"s, &create_instruction_structure_command},
		        {"@@object@@roofl"s, &create_instruction_structure_command},
		        {"@@object@@roofr"s, &create_instruction_structure_command},
		        {"@@object@@roofcl"s, &create_instruction_structure_command},
		        {"@@object@@roofcr"s, &create_instruction_structure_command},
		        {"@@object@@crackl"s, &create_instruction_structure_command},
		        {"@@object@@crackr"s, &create_instruction_structure_command},
		        {"@@object@@freeobj"s, &create_instruction_structure_command},
		        {"@@object@@pole"s, &create_instruction_structure_pole},
		        {"@@object@@back"s, &create_instruction_texture_background},

		        // [Cycle]
		        {"@@cycle@@groundstructureindex"s, &create_instruction_cycle_ground},

		        // [Signal]
		        {"@@signal@@signalindex"s, &create_instruction_signal},

		        // [Railway]

		        // Rails
		        {"@@railway@@railstart"s, &create_instruction_track_railstart},
		        {"@@railway@@rail"s, &create_instruction_track_rail},
		        {"@@railway@@railtype"s, &create_instruction_track_railtype},
		        {"@@railway@@railend"s, &create_instruction_track_railend},
		        {"@@railway@@accuracy"s, &create_instruction_track_accuracy},
		        {"@@railway@@adhesion"s, &create_instruction_track_adhesion},

		        // Geometry
		        {"@@railway@@pitch"s, &create_instruction_track_pitch},
		        {"@@railway@@curve"s, &create_instruction_track_curve},
		        {"@@railway@@turn"s, &create_instruction_track_turn},
		        {"@@railway@@height"s, &create_instruction_track_height},

		        // Objects
		        {"@@railway@@freeobj"s, &create_instruction_track_freeobj},
		        {"@@railway@@wall"s, &create_instruction_track_wall},
		        {"@@railway@@wallend"s, &create_instruction_track_wallend},
		        {"@@railway@@dike"s, &create_instruction_track_dike},
		        {"@@railway@@dikeend"s, &create_instruction_track_dikeend},
		        {"@@railway@@pole"s, &create_instruction_track_pole},
		        {"@@railway@@poleend"s, &create_instruction_track_poleend},
		        {"@@railway@@crack"s, &create_instruction_track_crack},
		        {"@@railway@@ground"s, &create_instruction_track_ground},

		        // Stations
		        {"@@railway@@sta"s, &create_instruction_track_sta},
		        {"@@railway@@station"s, &create_instruction_track_station},
		        {"@@railway@@stop"s, &create_instruction_track_stop},
		        {"@@railway@@form"s, &create_instruction_track_form},

		        // Signalling and speed limits
		        {"@@railway@@limit"s, &create_instruction_track_limit},
		        {"@@railway@@section"s, &create_instruction_track_section},
		        {"@@railway@@sigf"s, &create_instruction_track_sigf},
		        {"@@railway@@signal"s, &create_instruction_track_signal},
		        {"@@railway@@relay"s, &create_instruction_track_relay},

		        // Safety systems
		        {"@@railway@@beacon"s, &create_instruction_track_beacon},
		        {"@@railway@@transponder"s, &create_instruction_track_transponder},
		        {"@@railway@@atssn"s, &create_instruction_track_atssn},
		        {"@@railway@@atsp"s, &create_instruction_track_atsp},
		        {"@@railway@@pattern"s, &create_instruction_track_pattern},
		        {"@@railway@@plimit"s, &create_instruction_track_plimit},

		        // Miscellaneous
		        {"@@railway@@back"s, &create_instruction_track_back},
		        {"@@railway@@fog"s, &create_instruction_track_fog},
		        {"@@railway@@brightness"s, &create_instruction_track_brightness},
		        {"@@railway@@marker"s, &create_instruction_track_marker},
		        {"@@railway@@pointofinterest"s, &create_instruction_track_pointofinterest},
		        {"@@railway@@pretrain"s, &create_instruction_track_pretrain},
		        {"@@railway@@announce"s, &create_instruction_track_announce},
		        {"@@railway@@doppler"s, &create_instruction_track_doppler},
		        {"@@railway@@buffer"s, &create_instruction_track_buffer},
		};
	} // namespace

	static instruction generate_instruction(const preprocessed_lines& lines, const preprocessed_line& line,
	                                        errors::multi_error& errors, std::string& with_value, file_type ft) {
		instruction i;

		auto parsed = ft == file_type::csv ? line_splitting::csv(line) : line_splitting::rw(line);

		if (parsed.track_position) {
			return create_instruction_location_statement(parsed);
		}

		util::lower(parsed.name);

		if (ft == file_type::csv) {
			auto dot_iter = std::find(parsed.name.begin(), parsed.name.end(), '.');
			bool has_dot = dot_iter != parsed.name.end();

			// get fully qualified name
			if (has_dot && dot_iter == parsed.name.begin()) {
				parsed.name = with_value + parsed.name;
			}
		}
		else if (parsed.name != "with"s) {
			// Deal with special cases
			if (with_value.empty() && parsed.name != "with"s) {
				return instructions::route::Comment{line.contents};
			}
			if (with_value == "signal") {
				parsed.indices.emplace_back(std::move(parsed.name));
				parsed.name = "@@signal@@signalindex"s;
			}
			else if (with_value == "cycle") {
				parsed.indices.emplace_back(std::move(parsed.name));
				parsed.name = "@@cycle@@groundstructureindex"s;
			}
			// Get normal qualitifed name
			else {
				parsed.name = "@@"s + with_value + "@@" + parsed.name;
			}
		}

		// lookup function
		auto func_iter = function_mapping.find(parsed.name);
		if (func_iter == function_mapping.end()) {
			if (parsed.name == "with") {
				with_value = util::lower_copy(parsed.args[0]);
			}
			else {
				bool ignored = false;
				if (ft == file_type::csv) {
					ignored = parsed.name == "route.developerid"s || parsed.name == "train.acceleration"s ||
					          parsed.name == "train.station"s;
				}
				else {
					ignored = parsed.name == "@@route@@developerid"s || parsed.name == "@@train@@acceleration"s ||
					          parsed.name == "@@train@@station"s;
				}

				if (!ignored) {
					std::ostringstream oss;
					oss << "\"" << parsed.name << "\" is not a known function in a "
					    << (ft == file_type::csv ? "csv" : "rw") << " file";
					errors[lines.filenames[line.filename_index]].emplace_back(errors::error_t{line.line, oss.str()});
				}
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

	instruction_list generate_instructions(const preprocessed_lines& lines, errors::multi_error& errors, file_type ft) {
		instruction_list i_list;
		i_list.instructions.reserve(lines.lines.size());

		std::string with_value = "";
		for (auto& line : lines.lines) {
			auto i = generate_instruction(lines, line, errors, with_value, ft);

			mapbox::util::apply_visitor(
			    [&line](auto& i) {
				    i.file_index = line.filename_index;
				    i.line = line.line;
			    },
			    i);

			i_list.instructions.emplace_back(i);
		}

		i_list.filenames = lines.filenames;

		return i_list;
	}
} // namespace csv_rw_route
} // namespace parsers
