#pragma once

#include "boost/optional.hpp"
#include "core/datatypes.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace parsers {
namespace csv_rw_route {
	namespace instructions {

		struct instruction_base {
			std::size_t file_index = 0;
			std::size_t line = 0;
			float absolute_position = -1;
		};

		namespace naked {
			struct position : public instruction_base {
				// UnitOfLength
				std::vector<float> distances;
			};

			// for ignored instructions
			struct None : public instruction_base {};
		} // namespace naked

		namespace options {
			struct UnitOfLength : public instruction_base {
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

			struct ObjectVisibility : public instruction_base {
				enum { Legacy, TrackBased } mode = Legacy;
			};

			struct SectionBehavior : public instruction_base {
				enum { Default, Simplified } mode = Default;
			};

			struct CantBehavior : public instruction_base {
				enum { Unsigned, Signed } mode = Unsigned;
			};

			struct FogBehavior : public instruction_base {
				enum { BlockBased, Interpolated } mode = BlockBased;
			};

			struct CompatibleTransparencyMode : public instruction_base {
				enum { Off, On } mode = Off;
			};

			struct EnableBveTsHacks : public instruction_base {
				enum { Off, On } mode = Off;
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

			struct Change : public instruction_base {
				enum {
					SaftyActiviatedServiceBrakes,
					SaftyActiviatedEmergencyBrakes,
					SaftyDeactivatedEmergencyBrakes
				} mode = SaftyActiviatedServiceBrakes;
			};

			struct Guage {
				// millimeters
				float guage = 1435;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct Signal : public instruction_base {
				std::size_t aspect_index;
				// UnitOfSpeed
				float speed;
				float absolute_position = -1;
			};

