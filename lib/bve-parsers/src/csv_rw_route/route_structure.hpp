#pragma once

#include "parsers/xml/dynamic_background.hpp"
#include "parsers/xml/dynamic_lighting.hpp"
#include "parsers/xml/route_marker.hpp"
#include "parsers/xml/stations.hpp"
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace bve::parsers::csv_rw_route {
	struct RailBlockInfo {
		// Primitive Parts
		float position = 0;
		float length = 25;
		float pitch = 0;
		float radius = 0;
		float cant = 0;
		float height = 0;

		struct CachedLocations {
			glm::vec3 location;
			glm::vec3 direction;
			bool valid = false;
		} cache;
	};

	using FilenameSet = std::set<std::string>;
	using FilenameSetIterator = std::set<std::string>::const_iterator;

	struct RailObjectInfo {
		FilenameSetIterator filename;
		glm::vec3 position;
		glm::vec3 rotation;
		bool flip_x = false;
	};

	struct DisplayUnitInfo {
		std::string unit_name;
		float conversion_factor;
	};

	struct AnimatedSignal {
		std::string filename;
	};

	struct TraditionalSignal {
		std::string signal_filename;
		std::string glow_filename;
	};

	using SignalInfo = mapbox::util::variant<AnimatedSignal, TraditionalSignal>;

	// More compatibility levels to come in the future
	struct CompatibilityModes {
		bool bve2_4_transparency : 1;
		bool bve2_4_content : 1;
	};

	enum class SafetySystemStatus {
		safety_activated_service_brakes,
		safety_activated_emergency_brakes,
		safety_deactivated_emergency_brakes
	};

	enum class Direction { left, none, right };

	enum class BiDirection { left, both, right };

	struct RailStationStop {
		float position;
		Direction direction;
		float backward_tolerance;
		float forward_tolerance;
		std::size_t car_count;
	};

	struct RailStation {
		xml::stations::RequestStopMarker request_stop_info;
		std::string name;
		FilenameSetIterator arrival_sound;
		FilenameSetIterator departure_sound;
		std::size_t timetable_index = 0;
		std::vector<RailStationStop> stop_points;
		core::datatypes::Time arrival;
		core::datatypes::Time departure;
		float stop_duration = 15;
		float passenger_ratio = 100;
		bool pass_alarm = false;
		bool force_red = false;
		bool system = false;
		enum class ArrivalTime : uint8_t {
			time,
			any_time,
			all_pass,
			player_pass,
			player_stop,
			all_stop
		} arrival_tag = ArrivalTime::any_time;
		enum class DepartureTime : uint8_t {
			time,
			any_time,
			terminal,
			terminal_time,
			change_ends,
			change_ends_time
		} departure_tag = DepartureTime::any_time;
		enum class Doors : std::uint8_t { left, none, right, both } doors = Doors::none;
	};

	struct Beacon {
		float position = 0;
		std::intmax_t beacon_type;
		std::intmax_t beacon_data;
		std::intmax_t section_offset;
	};

	struct ATSPSection {
		float position = 0;
		float speed = 0;
		bool permanent = false;
	};

	struct Fog {
		float position = 0;
		float starting_distance = 0;
		float ending_distance = 0;
		core::datatypes::Color8RGB color;
	};

	struct Marker {
		xml::route_marker::ParsedRouteMarker marker;
		float start;
		float end;
	};

	struct PointOfInterest {
		glm::vec3 position;
		glm::vec3 camera_rotation;
		std::string text;
	};

	struct Announcement {
		float position;
		float speed = 0;
		FilenameSetIterator filename;
	};

	struct Sound {
		glm::vec3 position;
		FilenameSetIterator filename;
	};

	template <class T, std::intmax_t Def>
	struct PositionDataPairDefault {
		float position = 0;
		T value = Def;
	};

	template <class T>
	struct PositionDataPair {
		float position = 0;
		T value;
	};

	using GroundHeight = PositionDataPairDefault<float, 0>;
	using RailAdhesion = PositionDataPairDefault<float, 0>;
	using TrackLimit = PositionDataPairDefault<float, 0>;
	using Section = PositionDataPair<std::vector<std::size_t>>;
	using Background = PositionDataPair<xml::dynamic_background::ParsedDynamicBackground>;
	using BrightnessLevel = PositionDataPair<std::uint8_t>;
	using Pretrain = PositionDataPair<core::datatypes::Time>;

	struct ParsedRoute {
		// Core route info
		std::vector<RailBlockInfo> blocks;
		std::vector<GroundHeight> ground_height;
		std::vector<float> bumpers;

		// Secondary route info
		std::vector<RailAdhesion> adhesion;

		// Objects
		std::vector<RailObjectInfo> objects;

		// Stations
		std::vector<RailStation> stations;

		// Speed limits
		std::vector<TrackLimit> limits;

		// file references
		FilenameSet object_filenames;
		FilenameSet texture_filenames;
		FilenameSet sound_filenames;

		// Lighting and Background
		std::vector<xml::dynamic_lighting::LightingInfo> lighting;
		std::vector<Background> backgrounds;
		std::vector<Fog> fog;
		std::vector<BrightnessLevel> brightness_levels;

		// Signalling
		std::vector<float> signal_speed = {0, 25, 55, 75, -1, -1};
		std::vector<Section> sections;
		std::vector<Beacon> beacons;
		std::vector<ATSPSection> patterns;

		// Other trains
		std::vector<Pretrain> pretrain_points;
		std::vector<float> ai_train_start_intervals;
		float ai_max_speed = 0;

		CompatibilityModes compatibility;

		// Start Conditions
		SafetySystemStatus safety_system_status;
		core::datatypes::Time game_start_time = -1;

		// Sound indices
		std::vector<Sound> sounds;
		std::vector<Announcement> announcements;
		std::unordered_map<std::size_t, std::size_t> rail_runsound_mapping;
		std::unordered_map<std::size_t, std::size_t> rail_flangesound_mapping;

		// Customizations
		std::string default_train;
		std::string image_location;
		std::string loading_image_location;
		std::string comment;
		std::string timetable_text;
		DisplayUnitInfo display_unit;
		std::vector<Marker> markers;
		std::vector<PointOfInterest> points_of_interest;

		// World State -- Ignored
		float gauge = 1435;
		float acceleration_due_to_gravity = 9.80665f;
		float temperature = 20;
		float pressure = 101.325f;
		float altitude = 0;
	};
} // namespace bve::parsers::csv_rw_route
