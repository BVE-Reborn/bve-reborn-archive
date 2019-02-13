#pragma once

#include "parsers/dependencies.hpp"
#include "parsers/function_scripts.hpp"
#include <string>
#include <vector>

namespace bve::parsers::animated_object {
	using FunctionScript = function_scripts::InstructionList;

	struct AnimatedInclude {
		std::string file;
		glm::vec3 location;
	};

	struct AnimatedSubobject {
		glm::vec3 position{};
		std::vector<std::string> states;
		FunctionScript state_function;

		glm::vec3 translate_x_direction = {1, 0, 0};
		glm::vec3 translate_y_direction = {0, 1, 0};
		glm::vec3 translate_z_direction = {0, 0, 1};
		FunctionScript translate_x_function;
		FunctionScript translate_y_function;
		FunctionScript translate_z_function;

		glm::vec3 rotate_x_direction = {1, 0, 0};
		glm::vec3 rotate_y_direction = {0, 1, 0};
		glm::vec3 rotate_z_direction = {0, 0, 1};
		FunctionScript rotate_x_function;
		FunctionScript rotate_y_function;
		FunctionScript rotate_z_function;

		struct Damping {
			float frequency = -1;
			float ratio = -1;
		};

		Damping rotate_x_damping;
		Damping rotate_y_damping;
		Damping rotate_z_damping;

		glm::vec2 texture_shift_x_direction = {1, 0};
		glm::vec2 texture_shift_y_direction = {0, 1};
		FunctionScript texture_shift_x_function;
		FunctionScript texture_shift_y_function;

		FunctionScript track_follower_function;
		bool timetable_override = false;

		float refresh_rate = 0;
	};

	struct ParsedAnimatedObject {
		std::vector<AnimatedInclude> includes;
		std::vector<AnimatedSubobject> subobjects;
		dependencies::Dependencies dependencies;
		errors::Errors errors;
	};

	ParsedAnimatedObject parse(const std::string& file_string);
} // namespace bve::parsers::animated_object
