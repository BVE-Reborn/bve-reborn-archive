#pragma once

// Header is to only be included in csv_rw_route.hpp

#include "core/datatypes.hpp"
#include <string>
#include <vector>

namespace instructions {
namespace naked {
	struct position {
		// UnitOfLength
		std::vector<float> distances;
	};

	// for ignored instructions
	struct None {};
} // namespace naked

namespace options {
	struct UnitOfLength {
		std::vector<float> factors_in_meters;
	};

	struct UnitOfSpeed {
		float factor_in_kph;
	};

	struct BlockLength {
		// UnitOfLength
		float length;
	};

	struct ObjectVisibility {
		enum { Legacy, TrackBased } mode = Legacy;
	};

	struct SectionBehavior {
		enum { Default, Simplified } mode = Default;
	};

	struct CantBehavior {
		enum { Unsigned, Signed } mode = Unsigned;
	};

	struct FogBehavior {
		enum { BlockBased, Interpolated } mode = BlockBased;
	};

	struct CompatibleTransparencyMode {
		enum { Off, On } mode = Off;
	};

	struct EnableBveTsHacks {
		enum { Off, On } mode = Off;
	};
} // namespace options

namespace route {
	struct Comment {
		std::string text;
	};

	struct Image {
		std::string filename;
	};

	struct Timetable {
		std::string text;
	};

	struct Change {
		enum {
			SaftyActiviatedServiceBrakes,
			SaftyActiviatedEmergencyBrakes,
			SaftyDeactivatedEmergencyBrakes
		} mode = SaftyActiviatedServiceBrakes;
	};

	struct Guage {
		// millimeters
		float guage = 1435;
	};

	struct Signal {
		std::size_t aspect_index;
		// UnitOfSpeed
		float speed;
	};

	struct RunInterval {
		// seconds
		std::vector<float> time_interval;
	};

	struct AccelerationDueToGravity {
		// m / s^2
		float value;
	};

	struct Elevation {
		// UnitOfLength
		float height = 0;
	};

	struct Temperature {
		// celcius
		float celcius;
	};

	struct Pressure {
		// kPa
		float kPa = 101.325f;
	};

	struct DisplaySpeed {
		std::string unit_string;
		// relative to kph
		float conversion_factor;
	};

	struct LoadingScreen {
		std::string filename;
	};

	struct StartTime {
		openbve2::datatypes::time time;
	};

	struct DynamicLight {
		std::string filename;
	};

	struct AmbiantLight {
		openbve2::datatypes::color8_rgb color = {160, 160, 160};
	};

	struct DirectionalLight {
		openbve2::datatypes::color8_rgb color = {160, 160, 160};
	};

	struct LightDirection {
		// degrees
		float theta = 60.0f;
		// degrees
		float phi = -26.57f;
	};
} // namespace route

namespace train {
	// Train.Folder
	// Train.File
	struct Folder {
		std::string filepath;
	};

	// Train.Run
	// Train.Rail
	struct Rail {
		std::size_t rail_type_index;
		std::size_t run_sound_index;
	};

	struct Flange {
		std::size_t rail_type_index;
		std::size_t flange_sound_index;
	};

	struct Timetable {
		bool day;
		std::size_t timetable_index;
		std::string filename;
	};

	using Guage = route::Guage;

	using Interval = route::RunInterval;

	struct Velocity {
		// UnitOfSpeed
		float speed;
	};
} // namespace train

namespace structure {
	struct Command {
		enum : uint8_t {
			Ground,
			Rail,
			WallL,
			WallR,
			DikeL,
			DikeR,
			FormL,
			FormR,
			FormCL,
			FormCR,
			RoofL,
			RoofR,
			RoofCL,
			RoofCR,
			CrackL,
			CrackR,
			FreeObj,
			Beacon
		} command;

		std::size_t structure_index;

		std::string filename;
	};

	struct Pole {
		std::size_t additional_rails;
		std::size_t pole_structure_index;
		std::string filename;
	};
} // namespace structure

namespace texture {
	struct Background_Load {
		std::size_t background_texture_index;
		std::string filename;
	};
	struct Background_X {
		std::size_t background_texture_index;
		std::size_t repetition_count;
	};

	struct Background_Aspect {
		std::size_t background_texture_index;
		enum : uint8_t { Fixed, Aspect } mode = Fixed;
	};
} // namespace texture

namespace cycle {
	struct Ground {
		std::size_t ground_structure_index;
		std::vector<std::size_t> input_indices;
	};

	struct Rail {
		std::size_t rail_structure_index;
		std::vector<std::size_t> input_indices;
	};
} // namespace cycle

namespace naked {
	struct SignalAnimated {
		std::size_t signal_index;
		std::string filename;
	};

	struct Signal {
		std::size_t signal_index;
		// Without file extant
		std::string signal_filename;
		// Without file extant
		std::string glow_filename;
	};
} // namespace naked

namespace track {
	struct RailStart {
		std::size_t rail_index;
		// UnitOfLength
		float x_offset;
		// UnitOfLength
		float y_offset;
		std::size_t rail_type;
	};

	struct Rail {
		std::size_t rail_index;
		// UnitOfLength
		float x_offset;
		// UnitOfLength
		float y_offset;
		std::size_t rail_type;
	};

	struct RailType {
		std::size_t rail_index;
		std::size_t rail_type;
	};

	struct RailEnd {
		std::size_t rail_index;
		// UnitOfLength
		float x_offset;
		// UnitOfLength
		float y_offset;
	};

	struct Adhesion {
		float value = 100;
	};

	struct Pitch {
		// Per thousands
		float rate = 0;
	};

	struct Curve {
		// UnitOfLength
		float radius = 0;
		// Millimeters
		float cant = 0;
	};

