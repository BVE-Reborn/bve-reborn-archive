#pragma once

#include <cstddef>
#include <iosfwd>
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

	std::ostream& operator<<(std::ostream& os, error_t&);
	std::ostream& operator<<(std::ostream& os, errors_t&);
	std::ostream& operator<<(std::ostream& os, multi_error&);
} // namespace errors
} // namespace parsers
