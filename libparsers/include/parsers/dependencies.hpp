#pragma once

#include "core/datatypes.hpp"
#include <set>
#include <string>
#include <tuple>

namespace parsers {
namespace dependencies {
	struct texture_t {
		std::string file;
		openbve2::datatypes::color8_rgb decal_transparent_color = {0, 0, 0};
		bool has_transparent_color = false;

		// less than operator for use in std::set
		friend bool operator<(const texture_t& lhs, const texture_t& rhs) {
			return std::make_tuple(lhs.file, lhs.decal_transparent_color.x,
			                       lhs.decal_transparent_color.y, lhs.decal_transparent_color.z,
			                       lhs.has_transparent_color)
			       < std::make_tuple(rhs.file, rhs.decal_transparent_color.x,
			                         rhs.decal_transparent_color.y, rhs.decal_transparent_color.z,
			                         rhs.has_transparent_color);
		}
	};

	struct dependencies_t {
		std::set<texture_t> textures;
	};
} // namespace dependencies
} // namespace parsers