	struct Turn {
		float ratio = 0;
	};

	struct Height {
		// UnitOfLength
		float y;
	};

	struct FreeObj {
		std::size_t rail_index = 0;
		std::size_t free_obj_structure_index = 0;
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
		// degrees
		float yaw = 0;
		// degrees
		float pitch = 0;
		// degrees
		float roll = 0;
	};

	struct Wall {
		std::size_t rail_index;
		enum : uint8_t { Left, Both, Right } direction = Both;
		std::size_t wall_structure_index;
	};

	struct WallEnd {
		std::size_t rail_index;
	};

	struct Dike {
		std::size_t rail_index;
		enum : uint8_t { Left, Both, Right } direction = Both;
		std::size_t dike_structure_index;
	};

	struct DikeEnd {
		std::size_t rail_index;
	};

	struct Pole {
		std::size_t rail_index = 0;
		std::size_t additional_rails = 0;
		std::intmax_t location = 0;
		// UnitOfLength
		std::intmax_t interval;
		std::size_t pole_structure_index;
	};

	struct PoleEnd {
		std::size_t rail_index;
	};

	struct Crack {
		std::size_t rail_index_1;
		std::size_t rail_index_2;
		std::size_t crack_structure_index;
	};

	struct Ground {
		std::size_t ground_structure_index;
	};

	struct Sta {
		std::string name;
		std::string arrival_sound;
		std::string departure_sound;
		std::size_t timetable_index;
		openbve2::datatypes::time arrival;
		openbve2::datatypes::time departure;
		float stop_duration;
		float passenger_ratio;
		bool pass_alarm = false;
		bool force_red = false;
		bool system = false;
		enum class ArrivalTime_t : uint8_t {
			Time,
			AnyTime,
			AllPass,
			PlayerPass,
			PlayerStop,
			AllStop
		} arrival_tag = ArrivalTime_t::Time;
		enum class DepartureTime_t : uint8_t {
			Time,
			AnyTime,
			Terminal,
			TerminalTime,
			ChangeEnds,
			ChangeEndsTime
		} departure_tag = DepartureTime_t::Time;
		enum class Doors_t : uint8_t { Left, None, Right, Both } doors = Doors_t::None;
	};

	struct Stop {
		enum {
			Left,
			None,
			Right,
		} stop_post = None;

		float forwards_tolerance = 5;
		float backwards_tolerance = 5;
		std::size_t cars;
	};

	struct Form {
		std::size_t rail_index_1;
		std::size_t rail_index_2;
		std::size_t roof_structure_index;
		std::size_t form_structure_index;
	};

	struct Limit {
		// Unit of speed
		float speed = 0;
		enum class Post_t { Left, None, Right } post = Post_t::None;
		enum class Cource_t { Left, None, Right } cource = Cource_t::None;
	};

	struct Section {
		std::vector<std::size_t> a_term;
	};

	struct SigF {
		std::size_t signal_index;
		std::size_t section;
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
		// Degrees
		float yaw = 0;
		// Degrees
		float pitch = 0;
		// Degrees
		float roll = 0;
	};

	// Track.Signal
	// Track.Sig
	struct Signal {
		enum : uint8_t { R_Y, R_G, R_Y_G, R_YY_Y_G, R_Y_YG_G, R_YY_Y_YG_G, R_Y_YG_G_GG, R_YY_Y_YG_G_GG } type = R_G;
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
		// Degrees
		float yaw = 0;
		// Degrees
		float pitch = 0;
		// Degrees
		float roll = 0;
	};

	struct Relay {
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
		// Degrees
		float yaw = 0;
		// Degrees
		float pitch = 0;
		// Degrees
		float roll = 0;
	};

	struct Beacon {
		std::size_t type;
		std::size_t beacon_structure_index;
		std::intmax_t section;
		std::intmax_t data;
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
		// Degrees
		float yaw = 0;
		// Degrees
		float pitch = 0;
		// Degrees
		float roll = 0;
	};

	// Track.Transponder
	// Track.Tr
	// Track.AtsSn
	// Track.AtsP
	struct Transponder {
		enum : uint8_t { S_type, SN_type, Departure, ATS_P_RENEWAL, ATS_P_STOP } type = S_type;
		std::size_t signal;
		bool switch_system;
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
		// Degrees
		float yaw = 0;
		// Degrees
		float pitch = 0;
		// Degrees
		float roll = 0;
	};

	struct Pattern {
		enum : bool { Temporary, Permanent } type = Temporary;
		// UnitOfSpeed
		float speed;
	};

	struct Back {
		std::size_t background_texture_index;
	};

	struct Fog {
		// UnitOfLength
		float starting_distance;
		// UnitOfLength
		float ending_distance;
		openbve2::datatypes::color8_rgb color = {128, 128, 128};
	};

	struct Brightness {
		std::uint8_t value;
	};

	struct Marker {
		std::string filename;
		// UnitOfLength
		float distance;
	};

	struct MarkerXML {
		std::string filename;
	};

	struct TextMarker {
		std::string text;
		// UnitOfLength
		float distance;
		enum : uint8_t { Black, Gray, White, Red, Orange, Green, Blue, Magenta } font_color;
	};

	struct PointOfInterest {
		std::size_t rail_index;
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
		// Degrees
		float yaw = 0;
		// Degrees
		float pitch = 0;
		// Degrees
		float roll = 0;
		std::string text;
	};

	struct PreTrain {
		openbve2::datatypes::time time;
	};

	struct Announce {
		std::string filename;
		// UnitOfSpeed
		float speed;
	};

	struct Doppler {
		std::string filename;
		// UnitOfLength
		float x_offset = 0;
		// UnitOfLength
		float y_offset = 0;
	};

	struct Buffer {};
} // namespace track
} // namespace instructions
