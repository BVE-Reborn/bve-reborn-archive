#pragma once

#include "core/datatypes.hpp"
#include "route_structure.hpp"
#include <absl/types/optional.h>
#include <cstddef>
#include <string>
#include <vector>

namespace bve::parsers::csv_rw_route::instructions {

	struct InstructionBase {
		std::size_t file_index = 0;
		std::size_t line = 0;
		float absolute_position = -1;
	};

	namespace naked {
		struct Position : InstructionBase {
			// UnitOfLength
			std::vector<float> distances;
		};

		// for ignored instructions
		struct None : InstructionBase {};
	} // namespace naked

	namespace options {
		struct UnitOfLength : InstructionBase {
			std::vector<float> factors_in_meters;
		};

		struct UnitOfSpeed {
			float factor_in_kph;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct BlockLength {
			// UnitOfLength
			float length;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct ObjectVisibility : InstructionBase {
			enum class Mode { legacy, track_based } mode = Mode::legacy;
		};

		struct SectionBehavior : InstructionBase {
			enum class Mode { normal, simplified } mode = Mode::normal;
		};

		struct CantBehavior : InstructionBase {
			enum class Mode { unsigned_cant, signed_cant } mode = Mode::unsigned_cant;
		};

		struct FogBehavior : InstructionBase {
			enum class Mode { block_based, interpolated } mode = Mode::block_based;
		};

		struct CompatibleTransparencyMode : InstructionBase {
			enum class Mode { off, on } mode = Mode::off;
		};

		struct EnableBveTsHacks : InstructionBase {
			enum class Mode { off, on } mode = Mode::off;
		};
	} // namespace options

