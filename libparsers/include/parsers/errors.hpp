#pragma once

#include <cstddef>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace parsers {
namespace errors {
	struct error_t {
		std::intmax_t line;
		std::string error;
	};

	using errors_t = std::vector<error_t>;

	using multi_error_t = std::map<std::string, errors_t>;

	void add_error(errors_t& errors, std::intmax_t line, std::string msg);
	void add_error(errors_t& errors, std::intmax_t line, const std::ostringstream& msg);
	void add_error(multi_error_t& errors,
	               const std::string& filename,
	               std::intmax_t line,
	               std::string msg);
	void add_error(multi_error_t& errors,
	               const std::string& filename,
	               std::intmax_t line,
	               const std::ostringstream& msg);

	std::ostream& operator<<(std::ostream& os, error_t& /*e*/);
	std::ostream& operator<<(std::ostream& os, errors_t& /*es*/);
	std::ostream& operator<<(std::ostream& os, multi_error_t& /*me*/);
} // namespace errors
} // namespace parsers
