#include "csv_rw_route.hpp"
#include <boost/optional/optional_io.hpp>
#include <iomanip>
#include <ostream>

// MSVC doesn't expand __VA_ARGS__ right, this is a workaround
#define EXPAND(x) x

#define PRINT_VALUE(name) _os << ", " #name " = " << inst.name;
#define PRINT_BYTE(name) _os << ", " #name " = " << int(inst.name);
#define PRINT_VECTOR(name)                                                                         \
	_os << ", " #name " = ";                                                                       \
	print_vector(inst.name);
#define PRINT_TIME(name)                                                                           \
	_os << ", " #name " = " << std::setw(2) << std::setfill('0') << (inst.name / 3600) << ':'      \
	    << std::setw(2) << std::setfill('0') << ((inst.name / 60) % 60) << ':' << std::setw(2)     \
	    << std::setfill('0') << (inst.name % 60);
#define PRINT_COLOR(name)                                                                          \
	_os << ", " #name " = "                                                                        \
	    << "(r = " << int(inst.name.r) << ", g = " << int(inst.name.g)                             \
	    << ", b = " << int(inst.name.b) << ')';

#define PRINT_ENUM1(name, nsp, value1)                                                             \
	if (inst.name == instructions::nsp::value1)                                                    \
		_os << ", " #name " = " #value1;
#define PRINT_ENUM2(name, nsp, value2, ...)                                                        \
	if (inst.name == instructions::nsp::value2)                                                    \
		_os << ", " #name " = " #value2;                                                           \
	EXPAND(PRINT_ENUM1(name, nsp, __VA_ARGS__))
#define PRINT_ENUM3(name, nsp, value3, ...)                                                        \
	if (inst.name == instructions::nsp::value3)                                                    \
		_os << ", " #name " = " #value3;                                                           \
	EXPAND(PRINT_ENUM2(name, nsp, __VA_ARGS__))
#define PRINT_ENUM4(name, nsp, value4, ...)                                                        \
	if (inst.name == instructions::nsp::value4)                                                    \
		_os << ", " #name " = " #value4;                                                           \
	EXPAND(PRINT_ENUM3(name, nsp, __VA_ARGS__))
#define PRINT_ENUM5(name, nsp, value5, ...)                                                        \
	if (inst.name == instructions::nsp::value5)                                                    \
		_os << ", " #name " = " #value5;                                                           \
	EXPAND(PRINT_ENUM4(name, nsp, __VA_ARGS__))
#define PRINT_ENUM6(name, nsp, value6, ...)                                                        \
	if (inst.name == instructions::nsp::value6)                                                    \
		_os << ", " #name " = " #value6;                                                           \
	EXPAND(PRINT_ENUM5(name, nsp, __VA_ARGS__))
#define PRINT_ENUM7(name, nsp, value7, ...)                                                        \
	if (inst.name == instructions::nsp::value7)                                                    \
		_os << ", " #name " = " #value7;                                                           \
	EXPAND(PRINT_ENUM6(name, nsp, __VA_ARGS__))
#define PRINT_ENUM8(name, nsp, value8, ...)                                                        \
	if (inst.name == instructions::nsp::value8)                                                    \
		_os << ", " #name " = " #value8;                                                           \
	EXPAND(PRINT_ENUM7(name, nsp, __VA_ARGS__))
#define PRINT_ENUM9(name, nsp, value9, ...)                                                        \
	if (inst.name == instructions::nsp::value9)                                                    \
		_os << ", " #name " = " #value9;                                                           \
	EXPAND(PRINT_ENUM8(name, nsp, __VA_ARGS__))
#define PRINT_ENUM10(name, nsp, value10, ...)                                                      \
	if (inst.name == instructions::nsp::value10)                                                   \
		_os << ", " #name " = " #value10;                                                          \
	EXPAND(PRINT_ENUM9(name, nsp, __VA_ARGS__))
