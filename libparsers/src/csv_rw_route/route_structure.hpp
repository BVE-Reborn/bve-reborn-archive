#pragma once

#include "parsers/xml/dynamic_backgrounds.hpp"
#include "parsers/xml/dynamic_lighting.hpp"
#include <glm/vec3.hpp>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace parsers {
namespace csv_rw_route {
	struct rail_block_info {
		// Primative Parts
		float position = 0;
		float length = 25;
		float pitch = 0;
		float radius = 0;
		float cant = 0;
		float height = 0;

		struct cached_locations {
			glm::vec3 location;
			glm::vec3 direction;
			bool valid = false;
		} cache;
	};

	using filename_set = std::set<std::string>;
	using filename_set_iterator = std::set<std::string>::const_iterator;

	struct rail_object_info {
		filename_set_iterator filename;
		glm::vec3 position;
		glm::vec3 rotation;
		bool flip_x = false;
	};

	struct display_unit_info {
		std::string unit_name;
		float conversion_factor;
	};

	struct animated_signal {
		std::string filename;
	};

	struct traditional_signal {
		std::string signal_filename;
		std::string glow_filename;
	};

	using signal_info = mapbox::util::variant<animated_signal, traditional_signal>;

	// More compatibility levels to come in the future
	struct CompatibilityModes {
		bool bve2_4_transparancy : 1;
		bool bve2_4_content : 1;
	};

	enum class SaftySystemStatus {
		SaftyActiviatedServiceBrakes,
		SaftyActiviatedEmergencyBrakes,
		SaftyDeactivatedEmergencyBrakes
	};

	template <class T>
	struct position_data_pair {
		float position = 0;
		T value = 0;
	};

	using ground_height_info = position_data_pair<float>;
	using rail_adheason_info = position_data_pair<float>;

	struct parsed_route_data {
		// Core route info
		std::vector<rail_block_info> blocks;
		std::vector<ground_height_info> ground_height;

		// Secondary route info
		std::vector<rail_adheason_info> adheason;

		// Objects
		std::vector<rail_object_info> objects;

		// file references
		filename_set object_filenames;
		filename_set texture_filenames;

		// Lighting and Background
		std::vector<xml::dynamic_lighting::lighting_info> lighting;

		// Signalling
		std::vector<float> signal_speed = {0, 25, 55, 75, -1, -1};

		// Other trains
		std::vector<float> ai_train_start_intervals;
		float ai_max_speed = 0;

		CompatibilityModes compatibility;

		// Start Conditions
		SaftySystemStatus safty_system_status;
		openbve2::datatypes::time game_start_time = -1;

		// Sound indices
		std::unordered_map<std::size_t, std::size_t> rail_runsound_mapping;
		std::unordered_map<std::size_t, std::size_t> rail_flangesound_mapping;

		// Customizations
		std::string default_train;
		std::string image_location;
		std::string loading_image_location;
		std::string comment;
		std::string timetable_text;
		display_unit_info display_unit;

		// World State -- Ignored
		float guage = 1435;
		float acceleration_due_to_gravity = 9.80665f;
		float temperature = 20;
		float pressure = 101.325f;
		float altitude = 0;
	};
} // namespace csv_rw_route
} // namespace parsers
