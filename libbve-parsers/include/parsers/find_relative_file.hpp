#pragma once

#include <functional>
#include <string>

namespace parsers {
	using find_relative_file_func =
	    std::function<std::string(const std::string& base_file, const std::string& relative)>;
} // namespace parsers
