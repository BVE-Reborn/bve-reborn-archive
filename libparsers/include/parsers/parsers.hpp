#pragma once

#include "core/datatypes.hpp"
#include "glm/vec3.hpp"
#include <array>
#include <set>
#include <string>
#include <vector>

namespace parsers {
namespace errors {
	struct error_t {
		std::size_t line;
		std::string error;
	};

	using errors_t = std::vector<error_t>;
} // namespace errors

namespace dependencies {
	struct texture_t {
		std::string file;
		openbve2::datatypes::color8_rgb decal_transparent_color = {0, 0, 0};
		bool has_transparent_color = false;

		// less than operator for use in std::set
		friend bool operator<(const texture_t& lhs, const texture_t& rhs) {
			auto lhs_dtc_val = uint32_t(lhs.decal_transparent_color.r) << 16 &
			                   uint32_t(lhs.decal_transparent_color.g) << 8 & lhs.decal_transparent_color.r;
			auto rhs_dtc_val = uint32_t(rhs.decal_transparent_color.r) << 16 &
			                   uint32_t(rhs.decal_transparent_color.g) << 8 & rhs.decal_transparent_color.r;

			return lhs.file < rhs.file && lhs_dtc_val < rhs_dtc_val &&
			       lhs.has_transparent_color < rhs.has_transparent_color;
		}
	};

	struct dependencies_t {
		std::set<texture_t> textures;
	};
} // namespace dependencies

namespace b3d_csv_object {
	struct vertex_t {
		glm::vec3 position = glm::vec3(0);
		glm::vec3 normal = glm::vec3(0);
		glm::vec2 texture_coord = glm::vec2(0);
	};

	struct face_data_t {
		openbve2::datatypes::color8_rgba color = {255, 255, 255, 255};
		openbve2::datatypes::color8_rgb emissive_color = {0, 0, 0};
	};

	struct mesh_t {
		std::vector<vertex_t> verts;
		std::vector<std::size_t> indices;
		std::vector<face_data_t> face_data;
		dependencies::texture_t texture;
		enum BlendMode_t { Normal, Additive } BlendMode;
		enum GlowAttenuationMode_t { DivideExponent2, DivideExponent4 } GlowAttenuationMode;
		uint16_t GlowHalfDistance = 0;
	};

	struct parsed_b3d_csv_object_t {
		std::vector<mesh_t> meshes;
		dependencies::dependencies_t dependencies;
		errors::errors_t errors;
	};

	// defined in b3d_csv_object/parse.cpp
	parsed_b3d_csv_object_t parse_b3d(std::string& filename);
	parsed_b3d_csv_object_t parse_csv(std::string& filename);
} // namespace b3d_csv_object

namespace animated_object {
	using function_script = std::string;

	struct animated_include {
		std::string file;
		glm::vec3 location;
	};

	struct animated_subobject {
		glm::vec3 position;
		std::string states;
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

	parsed_animated_object parse(const std::string& file);
} // namespace animated_object
} // namespace parsers
