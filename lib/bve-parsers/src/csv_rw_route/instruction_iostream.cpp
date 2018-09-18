#include "core/macro_helpers.hpp"
#include "csv_rw_route.hpp"
#include <gsl/gsl_util>
#include <iomanip>
#include <ostream>
#include <tl/optional.hpp>

template <class T>
std::ostream& operator<<(std::ostream& os, tl::optional<T> const& val) {
	val.map_or_else([&](T t) { os << t; }, [&] { os << '-'; });
	return os;
}

#define PRINT_VALUE(name) os_ << ", " #name " = " << inst.name;
#define PRINT_BYTE(name) os_ << ", " #name " = " << gsl::narrow<int>(inst.name);
#define PRINT_VECTOR(name)                                                                         \
	os_ << ", " #name " = ";                                                                       \
	print_vector(inst.name);
#define PRINT_TIME(name)                                                                           \
	os_ << ", " #name " = " << std::setw(2) << std::setfill('0') << (inst.name / 3600) << ':'      \
	    << std::setw(2) << std::setfill('0') << ((inst.name / 60) % 60) << ':' << std::setw(2)     \
	    << std::setfill('0') << (inst.name % 60);
#define PRINT_COLOR(name)                                                                          \
	os_ << ", " #name " = "                                                                        \
	    << "(r = " << static_cast<int>(inst.name.r) << ", g = " << static_cast<int>(inst.name.g)   \
	    << ", b = " << static_cast<int>(inst.name.b) << ')';

#define PRINT_ENUM1(name, nsp, value1)                                                             \
	if (inst.name == instructions::nsp::value1)                                                    \
		os_ << ", " #name " = " #value1;
#define PRINT_ENUM2(name, nsp, value2, ...)                                                        \
	if (inst.name == instructions::nsp::value2)                                                    \
		os_ << ", " #name " = " #value2;                                                           \
	EXPAND(PRINT_ENUM1(name, nsp, __VA_ARGS__))
#define PRINT_ENUM3(name, nsp, value3, ...)                                                        \
	if (inst.name == instructions::nsp::value3)                                                    \
		os_ << ", " #name " = " #value3;                                                           \
	EXPAND(PRINT_ENUM2(name, nsp, __VA_ARGS__))
#define PRINT_ENUM4(name, nsp, value4, ...)                                                        \
	if (inst.name == instructions::nsp::value4)                                                    \
		os_ << ", " #name " = " #value4;                                                           \
	EXPAND(PRINT_ENUM3(name, nsp, __VA_ARGS__))
#define PRINT_ENUM5(name, nsp, value5, ...)                                                        \
	if (inst.name == instructions::nsp::value5)                                                    \
		os_ << ", " #name " = " #value5;                                                           \
	EXPAND(PRINT_ENUM4(name, nsp, __VA_ARGS__))
#define PRINT_ENUM6(name, nsp, value6, ...)                                                        \
	if (inst.name == instructions::nsp::value6)                                                    \
		os_ << ", " #name " = " #value6;                                                           \
	EXPAND(PRINT_ENUM5(name, nsp, __VA_ARGS__))
#define PRINT_ENUM7(name, nsp, value7, ...)                                                        \
	if (inst.name == instructions::nsp::value7)                                                    \
		os_ << ", " #name " = " #value7;                                                           \
	EXPAND(PRINT_ENUM6(name, nsp, __VA_ARGS__))
#define PRINT_ENUM8(name, nsp, value8, ...)                                                        \
	if (inst.name == instructions::nsp::value8)                                                    \
		os_ << ", " #name " = " #value8;                                                           \
	EXPAND(PRINT_ENUM7(name, nsp, __VA_ARGS__))
#define PRINT_ENUM9(name, nsp, value9, ...)                                                        \
	if (inst.name == instructions::nsp::value9)                                                    \
		os_ << ", " #name " = " #value9;                                                           \
	EXPAND(PRINT_ENUM8(name, nsp, __VA_ARGS__))
#define PRINT_ENUM10(name, nsp, value10, ...)                                                      \
	if (inst.name == instructions::nsp::value10)                                                   \
		os_ << ", " #name " = " #value10;                                                          \
	EXPAND(PRINT_ENUM9(name, nsp, __VA_ARGS__))
