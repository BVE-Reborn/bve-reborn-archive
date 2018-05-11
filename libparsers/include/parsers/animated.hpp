#pragma once

#include "parsers/dependencies.hpp"
#include "parsers/function_scripts.hpp"
#include <string>
#include <vector>

namespace parsers {
namespace animated_object {
	using function_script = function_scripts::instruction_list;

	struct animated_include {
		std::string file;
		glm::vec3 location;
	};

	struct animated_subobject {
		glm::vec3 position;
		std::vector<std::string> states;
		function_script state_function;

		glm::vec3 translate_x_direction = {1, 0, 0};
		glm::vec3 translate_y_direction = {0, 1, 0};
		glm::vec3 translate_z_direction = {0, 0, 1};
		function_script translate_x_function;
		function_script translate_y_function;
		function_script translate_z_function;

		glm::vec3 rotate_x_direction = {1, 0, 0};
		glm::vec3 rotate_y_direction = {0, 1, 0};
		glm::vec3 rotate_z_direction = {0, 0, 1};
		function_script rotate_x_function;
		function_script rotate_y_function;
		function_script rotate_z_function;

		struct damping {
			float frequency = -1;
			float ratio = -1;
		};

		damping rotate_x_damping;
		damping rotate_y_damping;
		damping rotate_z_damping;

		glm::vec2 texture_shift_x_direction = {1, 0};
		glm::vec2 texture_shift_y_direction = {0, 1};
		function_script texture_shift_x_function;
		function_script texture_shift_y_function;

		function_script track_follower_function;
		bool timetable_override = false;

		float refresh_rate = 0;
	};

	struct parsed_animated_object {
		std::vector<animated_include> includes;
		std::vector<animated_subobject> subobjects;
		dependencies::dependencies_t dependencies;
		errors::errors_t errors;
	};

	parsed_animated_object parse(const std::string& file_string);
} // namespace animated_object
} // namespace parsers