#define PRINT_ENUM11(name, nsp, value11, ...)                                                      \
	if (inst.name == instructions::nsp::value11)                                                   \
		_os << ", " #name " = " #value11;                                                          \
	EXPAND(PRINT_ENUM10(name, nsp, __VA_ARGS__))
#define PRINT_ENUM12(name, nsp, value12, ...)                                                      \
	if (inst.name == instructions::nsp::value12)                                                   \
		_os << ", " #name " = " #value12;                                                          \
	EXPAND(PRINT_ENUM11(name, nsp, __VA_ARGS__))
#define PRINT_ENUM13(name, nsp, value13, ...)                                                      \
	if (inst.name == instructions::nsp::value13)                                                   \
		_os << ", " #name " = " #value13;                                                          \
	EXPAND(PRINT_ENUM12(name, nsp, __VA_ARGS__))
#define PRINT_ENUM14(name, nsp, value14, ...)                                                      \
	if (inst.name == instructions::nsp::value14)                                                   \
		_os << ", " #name " = " #value14;                                                          \
	EXPAND(PRINT_ENUM13(name, nsp, __VA_ARGS__))
#define PRINT_ENUM15(name, nsp, value15, ...)                                                      \
	if (inst.name == instructions::nsp::value15)                                                   \
		_os << ", " #name " = " #value15;                                                          \
	EXPAND(PRINT_ENUM14(name, nsp, __VA_ARGS__))
#define PRINT_ENUM16(name, nsp, value16, ...)                                                      \
	if (inst.name == instructions::nsp::value16)                                                   \
		_os << ", " #name " = " #value16;                                                          \
	EXPAND(PRINT_ENUM15(name, nsp, __VA_ARGS__))
#define PRINT_ENUM17(name, nsp, value17, ...)                                                      \
	if (inst.name == instructions::nsp::value17)                                                   \
		_os << ", " #name " = " #value17;                                                          \
	EXPAND(PRINT_ENUM16(name, nsp, __VA_ARGS__))
#define PRINT_ENUM18(name, nsp, value18, ...)                                                      \
	if (inst.name == instructions::nsp::value18)                                                   \
		_os << ", " #name " = " #value18;                                                          \
	EXPAND(PRINT_ENUM17(name, nsp, __VA_ARGS__))

namespace parsers {
namespace csv_rw_route {
	namespace {
		struct csv_rw_route_instruction_io_class {
			std::ostream& _os;

			explicit csv_rw_route_instruction_io_class(std::ostream& os) : _os(os){};

			template <class T>
			void print_vector(const std::vector<T>& vec) {
				std::size_t i = 0;
				_os << "[";
				for (auto& item : vec) {
					_os << item;
					if (++i != vec.size()) {
						_os << ", ";
					}
				}
				_os << "]";
			}

			template <class T>
			void start(const T& inst, const char* name) {
				_os << "(" << inst.absolute_position << ":" << name
				    << ", filename = " << inst.file_index << ", line = " << inst.line;
			}

			void end() {
				_os << ")";
			}