	namespace route {
		struct Comment {
			std::string text;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Image {
			std::string filename;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Timetable {
			std::string text;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Change : InstructionBase {
			enum class Mode {
				safety_activated_service_brakes,
				safety_activated_emergency_brakes,
				safety_deactivated_emergency_brakes
			} mode = Mode::safety_activated_service_brakes;
		};

		struct Gauge {
			// millimeters
			float width = 1435;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Signal : InstructionBase {
			std::size_t aspect_index;
			// UnitOfSpeed
			float speed;
			float absolute_position = -1;
		};

		struct RunInterval : InstructionBase {
			// seconds
			std::vector<float> time_interval;
			float absolute_position = -1;
		};

		struct AccelerationDueToGravity {
			// m / s^2
			float value;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Elevation {
			// UnitOfLength
			float height = 0;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Temperature {
			// celsius
			float celsius;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Pressure {
			// kpa
			float kpa = 101.325f;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct DisplaySpeed : InstructionBase {
			std::string unit_string;
			// relative to kph
			float conversion_factor;
		};

		struct LoadingScreen {
			std::string filename;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct StartTime {
			core::datatypes::Time time;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct DynamicLight {
			std::string filename;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct AmbientLight : InstructionBase {
			core::datatypes::Color8RGB color = {160, 160, 160};
		};

		struct DirectionalLight : InstructionBase {
			core::datatypes::Color8RGB color = {160, 160, 160};
		};

		struct LightDirection : InstructionBase {
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
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		// Train.Run
		// Train.Rail
		struct Rail : InstructionBase {
			std::size_t rail_type_index;
			std::size_t run_sound_index;
		};

		struct Flange : InstructionBase {
			std::size_t rail_type_index;
			std::size_t flange_sound_index;
		};

		struct Timetable : InstructionBase {
			bool day;
			std::size_t timetable_index;
			std::string filename;
		};

		using Gauge = route::Gauge;

		using Interval = route::RunInterval;

		struct Velocity {
			// UnitOfSpeed
			float speed;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};
	} // namespace train

	namespace structure {
		struct Command : InstructionBase {
			enum class Type : uint8_t {
				ground,
				rail,
				wall_l,
				wall_r,
				dike_l,
				dike_r,
				form_l,
				form_r,
				form_cl,
				form_cr,
				roof_l,
				roof_r,
				roof_cl,
				roof_cr,
				crack_l,
				crack_r,
				free_obj,
				beacon
			} command_type;

			std::size_t structure_index;

			std::string filename;
		};

		struct Pole : InstructionBase {
			std::size_t additional_rails;
			std::size_t pole_structure_index;
			std::string filename;
		};
	} // namespace structure

	namespace texture {
		struct BackgroundLoad : InstructionBase {
			std::size_t background_texture_index;
			std::string filename;
		};

		struct BackgroundX : InstructionBase {
			std::size_t background_texture_index;
			std::size_t repetition_count;
		};

		struct BackgroundAspect : InstructionBase {
			std::size_t background_texture_index;
			enum class Mode : uint8_t { fixed, aspect } mode = Mode::fixed;
		};
	} // namespace texture

	namespace cycle {
		struct Ground : InstructionBase {
			std::size_t cycle_structure_index;
			std::vector<std::size_t> input_indices;
		};

		struct Rail : InstructionBase {
			std::size_t cycle_structure_index;
			std::vector<std::size_t> input_indices;
		};
	} // namespace cycle

	namespace naked {
		struct SignalAnimated : InstructionBase {
			std::size_t signal_index;
			std::string filename;
		};

		struct Signal : InstructionBase {
			std::size_t signal_index;
			// Without file extant
			std::string signal_filename;
			// Without file extant
			std::string glow_filename;
		};
	} // namespace naked

	namespace track {
		struct RailStart : InstructionBase {
			std::size_t rail_index;
			// UnitOfLength
			absl::optional<float> x_offset;
			// UnitOfLength
			absl::optional<float> y_offset;
			absl::optional<std::size_t> rail_type;
		};

		struct Rail : InstructionBase {
			std::size_t rail_index;
			// UnitOfLength
			absl::optional<float> x_offset;
			// UnitOfLength
			absl::optional<float> y_offset;
			absl::optional<std::size_t> rail_type;
		};

		struct RailType : InstructionBase {
			std::size_t rail_index = 0;
			std::size_t rail_type_number = 0;
		};

		struct RailEnd : InstructionBase {
			std::size_t rail_index;
			// UnitOfLength
			absl::optional<float> x_offset;
			// UnitOfLength
			absl::optional<float> y_offset;
		};

		struct Adhesion {
			float value = 100;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Pitch {
			// Per thousands
			float rate = 0;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Curve : InstructionBase {
			// UnitOfLength
			float radius = 0;
			// Millimeters
			float cant = 0;
		};

		struct Turn {
			float ratio = 0;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Height {
			// UnitOfLength
			float y;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct FreeObj : InstructionBase {
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

		struct Wall : InstructionBase {
			std::size_t rail_index;
			enum class Direction : uint8_t { left, both, right } direction = Direction::both;
			std::size_t wall_structure_index = 0;
		};

		struct WallEnd {
			std::size_t rail_index;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Dike : InstructionBase {
			std::size_t rail_index;
			enum class Direction : uint8_t { left, both, right } direction = Direction::both;
			std::size_t dike_structure_index = 0;
		};

		struct DikeEnd {
			std::size_t rail_index;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Pole : InstructionBase {
			std::size_t rail_index = 0;
			std::size_t additional_rails = 0;
			std::intmax_t location = 0;
			// UnitOfLength
			std::intmax_t interval = 1;
			std::size_t pole_structure_index = 0;
		};

		struct PoleEnd {
			std::size_t rail_index;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Crack : InstructionBase {
			std::size_t rail_index_1;
			std::size_t rail_index_2;
			std::size_t crack_structure_index = 0;
		};

		struct Ground {
			std::size_t ground_structure_index;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Sta : InstructionBase {
			std::string name;
			std::string arrival_sound;
			std::string departure_sound;
			std::size_t timetable_index = 0;
			core::datatypes::Time arrival = 0;
			core::datatypes::Time departure = 0;
			float stop_duration = 15;
			float passenger_ratio = 100;
			bool pass_alarm = false;
			bool force_red = false;
			bool system = false;
			using ArrivalTime = RailStation::ArrivalTime;
			ArrivalTime arrival_tag = ArrivalTime::any_time;
			using DepartureTime = RailStation::DepartureTime;
			DepartureTime departure_tag = DepartureTime::any_time;
			using Doors = RailStation::Doors;
			Doors doors = Doors::none;
		};

		struct StationXml : InstructionBase {
			std::string filename;
		};

		struct Stop : InstructionBase {
			using StopPost = Direction;
			StopPost stop_post = StopPost::none;

			float forwards_tolerance = 5;
			float backwards_tolerance = 5;
			std::size_t cars = 0;
		};

		struct Form : InstructionBase {
			std::size_t rail_index_1;
			std::size_t rail_index_2 = 0;
			std::size_t roof_structure_index = 0;
			std::size_t form_structure_index = 0;
			enum class Placement { left, right, rail_index } placement = Placement::rail_index;
		};

		struct Limit : InstructionBase {
			// Unit of speed
			float speed = 0;
			enum class Post { left, none, right } post = Post::none;
			enum class Course { left, none, right } course = Course::none;
		};

		struct Section : InstructionBase {
			std::vector<std::size_t> a_term;
		};

		struct SigF : InstructionBase {
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
		struct Signal : InstructionBase {
			enum class Aspect : uint8_t {
				r_y,
				r_g,
				r_y_g,
				r_yy_y_g,
				r_y_yg_g,
				r_yy_y_yg_g,
				r_y_yg_g_gg,
				r_yy_y_yg_g_gg
			} type = Aspect::r_g;
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

		struct Relay : InstructionBase {
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

		struct Beacon : InstructionBase {
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
		struct Transponder : InstructionBase {
			enum class Type : uint8_t {
				s_type = 0,
				sn_type = 1,
				departure = 2,
				ats_p_renewal = 3,
				ats_p_stop = 4
			} type = Type::s_type;
			std::size_t signal = 0;
			bool switch_system = true;
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

		struct Pattern : InstructionBase {
			enum class Type : bool { temporary, permanent } type = Type::temporary;
			// UnitOfSpeed
			float speed;
		};

		struct Back {
			std::size_t background_texture_index;
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		struct Fog : InstructionBase {
			// UnitOfLength
			float starting_distance = 0;
			// UnitOfLength
			float ending_distance = 0;
			core::datatypes::Color8RGB color = {128, 128, 128};
		};

		struct Brightness : InstructionBase {
			std::uint8_t value = 255;
		};

		struct Marker : InstructionBase {
			std::string filename;
			// UnitOfLength
			float distance;
		};

		struct MarkerXML : InstructionBase {
			std::string filename;
		};

		struct TextMarker : InstructionBase {
			std::string text;
			// UnitOfLength
			float distance;
			enum class FontColor : uint8_t {
				black,
				gray,
				white,
				red,
				orange,
				green,
				blue,
				magenta
			} font_color;
		};

		struct PointOfInterest : InstructionBase {
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

		struct PreTrain : InstructionBase {
			core::datatypes::Time time;
		};

		struct Announce : InstructionBase {
			std::string filename;
			// UnitOfSpeed
			float speed = 0;
		};

		struct Doppler : InstructionBase {
			std::string filename;
			// UnitOfLength
			float x_offset = 0;
			// UnitOfLength
			float y_offset = 0;
		};

		struct Buffer : InstructionBase {};
	} // namespace track
} // namespace bve::parsers::csv_rw_route::instructions
