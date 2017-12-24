#pragma once
#ifdef _MSC_VER
	#include "../../../libcore/include/core/datatypes.hpp"
	#include "../../../dependencies/glm/glm/vec3.hpp"
#else
	#include "core/datatypes.hpp"
	#include "glm/vec3.hpp"
#endif
#include <array>
#include <vector>
#include <string>

namespace parsers {
namespace csv_object {
	struct vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 texture_coord;
	};

	struct face_data_t {
		openbve2::datatypes::color8_rgba color;
		openbve2::datatypes::color8_rgba emissive_color;
	};

	struct mesh_t {
		std::vector<vertex> verts;
		std::vector<std::size_t> faces;
		std::vector<face_data_t> face_data;
		std::string texture;
	};

	struct dependencies_t {
		std::vector<std::string> textures;
	};

	struct errors_t {
		//TODO(sirflankalot): Error
	};

	struct parsed_csv_object {
		std::vector<mesh_t> meshes;
		dependencies_t dependencies;
		errors_t errors;
	};
}
}