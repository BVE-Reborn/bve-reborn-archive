#pragma once

#include "core/datatypes.hpp"
#include <set>
#include <string>

namespace parsers {
namespace dependencies {
	struct texture_t {
		std::string file;
		openbve2::datatypes::color8_rgb decal_transparent_color = {0, 0, 0};
		bool has_transparent_color = false;

		// less than operator for use in std::set
		friend bool operator<(const texture_t& lhs, const texture_t& rhs) {
			auto lhs_dtc_val = uint32_t(lhs.decal_transparent_color.r) << 16
			                   & uint32_t(lhs.decal_transparent_color.g) << 8
			                   & lhs.decal_transparent_color.r;
			auto rhs_dtc_val = uint32_t(rhs.decal_transparent_color.r) << 16
			                   & uint32_t(rhs.decal_transparent_color.g) << 8
			                   & rhs.decal_transparent_color.r;

			return lhs.file < rhs.file && lhs_dtc_val < rhs_dtc_val
			       && static_cast<int>(lhs.has_transparent_color)
			              < static_cast<int>(rhs.has_transparent_color);
		}
	};

	struct dependencies_t {
		std::set<texture_t> textures;
	};
} // namespace dependencies
} // namespace parsers
