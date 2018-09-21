#pragma once

#include <functional>
#include <string>

namespace bve::parsers {
	using RelativeFileFunc =
	    std::function<std::string(const std::string& base_file, const std::string& relative)>;
} // namespace bve::parsers
