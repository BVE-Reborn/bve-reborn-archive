#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace parsers {
namespace xml {
	namespace dynamic_backgrounds {
		struct texture_background_info {
			openbve2::datatypes::time time = 0;
			enum { FadeIn, FadeOut, None } transition_mode = None;
			std::size_t repetitions = 6;
			std::string filename;
			std::size_t transition_time = 10;
		};

		struct object_background_info {
			std::string filename;
		};

		using parsed_dynamic_background =
		    mapbox::util::variant<std::vector<texture_background_info>, object_background_info>;

		parsed_dynamic_background parse(const std::string& filename, errors::multi_error& err,
		                                const find_relative_file_func& get_relative_file);
	} // namespace dynamic_backgrounds
} // namespace xml
} // namespace parsers