#define PRINT_ENUM11(name, nsp, value11, ...)                                                      \
	if (inst.name == instructions::nsp::value11)                                                   \
		os_ << ", " #name " = " #value11;                                                          \
	EXPAND(PRINT_ENUM10(name, nsp, __VA_ARGS__))
#define PRINT_ENUM12(name, nsp, value12, ...)                                                      \
	if (inst.name == instructions::nsp::value12)                                                   \
		os_ << ", " #name " = " #value12;                                                          \
	EXPAND(PRINT_ENUM11(name, nsp, __VA_ARGS__))
#define PRINT_ENUM13(name, nsp, value13, ...)                                                      \
	if (inst.name == instructions::nsp::value13)                                                   \
		os_ << ", " #name " = " #value13;                                                          \
	EXPAND(PRINT_ENUM12(name, nsp, __VA_ARGS__))
#define PRINT_ENUM14(name, nsp, value14, ...)                                                      \
	if (inst.name == instructions::nsp::value14)                                                   \
		os_ << ", " #name " = " #value14;                                                          \
	EXPAND(PRINT_ENUM13(name, nsp, __VA_ARGS__))
#define PRINT_ENUM15(name, nsp, value15, ...)                                                      \
	if (inst.name == instructions::nsp::value15)                                                   \
		os_ << ", " #name " = " #value15;                                                          \
	EXPAND(PRINT_ENUM14(name, nsp, __VA_ARGS__))
#define PRINT_ENUM16(name, nsp, value16, ...)                                                      \
	if (inst.name == instructions::nsp::value16)                                                   \
		os_ << ", " #name " = " #value16;                                                          \
	EXPAND(PRINT_ENUM15(name, nsp, __VA_ARGS__))
#define PRINT_ENUM17(name, nsp, value17, ...)                                                      \
	if (inst.name == instructions::nsp::value17)                                                   \
		os_ << ", " #name " = " #value17;                                                          \
	EXPAND(PRINT_ENUM16(name, nsp, __VA_ARGS__))
#define PRINT_ENUM18(name, nsp, value18, ...)                                                      \
	if (inst.name == instructions::nsp::value18)                                                   \
		os_ << ", " #name " = " #value18;                                                          \
	EXPAND(PRINT_ENUM17(name, nsp, __VA_ARGS__))

namespace bve::parsers::csv_rw_route {
	namespace {
		struct csv_rw_route_instruction_io_class {
		  private:
			std::ostream& os_;
			const std::vector<std::string> filenames_;
			bool has_filenames_;

		  public:
			explicit csv_rw_route_instruction_io_class(std::ostream& os) :
			    os_(os),
			    has_filenames_(false) {}
			explicit csv_rw_route_instruction_io_class(std::ostream& os,
			                                           std::vector<std::string> filenames) :
			    os_(os),
			    filenames_(std::move(filenames)),
			    has_filenames_(true) {}

			template <class T>
			void print_vector(const std::vector<T>& vec) {
				std::size_t i = 0;
				os_ << "[";
				for (auto& item : vec) {
					os_ << item;
					if (++i != vec.size()) {
						os_ << ", ";
					}
				}
				os_ << "]";
			}

			template <class T>
			void start(const T& inst, const char* name) {
				os_ << inst.absolute_position << ", " << name;
				if (has_filenames_) {
					os_ << ", filename = " << filenames_[inst.file_index];
				}
				else {
					os_ << ", file_index = " << inst.file_index;
				}
				os_ << ", line = " << inst.line;
			}

			static void end() {}

