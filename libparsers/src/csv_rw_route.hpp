#pragma once

#include "core/datatypes.hpp"
#include "parsers/errors.hpp"
#include <functional>
#include <string>

namespace parsers {
namespace csv_rw_route {
	struct preprocessed_line {
		std::string contents;
		std::size_t filename_index;
		std::size_t line;
		float offset;
	};

	struct preprocessed_lines {
		std::vector<preprocessed_line> lines;
		std::vector<std::string> filenames;
	};

	preprocessed_lines process_include_files(
	    const std::string& filename, openbve2::datatypes::rng& rng, errors::multi_error& errors,
	    const std::function<std::string(const std::string& base_file, const std::string& relative)>& get_abs_path);

	void preprocess_file(preprocessed_lines& lines, openbve2::datatypes::rng& rng, errors::multi_error& errors);
} // namespace csv_rw_route
} // namespace parsers