			struct RunInterval : public instruction_base {
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
				// celcius
				float celcius;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct Pressure {
				// kPa
				float kPa = 101.325f;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct DisplaySpeed : public instruction_base {
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
				openbve2::datatypes::time time;
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

			struct AmbiantLight : public instruction_base {
				openbve2::datatypes::color8_rgb color = {160, 160, 160};
			};

			struct DirectionalLight : public instruction_base {
				openbve2::datatypes::color8_rgb color = {160, 160, 160};
			};

			struct LightDirection : public instruction_base {
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
			struct Rail : public instruction_base {
				std::size_t rail_type_index;
				std::size_t run_sound_index;
			};

			struct Flange : public instruction_base {
				std::size_t rail_type_index;
				std::size_t flange_sound_index;
			};

			struct Timetable : public instruction_base {
				bool day;
				std::size_t timetable_index;
				std::string filename;
			};

			using Guage = route::Guage;

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
			struct Command : public instruction_base {
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

			struct Pole : public instruction_base {
				std::size_t additional_rails;
				std::size_t pole_structure_index;
				std::string filename;
			};
		} // namespace structure

		namespace texture {
			struct Background_Load : public instruction_base {
				std::size_t background_texture_index;
				std::string filename;
			};
			struct Background_X : public instruction_base {
				std::size_t background_texture_index;
				std::size_t repetition_count;
			};

			struct Background_Aspect : public instruction_base {
				std::size_t background_texture_index;
				enum : uint8_t { Fixed, Aspect } mode = Fixed;
			};
		} // namespace texture

		namespace cycle {
			struct Ground : public instruction_base {
				std::size_t ground_structure_index;
				std::vector<std::size_t> input_indices;
			};

			struct Rail : public instruction_base {
				std::size_t rail_structure_index;
				std::vector<std::size_t> input_indices;
			};
		} // namespace cycle

		namespace naked {
			struct SignalAnimated : public instruction_base {
				std::size_t signal_index;
				std::string filename;
			};

			struct Signal : public instruction_base {
				std::size_t signal_index;
				// Without file extant
				std::string signal_filename;
				// Without file extant
				std::string glow_filename;
			};
		} // namespace naked

		namespace track {
			struct RailStart : public instruction_base {
				std::size_t rail_index;
				// UnitOfLength
				boost::optional<float> x_offset;
				// UnitOfLength
				boost::optional<float> y_offset;
				boost::optional<std::size_t> rail_type;
			};

			struct Rail : public instruction_base {
				std::size_t rail_index;
				// UnitOfLength
				boost::optional<float> x_offset;
				// UnitOfLength
				boost::optional<float> y_offset;
				boost::optional<std::size_t> rail_type;
			};

			struct RailType : public instruction_base {
				std::size_t rail_index;
				std::size_t rail_type;
			};

			struct RailEnd : public instruction_base {
				std::size_t rail_index;
				// UnitOfLength
				boost::optional<float> x_offset;
				// UnitOfLength
				boost::optional<float> y_offset;
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

			struct Curve : public instruction_base {
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

			struct FreeObj : public instruction_base {
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

			struct Wall : public instruction_base {
				std::size_t rail_index;
				enum : uint8_t { Left, Both, Right } direction = Both;
				std::size_t wall_structure_index = 0;
			};

			struct WallEnd {
				std::size_t rail_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct Dike : public instruction_base {
				std::size_t rail_index;
				enum : uint8_t { Left, Both, Right } direction = Both;
				std::size_t dike_structure_index;
			};

			struct DikeEnd {
				std::size_t rail_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct Pole : public instruction_base {
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

			struct Crack : public instruction_base {
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

			struct Sta : public instruction_base {
				std::string name;
				std::string arrival_sound;
				std::string departure_sound;
				std::size_t timetable_index = 0;
				openbve2::datatypes::time arrival;
				openbve2::datatypes::time departure;
				float stop_duration = 15;
				float passenger_ratio = 100;
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
				} arrival_tag = ArrivalTime_t::AnyTime;
				enum class DepartureTime_t : uint8_t {
					Time,
					AnyTime,
					Terminal,
					TerminalTime,
					ChangeEnds,
					ChangeEndsTime
				} departure_tag = DepartureTime_t::AnyTime;
				enum class Doors_t : uint8_t { Left, None, Right, Both } doors = Doors_t::None;
			};

			struct Stop : public instruction_base {
				enum { Left, None, Right } stop_post = None;

				float forwards_tolerance = 5;
				float backwards_tolerance = 5;
				std::size_t cars = 0;
			};

			struct Form : public instruction_base {
				std::size_t rail_index_1;
				std::size_t rail_index_2 = 0;
				std::size_t roof_structure_index = 0;
				std::size_t form_structure_index = 0;
				enum { Left, Right, RailIndex } placement = RailIndex;
			};

			struct Limit : public instruction_base {
				// Unit of speed
				float speed = 0;
				enum class Post_t { Left, None, Right } post = Post_t::None;
				enum class Cource_t { Left, None, Right } cource = Cource_t::None;
			};

			struct Section : public instruction_base {
				std::vector<std::size_t> a_term;
			};

			struct SigF : public instruction_base {
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
			struct Signal : public instruction_base {
				enum : uint8_t {
					R_Y,
					R_G,
					R_Y_G,
					R_YY_Y_G,
					R_Y_YG_G,
					R_YY_Y_YG_G,
					R_Y_YG_G_GG,
					R_YY_Y_YG_G_GG
				} type = R_G;
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

			struct Relay : public instruction_base {
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

			struct Beacon : public instruction_base {
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
			struct Transponder : public instruction_base {
				enum : uint8_t { S_type, SN_type, Departure, ATS_P_RENEWAL, ATS_P_STOP } type = S_type;
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

			struct Pattern : public instruction_base {
				enum : bool { Temporary, Permanent } type = Temporary;
				// UnitOfSpeed
				float speed;
			};

			struct Back {
				std::size_t background_texture_index;
				std::size_t file_index = 0;
				std::size_t line = 0;
				float absolute_position = -1;
			};

			struct Fog : public instruction_base {
				// UnitOfLength
				float starting_distance;
				// UnitOfLength
				float ending_distance;
				openbve2::datatypes::color8_rgb color = {128, 128, 128};
			};

			struct Brightness : public instruction_base {
				std::uint8_t value = 255;
			};

			struct Marker : public instruction_base {
				std::string filename;
				// UnitOfLength
				float distance;
			};

			struct MarkerXML : public instruction_base {
				std::string filename;
			};

			struct TextMarker : public instruction_base {
				std::string text;
				// UnitOfLength
				float distance;
				enum : uint8_t { Black, Gray, White, Red, Orange, Green, Blue, Magenta } font_color;
			};

			struct PointOfInterest : public instruction_base {
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

			struct PreTrain : public instruction_base {
				openbve2::datatypes::time time;
			};

			struct Announce : public instruction_base {
				std::string filename;
				// UnitOfSpeed
				float speed;
			};

			struct Doppler : public instruction_base {
				std::string filename;
				// UnitOfLength
				float x_offset = 0;
				// UnitOfLength
				float y_offset = 0;
			};

			struct Buffer : public instruction_base {};
		} // namespace track
	}     // namespace instructions
} // namespace csv_rw_route
} // namespace parsers