			void operator()(const instructions::naked::position& inst) {
				start(inst, "Position");

				PRINT_VECTOR(distances);

				end();
			}
			void operator()(const instructions::naked::none& inst) {
				start(inst, "None");

				end();
			}
			void operator()(const instructions::options::unit_of_length& inst) {
				start(inst, "Options.UnitOfLength");

				PRINT_VECTOR(factors_in_meters);

				end();
			}
			void operator()(const instructions::options::unit_of_speed& inst) {
				start(inst, "Options.UnitOfSpeed");

				PRINT_VALUE(factor_in_kph);

				end();
			}
			void operator()(const instructions::options::block_length& inst) {
				start(inst, "Options.BlockLength");

				PRINT_VALUE(length);

				end();
			}
			void operator()(const instructions::options::object_visibility& inst) {
				start(inst, "Options.ObjectVisibility");

				PRINT_ENUM2(mode, options::object_visibility, legacy, track_based);

				end();
			}
			void operator()(const instructions::options::section_behavior& inst) {
				start(inst, "Options.SectionBehavior");

				PRINT_ENUM2(mode, options::section_behavior, normal, simplified);

				end();
			}
			void operator()(const instructions::options::cant_behavior& inst) {
				start(inst, "Options.CantBehavior");

				PRINT_ENUM2(mode, options::cant_behavior, unsigned_cant, signed_cant);

				end();
			}
			void operator()(const instructions::options::fog_behavior& inst) {
				start(inst, "Options.FogBehavior");

				PRINT_ENUM2(mode, options::fog_behavior, block_based, interpolated);

				end();
			}
			void operator()(const instructions::options::compatible_transparency_mode& inst) {
				start(inst, "Options.CompatibleTransparencyMode");

				PRINT_ENUM2(mode, options::compatible_transparency_mode, off, on);

				end();
			}
			void operator()(const instructions::options::enable_bve_ts_hacks& inst) {
				start(inst, "Options.EnableBveTsHacks");

				PRINT_ENUM2(mode, options::enable_bve_ts_hacks, off, on);

				end();
			}
			void operator()(const instructions::route::comment& inst) {
				start(inst, "Route.Comment");

				PRINT_VALUE(text);

				end();
			}
			void operator()(const instructions::route::image& inst) {
				start(inst, "Route.Image");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::route::timetable& inst) {
				start(inst, "Route.Timetable");

				PRINT_VALUE(text);

				end();
			}
			void operator()(const instructions::route::change& inst) {
				start(inst, "Route.Change");

				PRINT_ENUM3(mode, route::change, safty_activiated_service_brakes,
				            safty_activiated_emergency_brakes, safty_deactivated_emergency_brakes);

				end();
			}
			void operator()(const instructions::route::guage& inst) {
				start(inst, "Route.Guage");

				PRINT_VALUE(width);

				end();
			}
			void operator()(const instructions::route::signal& inst) {
				start(inst, "Route.Signal");

				PRINT_VALUE(aspect_index);

				end();
			}
			void operator()(const instructions::route::run_interval& inst) {
				start(inst, "Route.RunInterval");

				PRINT_VECTOR(time_interval);

				end();
			}
			void operator()(const instructions::route::acceleration_due_to_gravity& inst) {
				start(inst, "Route.AccelerationDueToGravity");

				PRINT_VALUE(value);

				end();
			}
			void operator()(const instructions::route::elevation& inst) {
				start(inst, "Route.Elevation");

				PRINT_VALUE(height);

				end();
			}
			void operator()(const instructions::route::temperature& inst) {
				start(inst, "Route.Temperature");

				PRINT_VALUE(celcius);

				end();
			}
			void operator()(const instructions::route::pressure& inst) {
				start(inst, "Route.Pressure");

				PRINT_VALUE(kpa);

				end();
			}
			void operator()(const instructions::route::display_speed& inst) {
				start(inst, "Route.DisplaySpeed");

				PRINT_VALUE(unit_string);
				PRINT_VALUE(conversion_factor);

				end();
			}
			void operator()(const instructions::route::loading_screen& inst) {
				start(inst, "Route.LoadingScreen");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::route::start_time& inst) {
				start(inst, "Route.StartTime");

				PRINT_TIME(time);

				end();
			}
			void operator()(const instructions::route::dynamic_light& inst) {
				start(inst, "Route.DynamicLight");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::route::ambiant_light& inst) {
				start(inst, "Route.AmbiantLight");

				PRINT_COLOR(color);

				end();
			}
			void operator()(const instructions::route::directional_light& inst) {
				start(inst, "Route.DirectionalLight");

				PRINT_COLOR(color);

				end();
			}
			void operator()(const instructions::route::light_direction& inst) {
				start(inst, "Route.LightDirection");

				PRINT_VALUE(theta);
				PRINT_VALUE(phi);

				end();
			}
			void operator()(const instructions::train::folder& inst) {
				start(inst, "Train.Folder");

				PRINT_VALUE(filepath);

				end();
			}
			void operator()(const instructions::train::rail& inst) {
				start(inst, "Train.Rail");

				PRINT_VALUE(rail_type_index);
				PRINT_VALUE(run_sound_index);

				end();
			}
			void operator()(const instructions::train::flange& inst) {
				start(inst, "Train.Flange");

				PRINT_VALUE(rail_type_index);
				PRINT_VALUE(flange_sound_index);

				end();
			}
			void operator()(const instructions::train::timetable& inst) {
				start(inst, "Train.Timetable");

				PRINT_VALUE(timetable_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::train::velocity& inst) {
				start(inst, "Train.Velocity");

				PRINT_VALUE(speed);

				end();
			}
			void operator()(const instructions::structure::command& inst) {
				start(inst, "Structure.Command");

				PRINT_ENUM18(command_type, structure::command, ground, rail, wall_l, wall_r, dike_l,
				             dike_r, form_l, form_r, form_cl, form_cr, roof_l, form_r, form_cl,
				             form_cr, crack_l, crack_r, free_obj, beacon);

				PRINT_VALUE(structure_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::structure::pole& inst) {
				start(inst, "Structure.Pole");

				PRINT_VALUE(additional_rails);
				PRINT_VALUE(pole_structure_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::texture::background_load& inst) {
				start(inst, "Texture.Background_Load");

				PRINT_VALUE(background_texture_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::texture::background_x& inst) {
				start(inst, "Texture.Background_X");

				PRINT_VALUE(background_texture_index);
				PRINT_VALUE(repetition_count);

				end();
			}
			void operator()(const instructions::texture::background_aspect& inst) {
				start(inst, "Texture.Background_Aspect");

				PRINT_VALUE(background_texture_index);
				PRINT_ENUM2(mode, texture::background_aspect, fixed, aspect);

				end();
			}
			void operator()(const instructions::cycle::ground& inst) {
				start(inst, "Cycle.Ground");

				PRINT_VALUE(cycle_structure_index);
				PRINT_VECTOR(input_indices);

				end();
			}
			void operator()(const instructions::cycle::rail& inst) {
				start(inst, "Cycle.Rail");

				PRINT_VALUE(cycle_structure_index);
				PRINT_VECTOR(input_indices);

				end();
			}
			void operator()(const instructions::naked::signal_animated& inst) {
				start(inst, "SignalAnimated");

				PRINT_VALUE(signal_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::naked::signal& inst) {
				start(inst, "Signal");

				PRINT_VALUE(signal_index);
				PRINT_VALUE(signal_filename);
				PRINT_VALUE(glow_filename);

				end();
			}
			void operator()(const instructions::track::rail_start& inst) {
				start(inst, "Track.RailStart");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(rail_type);

				end();
			}
			void operator()(const instructions::track::rail& inst) {
				start(inst, "Track.Rail");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(rail_type);

				end();
			}
			void operator()(const instructions::track::rail_type& inst) {
				start(inst, "Track.RailType");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(rail_type_number);

				end();
			}
			void operator()(const instructions::track::rail_end& inst) {
				start(inst, "Track.RailEnd");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);

				end();
			}
			void operator()(const instructions::track::adhesion& inst) {
				start(inst, "Track.Adhesion");

				PRINT_VALUE(value);

				end();
			}
			void operator()(const instructions::track::pitch& inst) {
				start(inst, "Track.Pitch");

				PRINT_VALUE(rate);

				end();
			}
			void operator()(const instructions::track::curve& inst) {
				start(inst, "Track.Curve");

				PRINT_VALUE(radius);
				PRINT_VALUE(cant);

				end();
			}
			void operator()(const instructions::track::turn& inst) {
				start(inst, "Track.Turn");

				PRINT_VALUE(ratio);

				end();
			}
			void operator()(const instructions::track::height& inst) {
				start(inst, "Track.Height");

				PRINT_VALUE(y);

				end();
			}
			void operator()(const instructions::track::free_obj& inst) {
				start(inst, "Track.FreeObj");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(free_obj_structure_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::wall& inst) {
				start(inst, "Track.Wall");

				PRINT_VALUE(rail_index);
				PRINT_ENUM3(direction, track::wall, left, both, right);
				PRINT_VALUE(wall_structure_index);

				end();
			}
			void operator()(const instructions::track::wall_end& inst) {
				start(inst, "Track.WallEnd");

				PRINT_VALUE(rail_index);

				end();
			}
			void operator()(const instructions::track::dike& inst) {
				start(inst, "Track.Dike");

				PRINT_VALUE(rail_index);
				PRINT_ENUM3(direction, track::dike, left, both, right);
				PRINT_VALUE(dike_structure_index);

				end();
			}
			void operator()(const instructions::track::dike_end& inst) {
				start(inst, "Track.DikeEnd");

				PRINT_VALUE(rail_index);

				end();
			}
			void operator()(const instructions::track::pole& inst) {
				start(inst, "Track.Pole");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(additional_rails);
				PRINT_VALUE(location);
				PRINT_VALUE(interval);
				PRINT_VALUE(pole_structure_index);

				end();
			}
			void operator()(const instructions::track::pole_end& inst) {
				start(inst, "Track.PoleEnd");

				PRINT_VALUE(rail_index);

				end();
			}
			void operator()(const instructions::track::crack& inst) {
				start(inst, "Track.Crack");

				PRINT_VALUE(rail_index_1);
				PRINT_VALUE(rail_index_2);
				PRINT_VALUE(crack_structure_index);

				end();
			}
			void operator()(const instructions::track::ground& inst) {
				start(inst, "Track.Ground");

				PRINT_VALUE(ground_structure_index);

				end();
			}
			void operator()(const instructions::track::sta& inst) {
				start(inst, "Track.Sta");

				PRINT_VALUE(name);
				PRINT_TIME(arrival);
				PRINT_ENUM6(arrival_tag, track::sta::arrival_time_t, time, any_time, all_pass,
				            player_pass, player_stop, all_stop);
				PRINT_TIME(departure);
				PRINT_ENUM6(departure_tag, track::sta::departure_time_t, time, any_time, terminal,
				            terminal_time, change_ends, change_ends_time);
				PRINT_VALUE(pass_alarm);
				PRINT_ENUM4(doors, track::sta::doors_t, left, none, right, both);
				PRINT_VALUE(force_red);
				PRINT_VALUE(system);
				PRINT_VALUE(arrival_sound);
				PRINT_VALUE(stop_duration);
				PRINT_VALUE(passenger_ratio);
				PRINT_VALUE(departure_sound);
				PRINT_VALUE(timetable_index);

				end();
			}
			void operator()(const instructions::track::station_xml& inst) {
				start(inst, "Track.StationXML");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::track::stop& inst) {
				start(inst, "Track.Stop");

				PRINT_ENUM3(stop_post, track::stop::stop_post_t, left, none, right);
				PRINT_VALUE(forwards_tolerance);
				PRINT_VALUE(backwards_tolerance);
				PRINT_VALUE(cars);

				end();
			}
			void operator()(const instructions::track::form& inst) {
				start(inst, "Track.Form");

				PRINT_VALUE(rail_index_1);
				PRINT_VALUE(rail_index_2);
				PRINT_VALUE(roof_structure_index);
				PRINT_VALUE(form_structure_index);

				end();
			}
			void operator()(const instructions::track::limit& inst) {
				start(inst, "Track.Limit");

				PRINT_VALUE(speed);
				PRINT_ENUM3(post, track::limit::post_t, left, none, right);
				PRINT_ENUM3(cource, track::limit::cource_t, left, none, right);

				end();
			}
			void operator()(const instructions::track::section& inst) {
				start(inst, "Track.Section");

				PRINT_VECTOR(a_term);

				end();
			}
			void operator()(const instructions::track::sig_f& inst) {
				start(inst, "Track.SigF");

				PRINT_VALUE(signal_index);
				PRINT_VALUE(section);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::signal& inst) {
				start(inst, "Track.Signal");

				PRINT_ENUM8(type, track::signal, r_y, r_g, r_y_g, r_yy_y_g, r_y_yg_g, r_yy_y_yg_g,
				            r_y_yg_g_gg, r_yy_y_yg_g_gg);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::relay& inst) {
				start(inst, "Track.Relay");

				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::beacon& inst) {
				start(inst, "Track.Beacon");

				PRINT_VALUE(type);
				PRINT_VALUE(beacon_structure_index);
				PRINT_VALUE(section);
				PRINT_VALUE(data);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::transponder& inst) {
				start(inst, "Track.Transponder");

				PRINT_ENUM5(type, track::transponder, s_type, sn_type, departure, ats_p_renewal,
				            ats_p_stop);
				PRINT_VALUE(signal);
				PRINT_VALUE(switch_system);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::pattern& inst) {
				start(inst, "Track.Pattern");

				PRINT_ENUM2(type, track::pattern, temporary, permanent);
				PRINT_VALUE(speed);

				end();
			}
			void operator()(const instructions::track::back& inst) {
				start(inst, "Track.Back");

				PRINT_VALUE(background_texture_index);

				end();
			}
			void operator()(const instructions::track::fog& inst) {
				start(inst, "Track.Fog");

				PRINT_VALUE(starting_distance);
				PRINT_VALUE(ending_distance);
				PRINT_COLOR(color);

				end();
			}
			void operator()(const instructions::track::brightness& inst) {
				start(inst, "Track.Brightness");

				PRINT_BYTE(value);

				end();
			}
			void operator()(const instructions::track::marker& inst) {
				start(inst, "Track.Marker");

				PRINT_VALUE(filename);
				PRINT_VALUE(distance);

				end();
			}
			void operator()(const instructions::track::marker_xml& inst) {
				start(inst, "Track.MarkerXML");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::track::text_marker& inst) {
				start(inst, "Track.TextMarker");

				PRINT_VALUE(text);
				PRINT_VALUE(distance);
				PRINT_ENUM8(font_color, track::text_marker, black, gray, white, red, orange, green,
				            blue, magenta);

				end();
			}
			void operator()(const instructions::track::point_of_interest& inst) {
				start(inst, "Track.PointOfInterest");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);
				PRINT_VALUE(text);

				end();
			}
			void operator()(const instructions::track::pre_train& inst) {
				start(inst, "Track.PreTrain");

				PRINT_TIME(time)

				end();
			}
			void operator()(const instructions::track::announce& inst) {
				start(inst, "Track.Announce");

				PRINT_VALUE(filename);
				PRINT_VALUE(speed);

				end();
			}
			void operator()(const instructions::track::doppler& inst) {
				start(inst, "Track.Doppler");

				PRINT_VALUE(filename);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);

				end();
			}
			void operator()(const instructions::track::buffer& inst) {
				start(inst, "Track.Buffer");

				end();
			}
		};
	} // namespace
} // namespace bve::parsers::csv_rw_route

std::ostream& operator<<(std::ostream& os, const bve::parsers::csv_rw_route::instruction& i) {
	os << std::boolalpha;

	bve::parsers::csv_rw_route::csv_rw_route_instruction_io_class crriic(os);

	apply_visitor(crriic, i);

	return os;
}

std::ostream& operator<<(std::ostream& os,
                         const bve::parsers::csv_rw_route::instruction_list& list) {
	os << "Files:\n";
	for (std::size_t i = 0; i < list.filenames.size(); ++i) {
		os << i << ": " << list.filenames[i] << '\n';
	}
	os << "Instructions:\n";
	for (auto& i : list.instructions) {
		os << std::boolalpha;

		bve::parsers::csv_rw_route::csv_rw_route_instruction_io_class crriic(os, list.filenames);

		apply_visitor(crriic, i);

		os << "\n";
	}
	return os;
}