			void operator()(const instructions::naked::position& inst) {
				start(inst, "Position");

				PRINT_VECTOR(distances);

				end();
			}
			void operator()(const instructions::naked::None& inst) {
				start(inst, "None");

				end();
			}
			void operator()(const instructions::options::UnitOfLength& inst) {
				start(inst, "Options.UnitOfLength");

				PRINT_VECTOR(factors_in_meters);

				end();
			}
			void operator()(const instructions::options::UnitOfSpeed& inst) {
				start(inst, "Options.UnitOfSpeed");

				PRINT_VALUE(factor_in_kph);

				end();
			}
			void operator()(const instructions::options::BlockLength& inst) {
				start(inst, "Options.BlockLength");

				PRINT_VALUE(length);

				end();
			}
			void operator()(const instructions::options::ObjectVisibility& inst) {
				start(inst, "Options.ObjectVisibility");

				PRINT_ENUM2(mode, options::ObjectVisibility, Legacy, TrackBased);

				end();
			}
			void operator()(const instructions::options::SectionBehavior& inst) {
				start(inst, "Options.SectionBehavior");

				PRINT_ENUM2(mode, options::SectionBehavior, Default, Simplified);

				end();
			}
			void operator()(const instructions::options::CantBehavior& inst) {
				start(inst, "Options.CantBehavior");

				PRINT_ENUM2(mode, options::CantBehavior, Unsigned, Signed);

				end();
			}
			void operator()(const instructions::options::FogBehavior& inst) {
				start(inst, "Options.FogBehavior");

				PRINT_ENUM2(mode, options::FogBehavior, BlockBased, Interpolated);

				end();
			}
			void operator()(const instructions::options::CompatibleTransparencyMode& inst) {
				start(inst, "Options.CompatibleTransparencyMode");

				PRINT_ENUM2(mode, options::CompatibleTransparencyMode, Off, On);

				end();
			}
			void operator()(const instructions::options::EnableBveTsHacks& inst) {
				start(inst, "Options.EnableBveTsHacks");

				PRINT_ENUM2(mode, options::EnableBveTsHacks, Off, On);

				end();
			}
			void operator()(const instructions::route::Comment& inst) {
				start(inst, "Route.Comment");

				PRINT_VALUE(text);

				end();
			}
			void operator()(const instructions::route::Image& inst) {
				start(inst, "Route.Image");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::route::Timetable& inst) {
				start(inst, "Route.Timetable");

				PRINT_VALUE(text);

				end();
			}
			void operator()(const instructions::route::Change& inst) {
				start(inst, "Route.Change");
				PRINT_ENUM3(mode, route::Change, SaftyActiviatedServiceBrakes,
				            SaftyActiviatedEmergencyBrakes, SaftyDeactivatedEmergencyBrakes);

				end();
			}
			void operator()(const instructions::route::Guage& inst) {
				start(inst, "Route.Guage");

				PRINT_VALUE(guage);

				end();
			}
			void operator()(const instructions::route::Signal& inst) {
				start(inst, "Route.Signal");

				PRINT_VALUE(aspect_index);

				end();
			}
			void operator()(const instructions::route::RunInterval& inst) {
				start(inst, "Route.RunInterval");

				PRINT_VECTOR(time_interval);

				end();
			}
			void operator()(const instructions::route::AccelerationDueToGravity& inst) {
				start(inst, "Route.AccelerationDueToGravity");

				PRINT_VALUE(value);

				end();
			}
			void operator()(const instructions::route::Elevation& inst) {
				start(inst, "Route.Elevation");

				PRINT_VALUE(height);

				end();
			}
			void operator()(const instructions::route::Temperature& inst) {
				start(inst, "Route.Temperature");

				PRINT_VALUE(celcius);

				end();
			}
			void operator()(const instructions::route::Pressure& inst) {
				start(inst, "Route.Pressure");

				PRINT_VALUE(kPa);

				end();
			}
			void operator()(const instructions::route::DisplaySpeed& inst) {
				start(inst, "Route.DisplaySpeed");

				PRINT_VALUE(unit_string);
				PRINT_VALUE(conversion_factor);

				end();
			}
			void operator()(const instructions::route::LoadingScreen& inst) {
				start(inst, "Route.LoadingScreen");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::route::StartTime& inst) {
				start(inst, "Route.StartTime");

				PRINT_TIME(time);

				end();
			}
			void operator()(const instructions::route::DynamicLight& inst) {
				start(inst, "Route.DynamicLight");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::route::AmbiantLight& inst) {
				start(inst, "Route.AmbiantLight");

				PRINT_COLOR(color);

				end();
			}
			void operator()(const instructions::route::DirectionalLight& inst) {
				start(inst, "Route.DirectionalLight");

				PRINT_COLOR(color);

				end();
			}
			void operator()(const instructions::route::LightDirection& inst) {
				start(inst, "Route.LightDirection");

				PRINT_VALUE(theta);
				PRINT_VALUE(phi);

				end();
			}
			void operator()(const instructions::train::Folder& inst) {
				start(inst, "Train.Folder");

				PRINT_VALUE(filepath);

				end();
			}
			void operator()(const instructions::train::Rail& inst) {
				start(inst, "Train.Rail");

				PRINT_VALUE(rail_type_index);
				PRINT_VALUE(run_sound_index);

				end();
			}
			void operator()(const instructions::train::Flange& inst) {
				start(inst, "Train.Flange");

				PRINT_VALUE(rail_type_index);
				PRINT_VALUE(flange_sound_index);

				end();
			}
			void operator()(const instructions::train::Timetable& inst) {
				start(inst, "Train.Timetable");

				PRINT_VALUE(timetable_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::train::Velocity& inst) {
				start(inst, "Train.Velocity");

				PRINT_VALUE(speed);

				end();
			}
			void operator()(const instructions::structure::Command& inst) {
				start(inst, "Structure.Command");

				PRINT_ENUM18(command, structure::Command, Ground, Rail, WallL, WallR, DikeL, DikeR,
				             FormL, FormR, FormCL, FormCR, RoofL, RoofR, RoofCL, RoofCR, CrackL,
				             CrackR, FreeObj, Beacon);

				PRINT_VALUE(structure_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::structure::Pole& inst) {
				start(inst, "Structure.Pole");

				PRINT_VALUE(additional_rails);
				PRINT_VALUE(pole_structure_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::texture::Background_Load& inst) {
				start(inst, "Texture.Background_Load");

				PRINT_VALUE(background_texture_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::texture::Background_X& inst) {
				start(inst, "Texture.Background_X");

				PRINT_VALUE(background_texture_index);
				PRINT_VALUE(repetition_count);

				end();
			}
			void operator()(const instructions::texture::Background_Aspect& inst) {
				start(inst, "Texture.Background_Aspect");

				PRINT_VALUE(background_texture_index);
				PRINT_ENUM2(mode, texture::Background_Aspect, Fixed, Aspect);

				end();
			}
			void operator()(const instructions::cycle::Ground& inst) {
				start(inst, "Cycle.Ground");

				PRINT_VALUE(cycle_structure_index);
				PRINT_VECTOR(input_indices);

				end();
			}
			void operator()(const instructions::cycle::Rail& inst) {
				start(inst, "Cycle.Rail");

				PRINT_VALUE(cycle_structure_index);
				PRINT_VECTOR(input_indices);

				end();
			}
			void operator()(const instructions::naked::SignalAnimated& inst) {
				start(inst, "SignalAnimated");

				PRINT_VALUE(signal_index);
				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::naked::Signal& inst) {
				start(inst, "Signal");

				PRINT_VALUE(signal_index);
				PRINT_VALUE(signal_filename);
				PRINT_VALUE(glow_filename);

				end();
			}
			void operator()(const instructions::track::RailStart& inst) {
				start(inst, "Track.RailStart");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(rail_type);

				end();
			}
			void operator()(const instructions::track::Rail& inst) {
				start(inst, "Track.Rail");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(rail_type);

				end();
			}
			void operator()(const instructions::track::RailType& inst) {
				start(inst, "Track.RailType");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(rail_type);

				end();
			}
			void operator()(const instructions::track::RailEnd& inst) {
				start(inst, "Track.RailEnd");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);

				end();
			}
			void operator()(const instructions::track::Adhesion& inst) {
				start(inst, "Track.Adhesion");

				PRINT_VALUE(value);

				end();
			}
			void operator()(const instructions::track::Pitch& inst) {
				start(inst, "Track.Pitch");

				PRINT_VALUE(rate);

				end();
			}
			void operator()(const instructions::track::Curve& inst) {
				start(inst, "Track.Curve");

				PRINT_VALUE(radius);
				PRINT_VALUE(cant);

				end();
			}
			void operator()(const instructions::track::Turn& inst) {
				start(inst, "Track.Turn");

				PRINT_VALUE(ratio);

				end();
			}
			void operator()(const instructions::track::Height& inst) {
				start(inst, "Track.Height");

				PRINT_VALUE(y);

				end();
			}
			void operator()(const instructions::track::FreeObj& inst) {
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
			void operator()(const instructions::track::Wall& inst) {
				start(inst, "Track.Wall");

				PRINT_VALUE(rail_index);
				PRINT_ENUM3(direction, track::Wall, Left, Both, Right);
				PRINT_VALUE(wall_structure_index);

				end();
			}
			void operator()(const instructions::track::WallEnd& inst) {
				start(inst, "Track.WallEnd");

				PRINT_VALUE(rail_index);

				end();
			}
			void operator()(const instructions::track::Dike& inst) {
				start(inst, "Track.Dike");

				PRINT_VALUE(rail_index);
				PRINT_ENUM3(direction, track::Dike, Left, Both, Right);
				PRINT_VALUE(dike_structure_index);

				end();
			}
			void operator()(const instructions::track::DikeEnd& inst) {
				start(inst, "Track.DikeEnd");

				PRINT_VALUE(rail_index);

				end();
			}
			void operator()(const instructions::track::Pole& inst) {
				start(inst, "Track.Pole");

				PRINT_VALUE(rail_index);
				PRINT_VALUE(additional_rails);
				PRINT_VALUE(location);
				PRINT_VALUE(interval);
				PRINT_VALUE(pole_structure_index);

				end();
			}
			void operator()(const instructions::track::PoleEnd& inst) {
				start(inst, "Track.PoleEnd");

				PRINT_VALUE(rail_index);

				end();
			}
			void operator()(const instructions::track::Crack& inst) {
				start(inst, "Track.Crack");

				PRINT_VALUE(rail_index_1);
				PRINT_VALUE(rail_index_2);
				PRINT_VALUE(crack_structure_index);

				end();
			}
			void operator()(const instructions::track::Ground& inst) {
				start(inst, "Track.Ground");

				PRINT_VALUE(ground_structure_index);

				end();
			}
			void operator()(const instructions::track::Sta& inst) {
				start(inst, "Track.Sta");

				PRINT_VALUE(name);
				PRINT_TIME(arrival);
				PRINT_ENUM6(arrival_tag, track::Sta::ArrivalTime_t, Time, AnyTime, AllPass,
				            PlayerPass, PlayerStop, AllStop);
				PRINT_TIME(departure);
				PRINT_ENUM6(departure_tag, track::Sta::DepartureTime_t, Time, AnyTime, Terminal,
				            TerminalTime, ChangeEnds, ChangeEndsTime);
				PRINT_VALUE(pass_alarm);
				PRINT_ENUM4(doors, track::Sta::Doors_t, Left, None, Right, Both);
				PRINT_VALUE(force_red);
				PRINT_VALUE(system);
				PRINT_VALUE(arrival_sound);
				PRINT_VALUE(stop_duration);
				PRINT_VALUE(passenger_ratio);
				PRINT_VALUE(departure_sound);
				PRINT_VALUE(timetable_index);

				end();
			}
			void operator()(const instructions::track::Stop& inst) {
				start(inst, "Track.Stop");

				PRINT_ENUM3(stop_post, track::Stop::Stop_Post_t, Left, None, Right);
				PRINT_VALUE(forwards_tolerance);
				PRINT_VALUE(backwards_tolerance);
				PRINT_VALUE(cars);

				end();
			}
			void operator()(const instructions::track::Form& inst) {
				start(inst, "Track.Form");

				PRINT_VALUE(rail_index_1);
				PRINT_VALUE(rail_index_2);
				PRINT_VALUE(roof_structure_index);
				PRINT_VALUE(form_structure_index);

				end();
			}
			void operator()(const instructions::track::Limit& inst) {
				start(inst, "Track.Limit");

				PRINT_VALUE(speed);
				PRINT_ENUM3(post, track::Limit::Post_t, Left, None, Right);
				PRINT_ENUM3(cource, track::Limit::Cource_t, Left, None, Right);

				end();
			}
			void operator()(const instructions::track::Section& inst) {
				start(inst, "Track.Section");

				PRINT_VECTOR(a_term);

				end();
			}
			void operator()(const instructions::track::SigF& inst) {
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
			void operator()(const instructions::track::Signal& inst) {
				start(inst, "Track.Signal");

				PRINT_ENUM8(type, track::Signal, R_Y, R_G, R_Y_G, R_YY_Y_G, R_Y_YG_G, R_YY_Y_YG_G,
				            R_Y_YG_G_GG, R_YY_Y_YG_G_GG);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::Relay& inst) {
				start(inst, "Track.Relay");

				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::Beacon& inst) {
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
			void operator()(const instructions::track::Transponder& inst) {
				start(inst, "Track.Transponder");

				PRINT_ENUM5(type, track::Transponder, S_type, SN_type, Departure, ATS_P_RENEWAL,
				            ATS_P_STOP);
				PRINT_VALUE(signal);
				PRINT_VALUE(switch_system);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);
				PRINT_VALUE(yaw);
				PRINT_VALUE(pitch);
				PRINT_VALUE(roll);

				end();
			}
			void operator()(const instructions::track::Pattern& inst) {
				start(inst, "Track.Pattern");

				PRINT_ENUM2(type, track::Pattern, Temporary, Permanent);
				PRINT_VALUE(speed);

				end();
			}
			void operator()(const instructions::track::Back& inst) {
				start(inst, "Track.Back");

				PRINT_VALUE(background_texture_index);

				end();
			}
			void operator()(const instructions::track::Fog& inst) {
				start(inst, "Track.Fog");

				PRINT_VALUE(starting_distance);
				PRINT_VALUE(ending_distance);
				PRINT_COLOR(color);

				end();
			}
			void operator()(const instructions::track::Brightness& inst) {
				start(inst, "Track.Brightness");

				PRINT_BYTE(value);

				end();
			}
			void operator()(const instructions::track::Marker& inst) {
				start(inst, "Track.Marker");

				PRINT_VALUE(filename);
				PRINT_VALUE(distance);

				end();
			}
			void operator()(const instructions::track::MarkerXML& inst) {
				start(inst, "Track.MarkerXML");

				PRINT_VALUE(filename);

				end();
			}
			void operator()(const instructions::track::TextMarker& inst) {
				start(inst, "Track.TextMarker");

				PRINT_VALUE(text);
				PRINT_VALUE(distance);
				PRINT_ENUM8(font_color, track::TextMarker, Black, Gray, White, Red, Orange, Green,
				            Blue, Magenta);

				end();
			}
			void operator()(const instructions::track::PointOfInterest& inst) {
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
			void operator()(const instructions::track::PreTrain& inst) {
				start(inst, "Track.PreTrain");

				PRINT_TIME(time)

				end();
			}
			void operator()(const instructions::track::Announce& inst) {
				start(inst, "Track.Announce");

				PRINT_VALUE(filename);
				PRINT_VALUE(speed);

				end();
			}
			void operator()(const instructions::track::Doppler& inst) {
				start(inst, "Track.Doppler");

				PRINT_VALUE(filename);
				PRINT_VALUE(x_offset);
				PRINT_VALUE(y_offset);

				end();
			}
			void operator()(const instructions::track::Buffer& inst) {
				start(inst, "Track.Buffer");

				end();
			}
		};
	} // namespace
} // namespace csv_rw_route
} // namespace parsers

std::ostream& operator<<(std::ostream& os, const parsers::csv_rw_route::instruction& i) {
	os << std::boolalpha;

	parsers::csv_rw_route::csv_rw_route_instruction_io_class crriic(os);

	mapbox::util::apply_visitor(crriic, i);

	return os;
}

std::ostream& operator<<(std::ostream& os, const parsers::csv_rw_route::instruction_list& list) {
	os << "Files:\n";
	for (std::size_t i = 0; i < list.filenames.size(); ++i) {
		os << i << ": " << list.filenames[i] << '\n';
	}
	os << "Instructions:\n";
	for (auto& i : list.instructions) {
		::operator<<(os, i) << "\n";
	}
	return os;
}
