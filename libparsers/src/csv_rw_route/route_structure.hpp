#pragma once

#include <glm/vec3.hpp>
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
			glm::vec3 secant_line;
		} cache;
	};

	struct rail_object_info {
		std::string filename;
		glm::vec3 position;
		glm::vec3 rotation;
	};

	struct parsed_route_data {
		std::vector<rail_block_info> blocks;
		std::vector<rail_object_info> objects;
	};
} // namespace csv_rw_route
} // namespace parsers
