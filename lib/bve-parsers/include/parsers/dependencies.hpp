#pragma once

#include "core/datatypes.hpp"
#include <set>
#include <string>
#include <tuple>

namespace bve::parsers::dependencies {
	struct Texture {
		std::string file;
		core::datatypes::Color8RGB decal_transparent_color = {0, 0, 0};
		bool has_transparent_color = false;

		// less than operator for use in std::set
		friend bool operator<(const Texture& lhs, const Texture& rhs) {
			return std::make_tuple(lhs.file, lhs.decal_transparent_color.x,
			                       lhs.decal_transparent_color.y, lhs.decal_transparent_color.z,
			                       lhs.has_transparent_color)
			       < std::make_tuple(rhs.file, rhs.decal_transparent_color.x,
			                         rhs.decal_transparent_color.y, rhs.decal_transparent_color.z,
			                         rhs.has_transparent_color);
		}
	};

	struct Dependencies {
		std::set<Texture> textures;
	};
} // namespace bve::parsers::dependencies
