#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace parsers {
namespace errors {
	struct error_t {
		std::size_t line;
		std::string error;
	};

	using errors_t = std::vector<error_t>;

	using multi_error = std::unordered_map<std::string, errors_t>;
} // namespace errors
} // namespace parsers
