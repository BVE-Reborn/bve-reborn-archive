#pragma once

#include "core/datatypes.hpp"
#include "parsers/dependencies.hpp"
#include "parsers/errors.hpp"
#include <vector>

namespace parsers {
// ReSharper disable once CppInconsistentNaming
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
		enum blend_mode_t { normal, additive } blend_mode;
		enum glow_attenuation_mode_t { divide_exponent2, divide_exponent4 } glow_attenuation_mode;
		uint16_t glow_half_distance = 0;
	};

	// ReSharper disable once CppInconsistentNaming
	struct parsed_b3d_csv_object_t {
		std::vector<mesh_t> meshes;
		dependencies::dependencies_t dependencies;
		errors::errors_t errors;
	};

	// defined in b3d_csv_object/parse.cpp
	// ReSharper disable once CppInconsistentNaming
	parsed_b3d_csv_object_t parse_b3d(std::string& filename);
	parsed_b3d_csv_object_t parse_csv(std::string& filename);
} // namespace b3d_csv_object
} // namespace parsers
