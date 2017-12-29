#pragma once

#include "core/datatypes.hpp"
#include "parsers/dependencies.hpp"
#include "parsers/errors.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace parsers {
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
} // namespace parsers
