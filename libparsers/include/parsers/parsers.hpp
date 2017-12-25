#pragma once

#include "core/datatypes.hpp"
#include "glm/vec3.hpp"
#include <array>
#include <string>
#include <set>
#include <vector>

namespace parsers {
namespace csv_object {
	struct vertex_t {
		glm::vec3 position = glm::vec3(0);
		glm::vec3 normal = glm::vec3(0);
		glm::vec2 texture_coord = glm::vec2(0);
	};

	struct face_data_t {
		openbve2::datatypes::color8_rgba color = {255, 255, 255, 255};
		openbve2::datatypes::color8_rgb emissive_color = {0, 0, 0};
	};

	struct texture_t {
		std::string file;
		openbve2::datatypes::color8_rgb decal_transparent_color = {0, 0, 0};
		bool has_transparent_color = false;

		// less than operator for use in std::set
		friend bool operator<(const texture_t& lhs, const texture_t& rhs) {
			auto lhs_dtc_val = uint32_t(lhs.decal_transparent_color.r) << 16 & uint32_t(lhs.decal_transparent_color.g) << 8 & lhs.decal_transparent_color.r;
			auto rhs_dtc_val = uint32_t(rhs.decal_transparent_color.r) << 16 & uint32_t(rhs.decal_transparent_color.g) << 8 & rhs.decal_transparent_color.r;

			return lhs.file < rhs.file && lhs_dtc_val < rhs_dtc_val && lhs.has_transparent_color < rhs.has_transparent_color;
		}
	};

	struct mesh_t {
		std::vector<vertex_t> verts;
		std::vector<std::size_t> indices;
		std::vector<face_data_t> face_data;
		texture_t texture;
		enum BlendMode_t { Normal, Additive } BlendMode;
		enum GlowAttenuationMode_t { DivideExponent2, DivideExponent4 } GlowAttenuationMode;
		uint16_t GlowHalfDistance = 0;
	};

	struct dependencies_t {
		std::set<texture_t> textures;
	};

	struct errors_t {
		// TODO(sirflankalot): Error
	};

	struct parsed_csv_object_t {
		std::vector<mesh_t> meshes;
		dependencies_t dependencies;
		errors_t errors;
	};
} // namespace csv_object
} // namespace parsers